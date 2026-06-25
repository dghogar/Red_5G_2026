import os
import psutil
import time
import subprocess
import re

class ModuloTelemetria:
    def __init__(self, state_dict):
        self.state = state_dict
        self.ultima_actualizacion = 0  # <--- NUEVO: Temporizador

    def update_telemetry(self):
        """Lee el hardware solo cada 0.5 segundos para no ahogar el servidor (Cero Lag)"""
        ahora = time.time()
        # Si hace menos de medio segundo que lo leímos, salimos al instante
        if ahora - self.ultima_actualizacion < 0.5:
            return 
            
        self.ultima_actualizacion = ahora
        
        try:
            self.state['telemetry']['cpu'] = psutil.cpu_percent()
            self.state['telemetry']['ram'] = psutil.virtual_memory().percent
            
            if os.path.exists("/sys/class/thermal/thermal_zone0/temp"):
                with open("/sys/class/thermal/thermal_zone0/temp", "r") as f:
                    self.state['telemetry']['temp'] = round(int(f.read())/1000.0, 1)
            
            #--- LECTURA DEL WIFI (SIGNAL-FENCE BLINDADO) ---
            wifi = 100 
            try:
                salida = subprocess.getoutput("iwconfig")
                match = re.search(r"Link Quality=(\d+)/(\d+)", salida)
                if match:
                    calidad = float(match.group(1))
                    maximo = float(match.group(2))
                    wifi = int((calidad / maximo) * 100)
            except Exception: 
                pass
            
            self.state['telemetry']['wifi'] = wifi
            self.state['wifi_critico'] = (wifi < 30)
            
        except Exception as e:
            pass