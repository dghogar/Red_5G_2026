import os
import json
from flask import Flask, request, jsonify

app_ue = Flask(__name__)
PUERTO_ESCUCHA = 9090

def actualizar_hosts_linux(ip, dominio):
    """Lógica TFG: Simula la resolución DNS Edge modificando /etc/hosts"""
    ruta_hosts = '/etc/hosts'
    marcador = "# --- EDGE COMPUTING TFG DNS ---"
    nueva_linea = f"{ip} {dominio} {marcador}\n"
    
    try:
        with open(ruta_hosts, 'r') as f:
            lineas = f.readlines()
            
        # FIX TFG: Solo borramos la línea si ya existe ESE dominio concreto
        lineas_limpias = [l for l in lineas if dominio not in l]
        lineas_limpias.append(nueva_linea)
        
        with open(ruta_hosts, 'w') as f:
            f.writelines(lineas_limpias)
            
        print(f"     [!] ⚡ CACHÉ DNS DEL SISTEMA ACTUALIZADA ⚡")
        print(f"         🌐 Ya puedes abrir en tu navegador: http://{dominio}")
    except PermissionError:
        print(f"     [!] ❌ No hay permisos para escribir en /etc/hosts.")
        print(f"         ⚠️ Por favor, cancela y ejecuta este script con 'sudo'.")
    except Exception as e:
        print(f"     [!] ❌ Error actualizando DNS local: {e}")

@app_ue.route('/notificacion_edge', methods=['POST'])
def recibir_notificacion():
    datos = request.json
    print("\n" + "⭐"*25)
    print("📱 [MÓVIL] ¡MENSAJE PUSH RECIBIDO!")
    
    # === EL CHIVATO: IMPRIME EL JSON PURO QUE MANDA EL PROXY ===
    print("\n--- DATOS EN CRUDO DEL PROXY ---")
    print(json.dumps(datos, indent=2))
    print("--------------------------------\n")
    
    apps = datos.get('aplicaciones', [])
    for app in apps:
        nombre = app.get('appName', 'App')
        desc = app.get('appDescription', '')
        print(f"  👉 {nombre}: {desc}")
        
        reglas_dns = app.get('dns', [])
        if isinstance(reglas_dns, list) and len(reglas_dns) > 0:
            for regla in reglas_dns:
                if isinstance(regla, dict):
                    dominio = regla.get('domainName')
                    ip_destino = regla.get('ipAddress')
                    if dominio and ip_destino:
                        actualizar_hosts_linux(ip_destino, dominio)
                        
    print("⭐"*25 + "\n")
    return jsonify({"status": "Recibido"}), 200

if __name__ == '__main__':
    print(f"📱 Móvil encendido. Esperando Push en el puerto {PUERTO_ESCUCHA}...")
    # Desactivamos el aviso de "Development server" de Flask para que quede más limpio
    import logging
    log = logging.getLogger('werkzeug')
    log.setLevel(logging.ERROR)
    
    app_ue.run(host='0.0.0.0', port=PUERTO_ESCUCHA)
