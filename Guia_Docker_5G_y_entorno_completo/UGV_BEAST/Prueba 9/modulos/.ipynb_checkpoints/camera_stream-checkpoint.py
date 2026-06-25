# modulos/camera_stream.py
import cv2
import time
import os
import threading
import datetime
from modulos.logger import log
import asyncio
from aiortc import VideoStreamTrack
from av import VideoFrame

class UGVVideoTrack(VideoStreamTrack):
    """Clase puente para empaquetar frames de OpenCV en datagramas UDP vía aiortc/av"""
    def __init__(self, camara_module):
        super().__init__()
        self.cam = camara_module
        # Al crear la conexión UDP, sumamos un cliente
        self.cam.conectar_cliente()

    def stop(self):
        # Al cerrarse la conexión WebRTC, aiortc llama a esta función automáticamente
        super().stop()
        self.cam.desconectar_cliente()

    async def recv(self):
        pts, time_base = await self.next_timestamp()
        
        frame = None
        while frame is None:
            with self.cam.frame_lock:
                if self.cam.output_frame is not None:
                    frame = self.cam.output_frame.copy()
            if frame is None:
                await asyncio.sleep(0.02)
        
        frame_web = cv2.resize(frame, (640, 360))
        v_frame = VideoFrame.from_ndarray(frame_web, format="bgr24")
        v_frame.pts = pts
        v_frame.time_base = time_base
        return v_frame


class ModuloCamara:
    def __init__(self, state_dict):
        self.state = state_dict 
        self.frame_lock = threading.Lock()
        self.output_frame = None
        self.camera = None
        self.video_mode = "udp"
        
        self.base_dir = "media"
        os.makedirs(os.path.join(self.base_dir, "fotos"), exist_ok=True)
        os.makedirs(os.path.join(self.base_dir, "videos"), exist_ok=True)
        
        self.take_photo_flag = False
        self.is_recording = False
        self.video_writer = None
        
        self.is_running = False
        self.capture_thread = None
        self.clientes_activos = 0
        
        log.info("[CÁMARA] 💤 Módulo cargado en modo reposo.")

    def conectar_cliente(self):
        """Suma un visor. Si es el primero, enciende la cámara."""
        with self.frame_lock:
            self.clientes_activos += 1
            if not self.is_running:
                self.is_running = True
                self.capture_thread = threading.Thread(target=self._bucle_captura, daemon=True)
                self.capture_thread.start()

    def desconectar_cliente(self):
        """Resta un visor. Si llega a 0 y no está grabando, apaga la cámara."""
        with self.frame_lock:
            self.clientes_activos -= 1
            if self.clientes_activos <= 0:
                self.clientes_activos = 0
                if not self.is_recording:
                    self.is_running = False

    def start_stream(self):
        self.conectar_cliente()
        
    def stop_stream(self):
        self.desconectar_cliente()

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
                log.info("[CÁMARA] 👁️ Sensor ENCENDIDO.")

    def _apagar_camara(self):
        self.is_running = False
        if self.is_recording and self.video_writer:
            self.video_writer.release()
            self.video_writer = None
            self.is_recording = False
            
        if self.camera and self.camera.isOpened():
            self.camera.release()
            self.camera = None
            with self.frame_lock:
                self.output_frame = None
            log.info("[CÁMARA] 💤 Sensor APAGADO. Volviendo a reposo.")

    def take_photo(self): 
        self.take_photo_flag = True
    
    def toggle_grabacion(self):
        """Si el usuario pulsa grabar, sumamos un 'cliente' fantasma para evitar que se apague"""
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
            
            # 1. PROCESAMIENTO DIGITAL (Zoom)
            z = self.state.get('zoom_level', 1.0)
            if z > 1.0:
                h, w = frame.shape[:2]
                cw, ch = int(w/z), int(h/z)
                x1, y1 = max(0, w//2 - cw//2), max(0, h//2 - ch//2)
                frame = cv2.resize(frame[y1:y1+ch, x1:x1+cw], (w, h))

            # Guardamos frame limpio
            with self.frame_lock: 
                self.output_frame = frame.copy()

            # 2. CAPTURA FOTO
            if self.take_photo_flag:
                ruta = os.path.join(self.base_dir, "fotos", f"pic_{datetime.datetime.now().strftime('%H%M%S')}.jpg")
                cv2.imwrite(ruta, frame)
                log.info(f"[CÁMARA] 📸 Foto guardada: {ruta}")
                self.take_photo_flag = False

            # 3. GRABACIÓN VÍDEO
            if self.is_recording:
                if self.video_writer is None:
                    ruta_v = os.path.join(self.base_dir, "videos", f"vid_{datetime.datetime.now().strftime('%H%M%S')}.avi")
                    self.video_writer = cv2.VideoWriter(ruta_v, cv2.VideoWriter_fourcc(*'XVID'), 15.0, (1280, 720))
                self.video_writer.write(cv2.resize(frame, (1280, 720)))
            elif self.video_writer is not None:
                self.video_writer.release()
                self.video_writer = None

            time.sleep(0.01)
            
        self._apagar_camara()

    def generate_frames(self):
        """Mantiene la compatibilidad con MJPEG. Suma un cliente al conectar y lo resta al desconectar"""
        self.conectar_cliente()
        try:
            while True:
                frame = None
                with self.frame_lock:
                    if self.output_frame is not None:
                        frame = self.output_frame.copy()
                
                if frame is not None:
                    frame_web = cv2.resize(frame, (640, 360))
                    ret, buffer = cv2.imencode('.jpg', frame_web, [cv2.IMWRITE_JPEG_QUALITY, 60])
                    if ret:
                        yield (b'--frame\r\nContent-Type: image/jpeg\r\n\r\n' + buffer.tobytes() + b'\r\n')
                time.sleep(0.05)
        except GeneratorExit:
            pass
        finally:
            self.desconectar_cliente()

    def __del__(self): 
        self._apagar_camara()