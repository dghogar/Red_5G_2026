/**
 * PROYECTO: Red de Conectividad de Drones Terrestres (UGV) en Situaciones de Emergencia.
 * MÓDULO: Lógica Principal de la Interfaz Táctica (Frontend)
 * ARCHIVO: script_main.js
 * DESCRIPCIÓN: Gestiona el estado global del enjambre, la cartografía interactiva,
 * la validación de rutas mediante geofencing y la recepción asíncrona
 * de telemetría vía WebSockets.
 */

// ============================================================================
// 1. GESTIÓN DINÁMICA DEL ESTADO DE LA FLOTA
// ============================================================================
let fleet = []; // Estructura de datos en memoria que mantiene los nodos activos

/**
 * Recupera la configuración de la red. Prioriza el servidor backend para 
 * mantener sincronía entre múltiples clientes. Si el servidor falla, 
 * implementa un fallback a la memoria local (localStorage).
 */
async function loadFleet() {
    try {
        const response = await fetch('/api/config');
        const serverFleet = await response.json();
        
        if (serverFleet && serverFleet.length > 0) {
            fleet = serverFleet;
        } else {
            const savedFleet = localStorage.getItem('swarmFleet');
            if (savedFleet) fleet = JSON.parse(savedFleet);
        }
        renderFleetUI();
    } catch (e) {
        console.error("[SISTEMA] Error de conexión con GCS local. Usando caché.", e);
    }
}

function abrirConfiguracionFlota() {
    document.getElementById('modal-fleet').style.display = "block";
    renderFleetList();
}

/**
 * Lógica de adición de nodos. Garantiza la unicidad de IDs en la red y 
 * aplica la regla de "Maestro Único" (Single Master Node) para el enjambre.
 */
function agregarRobotAFlota() {
    const id = document.getElementById('fleet-id').value.trim().toLowerCase();
    const ip = document.getElementById('fleet-ip').value.trim();
    const sensor = document.getElementById('fleet-sensor').value;
    const role = document.getElementById('fleet-role').value;

    if (!id || !ip) { alert("Error: ID e IP son campos obligatorios."); return; }
    if (fleet.find(r => r.id === id)) { alert("Error: Conflicto de ID en la subred."); return; }
    
    // Transición de roles: Si se designa un nuevo nodo maestro, los demás se degradan a esclavos
    if (role === 'master') {
        fleet.forEach(r => r.role = 'slave');
    }

    fleet.push({ id, ip, sensor, role });
    
    // Reseteo de campos de la interfaz
    document.getElementById('fleet-id').value = '';
    document.getElementById('fleet-ip').value = '';
    
    renderFleetList();
}

function borrarRobot(id) {
    fleet = fleet.filter(r => r.id !== id);
    renderFleetList();
}

/**
 * Renderiza la lista temporal de unidades en el modal de configuración.
 * Distingue visualmente entre Nodos Primarios (M) y Secundarios (S).
 */
function renderFleetList() {
    const ul = document.getElementById('fleet-list');
    ul.innerHTML = '';
    
    if (fleet.length === 0) {
        ul.innerHTML = '<li>Red inactiva: Sin unidades asignadas</li>';
        return;
    }

    fleet.forEach(robot => {
        const isMaster = robot.role === 'master';
        const cssClass = isMaster ? 'fleet-item fleet-item-master' : 'fleet-item';
        const roleIcon = isMaster ? '(M/P)' : '(E/S)';
        
        ul.innerHTML += `
            <li class="${cssClass}">
                <div>
                    <strong>${roleIcon} ${robot.id.toUpperCase()}</strong><br>
                    <span style="font-size: 0.8em; color: #888;">IP: ${robot.ip} | Sensor: ${robot.sensor}</span>
                </div>
                <button class="btn-del-robot" onclick="borrarRobot('${robot.id}')">X</button>
            </li>
        `;
    });
}

/**
 * Consolida la configuración de la red MANET. Exige validación topológica
 * (obligatorio un nodo maestro) antes de transmitir la tabla de enrutamiento al servidor.
 */
