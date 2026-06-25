"""
PROYECTO: Red de Conectividad de Drones Terrestres (UGV) en Situaciones de Emergencia.
MÓDULO: Estación de Control de Tierra (GCS - Ground Control Station)
ARCHIVO: estacion_tierra.py
DESCRIPCIÓN: Nodo central de la red táctica MANET. Gestiona la interfaz de usuario web,
             el puente de comunicaciones de radio de baja latencia (UDP) y la telemetría 
             bidireccional (WebSockets). La carga intensiva de procesamiento de vídeo 
             ha sido delegada al microservicio 'video_gateway.py'.
"""

import socket
import json
import threading
import os
import time
import logging
import psutil
from flask import Flask, request, jsonify, send_from_directory
from flask_socketio import SocketIO
from flask_cors import CORS

# Integración del microservicio de enrutamiento de vídeo (Separación de responsabilidades)
import video_gateway

# =============================================================================
# 1. CONFIGURACIÓN DEL SERVIDOR WEB Y PROTOCOLOS ASÍNCRONOS
# =============================================================================

# Supresión de logs HTTP estándar de Werkzeug para mantener la consola táctica limpia
log_flask = logging.getLogger('werkzeug')
log_flask.setLevel(logging.ERROR)

# Instanciación del servidor WSGI Flask. Se configuran las rutas estáticas para servir el frontend.
app = Flask(__name__, static_folder='.', static_url_path='')

# Habilitación de CORS (Cross-Origin Resource Sharing) para peticiones entre iframes o dominios locales
CORS(app)

# Instanciación de SocketIO en modo 'threading' para compatibilidad nativa con los hilos del sistema operativo
socketio = SocketIO(app, cors_allowed_origins="*", async_mode='threading')

# =============================================================================
# 2. CONFIGURACIÓN DE LA CAPA DE ENLACE TÁCTICO (MANET)
# =============================================================================

PUERTO_MANET = 65432

# Creación del socket no orientado a conexión (UDP) para priorizar latencia sobre fiabilidad de entrega
sock_manet = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Habilitación explícita del flag SO_BROADCAST a nivel de sistema operativo
sock_manet.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

# Escucha promiscua en todas las interfaces de red disponibles (0.0.0.0)
sock_manet.bind(('0.0.0.0', PUERTO_MANET))

#Memoria de traducción (IP -> ID Real de Hardware)
mapa_ips_hw = {}

def obtener_ip_broadcast():
    """Escanea las tarjetas de red y devuelve la IP de difusión de la subred actual"""
    for interfaz, direcciones in psutil.net_if_addrs().items():
        for addr in direcciones:
            # Filtramos localhost y buscamos la tarjeta IPv4 conectada
            if addr.family == socket.AF_INET and addr.broadcast and addr.address != '127.0.0.1':
                return addr.broadcast
    return '255.255.255.255' # Fallback extremo

IP_RED = "10.47.0.9" #IP de Android  obtener_ip_broadcast()

@app.route('/')
def index():
    """Servicio del panel de control principal (Frontend)"""
    return app.send_static_file('index.html')

# =============================================================================
# 3. GESTIÓN DE COMUNICACIONES DE RADIO (Gateway UDP <-> WebSocket)
# =============================================================================

def escuchar_radio_manet():
    """
    HILO DE RECEPCIÓN (RX): 
    Escucha de forma ininterrumpida los datagramas UDP provenientes de los drones.
    Deserializa las tramas JSON y las retransmite instantáneamente 
    a la interfaz web del operador mediante eventos WebSocket.
    """
    print("[RADIO] 🎧 Antena táctica activa. Escuchando en puerto 65432...")
    while True:
        try:
            # Recepción bloqueante con un buffer amplio (64KB) para evitar truncamiento de datagramas
            data, addr = sock_manet.recvfrom(65535)
            paquete = json.loads(data.decode('utf-8'))
            
            # Filtrado de topología: Solo se procesan latidos de telemetría de la flota
            if paquete.get("tipo") == "HEARTBEAT":

                ip_origen = addr[0] # Extraemos la IP física real
                hw_id = paquete.get("origen") # Extraemos cómo se llama internamente el dron
                # El servidor aprende silenciosamente quién es quién
                mapa_ips_hw[ip_origen] = hw_id

                datos_web = paquete.get("payload", {})
                datos_web["ip_real"] = ip_origen # Se lo pasamos a la web para el firewall
                
                # Emisión asíncrona a todos los clientes web (operadores) suscritos
                socketio.emit('telemetry_update', datos_web)
                
        except Exception as e: 
            print(f"[ERROR RADIO] Fallo en capa de recepción/deserialización: {e}")
            # Freno de seguridad temporal para evitar consumo del 100% de CPU si cae la interfaz WiFi física
            time.sleep(1)

