import math
import time
import json
import threading

try:
    import gps
except ImportError:
    print("❌ Falta la librería gps. Ejecuta: sudo apt-get install python3-gps")

class ModuloGPS_IMU:
    def __init__(self, state_dict):
        self.state = state_dict
        self.smooth_mx = None
        self.smooth_my = None
        self.smooth_gz = 0.0

    def procesar_giroscopio(self, gz_bruto):
        """Filtro de paso bajo (EMA) para eliminar el ruido eléctrico del ESP32"""
        self.smooth_gz = (0.2 * gz_bruto) + (0.8 * self.smooth_gz)
        # Ampliamos la zona muerta a 2.5 grados/s para evitar vibraciones
        self.state["current_gz"] = 0.0 if abs(self.smooth_gz) < 2.5 else self.smooth_gz

    def iniciar_hilo_gps(self):
        """Lanza el hilo de escucha de satélites."""
        threading.Thread(target=self._gpsd_loop, daemon=True).start()

    def _gpsd_loop(self):
        last_fix_time = 0
        while True:
            try:
                session = gps.gps(mode=gps.WATCH_ENABLE | gps.WATCH_NEWSTYLE)
                for report in session:
                    data = dict(report)
                    
                    if data.get('class') == 'TPV':
                        if data.get('mode', 0) >= 2 and 'lat' in data and 'lon' in data:
                            lat, lon = float(data['lat']), float(data['lon'])
                            if not math.isnan(lat) and not math.isnan(lon) and lat != 0.0:
                                if self.state['curr_lat'] == 0.0:
                                    self.state['curr_lat'] = lat
                                    self.state['curr_lon'] = lon
                                else:
                                    ALPHA = 0.05
                                    self.state['curr_lat'] = (ALPHA * lat) + ((1.0 - ALPHA) * self.state['curr_lat'])
                                    self.state['curr_lon'] = (ALPHA * lon) + ((1.0 - ALPHA) * self.state['curr_lon'])
                                
                                last_fix_time = time.time()
                                self.state['fix'] = True
                        
                        if time.time() - last_fix_time > 3.0:
                            self.state['fix'] = False
                            
                    elif data.get('class') == 'SKY':
                        if 'satellites' in data:
                            usados = sum(1 for s in data['satellites'] if s.get('used', False))
                            if usados > 0: self.state['sats'] = usados
                        elif 'uSat' in data:
                            self.state['sats'] = int(data['uSat'])
                            
            except Exception as e:
                print(f"\n⚠️ Re-conectando antena GPSD... Error: {e}")
                time.sleep(2)

    def calibrar_giroscopio(self, base):
        """Mide la desviación del giroscopio en reposo durante 2 segundos."""
        if base: 
            base.ser.reset_input_buffer()
            time.sleep(0.5) 
        suma_bias, lecturas = 0.0, 0
        t_fin = time.time() + 2.0
        while time.time() < t_fin:
            data = self.get_fresh_imu_data(base)
            if data and 'gz' in data:
                suma_bias += math.degrees(data["gz"])
                lecturas += 1
        self.state["bias_gz"] = suma_bias / lecturas if lecturas > 0 else 0.0

    # Modifica get_fresh_imu_data para que no bloquee (evita Lag)
    def get_fresh_imu_data(self, base):
        if not base or base.ser.in_waiting == 0:
            return None  
        try:
            line = base.ser.readline().decode('utf-8', errors='ignore').strip()
            if line.startswith('{'):
                return json.loads(line)
        except: pass
        return None

    def actualizar_orientacion(self, mx, my):
        """Aplica el Filtro EMA y la calibración magnética para sacar el Rumbo exacto."""
        if self.smooth_mx is None:
            self.smooth_mx = mx
            self.smooth_my = my
        else:
            self.smooth_mx = (0.05 * mx) + (0.95 * self.smooth_mx)
            self.smooth_my = (0.05 * my) + (0.95 * self.smooth_my)

        mx_corr = self.smooth_mx - self.state["bias_mx"]
        my_corr = self.smooth_my - self.state["bias_my"]
        
        h = math.degrees(math.atan2(my_corr, mx_corr))
        h_invertido = (360 - h) % 360
        OFFSET_NORTE = 325
        self.state["heading"] = round((h_invertido + OFFSET_NORTE + 360) % 360, 1)