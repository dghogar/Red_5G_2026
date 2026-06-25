import serial
import threading
import time

class ModuloEntorno:
    def __init__(self, state_dict):
        self.state = state_dict
        # Fíjate que quitamos la humedad, ya que tu Arduino actual solo envía Temp y Presión
        self.state['telemetry'].update({'temp_ext': 0.0, 'presion': 0.0})
        self.puerto = '/dev/ttyUSB0'

    def iniciar_hilo(self):
        threading.Thread(target=self._bucle_lectura, daemon=True).start()

    def _bucle_lectura(self):
        arduino = None
        while True:
            try:
                if not arduino:
                    arduino = serial.Serial(self.puerto, 115200, timeout=1)
                
                if arduino.in_waiting > 0:
                    linea = arduino.readline().decode('utf-8', errors='ignore').strip()
                    if linea.startswith('T:'):
                        partes = linea.split(',')
                        self.state['telemetry']['temp_ext'] = round(float(partes[0].split(':')[1]), 2)
                        self.state['telemetry']['presion'] = round(float(partes[1].split(':')[1]), 2)
            except Exception:
                arduino = None
                time.sleep(2) # Si el cable se suelta, intenta reconectar sin tirar errores