# Despliegue del hilo demonio de recepción de radio
threading.Thread(target=escuchar_radio_manet, daemon=True).start()

def gritar_por_radio(destino, comando):
    """
    MÉTODO DE TRANSMISIÓN (TX):
    Empaqueta directivas de la GCS y las inyecta en la red MANET.
    Implementa control de congestión de red: utiliza envíos dirigidos (Unicast)
    para comandos específicos y reserva la difusión masiva (Broadcast) solo para emergencias.
    """
    sobre = {
        "origen": "base_tierra",
        "destino": destino,
        "tipo": "COMANDO",
        "payload": comando
    }
    try:
        if destino == "ALL" or comando == "STOP":
            # Emergencia o directiva global: Inundación de red (Broadcast)
            sock_manet.sendto(json.dumps(sobre).encode('utf-8'), (IP_RED, PUERTO_MANET))
        else:
            # Tráfico nominal: Resolución dinámica de IP estática y envío dirigido (Unicast)
            flota = cargar_flota_disco()
            ip_destino = next((robot['ip'] for robot in flota if robot['id'] == destino), None)
            
            if ip_destino:
                # Traducimos el Alias de la web al ID real de la placa antes de enviarlo
                hw_id_real = mapa_ips_hw.get(ip_destino, destino)
                sobre["destino"] = hw_id_real
                sock_manet.sendto(json.dumps(sobre).encode('utf-8'), (ip_destino, PUERTO_MANET))
            else:
                print(f"[RADIO] Error TX: Tabla de enrutamiento sin IP registrada para el nodo '{destino}'")
                
    except Exception as e:
        print(f"[RADIO] Error físico en capa de transmisión MANET: {e}")

# =============================================================================
# 4. ENDPOINTS RESTful (Control Táctico y Operaciones de Vuelo)
# =============================================================================

@app.route('/goto', methods=['POST'])
def execute_goto():
    """API para inyección de vectores de aproximación GPS y perímetros de exclusión (Geofencing)"""
    datos = request.json
    gritar_por_radio(datos.get('robot_id'), datos)
    return jsonify({"status": "Misión vectorizada enviada por enlace de radio"})

@app.route('/stop', methods=['POST'])
def emergencia_stop():
    """Kill-Switch: Bloqueo global con redundancia táctica (Broadcast + Unicast dirigido)"""
    # 1. Envío nominal por inundación de red (Broadcast general)
    gritar_por_radio("ALL", "STOP")
    
    # 2. RÁFAGA REDUNDANTE: Aseguramos el tiro perforando muros mediante Unicast directo
    try:
        flota = cargar_flota_disco()
        for robot in flota:
            ip_destino = robot.get('ip')
            if ip_destino:
                sobre_redundante = {
                    "origen": "base_tierra",
                    "destino": robot.get('id'),
                    "tipo": "COMANDO",
                    "payload": "STOP"
                }
                # Forzamos el datagrama UDP directo a su interfaz física
                sock_manet.sendto(json.dumps(sobre_redundante).encode('utf-8'), (ip_destino, PUERTO_MANET))
    except Exception as e:
        print(f"[ALERT] Error en ráfaga de parada unicast: {e}")
        
    return jsonify({"status": "Código de freno global y ráfaga de seguridad emitidos."})

