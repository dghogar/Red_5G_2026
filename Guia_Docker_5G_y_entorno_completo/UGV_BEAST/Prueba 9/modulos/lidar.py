# modulos/lidar.py
import threading
import time
import glob
import serial
from modulos.logger import log

class ModuloLidar:
    def __init__(self, state_dict, base_controller):
        self.state = state_dict
        self.base = base_controller 
        self.state['obstaculo_critico'] = False
        
        # --- PARÁMETROS TÁCTICOS (EVASIÓN) ---
        self.DISTANCIA_FRENO = 280  # mm (28 cm)
        self.PUNTOS_PELIGRO = 3     
        self.DESFASE_GRADOS = 270   # Calibración exacta para tu chasis
        
        # --- VARIABLES INTERNAS ---
        self.lidar_ser = None
        self.last_start_angle = 0
        self.lidar_angles = []
        self.lidar_distances = []

    def iniciar_hilo(self):
        threading.Thread(target=self._bucle_escaneo, daemon=True).start()

    def parse_lidar_frame(self, data):
        start_angle = (data[5] << 8 | data[4]) * 0.01
        
        for i in range(12):
            offset = 6 + i * 3
            dist = data[offset+1] << 8 | data[offset]
            
            if 180 < dist < 2000:
                angulo = (start_angle + i * 0.83333 + self.DESFASE_GRADOS) % 360
                
                # ESTRICTO 180º: Solo de 270º (Izquierda) a 90º (Derecha)
                if angulo <= 90 or angulo >= 270:
                    self.lidar_angles.append(angulo)
                    self.lidar_distances.append(dist)
                
        return start_angle

    def _evaluar_radar(self):
        impactos = 0
        dist_frente = 99999
        dist_izq = 99999
        dist_der = 99999
        
        for a, d in zip(self.lidar_angles, self.lidar_distances):
            
            # EVASIÓN: Cualquier impacto frontal < 28cm suma un punto de peligro
            if d < self.DISTANCIA_FRENO and (a >= 290 or a <= 70):
                impactos += 1

            # 1. IZQUIERDA (270º a 290º)
            if 270 <= a < 290:
                dist_izq = min(dist_izq, d)
                
            # 2. FRENTE (290º a 70º)
            elif a >= 290 or a <= 70:
                dist_frente = min(dist_frente, d)
                    
            # 3. DERECHA (70º a 90º)
            elif 70 < a <= 90:
                dist_der = min(dist_der, d)
                
        # Actualizamos el estado del freno
        self.state['obstaculo_critico'] = (impactos >= self.PUNTOS_PELIGRO)
        
        # Exportar a la web en cm (string "--" si está libre)
        self.state['lidar_frontal_cm'] = round(dist_frente / 10, 1) if dist_frente != 99999 else "--"
        self.state['lidar_derecha_cm'] = round(dist_der / 10, 1) if dist_der != 99999 else "--"
        self.state['lidar_izquierda_cm'] = round(dist_izq / 10, 1) if dist_izq != 99999 else "--"
        
        self.lidar_angles.clear()
        self.lidar_distances.clear()

    def _bucle_escaneo(self):
        log.info("[LIDAR] 📡 Buscando sensor D500...")
        
        # Conexión simple y bruta (Buscando ACM y USB, ignorando el GPS)
        while self.lidar_ser is None:
            puertos = glob.glob('/dev/ttyACM*') + glob.glob('/dev/ttyUSB*')
            
            for puerto in puertos:
                if 'ACM0' in puerto: 
                    continue # Asumimos que aquí está el GPS
                try:
                    self.lidar_ser = serial.Serial(puerto, 230400, timeout=1)
                    log.info(f"[LIDAR] Vinculado en {puerto}")
                    break
                except Exception:
                    pass
            
            if self.lidar_ser is None:
                time.sleep(2)
                
        # Lectura a máxima velocidad
        while True:
            try:
                header = self.lidar_ser.read(1)
                if header == b'\x54':
                    data = header + self.lidar_ser.read(46)
                    if len(data) == 47:
                        start_angle = self.parse_lidar_frame(list(data))
                        
                        if self.last_start_angle > start_angle:
                            self._evaluar_radar()
                            
                        self.last_start_angle = start_angle
                else:
                    self.lidar_ser.flushInput()
            except Exception:
                pass