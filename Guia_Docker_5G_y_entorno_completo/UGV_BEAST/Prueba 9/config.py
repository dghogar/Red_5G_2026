# config.py

NAV_PARAMS = {
    'INVERTIR_SENSOR': -1,
    'KP_GIRO': 0.02,
    'FUERZA_MINIMA': 0.05,
    'VEL_RECTA': 0.5,
    'VEL_GIRO': 0.20,
    'TOLERANCIA_ANGULO': 15,
    'UMBRAL_LLEGADA': 0.8 #1.5
}

INITIAL_STATE = {
    "curr_lat": 0.0, "curr_lon": 0.0,
    "target_lat": None, "target_lon": None,
    "heading": 0.0, "active": False, "dist": 0.0,
    "fix": False, "sats": 0, "port": "GPSD Local",
    "bias_gz": 0.0, "current_gz": 0.0,
    "bias_mx": -74.67,  
    "bias_my": 25.46,
    "telemetry": {'cpu': 0, 'ram': 0, 'temp': 0, 'wifi': 0},
    "lights": False,
    "pan": 0, "tilt": 0,
    "speed": 0.6,
    "zoom_level": 1.0,
    "camera_active": False
}

# --- CONSTANTES FÍSICAS Y DE RED ---
RADIO_TIERRA_M = 6371000.0  # Radio medio volumétrico de la Tierra (REQUERIDO POR MATH_UTILS)
PUERTO_COMANDOS = 65432     # Puerto por el que escucha las órdenes MANET UDP del Maestro
IP_UGV = "10.43.31.12"          # (Opcional en el esclavo, pero buena práctica)
ROBOT_ID = "beast"
# --- RUTAS DE DIRECTORIOS ---
LOGS_DIR = "logs"