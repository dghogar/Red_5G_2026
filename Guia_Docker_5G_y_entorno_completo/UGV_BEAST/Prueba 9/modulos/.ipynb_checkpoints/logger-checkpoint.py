# modulos/logger.py
import logging
from logging.handlers import RotatingFileHandler
import os
import sys
import time

# --- 1. SILENCIADORES DE BAJO NIVEL ---
# Apaga el ruido de C++ (ej. "Corrupt JPEG data: extraneous bytes before marker 0xd9")
os.environ["OPENCV_LOG_LEVEL"] = "SILENT" 
os.environ['TZ'] = 'Europe/Madrid' 
time.tzset()

# --- 2. CONFIGURACIÓN DE RUTAS ---
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from config import LOGS_DIR

os.makedirs(LOGS_DIR, exist_ok=True)
archivo_log = os.path.join(LOGS_DIR, 'sistema_tactico.log')

formato = '%(asctime)s | [%(levelname)s] | %(message)s'
formato_fecha = '%H:%M:%S'

for handler in logging.root.handlers[:]:
    logging.root.removeHandler(handler)

# --- 3. CAJA NEGRA CON ROTACIÓN ---
logging.basicConfig(
    level=logging.WARNING, # Modo silencioso en consola (solo avisa de fallos)
    format=formato,
    datefmt=formato_fecha,
    handlers=[
        RotatingFileHandler(archivo_log, maxBytes=5242880, backupCount=3, encoding='utf-8'),
        logging.StreamHandler()
    ]
)

log = logging.getLogger("C3_DRONES")

# --- 4. SECUESTRO DE CRASHES DE PYTHON ---
# Obliga a que cualquier caída del sistema se guarde en el .log en vez de solo en la terminal
def handle_exception(exc_type, exc_value, exc_traceback):
    if issubclass(exc_type, KeyboardInterrupt):
        sys.__excepthook__(exc_type, exc_value, exc_traceback)
        return
    log.critical("EXCEPCIÓN CRÍTICA DE SISTEMA", exc_info=(exc_type, exc_value, exc_traceback))

sys.excepthook = handle_exception