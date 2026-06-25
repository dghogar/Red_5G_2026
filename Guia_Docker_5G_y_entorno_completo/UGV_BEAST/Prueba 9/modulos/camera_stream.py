# modulos/camera_stream.py
import cv2
import time
import os
import threading
import subprocess
import datetime
from modulos.logger import log

class ModuloCamara:
    def __init__(self, state_dict):
        self.state = state_dict 
        self.frame_lock = threading.Lock()
        self.camera = None
        
        # NUEVO: Variables de Red para la tubería UDP
        self.IP_PORTATIL = "10.233.35.1"#IP del movil 5G (donde se va a mandar las ordenes) (Android) # Cambiar por la IP del USB
        self.PUERTO_UDP = 5000
        self.ffmpeg_process = None

        self.base_dir = "media"
        os.makedirs(os.path.join(self.base_dir, "fotos"), exist_ok=True)
        os.makedirs(os.path.join(self.base_dir, "videos"), exist_ok=True)
        
        self.take_photo_flag = False
        self.is_recording = False
        self.video_writer = None
        
        self.is_running = False
        self.capture_thread = None
        self.clientes_activos = 0
        
        log.info("[CÁMARA] Módulo cargado en modo reposo (UDP-MPEG).")

    def conectar_cliente(self):
        with self.frame_lock:
            self.clientes_activos += 1
            if not self.is_running:
                self.is_running = True
                self.capture_thread = threading.Thread(target=self._bucle_captura, daemon=True)
                self.capture_thread.start()

    def desconectar_cliente(self):
        with self.frame_lock:
            self.clientes_activos -= 1
            if self.clientes_activos <= 0:
                self.clientes_activos = 0
                if not self.is_recording:
                    self.is_running = False

    def start_stream(self):
        self.conectar_cliente()
        
    def stop_stream(self):
        """Freno de emergencia directo"""
        with self.frame_lock:
            self.clientes_activos = 0
            self.is_running = False
        self._apagar_camara()

    def _encender_camara(self):
        if self.camera is None or not self.camera.isOpened():
            self.camera = cv2.VideoCapture(0, cv2.CAP_V4L2)
            if not self.camera.isOpened(): 
                self.camera = cv2.VideoCapture(-1, cv2.CAP_V4L2)
                
            if self.camera.isOpened():
                self.camera.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc('M','J','P','G'))
                self.camera.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
                self.camera.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)
                self.camera.set(cv2.CAP_PROP_FPS, 30)
                self.camera.set(cv2.CAP_PROP_BUFFERSIZE, 1)
                log.info("[CÁMARA]Sensor ENCENDIDO.")

                self._iniciar_tuberia_udp()

    def _iniciar_tuberia_udp(self):
        """Mata cualquier proceso colgado y levanta una tubería limpia"""
        import subprocess
        try:
            subprocess.run(["pkill", "-9", "ffmpeg"], stderr=subprocess.DEVNULL, stdout=subprocess.DEVNULL)
        except Exception:
            pass
            
        comando = [
            'ffmpeg', '-y',
            '-f', 'rawvideo', '-vcodec', 'rawvideo', '-s', '640x480', '-pix_fmt', 'bgr24', '-r', '30',
            '-i', '-', 
            '-f', 'mpegts', '-codec:v', 'mpeg1video', '-pix_fmt', 'yuv420p', '-s', '640x480', '-b:v', '1500k',
            '-bf', '0', f'udp://{self.IP_PORTATIL}:{self.PUERTO_UDP}'
        ]
        self.ffmpeg_process = subprocess.Popen(comando, stdin=subprocess.PIPE, stderr=subprocess.DEVNULL)
        log.info(f"[CÁMARA] Tubería hacia {self.IP_PORTATIL} (Re)abierta.")

    def _apagar_camara(self):
        self.is_running = False
        
        if self.is_recording and self.video_writer:
            self.video_writer.release()
            self.video_writer = None
            self.is_recording = False
            
        if self.ffmpeg_process:
            try:
                self.ffmpeg_process.kill()
            except Exception:
                pass
            self.ffmpeg_process = None
        try:
            subprocess.run(["pkill", "-9", "ffmpeg"], stderr=subprocess.DEVNULL, stdout=subprocess.DEVNULL)
        except Exception:
            pass

        if self.camera and self.camera.isOpened():
            self.camera.release()
            self.camera = None
            log.info("[CÁMARA] 💤 Sensor y tubería FFmpeg APAGADOS.")

    def take_photo(self): 
        self.take_photo_flag = True
    
    def cambiar_zoom(self, incremento):
        z = self.state.get('zoom_level', 1.0)
        z = max(1.0, min(3.0, z + incremento))
        self.state['zoom_level'] = z
        return round(z, 1)

    def toggle_grabacion(self):
        self.is_recording = not self.is_recording
        if self.is_recording:
            self.conectar_cliente()
        else:
            self.desconectar_cliente()
        return self.is_recording

    def _bucle_captura(self):
        self._encender_camara()
        if not self.camera or not self.camera.isOpened():
            self.is_running = False
            return

        while self.is_running:
            ret, frame = self.camera.read()
            if not ret: 
                time.sleep(0.1)
                continue
            
            # --- ZOOM DIGITAL ---
            z = self.state.get('zoom_level', 1.0)
            if z > 1.0:
                h, w = frame.shape[:2]
                cw, ch = int(w/z), int(h/z)
                x1, y1 = max(0, w//2 - cw//2), max(0, h//2 - ch//2)
                frame = cv2.resize(frame[y1:y1+ch, x1:x1+cw], (w, h))

            # --- FOTO LOCAL ---
            if self.take_photo_flag:
                ruta = os.path.join(self.base_dir, "fotos", f"pic_{datetime.datetime.now().strftime('%H%M%S')}.jpg")
                cv2.imwrite(ruta, frame)
                log.info(f"[CÁMARA] Foto guardada: {ruta}")
                self.take_photo_flag = False

            # --- VÍDEO LOCAL ---
            if self.is_recording:
                if self.video_writer is None:
                    ruta_v = os.path.join(self.base_dir, "videos", f"vid_{datetime.datetime.now().strftime('%H%M%S')}.avi")
                    self.video_writer = cv2.VideoWriter(ruta_v, cv2.VideoWriter_fourcc(*'XVID'), 15.0, (1280, 720))
                self.video_writer.write(cv2.resize(frame, (1280, 720)))
            elif self.video_writer is not None:
                self.video_writer.release()
                self.video_writer = None

           # --- ENVÍO WEB POR TUBERÍA UDP FFMPEG ---
            if self.ffmpeg_process:
                # 1. Comprobamos si FFmpeg ha muerto en silencio por latencia de red
                if self.ffmpeg_process.poll() is not None:
                    log.warning("[CÁMARA] Tubería asfixiada. Auto-resucitando FFmpeg...")
                    self._iniciar_tuberia_udp()
                else:
                    frame_web = cv2.resize(frame, (640, 480))
                    try:
                        self.ffmpeg_process.stdin.write(frame_web.tobytes())
                    except BrokenPipeError:
                        # 2. Si la tubería se rompe físicamente mientras escribimos
                        log.warning("[CÁMARA] Tubería rota detectada. Reiniciando...")
                        self._iniciar_tuberia_udp()
                    except Exception: 
                        pass

            time.sleep(0.01)
            
        self._apagar_camara()

    def __del__(self): 
        self._apagar_camara()