async function guardarFlotaYReiniciar() {
    if (fleet.length > 0 && !fleet.find(r => r.role === 'master')) {
        alert("Error Topológico: Se requiere exactamente UN nodo Maestro en la red.");
        return;
    }
    
    localStorage.setItem('swarmFleet', JSON.stringify(fleet));

    try {
        await fetch('/api/config', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(fleet)
        });
        closeModal('modal-fleet');
        location.reload(); // Recarga limpia para instanciar sockets y mapas con la nueva config
    } catch (e) {
        alert("Fallo crítico: No se pudo sincronizar con estacion_tierra.py.");
    }
}

// ============================================================================
// 2. RENDERIZADO DEL DASHBOARD Y MOTOR CARTOGRÁFICO (LEAFLET)
// ============================================================================
let markers = {};         // Diccionario de instancias L.marker de los drones
let targets = {};         // Diccionario de coordenadas de destino por ID
let activeTargeting = null; // Variable de estado para la herramienta de selección en mapa

// Inicialización del objeto Mapa (Sistema de Coordenadas WGS84)
// Se utilizan tiles locales para garantizar el funcionamiento sin internet (Off-Grid)
const map = L.map('map').setView([41.6624, -4.7056], 18);
L.tileLayer('assets/mapas/{z}/{x}/{y}.jpg', { maxNativeZoom: 18, minZoom: 5, maxZoom: 22 }).addTo(map);

const robotIcon = L.icon({
    iconUrl: 'css/images/marker-icon.png',     
    shadowUrl: 'css/images/marker-shadow.png', 
    iconSize: [25, 41], iconAnchor: [12, 41], shadowSize: [41, 41]
});

/**
 * Genera el DOM dinámicamente para las tarjetas de control lateral 
 * e instancia los objetos cartográficos de la flota.
 */
function renderFleetUI() {
    const container = document.getElementById('fleet-container');
    if (fleet.length === 0) return; 
    
    container.innerHTML = ''; 

    fleet.forEach(robot => {
        const isMaster = robot.role === 'master';
        const titleColor = isMaster ? '#ffcc00' : '#ff9900';
        const titleIcon = isMaster ? '(M/P)' : '(E/S)';
        
        // 1. Inyección de Tarjetas HTML de Telemetría
        container.innerHTML += `
            <div class="robot-card" id="card-${robot.id}">
                <h3 style="color: ${titleColor}">${titleIcon} ${robot.id.toUpperCase()}</h3>
                <div class="telemetry">
                    <p>Enlace: <span id="${robot.id}-wifi">--</span>% | Tº: <span id="${robot.id}-temp">--</span>°C | P: <span id="${robot.id}-pres">--</span> hPa</p>
                    <p>GNSS: <span id="${robot.id}-gps">--</span> | Heading: <span id="${robot.id}-head">--</span>º</p>
                    <p>Estado Nav: <strong id="${robot.id}-navstate">ESPERANDO</strong></p>
                    <p>Punto Control: <span id="${robot.id}-target">No asignado</span>
                        <button class="btn-clear" id="clear-${robot.id}" onclick="clearTarget('${robot.id}')" title="Purgar waypoint">🗑️</button>
                    </p>
                </div>
                <div class="button-group-row">
                    <button class="btn-action" onclick="activateTargetMode('${robot.id}')">Waypoint Visual</button>
                    <button class="btn-action-alt" onclick="openTargetModal('${robot.id}')">Waypoint Manual</button>
                </div>
                <button id="btn-manual-${robot.id}" class="btn-manual mt-15" onclick="irAControlManual('${robot.id}', '${robot.ip}')">Bypass: Control Manual</button>
            </div>
        `;

        // 2. Instanciación de Marcadores GIS
        targets[robot.id] = null;
        if (markers[robot.id]) map.removeLayer(markers[robot.id]);

        markers[robot.id] = L.marker([0, 0], { icon: robotIcon }).addTo(map);
        markers[robot.id].bindTooltip(robot.id.toUpperCase(), {permanent: true, direction: 'top'});

        // Interfaz táctil interactiva sobre el mapa
        markers[robot.id].on('click', function() {
            activateTargetMode(robot.id);
            console.log(`[GIS] Enfoque de waypoint para unidad: ${robot.id.toUpperCase()}`);
        });
    });
}

