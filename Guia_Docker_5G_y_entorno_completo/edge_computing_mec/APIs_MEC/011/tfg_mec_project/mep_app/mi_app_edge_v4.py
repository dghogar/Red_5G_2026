import requests
import signal
import sys
import threading
from flask import Flask

# ==========================================
# CONFIGURACIÓN
# ==========================================
MEP_BASE_URL = "http://localhost:8080/mec_app_support/v1"
PUERTO_SERVICIO = 5000
IP_HOST_NUC = "192.168.0.120" 
DOMINIO_DNS = "video.edge.tfg"  # <-- ¡EL DOMINIO MÁGICO!

APP_INSTANCE_ID = None
app_web = Flask(__name__)

# ==========================================
# PLANO DE CONTROL (Registro y DNS)
# ==========================================
def registrar_en_mep_y_dns():
    global APP_INSTANCE_ID
    print("\n[MEC 011] Registrando Aplicación en el Edge...")
    
    # 1. Registro Normal
    payload_app = {
        "appProfile": {
            "appName": "VideoApp-DNS",
            "appProvider": "Universidad",
            "appSoftVersion": "4.0.0",
            "appDescription": f"Servicio con DNS inyectada. Dominio: {DOMINIO_DNS}"
        }
    }
    
    try:
        # Petición POST para registrarnos
        res = requests.post(f"{MEP_BASE_URL}/registrations", json=payload_app)
        if res.status_code in [200, 201]:
            APP_INSTANCE_ID = res.json().get('appInstanceId')
            print(f"✅ [MEC 011] Registro exitoso. ID: {APP_INSTANCE_ID}")
            
            # 2. INYECCIÓN DE REGLA DNS (El PUT que hemos programado en la Fase 1)
            print(f"[MEC 011] Inyectando regla DNS para {DOMINIO_DNS}...")
            regla_id = "DNS-RULE-01"
            payload_dns = {
                "dnsRuleId": regla_id,
                "domainName": DOMINIO_DNS,
                "ipAddressType": "IP_V4",
                "ipAddress": IP_HOST_NUC, # Redirige el dominio a la IP de tu NUC
                "state": "ACTIVE"
            }
            url_dns = f"{MEP_BASE_URL}/applications/{APP_INSTANCE_ID}/dns_rules/{regla_id}"
            
            res_dns = requests.put(url_dns, json=payload_dns)
            if res_dns.status_code == 200:
                print(f"✅ [MEC 011] DNS inyectado en el servidor Edge: {DOMINIO_DNS} -> {IP_HOST_NUC}")
                
    except Exception as e:
        print(f"❌ Error conectando al MEP: {e}")

def darse_de_baja(sig, frame):
    """Limpieza automática al pulsar Ctrl+C"""
    global APP_INSTANCE_ID
    if APP_INSTANCE_ID:
        print(f"\n[MEC 011] Borrando app y sus reglas DNS ({APP_INSTANCE_ID})...")
        try:
            requests.delete(f"{MEP_BASE_URL}/registrations/{APP_INSTANCE_ID}", timeout=2)
            print("✅ [MEC 011] Limpieza completada.")
        except:
            pass
    sys.exit(0)

signal.signal(signal.SIGINT, darse_de_baja)

# ==========================================
# PLANO DE USUARIO (La web)
# ==========================================
@app_web.route('/')
def home():
    html = f"""
    <html>
        <body style="font-family: sans-serif; text-align: center; padding: 50px; background-color: #e3f2fd;">
            <div style="background: white; padding: 40px; border-radius: 15px; display: inline-block;">
                <h1 style="color: #1976d2;">🌐 Estás conectado a {DOMINIO_DNS}</h1>
                <p>Resolución Edge DNS funcionando a la perfección.</p>
                <p>IP real del servidor: {IP_HOST_NUC}:{PUERTO_SERVICIO}</p>
            </div>
        </body>
    </html>
    """
    return html

if __name__ == '__main__':
    # Lanzamos el registro en un hilo para no bloquear el servidor web
    threading.Thread(target=registrar_en_mep_y_dns).start()
    print(f"\n🚀 Servidor web escuchando en puerto {PUERTO_SERVICIO}...")
    app_web.run(host='0.0.0.0', port=PUERTO_SERVICIO, use_reloader=False)
