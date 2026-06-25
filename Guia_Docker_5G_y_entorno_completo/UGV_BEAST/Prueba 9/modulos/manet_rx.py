import socket
import threading
import json
import config
from modulos.logger import log
import modulos.robot_ctrl as robot_ctrl

class ManetReceptor(threading.Thread):
    def __init__(self, mi_id: str):
        super().__init__(daemon=True)
        self.puerto = config.PUERTO_COMANDOS
        self.mi_id = mi_id
        
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        if hasattr(socket, 'SO_REUSEPORT'):
            self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
        
        self.sock.bind(('0.0.0.0', self.puerto))

    def run(self):
        log.info(f"[MANET] Nodo RX activo. Escuchando malla táctica como '{self.mi_id}'")
        while True:
            try:
                data, addr = self.sock.recvfrom(2048)
                paquete = json.loads(data.decode('utf-8'))
                
                origen = paquete.get("origen")
                destino = paquete.get("destino")
                tipo = paquete.get("tipo")
                payload = paquete.get("payload")
                
                if origen == self.mi_id: continue
                if destino != self.mi_id and destino != "ALL": continue
                
                if tipo == "HEARTBEAT":
                    robot_ctrl.actualizar_aliado(origen, payload)
                
                # --- 2. PROCESAR COMANDO TÁCTICO ---
                elif tipo == "COMANDO":
                    if payload == "STOP":
                        log.warning(f"[MANET] Orden EMERGENCIA (Freno Total) de {origen}")
                        robot_ctrl.parada_tactica_global() 
                        
                    elif isinstance(payload, dict):
                        # A. Misión Autónoma
                        if "lat" in payload:
                            log.info(f"[MANET] Misión recibida por radio.")
                            robot_ctrl.ejecutar_mision_manet(payload) 
                            
                        # B. Joystick y Periféricos
                        elif "action" in payload:
                            accion = payload["action"]
                            if accion == "chasis":
                                robot_ctrl.mover_chasis(payload["cmd"])
                            elif accion == "camara_absoluto":
                                robot_ctrl.mover_camara_absoluto(payload.get("pan", 90), payload.get("tilt", 90))
                            elif accion == "camara_center":
                                robot_ctrl.mover_camara_absoluto(90, 90) # El Beast se centra yendo a 90 absoluto
                            elif accion == "velocidad":
                                # TRADUCTOR: De números a palabras
                                direccion = "up" if payload["val"] > 0 else "down"
                                robot_ctrl.cambiar_velocidad(direccion)
                            elif accion == "luces":
                                robot_ctrl.toggle_luces()
                            elif accion == "video_start":
                                if hasattr(robot_ctrl, 'cam_streamer') and robot_ctrl.cam_streamer is not None:
                                    # Atrapamos la IP de la web y redirigimos la tubería
                                    robot_ctrl.cam_streamer.IP_PORTATIL = addr[0]
                                    robot_ctrl.cam_streamer.start_stream()
                            elif accion == "video_stop":
                                if hasattr(robot_ctrl, 'cam_streamer') and robot_ctrl.cam_streamer is not None:
                                    robot_ctrl.cam_streamer.stop_stream()
                            elif accion == "radar":
                                pass # El Beast no tiene servo de radar, lo ignoramos
                            elif accion == "photo":
                                if hasattr(robot_ctrl, 'cam_streamer') and robot_ctrl.cam_streamer is not None:
                                    robot_ctrl.cam_streamer.take_photo()
                                    log.info("[MANET] Orden de captura fotográfica recibida.")
                            elif accion == "record":
                                if hasattr(robot_ctrl, 'cam_streamer') and robot_ctrl.cam_streamer is not None:
                                    grabando = robot_ctrl.cam_streamer.toggle_grabacion()
                                    estado = "INICIADA" if grabando else "DETENIDA"
                                    log.info(f"[MANET] Grabación de vídeo local {estado}.")
                            elif accion == "zoom":
                                try:
                                    valor = float(payload.get("val", 0))
                                    if hasattr(robot_ctrl, 'cam_streamer') and robot_ctrl.cam_streamer is not None:
                                        robot_ctrl.cam_streamer.cambiar_zoom(valor)
                                        log.info(f"[RADIO] Zoom digital ajustado: {valor}")
                                    else:
                                        log.error("[RADIO] Error: Objeto cam_streamer no vinculado.")
                                except Exception as e:
                                    log.error(f"[RADIO] Error procesando zoom: {e}")
                        
                    elif isinstance(payload, dict) and "lat" in payload:
                        log.info(f"[MANET] Misión de {origen} recibida por radio.")
                        # Llama a la nueva función puente
                        robot_ctrl.ejecutar_mision_manet(payload) 
                        
            except json.JSONDecodeError: pass
            except Exception as e: log.error(f"[MANET] Error RX: {e}")

# INSTANCIA UNIVERSAL: Lee el nombre automáticamente de la configuración
nodo_rx = ManetReceptor(config.ROBOT_ID)