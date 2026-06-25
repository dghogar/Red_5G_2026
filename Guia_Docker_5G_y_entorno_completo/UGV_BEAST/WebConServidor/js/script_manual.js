/**
 * PROYECTO: Red de Conectividad de Drones Terrestres (UGV) en Situaciones de Emergencia.
 * MÓDULO: Interfaz de Teleoperación y Control de Bajo Nivel (Manual Bypass)
 * ARCHIVO: script_manual.js
 * DESCRIPCIÓN: Script de control directo. Gestiona la entrada física (teclado/táctil),
 * la estabilización cinemática, la decodificación del flujo de vídeo en tiempo real 
 * (WebRTC/JSMpeg) y la captura de pruebas periciales.
 */

// ============================================================================
// 1. CONFIGURACIÓN DINÁMICA DE RED Y ENRUTAMIENTO (LECTOR)
// ============================================================================

// Extracción del ID del robot asignado desde los parámetros GET del Iframe
const urlParams = new URLSearchParams(window.location.search);
const currentRobotId = urlParams.get('robot');

// Lectura de la memoria compartida del navegador (Base de datos local)
const savedFleet = localStorage.getItem('swarmFleet');

// Variables de estado de red por defecto
let ROBOT_IP = "127.0.0.1";
let currentRobotData = null;

// Sincronización de los parámetros IP según el ID recibido
if (savedFleet) {
    const fleet = JSON.parse(savedFleet);
    currentRobotData = fleet.find(r => r.id === currentRobotId);
    if (currentRobotData) {
        ROBOT_IP = currentRobotData.ip;
    }
}

// Expulsión de seguridad si el nodo no está registrado en la topología
if (!currentRobotData) {
    alert(`Error Crítico: El robot '${currentRobotId}' no existe en la configuración de la red.`);
    window.location.href = "index.html"; 
}

// ============================================================================
// INICIALIZACIÓN DEL TÚNEL WEBSOCKET
// ============================================================================
const socket = io();

socket.on('connect', () => {
    console.log(`[LINK] Conectado por Sockets a: ${currentRobotData.id} (${ROBOT_IP})`);
    document.getElementById('robot-title').style.color = "#00ffcc";
});

socket.on('disconnect', () => {
    console.log(`[LINK] Desconectado de: ${currentRobotData.id}`);
    document.getElementById('robot-title').style.color = "#ff4d4d";
});

// ============================================================================
// 2. VARIABLES DE ESTADO GLOBALES Y CINEMÁTICA
// ============================================================================

let currentSpeed = 70; // Duty Cycle (PWM) base de los motores al 70%
let currentZoom = 1.0;
let lightsOn = false;

// Estado cinemático del Gimbal (Servos Pan-Tilt) inicializado en el centro (90º)
let ugvCamPan = 90;
let ugvCamTilt = 90;

// Matriz de eventos de entrada (Permite combinaciones de teclas simultáneas)
const keys = { w: false, a: false, s: false, d: false, i: false, j: false, k: false, l: false, q: false, e: false };

// Caché de comandos para mitigar la saturación de red (Collision Avoidance)
let lastChassisCmd = "";
let lastCamCmd = "";

// ============================================================================
// INICIALIZACIÓN DEL ENTORNO (ONLOAD)
// ============================================================================
window.onload = () => {
    document.getElementById('robot-title').innerText = currentRobotData.id.toUpperCase();
    
    // ESCALABILIDAD: Adaptación dinámica de la Interfaz Hombre-Máquina (HMI) según hardware
    if (currentRobotData.sensor === 'ultrasonic') {
        document.getElementById('panel-ultrasonic').classList.remove('hidden');
        document.getElementById('hud-radar-classic').style.display = 'block';
    } else if (currentRobotData.sensor === 'lidar') {
        document.getElementById('hud-radar-classic').style.display = 'none';
        document.getElementById('hud-radar-lidar').style.display = 'block';
    }
    
    document.getElementById('val-speed').innerText = currentSpeed;
    
    // Petición inicial de apertura del flujo de vídeo
    enviarPorRadio({ action: 'video_start' });
};

// ============================================================================
// 3. LECTURA DE TECLADO Y PROTOCOLOS DE SEGURIDAD
// ============================================================================

