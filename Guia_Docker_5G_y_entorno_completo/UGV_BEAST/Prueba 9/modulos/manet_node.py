import socket
import threading
import queue
import json
import time
import config
import psutil
from modulos.logger import log
import modulos.robot_ctrl as robot_ctrl

# def obtener_ip_broadcast():
    # """Detecta dinámicamente la IP de difusión para operar sin internet"""
    # for interfaz, direcciones in psutil.net_if_addrs().items():
        # for addr in direcciones:
            # if addr.family == socket.AF_INET and addr.broadcast and addr.address != '127.0.0.1':
                # return addr.broadcast
    # return '255.255.255.255'

# Calculamos la IP correcta una sola vez al arrancar
IP_BROADCAST = "10.233.35.1" #IP Termux (Android) #obtener_ip_broadcast()

class ManetNode(threading.Thread):
    def __init__(self, mi_id: str):
        super().__init__(daemon=True)
        self.puerto = config.PUERTO_COMANDOS
        self.cola_mensajes = queue.Queue()
        self.mi_id = mi_id
        
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

    def enviar_comando(self, destino: str, comando_str: str):
        sobre = {
            "origen": self.mi_id,
            "destino": destino,
            "tipo": "COMANDO",
            "payload": comando_str
        }
        self.cola_mensajes.put(sobre)

    def _hilo_latido(self):
        """Dispara la telemetría COMPLETA a la red para la Estación de Tierra"""
        while True:
            time.sleep(0.5) 
            
            try:
                datos_completos = robot_ctrl.obtener_telemetria()
                
                latido = {
                    "origen": self.mi_id,
                    "destino": "ALL",
                    "tipo": "HEARTBEAT",
                    "payload": datos_completos
                }
                
                self.sock.sendto(json.dumps(latido).encode('utf-8'), (IP_BROADCAST, self.puerto))
                
            except Exception:
                pass

    def run(self):
        log.info(f"[MANET] Nodo TX activo. Emitiendo latido como '{self.mi_id}'")
        threading.Thread(target=self._hilo_latido, daemon=True).start()
        
        while True:
            paquete = self.cola_mensajes.get()
            try:
                self.sock.sendto(json.dumps(paquete).encode('utf-8'), (IP_BROADCAST, self.puerto))
            except Exception as e:
                log.error(f"[MANET] Error TX Broadcast: {e}")
            finally:
                self.cola_mensajes.task_done()

# INSTANCIA UNIVERSAL: Lee el nombre automáticamente de la configuración
nodo_tx = ManetNode(config.ROBOT_ID)