/**
 * Transición de Control Autónomo a Teleoperación.
 * Implementa una parada de emergencia de hardware antes del handover para prevenir colisiones.
 */
function irAControlManual(id, ip) {
    const btn = document.getElementById(`btn-manual-${id}`);
    const statusMsg = document.getElementById('system-status');

    btn.innerText = "Sincronizando...";
    btn.style.backgroundColor = "#00ffcc"; 
    btn.style.color = "#000"; 

    statusMsg.innerText = `Negociando enlace dedicado con nodo ${id.toUpperCase()}...`;
    statusMsg.style.color = "#00ccff"; 

    // Bloqueo cinemático de seguridad pre-handover
    if (localSocket) {
        localSocket.emit('comando_manual', { robot_id: id, payload: { action: 'chasis', cmd: 'STOP' } });
    }

    // Despliegue de Iframe para aislar el contexto de control directo
    document.getElementById('manual-iframe').src = `manual.html?robot=${id}`;
    document.getElementById('manual-overlay').style.display = 'block';
    document.getElementById('manual-iframe').focus();
    
    setTimeout(() => {
        btn.innerText = "Bypass: Control Manual";
        btn.style.backgroundColor = "#0066cc";
        btn.style.color = "white";
    }, 1000);
}

// Escuchador de eventos del Iframe hijo para restaurar la interfaz principal
window.cerrarControlManual = function() {
    document.getElementById('manual-overlay').style.display = 'none';
    document.getElementById('manual-iframe').src = '';
    document.getElementById('system-status').innerText = "Enlace táctico global restablecido.";
}
window.addEventListener("message", function(event) {
    if (event.data === "cerrar_manual") window.cerrarControlManual();
});

let mapaCentrado = false;
function actualizarPosicionRobot(lat, lon, robotId) {
    if (markers[robotId]) markers[robotId].setLatLng([lat, lon]);
    // Auto-focus en el primer fix GPS válido
    if (!mapaCentrado) {
        map.flyTo([lat, lon], 18, { animate: true, duration: 2 });
        mapaCentrado = true; 
    }
}

// ============================================================================
// 3. CAPA DE SEGURIDAD GEOGRÁFICA (GEOFENCING)
// ============================================================================
const drawnItemsLayer = new L.FeatureGroup();
map.addLayer(drawnItemsLayer);

// Herramientas de dibujo vectorial
const drawControl = new L.Control.Draw({
    edit: { featureGroup: drawnItemsLayer },
    draw: {
        polygon: { shapeOptions: { color: '#00ffcc', fillColor: '#00ffcc', fillOpacity: 0.05, weight: 2 } }, // Misión
        rectangle: { shapeOptions: { color: '#000000', fillColor: '#1a1a1a', fillOpacity: 0.5, weight: 2, dashArray: '5, 5' } }, // Peligro
        polyline: false, circle: false, circlemarker: false, marker: false
    }
});
map.addControl(drawControl);

let forbiddenPolygons = []; // Zonas de Exclusión (Obstáculos, fuego, riesgo estructural)
let missionPolygons = [];   // Perímetro de Operación Autorizado

function reindexZones(type) {
    const list = type === 'forbidden' ? forbiddenPolygons : missionPolygons;
    const prefix = type === 'forbidden' ? 'Z-Riesgo-' : 'Z-Op-';
    
    list.forEach((item, index) => {
        const newName = `${prefix}${index + 1}`;
        item.name = newName; 
        const layer = drawnItemsLayer.getLayer(item.id);
        if (layer) layer.setTooltipContent(newName);
    });
}

