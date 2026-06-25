import requests
import time
import os

MEP_URL = "http://localhost:8080/mec_app_support/v1"
NGINX_CONF_PATH = "nginx.conf"

# --- PLANTILLA BASE DE NGINX (AHORA CON EL ESCUDO CATCH-ALL) ---
NGINX_TEMPLATE_START = """
events {}
http {
    # -------------------------------------------------------------
    # BLOQUE CATCH-ALL (CONTROL DE ERRORES MEC)
    # Atrapa peticiones a dominios de apps que ya han sido apagadas
    # para que NGINX no muestre por error la web de otra app.
    # -------------------------------------------------------------
    server {
        listen 80 default_server;
        server_name _;
        
        default_type text/html;
        return 404 "<!DOCTYPE html><html><head><meta charset='utf-8'><title>MEC Edge Error</title></head><body style='font-family:sans-serif; text-align:center; padding-top:50px; color:#333;'><h2>❌ ERROR 404: SERVICIO EDGE APAGADO</h2><p>La aplicación solicitada ha sido detenida o el dominio no existe en el catálogo actual.</p></body></html>";
    }
"""
NGINX_TEMPLATE_END = "}\n"

def generar_bloque_server(dominio, ip, puerto):
    return f"""
    server {{
        listen 80;
        server_name {dominio};
        location / {{
            proxy_pass http://{ip}:{puerto};
            proxy_set_header Host $host;
            proxy_set_header X-Real-IP $remote_addr;
        }}
    }}
"""

def obtener_configuracion_actual():
    if os.path.exists(NGINX_CONF_PATH):
        with open(NGINX_CONF_PATH, "r") as f:
            return f.read()
    return ""

def orquestar_nginx():
    print("🤖 [ORQUESTADOR] Iniciando sincronización dinámica MEC -> NGINX (Con Catch-All)...")
    
    while True:
        try:
            # 1. Preguntamos al MEC qué apps hay registradas
            res_apps = requests.get(f"{MEP_URL}/registrations")
            if res_apps.status_code == 200:
                apps = res_apps.json()
                nueva_conf = NGINX_TEMPLATE_START
                
                # 2. Por cada app, buscamos sus reglas DNS
                for app in apps:
                    app_id = app.get("appInstanceId")
                    desc = app.get("appProfile", {}).get("appDescription", "")
                    
                    # Extraemos el puerto de la descripción
                    puerto = "80" # Por defecto
                    if "Puerto " in desc:
                        # Extrae el número después de la palabra "Puerto " (ej: "Puerto 5050)")
                        puerto = desc.split("Puerto ")[1].split(")")[0]
                    elif "5000" in desc or app.get("appProfile", {}).get("appName") == "VideoApp-DNS":
                        puerto = "5000" # Caso especial
                    
                    res_dns = requests.get(f"{MEP_URL}/applications/{app_id}/dns_rules")
                    if res_dns.status_code == 200:
                        reglas = res_dns.json()
                        for regla in reglas:
                            dominio = regla.get("domainName")
                            ip = regla.get("ipAddress")
                            if dominio and ip:
                                nueva_conf += generar_bloque_server(dominio, ip, puerto)
                
                nueva_conf += NGINX_TEMPLATE_END
                
                # 3. Si la configuración ha cambiado, reescribimos y recargamos NGINX
                if nueva_conf != obtener_configuracion_actual():
                    print("\n🔄 [ORQUESTADOR] ¡Cambio detectado en el MEC! Actualizando NGINX...")
                    with open(NGINX_CONF_PATH, "w") as f:
                        f.write(nueva_conf)
                    
                    # Recargamos NGINX en caliente dentro del contenedor Docker
                    os.system("docker exec nginx-edge-proxy nginx -s reload")
                    print("✅ [ORQUESTADOR] Enrutamiento actualizado con éxito.")
                    
                    # NUEVO: Avisamos al RNIS para que actualice a los móviles conectados
                    try:
                        requests.post("http://localhost:6000/actualizacion_apps", timeout=2)
                        print("📡 [ORQUESTADOR] Aviso enviado al RNIS para Broadcast.")
                    except requests.exceptions.RequestException:
                        pass # Ignoramos si el RNIS está apagado o no responde
                
        except Exception as e:
            pass # Ignoramos errores temporales de conexión
            
        # Esperamos 5 segundos antes de volver a comprobar
        time.sleep(5)

if __name__ == '__main__':
    orquestar_nginx()
