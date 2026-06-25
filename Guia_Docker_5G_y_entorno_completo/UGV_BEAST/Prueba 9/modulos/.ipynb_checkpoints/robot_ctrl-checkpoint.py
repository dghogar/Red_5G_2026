# modulos/robot_ctrl.py
import time
import queue
import threading
from config import INITIAL_STATE
from modulos.logger import log

# Guardamos el estado global como en el Yahboom
estado_global = INITIAL_STATE.copy()
flota_aliada = {}
instancia_robot = None # Se inyectará desde app_slave.py

# Cola segura para no saturar el puerto serie del ESP32
cola_hw = queue.Queue()

def procesador_hardware():
    while True:
        tarea = cola_hw.get()
        accion = tarea.get("accion")
        datos = tarea.get("datos")

        try:
            if not instancia_robot or not instancia_robot.base: continue
            
            if accion == "chasis_dinamico":
                # Control manual directo, pero suavizado por la velocidad de la web
                s = estado_global.get('speed', 0.6)
                instancia_robot.base.base_speed_ctrl(datos['L'] * s, datos['R'] * s)
                
            elif accion == "chasis_tecla":
                v = estado_global.get('speed', 0.6)
                if datos == 'W': instancia_robot.base.base_speed_ctrl(v, v)
                elif datos == 'S': instancia_robot.base.base_speed_ctrl(-v, -v)
                elif datos == 'A': instancia_robot.base.base_speed_ctrl(-v, v)
                elif datos == 'D': instancia_robot.base.base_speed_ctrl(v, -v)
                elif datos == 'STOP': instancia_robot.base.base_speed_ctrl(0, 0)
                
            elif accion == "camara_absoluto":
                instancia_robot.hw.move_gimbal(datos['pan'], datos['tilt'])
                
            elif accion == "velocidad":
                instancia_robot.hw.set_speed(datos)
                
            elif accion == "zoom":
                # TRADUCTOR: La web manda 0.5 o -0.5, el hardware espera 'in' o 'out'
                val = 'in' if float(datos) > 0 else 'out'
                instancia_robot.hw.set_zoom(val)
                
            elif accion == "luces":
                instancia_robot.hw.toggle_lights()
                
        except Exception as e:
            log.error(f"[ROBOT_CTRL] ⚠️ Error en bus de hardware: {e}")
        finally:
            cola_hw.task_done()

# Arrancamos el hilo del hardware
threading.Thread(target=procesador_hardware, daemon=True).start()

# ==========================================
# FUNCIONES PUENTE (Llamadas por Web y MANET)
# ==========================================
def mover_chasis_dinamico(L: float, R: float): cola_hw.put({"accion": "chasis_dinamico", "datos": {"L": L, "R": R}})
def mover_chasis(comando: str): 
    # 🛡️ KILL-SWITCH: Si es una orden de freno, borramos las tareas acumuladas 
    # en la cola para que el chasis frene de forma instantánea.
    if comando == "STOP":
        with cola_hw.mutex:
            cola_hw.queue.clear()
            
    cola_hw.put({"accion": "chasis_tecla", "datos": comando})
def mover_camara_absoluto(pan: int, tilt: int): cola_hw.put({"accion": "camara_absoluto", "datos": {"pan": pan, "tilt": tilt}})
def cambiar_velocidad(val: str): cola_hw.put({"accion": "velocidad", "datos": val})
def cambiar_zoom(val: str): cola_hw.put({"accion": "zoom", "datos": val})
def toggle_luces(): cola_hw.put({"accion": "luces", "datos": None})

def ejecutar_mision_manet(datos: dict):
    if instancia_robot and hasattr(instancia_robot, 'navegacion'):
        instancia_robot.navegacion.set_target(datos.get('lat'), datos.get('lon'), datos.get('zones', []))

def actualizar_aliado(id_aliado: str, datos_vitales: dict):
    """Actualiza la posición y el estado del dron compañero y resetea su contador de vida"""
    datos_vitales['ultimo_visto'] = time.time()
    flota_aliada[id_aliado] = datos_vitales

def obtener_telemetria():
    # Actualiza CPU/RAM
    if instancia_robot: instancia_robot.telemetria.update_telemetry()

    ahora = time.time()
    aliados_procesados = {}
    for id_robot, datos in flota_aliada.items():
        esta_vivo = (ahora - datos.get('ultimo_visto', 0)) < 5.0
        datos['status'] = "ONLINE" if esta_vivo else "OFFLINE"
        aliados_procesados[id_robot] = datos
    
    return {
        "curr_lat": estado_global.get("curr_lat", 0.0),
        "curr_lon": estado_global.get("curr_lon", 0.0),
        "heading": estado_global.get("heading", 0.0),
        "sats": estado_global.get("sats", 0),
        "lidar_frontal_cm": estado_global.get("lidar_frontal_cm", "--"),
        "lidar_derecha_cm": estado_global.get("lidar_derecha_cm", "--"),
        "lidar_izquierda_cm": estado_global.get("lidar_izquierda_cm", "--"),
        "obstaculo_critico": estado_global.get("obstaculo_critico", False),
        "distancia": estado_global.get("lidar_frontal_cm", "--"),
        "nav_state": estado_global.get("nav_state", "ESPERANDO"), # <--- EL ESTADO DE MISIÓN
        "flota": aliados_procesados, # <--- LOS COMPAÑEROS
        "telemetry": {
            "temp": estado_global.get("telemetry", {}).get("temp_ext", "--"),
            "pres": estado_global.get("telemetry", {}).get("presion", "--"),
            "wifi": estado_global.get("telemetry", {}).get("wifi", 100)
        },
        "wifi_critico": estado_global.get("wifi_critico", False),
        "sys": {
            "cpu": estado_global.get("telemetry", {}).get("cpu", 0),
            "ram": estado_global.get("telemetry", {}).get("ram", 0)
        }
    }