function registerZone(layer, type, loadedName = null) {
    drawnItemsLayer.addLayer(layer);
    
    let zoneName = loadedName;
    if (!zoneName) {
        const count = type === 'forbidden' ? forbiddenPolygons.length : missionPolygons.length;
        zoneName = type === 'forbidden' ? `Z-Riesgo-${count + 1}` : `Z-Op-${count + 1}`;
    }

    const cssClass = type === 'forbidden' ? 'zone-label zone-label-bad' : 'zone-label';
    layer.bindTooltip(zoneName, { permanent: true, direction: 'center', className: cssClass }).openTooltip();

    // Exportación a estándar GeoJSON para procesamiento matemático con Turf.js
    const data = { id: L.stamp(layer), name: zoneName, geojson: layer.toGeoJSON(), type: type };
    
    if (type === 'forbidden') forbiddenPolygons.push(data);
    else missionPolygons.push(data);
    
    saveState(); 
    checkMissionReady();
}

// Listeners de Eventos GIS
map.on(L.Draw.Event.CREATED, function (e) {
    const type = e.layerType === 'rectangle' ? 'forbidden' : 'mission';
    registerZone(e.layer, type);
});
map.on(L.Draw.Event.DELETED, function (e) {
    e.layers.eachLayer(function (layer) {
        const id = L.stamp(layer);
        forbiddenPolygons = forbiddenPolygons.filter(p => p.id !== id);
        missionPolygons = missionPolygons.filter(p => p.id !== id);
    });
    reindexZones('forbidden'); reindexZones('mission');
    saveState(); checkMissionReady();
});

// ============================================================================
// 4. PLANIFICACIÓN DE RUTAS Y VALIDACIÓN MATEMÁTICA
// ============================================================================
let targetMarkers = {}; 

function clearTarget(robotId) {
    if (targetMarkers[robotId]) { map.removeLayer(targetMarkers[robotId]); targetMarkers[robotId] = null; }
    targets[robotId] = null; 
    document.getElementById(`${robotId}-target`).innerText = "No asignado";
    document.getElementById(`clear-${robotId}`).style.display = 'none';
    saveState(); checkMissionReady();
}

function activateTargetMode(robotId) {
    activeTargeting = robotId;
    document.getElementById('system-status').innerText = `Asignando vector de aproximación: ${robotId.toUpperCase()}`;
    document.getElementById('system-status').style.color = "#ffffff";
    document.getElementById('map').style.cursor = 'crosshair';
}

map.on('click', function(e) {
    if (!activeTargeting) return;
    setRobotTarget(activeTargeting, e.latlng.lat, e.latlng.lng);
    activeTargeting = null;
    document.getElementById('map').style.cursor = '';
});

/**
 * Validador de Waypoints. Aplica lógica booleana sobre matrices GeoJSON
 * para evitar que los operarios envíen drones a zonas restringidas.
 */
function setRobotTarget(robotId, lat, lng, isLoading = false) {
    const pt = turf.point([lng, lat]); // Instancia espacial del destino
    
    if (!isLoading) {
        if (missionPolygons.length === 0) {
            alert("Violación de Protocolo: Delimite una 'Zona de Operación' antes de despachar unidades.");
            return false;
        }

        // Restricción 1: El punto debe existir dentro de un perímetro seguro (Inclusión)
        let isInsideMission = false;
        for (let item of missionPolygons) {
            if (turf.booleanPointInPolygon(pt, item.geojson)) {
                isInsideMission = true; break;
            }
        }
        if (!isInsideMission) { alert("Waypoint rechazado: Fuera del área de cobertura designada."); return false; }
        
        // Restricción 2: El punto no debe solapar con geometrías peligrosas (Exclusión)
        let isForbidden = false;
        for (let item of forbiddenPolygons) {
            if (turf.booleanPointInPolygon(pt, item.geojson)) {
                isForbidden = true; break;
            }
        }
        if (isForbidden) { alert("Waypoint rechazado: Inserción en zona estructuralmente comprometida."); return false; }
    }

    const latlng = L.latLng(lat, lng);
    targets[robotId] = latlng;

    const robot = fleet.find(r => r.id === robotId);
    const colorMarcador = (robot && robot.role === 'master') ? '#ffcc00' : '#00ccff';

    if (targetMarkers[robotId]) {
        targetMarkers[robotId].setLatLng(latlng);
        targetMarkers[robotId].setStyle({ color: colorMarcador, fillColor: colorMarcador });
    } else {
        targetMarkers[robotId] = L.circleMarker(latlng, {
            color: colorMarcador, radius: 8, fillColor: colorMarcador, fillOpacity: 0.8, weight: 2
        }).addTo(map);
        
        targetMarkers[robotId].bindTooltip(`Destino: ${robotId.toUpperCase()}`, {
            permanent: true, direction: 'right', className: 'zone-label'
        });
    }

    document.getElementById(`${robotId}-target`).innerText = `${lat.toFixed(5)}, ${lng.toFixed(5)}`;
    document.getElementById(`clear-${robotId}`).style.display = 'inline-block';
    
    if (!isLoading) saveState(); 
    checkMissionReady();
    return true;
}

