# modulos/navigator.py
import math
import time
import threading
import modulos.math_utils as math_utils
from modulos.logger import log

class ModuloNavegacion(threading.Thread):
    def __init__(self, state_dict, nav_params, base_controller):
        super().__init__(daemon=True)
        self.state = state_dict
        self.params = nav_params 
        self.base = base_controller
        self.zonas_prohibidas = []

    def set_target(self, lat, lon, zonas=None):
        self.state["target_lat"], self.state["target_lon"] = lat, lon
        self.zonas_prohibidas = zonas if zonas else []
        self.state["active"] = True
        self.state["nav_state"] = "EN_RUTA"
        log.info(f"[NAV] Misión táctica iniciada -> Destino: Lat {lat:.5f}, Lon {lon:.5f}")

    def stop_nav(self):
        self.state["active"] = False
        self.state["nav_state"] = "ESPERANDO"
        if self.base: self.base.base_speed_ctrl(0, 0)
        log.info("[NAV] Misión detenida. Motores bloqueados.")

    def calcular_distancia_y_rumbo(self):
        l1, o1 = self.state["curr_lat"], self.state["curr_lon"]
        l2, o2 = self.state["target_lat"], self.state["target_lon"]
        self.state["dist"] = math_utils.calc_distancia(l1, o1, l2, o2)
        target_bearing = math_utils.calc_rumbo_deseado(l1, o1, l2, o2)
        error_rumbo = (target_bearing - self.state["heading"] + 180) % 360 - 180
        return target_bearing, error_rumbo

    def aplicar_esp(self, L_base, R_base, is_moving_straight):
        if not is_moving_straight: return L_base, R_base
        error_giro = 0.0 - self.state.get("current_gz", 0.0)
        correccion = error_giro * self.params['KP_GIRO'] * self.params['INVERTIR_SENSOR']
        correccion = max(-0.15, min(0.15, correccion))
        if L_base < 0: correccion = -correccion
        return max(-1.0, min(1.0, L_base + correccion)), max(-1.0, min(1.0, R_base - correccion))

    def run(self):
        log.info("[NAV] IA de Navegación del UGV Beast iniciada.")
        while True:
            if not self.state.get("active", False):
                time.sleep(0.5); continue

            # 0. SIGNAL-FENCE (Prioridad Absoluta)
            if self.state.get('wifi_critico', False):
                log.warning("[NAV] SEÑAL WIFI DÉBIL (<30%). Abortando misión para liberar control manual.")
                self.stop_nav() # Esto frena los motores Y apaga la bandera 'active'
                continue # Vuelve al inicio del bucle (como active es False, se quedará durmiendo sin molestarte)

            # 1. EVASIÓN LIDAR (Prioridad 1)
            if self.state.get('obstaculo_critico', False):
                log.warning("[NAV] Evasión LiDAR: Muro detectado.")
                self.base.base_speed_ctrl(-0.4, -0.4) # Marcha atrás suave
                time.sleep(1.0)
                self.base.base_speed_ctrl(0.4, -0.4)  # Giro para buscar salida
                time.sleep(1.0)
                self.base.base_speed_ctrl(0, 0)
                continue

            if not self.state.get("fix", False):
                time.sleep(0.5); continue

            # 2. LLEGADA
            target_brng, err_rumbo = self.calcular_distancia_y_rumbo()
            if self.state["dist"] < self.params['UMBRAL_LLEGADA']:
                log.info(f"[NAV] ¡OBJETIVO ALCANZADO! (Margen: {self.state['dist']:.2f}m)")
                self.state["nav_state"] = "LLEGADA"
                self.stop_nav()
                continue

            # 3. GEOFENCING PREDICTIVO (El dron mira 3.5m al futuro)
            futura_lat, futura_lon = math_utils.proyectar_punto_gps(
                self.state["curr_lat"], self.state["curr_lon"], self.state["heading"], 3.5
            )
            if math_utils.evaluar_zona_prohibida(futura_lat, futura_lon, self.zonas_prohibidas):
                log.warning("[NAV] Bordeando zona prohibida...")
                self.base.base_speed_ctrl(0.4, -0.4) # Giro derecha evasivo
                time.sleep(0.5)
                self.base.base_speed_ctrl(0, 0)
                continue

            # 4. DIRECCIÓN Y ESP
            v_actual = self.params['VEL_RECTA']
            if self.state["dist"] < 3.0: v_actual *= 0.6

            if abs(err_rumbo) > self.params['TOLERANCIA_ANGULO']:
                L = self.params['VEL_GIRO'] if err_rumbo > 0 else -self.params['VEL_GIRO']
                R = -L
                L_f, R_f = L, R # En giro no aplicamos ESP
            else:
                L_f, R_f = self.aplicar_esp(v_actual, v_actual, True)

            self.base.base_speed_ctrl(round(L_f, 3), round(R_f, 3))
            time.sleep(0.05)