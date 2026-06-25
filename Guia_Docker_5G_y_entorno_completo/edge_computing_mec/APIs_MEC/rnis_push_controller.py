# Archivo: rnis_push_controller.py
from flask import Flask, request, jsonify
import requests
import threading

app = Flask(__name__)

MEP_URL = "http://localhost:8080/mec_app_support/v1"
UE_PORT = 9090

def consultar_mep_y_enviar_push(ue_ip):
    """Consulta las apps activas en MEC011 y se las envía a la UE"""
    print(f"\n📢 [MEC012] Procesando contexto Edge para la nueva UE ({ue_ip})...")
    
    try:
        # 1. Obtener lista de apps actualmente registradas
        res_apps = requests.get(f"{MEP_URL}/registrations")
        if res_apps.status_code == 200:
            apps_registradas = res_apps.json()
            apps_enriquecidas = []
            
            # 2. Por cada app, buscar sus reglas DNS
            for app in apps_registradas:
                app_id = app.get("appInstanceId")
                perfil = app.get("appProfile", {})
                nombre = perfil.get("appName", "App")
                desc = perfil.get("appDescription", "")
                
                res_dns = requests.get(f"{MEP_URL}/applications/{app_id}/dns_rules")
                reglas_dns = res_dns.json() if res_dns.status_code == 200 else []
                
                # Solo la añadimos si tiene reglas DNS válidas
                if reglas_dns:
                    apps_enriquecidas.append({
                        "appName": nombre,
                        "appDescription": desc,
                        "dns": reglas_dns
                    })
            
            # 3. Enviar el Push al móvil
            if apps_enriquecidas:
                print(f"🚀 [MEC012] Enviando PUSH DNS a {ue_ip}...")
                push_url = f"http://{ue_ip}:{UE_PORT}/notificacion_edge"
                requests.post(push_url, json={"aplicaciones": apps_enriquecidas}, timeout=3)
                print(f"✅ [MEC012] Catálogo entregado con éxito a {ue_ip}.")
            else:
                print(f"⚠️ [MEC012] No hay apps Edge activas para enviar a {ue_ip}.")
                
    except Exception as e:
        print(f"❌ Error en el flujo MEC -> UE: {e}")

@app.route('/webhook_rnis', methods=['POST'])
def recibir_evento_mec012():
    """Endpoint (Webhook) que recibe la notificación RabEst del MEC012"""
    datos = request.json
    ue_ip = None
    
    # Extraemos la IP según el estándar ETSI MEC012 (AssociateId type 1 = IPv4)
    if "associateId" in datos:
        for assoc in datos["associateId"]:
            if assoc.get("type") == 1:
                ue_ip = assoc.get("value")
                break
                
    if ue_ip:
        print(f"\n📡 ¡Evento de Radio Detectado! UE conectada con IP: {ue_ip}")
        # Lo lanzamos en un hilo para responder con 200 OK inmediatamente al MEC012
        hilo = threading.Thread(target=consultar_mep_y_enviar_push, args=(ue_ip,))
        hilo.start()
        return jsonify({"status": "Notificación recibida, procesando"}), 200
    else:
        print("⚠️ Evento de radio recibido, pero no contenía IP (AssociateId).")
        return jsonify({"error": "Falta ueIpv4Address"}), 400

if __name__ == '__main__':
    import logging
    log = logging.getLogger('werkzeug')
    log.setLevel(logging.ERROR)
    
    print("👂 Escuchando notificaciones RNIS (MEC012) en el puerto 6000...")
    app.run(host='0.0.0.0', port=6000)
