import threading
import time
import requests
import json
from scapy.all import sniff, IP
from flask import Flask, jsonify, render_template_string

# --- CONFIGURACIÓN ---
MEP_URL = "http://localhost:8080/mec_app_support/v1"
UE_PORT = 9090
SUBRED_UE = "10.47.0."  
INTERFAZ_RED = "any" # Lo dejamos en any por la prueba del tcpdump

# MEMORIA MEC012
contexto_rnis = {
    "associateIds": [],
    "ultimos_logs": []
}

# --- SERVIDOR WEB DASHBOARD (FLASK) ---
app = Flask(__name__)

# Plantilla HTML inyectada directamente para no tener que crear más archivos
HTML_TEMPLATE = """
<!DOCTYPE html>
<html>
<head>
    <title>MEC012 - Radio Network Info</title>
    <style>
        body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background-color: #121212; color: #e0e0e0; padding: 30px; }
        h1 { color: #4CAF50; border-bottom: 2px solid #333; padding-bottom: 10px; }
        h2 { color: #bbb; margin-top: 30px; }
        table { width: 100%; border-collapse: collapse; margin-top: 15px; background-color: #1e1e1e; }
        th, td { border: 1px solid #333; padding: 12px; text-align: left; }
        th { background-color: #2c2c2c; color: #4CAF50; }
        tr:nth-child(even) { background-color: #181818; }
        pre { background: #000; padding: 15px; border-radius: 5px; color: #0f0; overflow-x: auto; border: 1px solid #333;}
        .log-list { background: #1e1e1e; padding: 15px; border-radius: 5px; list-style-type: none; border: 1px solid #333;}
        .log-list li { margin-bottom: 8px; font-family: monospace; }
        .timestamp { color: #888; font-size: 0.9em; margin-right: 10px; }
    </style>
    <meta http-equiv="refresh" content="3"> </head>
<body>
    <h1>📻 MEC012 - Panel de Observabilidad (Edge Node)</h1>
    <p>Monitorización pasiva de tráfico L3 en la frontera N6.</p>

    <h2>📱 Equipos de Usuario Detectados (AssociateIds)</h2>
    <table>
        <tr><th>ETSI Type</th><th>Value (IPv4)</th><th>Estado de Inyección</th></tr>
        {% for ue in contexto["associateIds"] %}
        <tr>
            <td>Type {{ ue.type }}</td>
            <td><strong>{{ ue.value }}</strong></td>
            <td style="color: #4CAF50;">✅ Contexto Entregado</td>
        </tr>
        {% else %}
        <tr><td colspan="3" style="text-align: center; color: #888;">No se han detectado UEs en la red. Esperando tráfico...</td></tr>
        {% endfor %}
    </table>

    <h2>📝 Últimos Eventos de Red</h2>
    <ul class="log-list">
        {% for log in logs[-10:]|reverse %}
        <li><span class="timestamp">{{ log.time }}</span> {{ log.msg }}</li>
        {% else %}
        <li>Sin eventos recientes.</li>
        {% endfor %}
    </ul>

    <h2>⚙️ Memoria JSON Cruda</h2>
    <pre>{{ json_crudo }}</pre>
</body>
</html>
"""

def agregar_log(mensaje):
    hora = time.strftime("%H:%M:%S")
    contexto_rnis["ultimos_logs"].append({"time": hora, "msg": mensaje})
    print(f"[{hora}] {mensaje}")

@app.route('/')
def dashboard():
    # Renderizamos el HTML pasándole nuestras variables de memoria
    json_formateado = json.dumps(contexto_rnis["associateIds"], indent=4)
    return render_template_string(HTML_TEMPLATE, contexto=contexto_rnis, logs=contexto_rnis["ultimos_logs"], json_crudo=json_formateado)

@app.route('/api/estado')
def api_estado():
    # Por si quieres consultarlo desde otro script en formato JSON puro
    return jsonify(contexto_rnis)

def iniciar_dashboard():
    # Quitamos los logs molestos de Flask en la terminal
    import logging
    log = logging.getLogger('werkzeug')
    log.setLevel(logging.ERROR)
    app.run(host='0.0.0.0', port=5000)

# --- LÓGICA CORE MEC012 ---

def crear_associate_id(ip_address):
    return {"type": 1, "value": ip_address}

def ip_ya_registrada(ip_address):
    return any(assoc["value"] == ip_address for assoc in contexto_rnis["associateIds"])

def inyectar_contexto_edge(associate_id):
    ue_ip = associate_id["value"]
    agregar_log(f"Iniciando flujo de inyección para {ue_ip}...")
    
    try:
        res_apps = requests.get(f"{MEP_URL}/registrations")
        if res_apps.status_code != 200:
            agregar_log(f"⚠️ Error al consultar MEC011.")
            return
            
        apps_registradas = res_apps.json()
        catalogo_edge = [{"appName": app.get("appProfile", {}).get("appName", "App"), 
                          "dns": requests.get(f"{MEP_URL}/applications/{app.get('appInstanceId')}/dns_rules").json() if requests.get(f"{MEP_URL}/applications/{app.get('appInstanceId')}/dns_rules").status_code == 200 else []} 
                         for app in apps_registradas]
            
        push_url = f"http://{ue_ip}:{UE_PORT}/notificacion_edge"
        
        for intento in range(1, 11): 
            try:
                requests.post(push_url, json={"aplicaciones": catalogo_edge}, timeout=3)
                agregar_log(f"✅ Contexto inyectado con éxito en {ue_ip}.")
                return 
            except requests.exceptions.RequestException:
                time.sleep(5)
                
        agregar_log(f"❌ Timeout al conectar con {ue_ip}. Borrando del registro.")
        contexto_rnis["associateIds"] = [a for a in contexto_rnis["associateIds"] if a["value"] != ue_ip]
                
    except Exception as e:
        agregar_log(f"❌ Error interno: {e}")

def procesar_paquete(paquete):
    if IP in paquete:
        ip_origen = paquete[IP].src
        if ip_origen.startswith(SUBRED_UE) and not ip_ya_registrada(ip_origen):
            nuevo_assoc_id = crear_associate_id(ip_origen)
            contexto_rnis["associateIds"].append(nuevo_assoc_id)
            agregar_log(f"✨ Nueva UE detectada. AssociateId creado para {ip_origen}")
            threading.Thread(target=inyectar_contexto_edge, args=(nuevo_assoc_id,)).start()

if __name__ == '__main__':
    # 1. Levantamos el servidor web en un hilo secundario
    threading.Thread(target=iniciar_dashboard, daemon=True).start()
    
    print("="*60)
    print("📻 MEC012 RNIS (Sniffer Autónomo + Dashboard Web)")
    print("🌐 Dashboard disponible en: http://localhost:5000")
    print(f"🔍 Sniffer L3 escuchando tráfico de la subred {SUBRED_UE}*")
    print("="*60)
    
    # 2. Nos quedamos escuchando la red en el hilo principal
    sniff(prn=procesar_paquete, store=0, iface=INTERFAZ_RED)
