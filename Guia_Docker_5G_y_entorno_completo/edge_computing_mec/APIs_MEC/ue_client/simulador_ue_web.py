import os
import time
from flask import Flask, request, jsonify, render_template_string
import logging

app_ue = Flask(__name__)
PUERTO_ESCUCHA = 9090

# Memoria local para el Dashboard
estado_ue = {
    "aplicaciones": [],
    "ultima_act": "Esperando conexión inicial..."
}

# Marcador único para identificar las líneas de nuestro TFG
MARCADOR_MEC = "# --- EDGE COMPUTING TFG ---"

def sincronizar_hosts_blindado(lista_dns):
    """
    Sincroniza el archivo /etc/hosts eliminando entradas antiguas 
    y añadiendo solo las actuales.
    """
    try:
        # 1. Leer el archivo original
        if not os.path.exists('/etc/hosts'):
            print("❌ El archivo /etc/hosts no existe.")
            return

        with open('/etc/hosts', 'r') as f:
            lineas = f.readlines()
        
        # 2. LIMPIEZA TOTAL: Filtramos cualquier línea que contenga nuestro marcador
        # Esto asegura que si una app se da de baja, su entrada desaparezca.
        lineas_limpias = [l for l in lineas if MARCADOR_MEC not in l]
        
        # 3. ADICIÓN: Si la lista recibida tiene apps, las inyectamos
        if lista_dns:
            print(f"💉 Inyectando {len(lista_dns)} reglas DNS en /etc/hosts...")
            for ip, dom in lista_dns:
                # Formato: IP <tab> DOMINIO <espacio> MARCADOR
                nueva_linea = f"{ip}\t{dom}\t{MARCADOR_MEC}\n"
                lineas_limpias.append(nueva_linea)
        else:
            print("🧹 Lista vacía recibida. Limpiando todas las entradas MEC del archivo hosts.")

        # 4. ESCRITURA: Sobrescribimos con el nuevo contenido
        with open('/etc/hosts', 'w') as f:
            f.writelines(lineas_limpias)
            
        print("✅ Archivo /etc/hosts sincronizado con éxito.")
        
    except PermissionError:
        print("❌ ERROR DE PERMISOS: Debes ejecutar este script con 'sudo'.")
    except Exception as e:
        print(f"❌ Error inesperado al sincronizar hosts: {e}")

# --- DASHBOARD WEB ---
HTML_TEMPLATE = """
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <title>💻 Portátil Edge Hub v2</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: 'Segoe UI', sans-serif; background: #f4f7f6; padding: 20px; color: #333; }
        .container { max-width: 700px; margin: auto; background: white; border-radius: 12px; padding: 25px; box-shadow: 0 10px 25px rgba(0,0,0,0.1); }
        .header { display: flex; justify-content: space-between; align-items: center; border-bottom: 3px solid #eee; padding-bottom: 15px; margin-bottom: 20px; }
        .status-badge { background: #28a745; color: white; padding: 5px 12px; border-radius: 20px; font-size: 0.85em; font-weight: bold; }
        .app-card { border: 1px solid #e0e0e0; border-radius: 10px; padding: 18px; margin-bottom: 15px; background: #fdfdfd; transition: transform 0.2s; }
        .app-card:hover { transform: translateY(-3px); box-shadow: 0 5px 15px rgba(0,0,0,0.05); }
        .app-title { font-size: 1.25em; font-weight: bold; color: #0056b3; margin-bottom: 10px; display: flex; align-items: center; }
        .app-link { display: inline-block; padding: 10px 20px; background: #007bff; color: white; text-decoration: none; border-radius: 6px; font-weight: 500; }
        .app-link:hover { background: #004a99; }
        .footer-info { font-size: 0.85em; color: #777; margin-top: 20px; border-top: 1px solid #eee; pt: 10px; }
    </style>
    <meta http-equiv="refresh" content="3">
</head>
<body>
    <div class="container">
        <div class="header">
            <h2>💻 Nodo UE Portátil</h2>
            <div class="status-badge">📶 Red 5G Activa</div>
        </div>
        
        <p><strong>Última actualización:</strong> {{ estado.ultima_act }}</p>

        {% if not estado.aplicaciones %}
            <div style="text-align:center; color:#999; padding:40px; border: 2px dashed #eee; border-radius: 10px;">
                <p>No hay servicios Edge disponibles en esta zona.</p>
                <small>Esperando PUSH del servicio RNIS (MEC012)...</small>
            </div>
        {% else %}
            {% for app in estado.aplicaciones %}
                <div class="app-card">
                    <div class="app-title">🟢 {{ app.appName }}</div>
                    {% for regla in app.dns %}
                        <a class="app-link" href="http://{{ regla.domainName }}/" target="_blank">
                            Acceder a {{ regla.domainName }}
                        </a>
                    {% endfor %}
                </div>
            {% endfor %}
        {% endif %}

        <div class="footer-info">
            🖥️ Sincronización automática con /etc/hosts activa.
        </div>
    </div>
</body>
</html>
"""

@app_ue.route('/')
def dashboard():
    return render_template_string(HTML_TEMPLATE, estado=estado_ue)

@app_ue.route('/notificacion_edge', methods=['POST'])
def recibir_contexto():
    datos = request.json
    ahora = time.strftime("%H:%M:%S")
    
    # 1. Actualizar memoria del Dashboard
    estado_ue["aplicaciones"] = datos.get('aplicaciones', [])
    estado_ue["ultima_act"] = ahora
    
    # 2. Filtrar y preparar lista de DNS para el archivo hosts
    lista_dns = []
    for app in estado_ue["aplicaciones"]:
        for regla in app.get('dns', []):
            ip = regla.get('ipAddress')
            dom = regla.get('domainName')
            if ip and dom:
                lista_dns.append((ip, dom))
    
    # 3. Lanzar sincronización física
    print(f"\n🔔 [PUSH] Recibida actualización a las {ahora}. Apps activas: {len(estado_ue['aplicaciones'])}")
    sincronizar_hosts_blindado(lista_dns)
    
    return jsonify({"status": "Sincronizado"}), 200

if __name__ == '__main__':
    # Desactivar logs basura de Flask
    log = logging.getLogger('werkzeug')
    log.setLevel(logging.ERROR)
    
    print("="*60)
    print("🚀 SIMULADOR UE PORTÁTIL v2 INICIADO")
    print(f"🌐 Panel de control: http://localhost:{PUERTO_ESCUCHA}")
    print("⚠️ Recuerda ejecutar con SUDO para actualizar /etc/hosts")
    print("="*60)
    
    app_ue.run(host='0.0.0.0', port=PUERTO_ESCUCHA)
