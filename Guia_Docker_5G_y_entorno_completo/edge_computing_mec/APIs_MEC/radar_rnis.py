import time
import requests
import socket

# Configuración
PROXY_URL = "http://localhost:8085/webhook_rnis"
RANGO_IPS = [f"10.47.0.{i}" for i in range(2, 20)] 
PUERTO_APP_MOVIL = 9090

# ¿Cada cuántos segundos queremos actualizar la pantalla del móvil?
INTERVALO_ACTUALIZACION = 10 

def detectar_movil_por_puerto(ip):
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(0.5)
        resultado = sock.connect_ex((ip, PUERTO_APP_MOVIL))
        sock.close()
        return resultado == 0
    except Exception:
        return False

def disparar_notificacion_edge(ip_movil):
    payload = {
        "notificationType": "RabEstNotification",
        "associateId": [{"type": 1, "value": ip_movil}]
    }
    try:
        requests.post(PROXY_URL, json=payload, timeout=2)
    except Exception as e:
        print(f"❌ Error al contactar con el Proxy para {ip_movil}: {e}")

print(f"📡 Iniciando Radar RNIS (Modo de Sincronización Continua cada {INTERVALO_ACTUALIZACION}s)...")

while True:
    moviles_conectados = []
    
    # 1. Escaneamos qué móviles están activos ahora mismo
    for ip in RANGO_IPS:
        if detectar_movil_por_puerto(ip):
            moviles_conectados.append(ip)
            
    if moviles_conectados:
        print(f"\n✅ Encontrados {len(moviles_conectados)} móviles activos.")
        # 2. Enviamos el Push a todos los que estén activos para refrescar sus pantallas
        for ip in moviles_conectados:
            print(f"🔄 Refrescando catálogo de servicios Edge en el móvil {ip}...")
            disparar_notificacion_edge(ip)
    else:
        print("\rBuscando móviles en la red 5G... (Ninguno activo)      ", end="", flush=True)

    # 3. Esperamos antes de la siguiente ronda
    time.sleep(INTERVALO_ACTUALIZACION)
