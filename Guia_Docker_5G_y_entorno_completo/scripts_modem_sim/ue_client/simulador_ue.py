import os
import json
from flask import Flask, request, jsonify

app_ue = Flask(__name__)
PUERTO_ESCUCHA = 9090

def sincronizar_hosts_linux(lista_dns):
    """
    Lógica TFG (Mejorada): Sincroniza la caché DNS local con la 'foto actual' de la red.
    Borra las apps muertas y añade solo las que están vivas.
    """
    ruta_hosts = '/etc/hosts'
    marcador = "# --- EDGE COMPUTING TFG DNS ---"
    
    try:
        with open(ruta_hosts, 'r') as f:
            lineas = f.readlines()
            
        # 1. PURGA: Borramos TODAS las líneas que tengan nuestro marcador (limpiamos el rastro antiguo)
        lineas_limpias = [l for l in lineas if marcador not in l]
        
        # 2. INYECCIÓN: Añadimos solo las DNS de las apps que están reportadas como VIVAS
        if lista_dns:
            for ip, dominio in lista_dns:
                lineas_limpias.append(f"{ip} {dominio} {marcador}\n")
                
        # 3. ESCRITURA FINAL
        with open(ruta_hosts, 'w') as f:
            f.writelines(lineas_limpias)
            
        if lista_dns:
            print(f"     [!] ⚡ CACHÉ DNS SINCRONIZADA: {len(lista_dns)} dominios Edge activos ⚡")
        else:
            print(f"     [!] 🧹 CACHÉ DNS LIMPIA: No hay servicios Edge con DNS en este momento.")
            
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
    
    apps = datos.get('aplicaciones', [])
    lista_dns_vivas = [] # Aquí guardaremos todas las DNS válidas de esta notificación
    
    print("\n 🚀 Servicios disponibles cerca de ti:")
    if not apps:
        print("  📭 La red informa que no hay servicios activos.")
    else:
        for app in apps:
            nombre = app.get('appName', 'App')
            desc = app.get('appDescription', '')
            print(f"  👉 {nombre}: {desc}")
            
            # Recopilamos las DNS de esta app
            reglas_dns = app.get('dns', [])
            if isinstance(reglas_dns, list) and len(reglas_dns) > 0:
                for regla in reglas_dns:
                    if isinstance(regla, dict):
                        dominio = regla.get('domainName')
                        ip_destino = regla.get('ipAddress')
                        if dominio and ip_destino:
                            lista_dns_vivas.append((ip_destino, dominio))
                            print(f"         🌐 Disponible en navegador: http://{dominio}")

    # Al terminar de procesar todas las apps, sincronizamos el archivo hosts de golpe
    sincronizar_hosts_linux(lista_dns_vivas)
                        
    print("⭐"*25 + "\n")
    return jsonify({"status": "Sincronizado"}), 200

if __name__ == '__main__':
    print(f"📱 Móvil encendido. Esperando Push en el puerto {PUERTO_ESCUCHA}...")
    import logging
    log = logging.getLogger('werkzeug')
    log.setLevel(logging.ERROR)
    
    app_ue.run(host='0.0.0.0', port=PUERTO_ESCUCHA)