/** Máquina de estados: Solo permite ejecución si se cumplen las precondiciones geométricas */
function checkMissionReady() {
    const btn = document.getElementById('btn-execute');
    const statusMsg = document.getElementById('system-status');
    let faltan = [];

    if (fleet.length === 0) {
        btn.disabled = true; statusMsg.innerText = "Configura la topología de red primero."; return;
    }

    if (missionPolygons.length === 0) faltan.push("Perímetro Base");
    fleet.forEach(robot => { if (!targets[robot.id]) faltan.push(`Waypoint ${robot.id.toUpperCase()}`); });

    if (faltan.length > 0) {
        btn.disabled = true;
        statusMsg.innerText = `Pre-Flight Check fallido. Requisito: ${faltan.join(', ')}`;
        statusMsg.style.color = "#ffcc00"; 
    } else {
        btn.disabled = false;
        statusMsg.innerText = "Clear to Launch: Parámetros de misión verificados.";
        statusMsg.style.color = "#33ff33"; 
    }
}

// ============================================================================
// 5. RECEPCIÓN DE TELEMETRÍA (WEBSOCKETS)
// ============================================================================
let localSocket = null;

/**
 * Establece el puente WebSocket para refresco del DOM a alta frecuencia.
 * Parsea los Heartbeats procesados por el backend en Python.
 */