document.addEventListener('keydown', (e) => {
    // Bloqueo de repetición del SO para no saturar el buffer
    if (e.repeat) return; 

    const key = e.key.toLowerCase();
   
    if (keys.hasOwnProperty(key)) {
        keys[key] = true;
        updateControlsUI(key, true);
    }
   
    // Mapeo de teclas de acción discreta
    if (key === '+') changeSpeed(10);
    if (key === '-') changeSpeed(-10);
    if (key === 'c') centerCamera();
    if (key === 'f') toggleLights();
    if (key === 'z') changeZoom(-0.5);
    if (key === 'x') changeZoom(0.5);
    if (key === 'r' && currentRobotData.sensor === 'ultrasonic') moveUltrasonic('center');
    if (key === 'p') takePhoto();
    if (key === 'v') { toggleRecord(); }
});

document.addEventListener('keyup', (e) => {
    const key = e.key.toLowerCase();
    if (keys.hasOwnProperty(key)) {
        keys[key] = false;
        updateControlsUI(key, false);
    }
});

/**
 * SISTEMA ANTI-FUGA (DEAD MAN'S SWITCH)
 * En caso de pérdida de enfoque en la ventana del navegador (cambio de pestaña, 
 * pop-ups), se fuerza una parada estructural cortando el PWM de la tracción.
 */
window.addEventListener('blur', () => {
    console.warn("⚠️ FOCO PERDIDO: Activando frenos de emergencia.");

    // Reseteo virtual de las matrices de entrada
    for (let k in keys) keys[k] = false; 
   
    updateControlsUI('w', false); updateControlsUI('a', false);
    updateControlsUI('s', false); updateControlsUI('d', false);

    // Inyección de parada por radio
    lastChassisCmd = "STOP";
    dispatchChassis("STOP");
   
    // Bloqueo del Gimbal
    lastCamCmd = "STOP";
    if (currentRobotData.role === 'master') socket.emit('manual_camera', { eje: 'stop' });
    enviarPorRadio({ action: 'video_stop' });
});

window.addEventListener('beforeunload', () => {
    enviarPorRadio({ action: 'video_stop' });
    dispatchChassis("STOP");
});

window.addEventListener('unload', () => {
    enviarPorRadio({ action: 'video_stop' });
});

function updateControlsUI(key, isActive) {
    const btn = document.getElementById(`btn-${key}`);
    if (btn) isActive ? btn.classList.add('active') : btn.classList.remove('active');
}

// ============================================================================
// SOPORTE HÍBRIDO: BOTONES EN PANTALLA (Ratón / Tablet)
// ============================================================================
const controlButtons = ['w', 'a', 's', 'd', 'i', 'j', 'k', 'l', 'q', 'e'];

controlButtons.forEach(letra => {
    const btn = document.getElementById(`btn-${letra}`);
    if (btn) {
        // Eventos de pulsación mantenida
        btn.addEventListener('mousedown', (e) => { e.preventDefault(); keys[letra] = true; updateControlsUI(letra, true); });
        btn.addEventListener('touchstart', (e) => { e.preventDefault(); keys[letra] = true; updateControlsUI(letra, true); });
        
        // Eventos de liberación
        btn.addEventListener('mouseup', (e) => { e.preventDefault(); keys[letra] = false; updateControlsUI(letra, false); });
        btn.addEventListener('mouseleave', (e) => { e.preventDefault(); keys[letra] = false; updateControlsUI(letra, false); });
        btn.addEventListener('touchend', (e) => { e.preventDefault(); keys[letra] = false; updateControlsUI(letra, false); });
    }
});

// ============================================================================
// 4. BUCLE TRADUCTOR TÁCTICO (Control de Tráfico)
// ============================================================================

let lastCamUpdate = 0;
let lastRadarUpdate = 0;

/**
 * TIMER PRINCIPAL: Optimizado a 10Hz (100ms) en la revisión.
 * Garantiza fluidez de maniobra y previene la tormenta de paquetes broadcast MANET.
 */
