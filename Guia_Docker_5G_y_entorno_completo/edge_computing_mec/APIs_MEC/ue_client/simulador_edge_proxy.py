import threading
import time
import urllib.request
import urllib.parse
from http.server import BaseHTTPRequestHandler, HTTPServer
from flask import Flask, request, jsonify, render_template_string
import logging

PUERTO_DASHBOARD = 9090
PUERTO_PROXY = 8080
dns_cache = {} 
estado_ue = {"aplicaciones": [], "ultima_act": "Esperando red..."}

app_ue = Flask(__name__)

HTML_TEMPLATE = """
<!DOCTYPE html>
<html>
<head>
    <title>📱 Móvil Edge Hub</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: sans-serif; background: #f0f2f5; padding: 20px; color: #333; }
        .container { max-width: 600px; margin: auto; background: white; border-radius: 15px; padding: 20px; box-shadow: 0 4px 10px rgba(0,0,0,0.1); }
        .header { display: flex; justify-content: space-between; border-bottom: 2px solid #eee; padding-bottom: 10px; margin-bottom: 20px; }
        .status { color: #28a745; font-weight: bold; }
        .app-card { border: 1px solid #ddd; border-radius: 10px; padding: 15px; margin-bottom: 15px; background: #fafafa; }
        .app-title { font-size: 1.2em; font-weight: bold; color: #0056b3; margin-bottom: 10px; }
        .app-link { display: inline-block; padding: 10px 15px; background: #007bff; color: white; text-decoration: none; border-radius: 5px; }
    </style>
    <meta http-equiv="refresh" content="3">
</head>
<body>
    <div class="container">
        <div class="header"><h2>📱 Panel Edge (Proxy Termux)</h2><div class="status">📶 5G Listo</div></div>
        <p style="font-size: 0.8em; color: #666;">Último PUSH: {{ estado.ultima_act }}</p>
        {% for app in estado.aplicaciones %}	
            <div class="app-card"><div class="app-title">🟢 {{ app.appName }}</div>
                {% for regla in app.dns %}
                    <a class="app-link" href="http://{{ regla.domainName }}/" target="_blank">Abrir {{ regla.domainName }}</a>
                {% endfor %}
            </div>
        {% else %}
            <div style="text-align:center; color:#888; padding:20px;">Esperando notificaciones de la red...</div>
        {% endfor %}
    </div>
</body>
</html>
"""

@app_ue.route('/')
def dashboard(): return render_template_string(HTML_TEMPLATE, estado=estado_ue)

@app_ue.route('/notificacion_edge', methods=['POST'])
def recibir_contexto():
    global dns_cache
    estado_ue["aplicaciones"] = request.json.get('aplicaciones', [])
    estado_ue["ultima_act"] = time.strftime("%H:%M:%S")
    
    dns_cache.clear()
    for app in estado_ue["aplicaciones"]:
        for regla in app.get('dns', []):
            dom, ip = regla.get('domainName'), regla.get('ipAddress')
            if dom and ip: dns_cache[dom] = ip
            
    print(f"[{estado_ue['ultima_act']}] 🔔 PUSH Recibido. Caché interna: {dns_cache}")
    return jsonify({"status": "OK"}), 200

class EdgeProxy(BaseHTTPRequestHandler):
    def do_GET(self):
        hostname = urllib.parse.urlparse(self.path).hostname or self.headers.get('Host')
        if hostname in dns_cache:
            ip_destino = dns_cache[hostname]
            nueva_url = f"http://{ip_destino}{urllib.parse.urlparse(self.path).path}"
            if urllib.parse.urlparse(self.path).query: nueva_url += f"?{urllib.parse.urlparse(self.path).query}"
            print(f"🕵️ [PROXY] Interceptado: {hostname} -> Redirigiendo a {ip_destino}")
            
            req = urllib.request.Request(nueva_url)
            req.add_header('Host', hostname)
            try:
                with urllib.request.urlopen(req) as res:
                    self.send_response(res.status)
                    for k, v in res.headers.items(): self.send_header(k, v)
                    self.end_headers()
                    self.wfile.write(res.read())
            except Exception as e: self.send_error(500, f"Error Edge: {e}")
        else: self.send_error(404, "Dominio no gestionado por el MEC")

def iniciar_proxy():
    HTTPServer(('0.0.0.0', PUERTO_PROXY), EdgeProxy).serve_forever()

if __name__ == '__main__':
    log = logging.getLogger('werkzeug')
    log.setLevel(logging.ERROR)
    threading.Thread(target=iniciar_proxy, daemon=True).start()
    print("📱 CLIENTE EDGE INICIADO EN TERMUX")
    print(f"🌐 Dashboard Web: http://127.0.0.1:{PUERTO_DASHBOARD}")
    print(f"🕵️ Proxy Activo: Puerto {PUERTO_PROXY}")
    app_ue.run(host='0.0.0.0', port=PUERTO_DASHBOARD)