function conectarTelemetriaContinua() {
    if (!localSocket) {
        localSocket = io(); 
        
        localSocket.on('telemetry_update', (data) => {
            //CORTAFUEGOS: Solo aceptamos telemetría de IPs que estén en nuestra lista actual
            const robotConfig = fleet.find(r => r.ip === data.ip_real);
            // Si la IP no está registrada en el panel, lo ignoramos totalmente (evita intrusos/redes antiguas)
            if (!robotConfig) return;
            // Ignoramos su nombre de fábrica e inyectamos el nombre que TÚ le pusiste
            const robotId = robotConfig.id;
            data.id = robotId;

            // Actualización GNSS
            const gpsSpan = document.getElementById(`${robotId}-gps`);
            if (gpsSpan) {
                if (data.curr_lat && data.curr_lon && data.curr_lat !== 0.0) {
                    gpsSpan.innerText = `${data.curr_lat.toFixed(5)}, ${data.curr_lon.toFixed(5)}`;
                    actualizarPosicionRobot(data.curr_lat, data.curr_lon, robotId);
                } else {
                    gpsSpan.innerText = "Adquiriendo fijación...";
                }
            }

            if (data.heading !== undefined) document.getElementById(`${robotId}-head`).innerText = data.heading.toFixed(1);
            
            // Actualización de Sensores Ambientales (BMP280 / ESP32)
            if (data.telemetry) {
                if (data.telemetry.temp !== undefined) document.getElementById(`${robotId}-temp`).innerText = data.telemetry.temp;
                if (data.telemetry.pres !== undefined) document.getElementById(`${robotId}-pres`).innerText = data.telemetry.pres;
            }

            // Actualización de Lógica de Ruta Local
            if (data.nav_state !== undefined) {
                const navSpan = document.getElementById(`${robotId}-navstate`);
                if (navSpan) {
                    navSpan.innerText = data.nav_state;
                    if (data.nav_state === "EN_RUTA") navSpan.style.color = "#ffcc00"; 
                    else if (data.nav_state === "LLEGADA") navSpan.style.color = "#33ff33"; 
                    else navSpan.style.color = "#ffffff"; 
                }
            }

            // Control de Atenuación de Señal (RSSI)
            if (data.telemetry && data.telemetry.wifi !== undefined) {
                const wifiSpan = document.getElementById(`${robotId}-wifi`);
                if (wifiSpan) {
                    wifiSpan.innerText = data.telemetry.wifi;
                    wifiSpan.style.color = data.telemetry.wifi < 30 ? "#ff4d4d" : "#00ffcc";
                }
            }
            
            // Alarmas Visuales de Pérdida de Enlace Crítico
            const cardDron = document.getElementById(`card-${robotId}`);
            const targetSpan = document.getElementById(`${robotId}-target`);
            
            if (data.wifi_critico && cardDron && targetSpan) {
                cardDron.style.border = "2px solid #ff4d4d";
                cardDron.style.boxShadow = "0 0 15px rgba(255, 77, 77, 0.4)";
                if (targets[robotId]) targetSpan.innerHTML = `<span style="color:#ff4d4d; font-weight:bold;">⚠️ ENLACE COMPROMETIDO. REQUIERE PILOTAJE MANUAL.</span>`;
            } else if (cardDron) {
                cardDron.style.border = "1px solid #333";
                cardDron.style.boxShadow = "none";
            }
        });

        localSocket.on('disconnect', () => {
            fleet.forEach(robot => {
                const gpsSpan = document.getElementById(`${robot.id}-gps`);
                if(gpsSpan) gpsSpan.innerText = "GCS DESCONECTADA";
            });
        });
    }
}

// ============================================================================
// 6. EJECUCIÓN TÁCTICA Y PARADAS DE EMERGENCIA
// ============================================================================
function executeMission() {
    const statusMsg = document.getElementById('system-status');
    statusMsg.innerText = "Upload de vectores de aproximación por radio link...";
    statusMsg.style.color = "#00ffcc";

    const btnParada = document.getElementById('btn-parada');
    btnParada.disabled = false; btnParada.innerHTML = "ABORTAR ENJAMBRE";

    // Extracción de coordenadas prohibidas para inyectarlas en el pathfinder de cada dron
    const forbiddenCoords = forbiddenPolygons.map(p => p.geojson.geometry.coordinates);
    if (fleet.length === 0) return;

    // Peticiones HTTP POST al backend Python para generar la transmisión MANET
    fleet.forEach(robot => {
        if (targets[robot.id]) {
            fetch('/goto', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ robot_id: robot.id, lat: targets[robot.id].lat, lon: targets[robot.id].lng, zones: forbiddenCoords })
            }).catch(err => console.error(`[TX FALLIDA] Nodo ${robot.id}:`, err));
        }
    });
}

/** Kill-Switch de Hardware: Detiene tracción en todos los nodos de la subred */
document.getElementById('btn-parada').addEventListener('click', () => {
    if (fleet.length === 0) return;
    const btn = document.getElementById('btn-parada');
    btn.innerHTML = "BLOQUEO CINEMÁTICO ACTIVO";
    btn.disabled = true; 
    
    document.getElementById('system-status').innerText = "ALARMA: EMISIÓN DE CÓDIGO DE PARADA GLOBAL.";
    document.getElementById('system-status').style.color = "#ff4d4d";

    fetch('/stop', { method: 'POST' }).catch(err => console.error(`Fallo de pasarela:`, err));
});