setInterval(() => {
    let now = Date.now();

    // --- 1. LÓGICA DE CHASIS ---
    let currentChassis = "STOP";
    if (keys.w) currentChassis = "FORWARD";
    else if (keys.s) currentChassis = "BACKWARD";
    else if (keys.a) currentChassis = "LEFT";
    else if (keys.d) currentChassis = "RIGHT";

    if (currentChassis !== lastChassisCmd) {
        dispatchChassis(currentChassis);
        lastChassisCmd = currentChassis;
    }

    // --- 2. LÓGICA DE CÁMARA (GIMBAL) ---
    if (now - lastCamUpdate >= 150) {
        let camMoved = false;
        const step = 5; 
        
        if (keys.i) { ugvCamTilt -= step; camMoved = true; }
        if (keys.k) { ugvCamTilt += step; camMoved = true; }
        if (keys.j) { ugvCamPan += step; camMoved = true; }
        if (keys.l) { ugvCamPan -= step; camMoved = true; }

        if (camMoved) {
            ugvCamPan = Math.max(0, Math.min(180, ugvCamPan));
            ugvCamTilt = Math.max(0, Math.min(180, ugvCamTilt));
            
            enviarPorRadio({ action: 'camara_absoluto', pan: Math.round(ugvCamPan), tilt: Math.round(ugvCamTilt) });
            lastCamCmd = "MOVING";
            lastCamUpdate = now;
        } 
        else if (lastCamCmd === "MOVING") {
            enviarPorRadio({ action: 'camara_stop' }); // Relajación de la señal PWM a los servos
            lastCamCmd = "STOP";
        }
    }

    // --- 3. LÓGICA DE RADAR ULTRASÓNICO ---
    if (currentRobotData.sensor === 'ultrasonic' && (now - lastRadarUpdate >= 150)) {
        if (keys.q) { enviarPorRadio({ action: 'radar', cmd: 'LEFT' }); lastRadarUpdate = now; }
        else if (keys.e) { enviarPorRadio({ action: 'radar', cmd: 'RIGHT' }); lastRadarUpdate = now; }
    }
}, 100);

// ============================================================================
// 5 & 6. ENVÍO DE COMANDOS UNIVERSALES (RADIO TX)
// ============================================================================

function enviarPorRadio(payload) {
    if (typeof socket !== 'undefined') {
        socket.emit('comando_manual', { robot_id: currentRobotId, payload: payload });
    }
}

function dispatchChassis(dir) {
    const map = { 'FORWARD': 'W', 'BACKWARD': 'S', 'LEFT': 'A', 'RIGHT': 'D', 'STOP': 'STOP' };
    enviarPorRadio({ action: 'chasis', cmd: map[dir] });
}

function centerCamera() {
    ugvCamPan = 90; 
    ugvCamTilt = 90; 
    enviarPorRadio({ action: 'camara_center' });
}

function changeSpeed(amount) {
    currentSpeed = Math.max(10, Math.min(100, currentSpeed + amount));
    document.getElementById('val-speed').innerText = currentSpeed;
    enviarPorRadio({ action: 'velocidad', val: amount });
    
    // Inyección forzada de actualización inmediata en las ruedas si el robot está en marcha
    if (lastChassisCmd !== "STOP") {
        dispatchChassis(lastChassisCmd);
    }
}

function toggleLights() {
    lightsOn = !lightsOn;
    document.getElementById('btn-lights').style.background = lightsOn ? "#ffcc00" : "#333";
    enviarPorRadio({ action: 'luces' });
}

function changeZoom(val) {
    enviarPorRadio({ action: 'zoom', val: val }); 
}

function moveUltrasonic(dir) {
    // Si la orden es centrar, mandamos un comando blindado específico
    if (dir.toLowerCase() === 'center') {
        enviarPorRadio({ action: 'radar_center' });
    } else {
        // Si es barrido lateral, enviamos la orientación directa
        enviarPorRadio({ action: 'radar', cmd: dir.toUpperCase() });
    }
}

// ============================================================================
// 7. LECTURA DE TELEMETRÍA (Vía WebSocket Ultrarrápido)
// ============================================================================

let videoInitialized = false;

