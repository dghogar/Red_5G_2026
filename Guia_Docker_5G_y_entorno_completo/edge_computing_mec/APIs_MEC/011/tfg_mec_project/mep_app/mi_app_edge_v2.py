import requests
import json
import signal
import sys
from flask import Flask

# ==========================================
# CONFIGURACIÓN
# ==========================================
MEP_BASE_URL = "http://localhost:8080/mec_app_support/v1"
PUERTO_SERVICIO = 5005
IP_HOST_NUC = "192.168.0.120" 
DOMINIO_DNS = "web2.edge.tfg"

APP_INSTANCE_ID = None
app_web = Flask(__name__)

# ==========================================
# PLANO DE CONTROL: Registro y DNS
# ==========================================
def registrar_en_mep_y_dns():
    global APP_INSTANCE_ID
    print("\n--- INICIANDO PLANO DE CONTROL MEC ---")
    
    payload = {
        "appProfile": {
            "appName": "ServidorWeb-TFG-v2",
            "appProvider": "Mi Universidad",
            "appSoftVersion": "2.0.0",
            "appDescription": f"Servidor interactivo. Dominio: {DOMINIO_DNS} (Puerto {PUERTO_SERVICIO})"
        }
    }
    
    try:
        respuesta = requests.post(f"{MEP_BASE_URL}/registrations", json=payload)
        if respuesta.status_code in [200, 201]:
            APP_INSTANCE_ID = respuesta.json().get('appInstanceId')
            print(f"✅ [MEP] Registro exitoso. ID: {APP_INSTANCE_ID}")
            
            # --- LÓGICA DNS TFG ---
            print(f"[MEP] Inyectando regla DNS para {DOMINIO_DNS}...")
            payload_dns = {
                "dnsRuleId": "DNS-RULE-V2",
                "domainName": DOMINIO_DNS,
                "ipAddressType": "IP_V4",
                "ipAddress": IP_HOST_NUC,
                "state": "ACTIVE"
            }
            url_dns = f"{MEP_BASE_URL}/applications/{APP_INSTANCE_ID}/dns_rules/DNS-RULE-V2"
            res_dns = requests.put(url_dns, json=payload_dns)
            if res_dns.status_code == 200:
                print(f"✅ [MEP] DNS inyectado: {DOMINIO_DNS} -> {IP_HOST_NUC}")
    except Exception as e:
        print(f"❌ [MEP] Error: {e}")

def darse_de_baja_mep(sig, frame):
    global APP_INSTANCE_ID
    if APP_INSTANCE_ID:
        print(f"\n\n[MEP] ⚠️ Apagando servidor... Dando de baja {APP_INSTANCE_ID}...")
        try:
            requests.delete(f"{MEP_BASE_URL}/registrations/{APP_INSTANCE_ID}", timeout=2)
            print("✅ [MEP] Baja completada correctamente.")
        except:
            pass
    sys.exit(0)

signal.signal(signal.SIGINT, darse_de_baja_mep)

# ==========================================
# PLANO DE USUARIO: Web
# ==========================================
@app_web.route('/')
def home():
    return f"""
    <html>
        <body style="font-family: Arial; text-align: center; margin-top: 50px; background-color: #f4f4f9;">
            <div style="background: white; padding: 30px; border-radius: 10px; display: inline-block;">
                <h1 style="color: #0056b3;">¡Bienvenido al Edge v2! 🚀</h1>
                <h2>Conectado vía DNS: {DOMINIO_DNS}</h2>
                <p>Tráfico desviado por el eUPF.</p>
            </div>
        </body>
    </html>
    """

if __name__ == '__main__':
    registrar_en_mep_y_dns()
    print(f"📡 Escuchando tráfico en el puerto {PUERTO_SERVICIO}...")
    app_web.run(host='0.0.0.0', port=PUERTO_SERVICIO, use_reloader=False)
