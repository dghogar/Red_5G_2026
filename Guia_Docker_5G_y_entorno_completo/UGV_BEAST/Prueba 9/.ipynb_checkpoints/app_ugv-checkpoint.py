# app_slave.py
import threading
import time
import sys

from modulos.logger import log
from config import NAV_PARAMS

# Hardware base
from base_ctrl import BaseController

# Módulos antiguos del Beast
from modulos.telemetria import ModuloTelemetria
from modulos.gps_imu import ModuloGPS_IMU
from modulos.hardware import ModuloHardware
from modulos.entorno import ModuloEntorno      
from modulos.lidar import ModuloLidar          

# Módulos nuevos de la arquitectura táctica
from modulos.camera_stream import ModuloCamara
from modulos.navigator import ModuloNavegacion
import modulos.robot_ctrl as rc
from modulos.video_server import iniciar_micro_servidor
from modulos.manet_node import nodo_tx
from modulos.manet_rx import nodo_rx

class UGVBeast:
    def __init__(self):
        self.state = rc.estado_global
        try:
            self.base = BaseController('/dev/serial0', 115200)
        except Exception as e:
            log.error(f"[SISTEMA] Error conectando con placa base ESP32: {e}")
            self.base = None

        self.camara = ModuloCamara(self.state)
        self.telemetria = ModuloTelemetria(self.state)
        self.sensores = ModuloGPS_IMU(self.state)
        self.navegacion = ModuloNavegacion(self.state, NAV_PARAMS, self.base)
        self.hw = ModuloHardware(self.state, self.base)
        self.entorno = ModuloEntorno(self.state) 
        self.lidar = ModuloLidar(self.state, self.base)

def main():
    log.info("==================================================")
    log.info("  C.C.C. DRONES TÁCTICOS - UGV BEAST (ESCLAVO) ")
    log.info("==================================================")

    # 1. Instanciamos el robot
    robot = UGVBeast()
    if robot.base: robot.hw.init_beast()

    nodo_tx.start()
    nodo_rx.start()

    # 2. Conectamos el robot al cerebro y a la web
    rc.instancia_robot = robot

    # 3. Arrancamos Hilos de Hardware
    log.info("[SISTEMA] Encendiendo sensores periféricos...")
    robot.sensores.iniciar_hilo_gps()
    robot.entorno.iniciar_hilo()
    robot.lidar.iniciar_hilo()
    robot.navegacion.start()

    rc.cam_streamer = robot.camara

    # 4. Arrancamos Micro-Servidor de Vídeo WebRTC
    log.info("[SISTEMA] Levantando enlace de vídeo WebRTC...")
    threading.Thread(target=iniciar_micro_servidor, args=(robot.camara,), daemon=True).start()

    # 5. Bucle de IMU Suavizada (Bucle Principal)
    log.info("[SISTEMA] BEAST OPERATIVO.")
    try:
        if robot.base: robot.sensores.calibrar_giroscopio(robot.base)
        while True:
            data_imu = robot.sensores.get_fresh_imu_data(robot.base)
            if data_imu:
                robot.sensores.actualizar_orientacion(data_imu['mx'], data_imu['my'])
                robot.sensores.procesar_giroscopio(math.degrees(data_imu["gz"]) - robot.state["bias_gz"])
            time.sleep(0.02)
    except KeyboardInterrupt:
        log.warning("[SISTEMA] Apagado manual (Ctrl+C).")
        robot.navegacion.stop_nav()
        if robot.camara: robot.camara._apagar_camara()
        sys.exit(0)

if __name__ == '__main__':
    import math
    main()