socket.on('telemetry_update', (data) => {
    // CORTAFUEGOS MODO MANUAL: Verificamos que la IP real sea la del dron que estamos pilotando
    if (data.ip_real !== ROBOT_IP) return;
    
    // TRADUCCIÓN DE ALIAS: Reemplazamos el ID de fábrica por el nombre que le pusiste en la web
    data.id = currentRobotData.id;
    
    // Deducimos el hardware por el sensor: Ultrasonidos = Yahboom (Canvas), LiDAR = Beast (WebRTC)
    let mode = (currentRobotData.sensor === 'ultrasonic') ? 'udp-mpeg' : 'udp';

    // Todos usan el mismo reproductor ahora, CERO dolores de cabeza
    if (!videoInitialized) {
        videoInitialized = true;
        console.log(`[SISTEMA] Iniciando visor de vídeo universal JSMPEG`);
        document.getElementById('canvas-jsmpeg').style.display = 'block';
        
        // Ocultamos la etiqueta WebRTC para siempre
        const webRtcVid = document.getElementById('video-webrtc');
        if (webRtcVid) webRtcVid.style.display = 'none';
        
        iniciarJSMpeg(); 
    }

    // Parseo de Telemetría Ambiental (Sensor BMP280)
    if (data.telemetry) {
        document.getElementById('val-temp').innerText = (data.telemetry.temp || "--") + " °C";
        document.getElementById('val-pres').innerText = (data.telemetry.pres || "--") + " hPa";
    }

    // Parseo GNSS y Brújula (Módulo Beitian BN-808)
    if (data.curr_lat && data.curr_lon) {
        document.getElementById('val-gps').innerText = `${data.curr_lat.toFixed(5)}, ${data.curr_lon.toFixed(5)}`;
    }
    if (data.heading !== undefined) {
        document.getElementById('val-head').innerText = data.heading.toFixed(1) + " º";
    }
    if (data.sats !== undefined) {
        document.getElementById('val-sats').innerText = data.sats;
    }

    // Rendimiento del Sistema Linux Integrado
    if (data.sys) {
        document.getElementById('val-cpu').innerText = data.sys.cpu + " %";
        document.getElementById('val-ram').innerText = data.sys.ram + " %";
    }

    // RSSI (Señal WiFi) y Alarmas de Link Perdido
    if (data.telemetry && data.telemetry.wifi !== undefined) {
        const spanWifi = document.getElementById('val-wifi');
        spanWifi.innerText = data.telemetry.wifi + " %";
        
        if (data.wifi_critico) {
            spanWifi.style.color = "#ff4d4d";
            document.getElementById('wifi-warning').style.display = 'block';
        } else {
            spanWifi.style.color = "#00ffcc";
            document.getElementById('wifi-warning').style.display = 'none';
        }
    }

    // --- REPRESENTACIÓN DE ENTORNOS ---
    // 1. Radar Clásico (Ultrasonidos HC-SR04)
    if (data.distancia !== undefined && document.getElementById('val-dist')) {
        document.getElementById('val-dist').innerText = data.distancia + " cm";
    }

    // 2. Radar Táctico Bidimensional (LiDAR D500)
    if (data.lidar_frontal_cm !== undefined) {
        if(document.getElementById('val-lidar-frente')) document.getElementById('val-lidar-frente').innerText = data.lidar_frontal_cm;
        if(document.getElementById('val-lidar-izq')) document.getElementById('val-lidar-izq').innerText = data.lidar_izquierda_cm;
        if(document.getElementById('val-lidar-der')) document.getElementById('val-lidar-der').innerText = data.lidar_derecha_cm;
       
        // Indicador visual de colisión inminente (Freno Auto del UGV)
        const alertaFreno = document.getElementById('alerta-freno');
        const hudLidar = document.getElementById('hud-radar-lidar');
       
        if (data.obstaculo_critico && alertaFreno && hudLidar) {
            alertaFreno.style.display = "block";
            hudLidar.style.borderColor = "#ff4d4d"; 
        } else if (alertaFreno && hudLidar) {
            alertaFreno.style.display = "none";
            hudLidar.style.borderColor = "#00ffcc"; 
        }
    }
});

// ============================================================================
// NEGOCIACIÓN WEBSOCKET JSMPEG (PARA EL YAHBOOM G1)
// ============================================================================
function iniciarJSMpeg() {
    const canvas = document.getElementById('canvas-jsmpeg');
    const url = 'ws://127.0.0.1:5001/';
    
    // El reproductor toma el stream MPEG retransmitido por el backend de Python
    const reproductor = new JSMpeg.Player(url, {
        canvas: canvas,
        autoplay: true,
        audio: false,
        disableGl: true,
        videoBufferSize: 512 * 1024 
    });
    console.log("[JSMPEG] Conexión establecida con el puente local.");
}


// ============================================================================
// 8. MULTIMEDIA (ENRUTADOR INTELIGENTE POR HARDWARE)
// ============================================================================

let mediaRecorder;
let recordedChunks = [];

