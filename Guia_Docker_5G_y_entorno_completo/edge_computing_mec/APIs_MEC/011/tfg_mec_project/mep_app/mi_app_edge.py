import requests
import json
import time
import signal
import sys

# ¡AQUÍ ESTÁ LA CLAVE! Usamos la ruta base /v1 que descubrimos
MEP_URL = "http://localhost:8080/mec_app_support/v1/registrations"

# Variable global para guardar el ID asignado
APP_INSTANCE_ID = None

print("--- INICIANDO APLICACIÓN EDGE (MEC APP) ---")

# Datos de la aplicación que se va a registrar en el Edge
payload = {
    "appProfile": {
        "appName": "VideoServer-TFG",
        "appProvider": "Mi Universidad",
        "appSoftVersion": "1.0.0",
        "appDescription": "Servidor de video de ultra-baja latencia"
    }
}

headers = {'Content-Type': 'application/json'}

def darse_de_baja_mep(sig, frame):
    """Atrapa el Ctrl+C y borra la aplicación del Edge"""
    global APP_INSTANCE_ID
    if APP_INSTANCE_ID:
        print(f"\n\n[MEC APP] ⚠️ Apagando script... Eliminando app {APP_INSTANCE_ID} del catálogo...")
        try:
            url_delete = f"{MEP_URL}/{APP_INSTANCE_ID}"
            res = requests.delete(url_delete, timeout=2)
            if res.status_code in [200, 204]:
                print("✅ [MEC APP] Baja completada. Catálogo limpio.")
        except Exception as e:
            print(f"❌ [MEC APP] Error al intentar darse de baja: {e}")
    else:
        print("\n[MEC APP] Apagando script (No había ID registrado).")
    
    sys.exit(0)

# Asignamos el manejador de la señal SIGINT (Ctrl+C)
signal.signal(signal.SIGINT, darse_de_baja_mep)

print(f"Contactando con el MEP en: {MEP_URL}")
try:
    respuesta = requests.post(MEP_URL, headers=headers, json=payload)
    
    if respuesta.status_code in [200, 201]:
        # Guardamos el ID que nos devuelve el servidor para poder borrarlo después
        APP_INSTANCE_ID = respuesta.json().get('appInstanceId')
        print("✅ ¡Registro en el Edge exitoso!")
        print("Respuesta del MEP:", json.dumps(respuesta.json(), indent=2))
    else:
        print(f"❌ Fallo en el registro. Código HTTP: {respuesta.status_code}")
        print(f"Detalle del error: {respuesta.text}")
        
except Exception as e:
    print(f"❌ Error de conexión: {e}")

# Mantener vivo el script
print("\nPresiona Ctrl+C para salir y eliminar la aplicación del servidor Edge.")
while True:
    time.sleep(10)