@socketio.on('comando_manual')
def handle_manual_cmd(data):
    """Escucha de túnel WebSocket para teleoperación de bajísima latencia (FPV/Joysticks)"""
    gritar_por_radio(data.get('robot_id'), data.get('payload'))

# =============================================================================
# 5. SISTEMA MULTIMEDIA Y ALMACENAMIENTO PERICIAL
# =============================================================================

@app.route('/upload_media', methods=['POST'])
def upload_media():
    """
    Receptor de evidencias de campo.
    Clasifica y almacena automáticamente fotografías y streams de vídeo procedentes 
    de los UGV en un árbol de directorios estructurado jerárquicamente por ID de dron.
    """
    robot_id = request.form.get('robot_id', 'desconocido')
    media_type = request.form.get('type', 'foto') 
    file = request.files.get('file')

    if file:
        # Creación dinámica de estructura: media/{robot_id}/{fotos|videos}/
        base_dir = os.path.join('media', robot_id, f"{media_type}s")
        os.makedirs(base_dir, exist_ok=True)
        
        extension = 'jpg' if media_type == 'foto' else 'webm'
        filename = f"{media_type}_{int(time.time())}.{extension}"
        file.save(os.path.join(base_dir, filename))
        
    return jsonify({"status": "ok"})

@app.route('/api/gallery/<robot_id>', methods=['GET'])
def get_gallery(robot_id):
    """Escanea el sistema de archivos local y devuelve el inventario multimedia de un dron específico"""
    base_dir = os.path.join('media', robot_id)
    if not os.path.exists(base_dir): 
        return jsonify([])
        
    files = []
    for root, _, filenames in os.walk(base_dir):
        for f in filenames:
            # Construcción de rutas relativas normalizadas para el cliente web
            rel_path = os.path.relpath(os.path.join(root, f), 'media').replace('\\', '/')
            files.append(rel_path)
            
    # Ordenación cronológica inversa para mostrar siempre las evidencias más recientes primero
    files.sort(reverse=True)
    return jsonify(files)

@app.route('/media/<path:filename>')
def serve_media(filename):
    """Servidor de recursos estáticos nativo de Flask para la visualización de la galería pericial"""
    return send_from_directory('media', filename)

# =============================================================================
# 6. CAPA DE PERSISTENCIA (Topología de Red MANET)
# =============================================================================

CONFIG_FILE = 'flota_config.json'

def cargar_flota_disco():
    """Lectura de la configuración estática (IPs y Sensores de nodos) desde memoria no volátil"""
    if os.path.exists(CONFIG_FILE):
        with open(CONFIG_FILE, 'r') as f:
            return json.load(f)
    return []

@app.route('/api/config', methods=['GET'])
def get_config():
    """Retorna la tabla de enrutamiento local a la interfaz web (Dashboard)"""
    return jsonify(cargar_flota_disco())

@app.route('/api/config', methods=['POST'])
def save_config():
    """Sobrescribe la topología de la flota al añadir/eliminar o reconfigurar nodos desde la GCS"""
    with open(CONFIG_FILE, 'w') as f:
        json.dump(request.json, f, indent=4)
    return jsonify({"status": "ok"})

# =============================================================================
# PUNTO DE ENTRADA PRINCIPAL (MAIN WORKER)
# =============================================================================

if __name__ == "__main__":
    print("=====================================================")
    print(" 🌍 ESTACIÓN DE TIERRA ACTIVA - CONTROLADOR TÁCTICO ")
    print("=====================================================")
    
    # 1. Arranque de la pasarela de vídeo como microservicio paralelo.
    # Esto descarga a Flask de la pesada tarea de gestionar el streaming I/O bloqueante por UDP.
    video_gateway.iniciar_gateway()
    
    # 2. Ejecución del servidor web principal y el motor de eventos SocketIO 
    # Mantenemos debug=False y use_reloader=False para garantizar la estabilidad en producción.
    socketio.run(app, host='0.0.0.0', port=5000, debug=False, use_reloader=False)