// --- A. CAPTURA DE FOTOS Y EVIDENCIAS ---
function takePhoto() {
    // 1. Orden táctica: El dron guarda una copia física en su tarjeta SD
    enviarPorRadio({ action: 'photo' });

    // 2. Orden local: El navegador guarda una copia en el portátil
    const canvas = document.getElementById('canvas-jsmpeg');
    if (!canvas) return;

    console.log("[MEDIA] Fotograma capturado.");

    canvas.toBlob(blob => {
        if (blob && blob.size > 5000) { 
            enviarArchivoAlServidor(blob, 'foto', 'foto.jpg');
        } else {
            console.error("Fallo al capturar frame. Reintentando...");
            setTimeout(takePhoto, 500);
        }
    }, 'image/jpeg', 0.92);
}

// --- B. GRABACIÓN DE VÍDEO CON CÓDEC ADAPTATIVO ---
function toggleRecord() {
    // 1. Orden táctica: El dron empieza a grabar en su tarjeta SD
    enviarPorRadio({ action: 'record' });

    // 2. Orden local: El navegador graba la pantalla
    const btn = document.getElementById('btn-record');
    
    if (mediaRecorder && mediaRecorder.state === 'recording') {
        mediaRecorder.stop();
        btn.style.background = "transparent";
        btn.innerText = "VÍDEO [V]";
        return;
    }

    const canvas = document.getElementById('canvas-jsmpeg');
    if (!canvas) return;
    
    const sourceStream = canvas.captureStream(30);
    const recorderOptions = { mimeType: 'video/webm' };

    recordedChunks = [];
    try {
        mediaRecorder = new MediaRecorder(sourceStream, recorderOptions);
    } catch (e) {
        mediaRecorder = new MediaRecorder(sourceStream); 
    }
    
    mediaRecorder.ondataavailable = e => { if (e.data.size > 0) recordedChunks.push(e.data); };
    mediaRecorder.onstop = () => {
        const blob = new Blob(recordedChunks, { type: 'video/webm' });
        enviarArchivoAlServidor(blob, 'video', 'video.webm');
    };

    mediaRecorder.start(500); 
    btn.style.background = "#ff4d4d";
    btn.innerText = "⏹ DETENER VÍDEO [V]";
}

// --- C. PUENTE DE SUBIDA DE ARCHIVOS (POST) ---
function enviarArchivoAlServidor(blob, tipo, nombrePorDefecto) {
    if (!blob) return;
    
    let fd = new FormData();
    fd.append('robot_id', currentRobotId);
    fd.append('type', tipo); 
    fd.append('file', blob, nombrePorDefecto);
    
    fetch('/upload_media', { method: 'POST', body: fd })
        .then(response => response.json())
        .then(data => {
            console.log(`[MULTIMEDIA] ${tipo.toUpperCase()} almacenado en la GCS.`);
        })
        .catch(err => console.error(`Error I/O:`, err));
}

// --- D. GESTIÓN DE LA GALERÍA PERICIAL ---
function openGallery() {
    document.getElementById('gallery-modal').style.display = 'flex';
    fetch(`/api/gallery/${currentRobotId}`)
        .then(response => response.json())
        .then(files => {
            const grid = document.getElementById('gallery-grid');
            grid.innerHTML = '';
            
            if (files.length === 0) {
                grid.innerHTML = `<p class="gallery-empty">La carpeta de ${currentRobotId.toUpperCase()} está vacía.</p>`;
                return;
            }
            
            // Renderizado dinámico de la interfaz de visualización
            files.forEach(filePath => {
                const fileName = filePath.split('/').pop();
                const isVideo = filePath.endsWith('.webm') || filePath.endsWith('.mp4');
                const fileUrl = `/media/${filePath}`;
                
                let mediaElement = isVideo
                    ? `<video src="${fileUrl}" controls></video>`
                    : `<img src="${fileUrl}" onclick="window.open('${fileUrl}', '_blank')" title="Clic para ampliar">`;
                
                grid.innerHTML += `
                    <div class="gallery-item">
                        ${mediaElement}
                        <div class="file-name" title="${filePath}">${fileName}</div>
                        <a href="${fileUrl}" download="${fileName}" class="btn-action btn-download">⬇ DESCARGAR</a>
                    </div>
                `;
            });
        })
        .catch(err => console.error("Fallo recuperando inventario de galería:", err));
}

function closeGallery() { 
    document.getElementById('gallery-modal').style.display = 'none'; 
}