// ============================================================================
// 7. PERSISTENCIA EN MEMORIA NO VOLÁTIL DEL NAVEGADOR
// ============================================================================
function saveState() {
    const state = { zones: [], savedTargets: {} };
    drawnItemsLayer.eachLayer(function(layer) {
        let type = layer.options.dashArray === '5, 5' ? 'forbidden' : 'mission';
        state.zones.push({ type: type, name: layer.getTooltip().getContent(), coords: layer.getLatLngs() });
    });
    Object.keys(targets).forEach(id => {
        if (targets[id]) state.savedTargets[id] = { lat: targets[id].lat, lng: targets[id].lng };
    });
    localStorage.setItem('mapState', JSON.stringify(state));
}

function loadState() {
    const saved = localStorage.getItem('mapState');
    if (!saved) return;
    try {
        const state = JSON.parse(saved);
        if (state.zones) {
            state.zones.forEach(zone => {
                const options = zone.type === 'forbidden' 
                    ? { color: '#000000', fillColor: '#1a1a1a', fillOpacity: 0.5, weight: 2, dashArray: '5, 5' }
                    : { color: '#00ffcc', fillColor: '#00ffcc', fillOpacity: 0.05, weight: 2 };
                const newPolygon = L.polygon(zone.coords, options);
                registerZone(newPolygon, zone.type, zone.name); 
            });
        }
        if (state.savedTargets) {
            Object.keys(state.savedTargets).forEach(id => {
                if (fleet.find(r => r.id === id)) setRobotTarget(id, state.savedTargets[id].lat, state.savedTargets[id].lng, true);
            });
        }
        checkMissionReady();
    } catch (e) {
        localStorage.removeItem('mapState'); 
    }
}

// Interfaces modales auxiliares para input de texto...
let currentManualRobot = null;
function closeModal(id) { document.getElementById(id).style.display = "none"; }
function openTargetModal(robotId) {
    currentManualRobot = robotId;
    document.getElementById('modal-target-title').innerText = `Inserción Manual de Vectores: ${robotId.toUpperCase()}`;
    document.getElementById('input-lat').value = ''; document.getElementById('input-lng').value = '';
    document.getElementById('modal-target').style.display = "block";
}
function applyManualTarget() {
    const lat = parseFloat(document.getElementById('input-lat').value);
    const lng = parseFloat(document.getElementById('input-lng').value);
    if (isNaN(lat) || isNaN(lng)) { alert("Cálculo denegado: Coordenadas flotantes no válidas."); return; }
    if (setRobotTarget(currentManualRobot, lat, lng)) {
        closeModal('modal-target'); map.panTo(new L.LatLng(lat, lng)); 
    }
}
function openZoneModal() {
    document.getElementById('input-polygon').value = ''; document.getElementById('modal-zone').style.display = "block";
}
function applyManualZone() {
    const type = document.getElementById('select-zone-type').value;
    const textLines = document.getElementById('input-polygon').value.split('\n');
    let coords = [];
    for (let line of textLines) {
        if (!line.trim()) continue;
        const parts = line.split(',');
        if (parts.length === 2) {
            const lat = parseFloat(parts[0].trim());
            const lng = parseFloat(parts[1].trim());
            if (!isNaN(lat) && !isNaN(lng)) coords.push([lat, lng]);
        }
    }
    if (coords.length < 3) { alert("Geometría inválida: Un polígono requiere mínimo 3 vértices."); return; }
    const options = type === 'forbidden' 
        ? { color: '#000000', fillColor: '#1a1a1a', fillOpacity: 0.5, weight: 2, dashArray: '5, 5' }
        : { color: '#00ffcc', fillColor: '#00ffcc', fillOpacity: 0.05, weight: 2 };
    const newPolygon = L.polygon(coords, options);
    registerZone(newPolygon, type); map.fitBounds(newPolygon.getBounds()); closeModal('modal-zone');
}

// ARRANQUE SECUENCIAL DEL MOTOR JS
window.onload = () => { 
    loadFleet(); 
    loadState(); 
    conectarTelemetriaContinua();
};