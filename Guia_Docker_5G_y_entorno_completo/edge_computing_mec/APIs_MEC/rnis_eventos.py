from flask import Flask, request, jsonify
import requests
import threading
import time

app = Flask(__name__)

MEP_URL = "http://localhost:8080/mec_app_support/v1"
UE_PORT = 9090

# MEMORIA: Aquí guardaremos las IPs de los móviles conectados
ues_activos = set()

def procesar_y_enviar_push(ue_ip):
    """Procesa el contexto y lo envía con sistema de reintentos"""
    print(f"\n📢 [RNIS] Procesando contexto Edge para el UE {ue_ip}...")
    
    try:
        # 1. Obtener lista de apps y enriquecer con DNS
        res_apps = requests.get(f"{MEP_URL}/registrations")
        if res_apps.status_code == 200:
            apps_registradas = res_apps.json()
            apps_enriquecidas = []
            
            for app in apps_registradas:
                app_id = app.get("appInstanceId")
                perfil = app.get("appProfile", {})
                
                res_dns = requests.get(f"{MEP_URL}/applications/{app_id}/dns_rules")
                reglas_dns = res_dns.json() if res_dns.status_code == 200 else []
                
                apps_enriquecidas.append({
                    "appName": perfil.get("appName", "App"),
                    "appDescription": perfil.get("appDescription", ""),
                    "dns": reglas_dns
                })
            
            # 2. Enviar el Push al móvil (SISTEMA DE REINTENTOS)
            push_url = f"http://{ue_ip}:{UE_PORT}/notificacion_edge"
            intentos_maximos = 10
            
            for intento in range(1, intentos_maximos + 1):
                try:
                    requests.post(push_url, json={"aplicaciones": apps_enriquecidas}, timeout=3)
                    print(f"✅ [RNIS] Push entregado con éxito a {ue_ip}.")
                    ues_activos.add(ue_ip) # Si tiene éxito, lo guardamos como activo
                    return # Salimos del bucle, éxito rotundo
                except requests.exceptions.RequestException:
                    print(f"⏳ Esperando rutas en {ue_ip}... (Intento {intento}/{intentos_maximos}). Reintento en 5s.")
                    time.sleep(5)
            
            # Si llega aquí, fallaron los 10 intentos
            print(f"❌ [RNIS] Imposible conectar con {ue_ip} tras 10 intentos. Descartando.")
            if ue_ip in ues_activos:
                ues_activos.remove(ue_ip) # Lo borramos por si se ha ido de la red
                
    except Exception as e:
        print(f"❌ Error en el flujo MEC -> UE: {e}")

@app.route('/evento_upf', methods=['POST'])
def recibir_evento_upf():
    """Ruta activada por la Máquina 1 cuando se conecta un móvil"""
    datos = request.json
    ue_ip = datos.get('ue_ip')
    # Lanzamos el hilo para procesar y reintentar sin bloquear al UPF
    threading.Thread(target=procesar_y_enviar_push, args=(ue_ip,)).start()
    return jsonify({"status": "Recibido, procesando en background"}), 200

@app.route('/actualizacion_apps', methods=['POST'])
def actualizacion_global():
    """Ruta activada por el Orquestador cuando una App se enciende/apaga"""
    print(f"\n🔄 [RNIS] ¡Cambio en las Apps detectado! Actualizando a {len(ues_activos)} UEs vivos...")
    for ip in list(ues_activos):
        threading.Thread(target=procesar_y_enviar_push, args=(ip,)).start()
    return jsonify({"status": "Actualización global lanzada"}), 200

if __name__ == '__main__':
    import logging
    log = logging.getLogger('werkzeug')
    log.setLevel(logging.ERROR)
    print("👂 RNIS (Event-Driven) esperando avisos en el puerto 6000...")
    app.run(host='0.0.0.0', port=6000)
