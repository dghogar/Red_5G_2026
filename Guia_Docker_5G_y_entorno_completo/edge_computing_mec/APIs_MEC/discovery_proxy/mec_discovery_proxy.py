import os
import requests
import time
import threading
from flask import Flask, request, jsonify

proxy_app = Flask(__name__)

# Leemos las URLs desde las variables de entorno de Docker
MEC011_URL = os.getenv("MEC011_URL", "http://mep-server:8080/mec_app_support/v1/registrations")
MEC012_SUBSCRIPTIONS_URL = os.getenv("MEC012_URL", "http://rnis-server:8080/rni/v2/subscriptions")

MI_PUERTO_PROXY = 8085
# URL donde el RNIS nos enviará el webhook (usamos el nombre del contenedor)
MI_URL_PROXY = f"http://discovery-proxy:{MI_PUERTO_PROXY}/webhook_rnis"

@proxy_app.route('/webhook_rnis', methods=['POST'])
def recibir_evento_radio():
    """Recibe la notificación del RNIS cuando un UE se conecta"""
    notificacion = request.json
    
    # Comprobamos que sea del tipo RabEstNotification definido en el estándar
    if notificacion.get("notificationType") == "RabEstNotification":
        print("\n📡 [PROXY] ¡El RNIS detectó un nuevo móvil (RAB Established)!")
        
        ip_movil = None
        # Buscamos la IP del móvil (type 1 es IPv4 en el estándar ETSI)
        for associate_id in notificacion.get("associateId", []):
            if associate_id.get("type") == 1: 
                ip_movil = associate_id.get("value")
                break
        
        if ip_movil:
            print(f"📡 [PROXY] IP del móvil: {ip_movil}")
            threading.Thread(target=enviar_catalogo_al_movil, args=(ip_movil,)).start()
            
    return jsonify({"status": "OK"}), 204

def enviar_catalogo_al_movil(ip_movil):
    """Consulta al MEC011 las Apps y sus reglas DNS, y hace Push al móvil"""
    print("🔍 [PROXY] Consultando aplicaciones y DNS en MEC011...")
    lista_apps = []
    
    try:
        res_apps = requests.get(MEC011_URL)
        if res_apps.status_code == 200 and len(res_apps.json()) > 0:
            datos_mep = res_apps.json()
            
            # Sacamos la URL base limpiando "/registrations" para poder consultar las DNS
            base_url = MEC011_URL.replace("/registrations", "")
            
            for app_registrada in datos_mep:
                perfil = app_registrada.get("appProfile", {})
                app_id = app_registrada.get("appInstanceId")
                
                # --- LÓGICA TFG IDEA 2: Buscamos si esta App tiene reglas DNS ---
                reglas_dns = []
                try:
                    url_dns = f"{base_url}/applications/{app_id}/dns_rules"
                    res_dns = requests.get(url_dns)
                    if res_dns.status_code == 200:
                        reglas_dns = res_dns.json()
                except Exception as e:
                    print(f"⚠️ [PROXY] Error buscando DNS de {app_id}: {e}")
                # ----------------------------------------------------------------
                
                lista_apps.append({
                    "appName": perfil.get("appName", "App Desconocida"),
                    "appDescription": perfil.get("appDescription", ""),
                    "dns": reglas_dns  # <-- Añadimos el bloque DNS al paquete
                })
        else:
            print(f"⚠️ [PROXY] El MEC011 devolvió código {res_apps.status_code} o lista vacía")
    except Exception as e:
        print(f"❌ [PROXY] Error conectando con MEC011: {e}")

    payload_push = {
        "mensaje": "¡Bienvenido a la red Edge!",
        "aplicaciones": lista_apps
    }
    
    url_movil = f"http://{ip_movil}:9090/notificacion_edge" 
    print(f"🚀 [PROXY] Enviando Push (con DNS) a {url_movil}...")
    
    try:
        requests.post(url_movil, json=payload_push, timeout=3)
        print("✅ [PROXY] Push enviado al UE.")
    except Exception as e:
        print(f"❌ [PROXY] Error enviando al UE: {e}")

def suscribirse_al_rnis():
    """Registra la suscripción RabEstSubscription en el MEC012"""
    time.sleep(5) # Esperar a que el contenedor RNIS levante del todo
    print("\n📝 [PROXY] Registrando suscripción en RNIS...")
    payload = {
        "subscriptionType": "RabEstSubscription",
        "callbackReference": MI_URL_PROXY,
        "filterCriteriaQci": {"qci": 9}
    }
    try:
        requests.post(MEC012_SUBSCRIPTIONS_URL, json=payload, headers={'Content-Type': 'application/json'})
        print("✅ [PROXY] Suscrito al RNIS.")
    except Exception as e:
        print(f"❌ [PROXY] Error de suscripción: {e}")

if __name__ == '__main__':
    threading.Thread(target=suscribirse_al_rnis).start()
    proxy_app.run(host='0.0.0.0', port=MI_PUERTO_PROXY)
