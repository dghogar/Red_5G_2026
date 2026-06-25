import subprocess
import requests
import re

# CONFIGURACIÓN
CONTENEDOR_UPF = "eupf-edge" # El nombre que sale en tus logs
MEC_PROXY_URL = "http://192.168.0.120:6000/evento_upf" # Pon aquí la IP de tu NUC

def vigilar_logs():
    print(f"👀 Enganchado a los logs de {CONTENEDOR_UPF}... Esperando conexiones.")
    
    # Ejecutamos 'docker logs -f' para leer en tiempo real (follow)
    proceso = subprocess.Popen(
        ["docker", "logs", "-f", CONTENEDOR_UPF],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True
    )

    # Leemos línea a línea según van saliendo
    for linea in iter(proceso.stdout.readline, ''):
        if "UE IPv4 Address:" in linea:
            # Extraemos la IP limpiando espacios
            # Ejemplo de línea: "eupf-edge |     UE IPv4 Address: 10.47.0.8"
            ip_ue = linea.split("UE IPv4 Address:")[1].strip()
            print(f"✨ ¡Nueva sesión detectada en los logs! IP: {ip_ue}")
            
            # Avisamos a la Máquina 2 instantáneamente
            try:
                requests.post(MEC_PROXY_URL, json={"ue_ip": ip_ue}, timeout=5)
                print(f"   -> 📡 Webhook enviado a Máquina 2 con éxito.")
            except Exception as e:
                print(f"   -> ❌ Fallo al avisar a Máquina 2: {e}")

if __name__ == "__main__":
    vigilar_logs()
