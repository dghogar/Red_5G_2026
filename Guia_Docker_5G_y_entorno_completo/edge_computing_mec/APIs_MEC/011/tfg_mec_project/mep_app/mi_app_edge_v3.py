import requests
import threading
import signal
import sys
from flask import Flask

# ==========================================
# CONFIGURACIÓN
# ==========================================
MEP_BASE_URL = "http://localhost:8080/mec_app_support/v1"
PUERTO_SERVICIO = 5050
IP_HOST_NUC = "192.168.0.120"
DOMINIO_DNS = "video3.edge.tfg"

APP_INSTANCE_ID = None
app_web = Flask(__name__)

# ==========================================
# PLANO DE CONTROL (Registro y DNS)
# ==========================================
def registrar_en_mep_y_dns():
    global APP_INSTANCE_ID
    print("\n[MEC 011] Registrando Aplicación v3...")
    
    payload = {
        "appProfile": {
            "appName": "VideoApp-Inteligente",
            "appProvider": "Universidad",
            "appSoftVersion": "3.0.0",
            "appDescription": f"Video Adaptativo. Dominio: {DOMINIO_DNS} (Puerto {PUERTO_SERVICIO})"
        }
    }
    try:
        res = requests.post(f"{MEP_BASE_URL}/registrations", json=payload)
        if res.status_code in [200, 201]:
            APP_INSTANCE_ID = res.json().get('appInstanceId')
            print(f"✅ [MEC 011] Registro exitoso. ID: {APP_INSTANCE_ID}")
            
            # --- LÓGICA DNS TFG ---
            print(f"[MEC 011] Inyectando regla DNS para {DOMINIO_DNS}...")
            payload_dns = {
                "dnsRuleId": "DNS-RULE-V3",
                "domainName": DOMINIO_DNS,
                "ipAddressType": "IP_V4",
                "ipAddress": IP_HOST_NUC,
                "state": "ACTIVE"
            }
            url_dns = f"{MEP_BASE_URL}/applications/{APP_INSTANCE_ID}/dns_rules/DNS-RULE-V3"
            res_dns = requests.put(url_dns, json=payload_dns)
            if res_dns.status_code == 200:
                print(f"✅ [MEC 011] DNS inyectado: {DOMINIO_DNS} -> {IP_HOST_NUC}")
    except Exception as e:
        print(f"❌ [MEC 011] Error en registro: {e}")

def darse_de_baja_mep(sig, frame):
    global APP_INSTANCE_ID
    if APP_INSTANCE_ID:
        print(f"\n\n[MEC 011] ⚠️ Apagando servidor... Dando de baja {APP_INSTANCE_ID}...")
        try:
            requests.delete(f"{MEP_BASE_URL}/registrations/{APP_INSTANCE_ID}", timeout=2)
            print("✅ [MEC 011] Baja completada.")
        except:
            pass
    sys.exit(0)

signal.signal(signal.SIGINT, darse_de_baja_mep)

# ==========================================
# PLANO DE USUARIO (La web)
# ==========================================
@app_web.route('/')
def home():
    return f"""
    <html>
        <body style="font-family: sans-serif; background: #d4edda; text-align: center; padding: 50px;">
            <div style="background: white; padding: 40px; border-radius: 15px; display: inline-block;">
                <h1>🟢 4K ULTRA HD ACTIVADO</h1>
                <p>Estás usando el dominio: <b>{DOMINIO_DNS}</b></p>
                <p><small>Conectado a Edge computing v3</small></p>
            </div>
        </body>
    </html>
    """

if __name__ == '__main__':
    threading.Thread(target=registrar_en_mep_y_dns).start()
    print(f"\n🚀 Levantando Aplicación Inteligente en el puerto {PUERTO_SERVICIO}...")
    app_web.run(host='0.0.0.0', port=PUERTO_SERVICIO, use_reloader=False)
