import math
import config

def calc_distancia(lat1: float, lon1: float, lat2: float, lon2: float) -> float:
    phi1 = math.radians(lat1)
    phi2 = math.radians(lat2)
    dphi = math.radians(lat2 - lat1)
    dlambda = math.radians(lon2 - lon1)
    
    a = math.sin(dphi / 2.0)**2 + math.cos(phi1) * math.cos(phi2) * math.sin(dlambda / 2.0)**2
    c = 2.0 * math.atan2(math.sqrt(a), math.sqrt(1.0 - a))
    return config.RADIO_TIERRA_M * c

def calc_rumbo_deseado(lat1: float, lon1: float, lat2: float, lon2: float) -> float:
    phi1 = math.radians(lat1)
    phi2 = math.radians(lat2)
    dlambda = math.radians(lon2 - lon1)
    
    y = math.sin(dlambda) * math.cos(phi2)
    x = math.cos(phi1) * math.sin(phi2) - math.sin(phi1) * math.cos(phi2) * math.cos(dlambda)
    brng = math.degrees(math.atan2(y, x))
    return (brng + 360.0) % 360.0

def punto_en_poligono(lat: float, lon: float, poligono_geojson: list) -> bool:
    if len(poligono_geojson) > 0 and isinstance(poligono_geojson[0], list) and isinstance(poligono_geojson[0][0], list):
        anillo = poligono_geojson[0]
    else:
        anillo = poligono_geojson
        
    adentro = False
    j = len(anillo) - 1
    
    for i in range(len(anillo)):
        lon_i, lat_i = anillo[i][0], anillo[i][1]
        lon_j, lat_j = anillo[j][0], anillo[j][1]
        intersecta = ((lat_i > lat) != (lat_j > lat)) and (lon < (lon_j - lon_i) * (lat - lat_i) / (lat_j - lat_i) + lon_i)
        if intersecta:
            adentro = not adentro
        j = i
    return adentro

def evaluar_zona_prohibida(lat: float, lon: float, zonas: list) -> bool:
    if not zonas: return False
    for zona in zonas:
        if punto_en_poligono(lat, lon, zona): return True
    return False

def proyectar_punto_gps(lat, lon, rumbo, distancia_m):
    """Calcula una coordenada GPS a X metros de distancia en un rumbo dado."""
    dist_rad = distancia_m / config.RADIO_TIERRA_M
    rumbo_rad = math.radians(rumbo)
    lat_rad = math.radians(lat)
    lon_rad = math.radians(lon)

    new_lat = math.asin(math.sin(lat_rad) * math.cos(dist_rad) +
                        math.cos(lat_rad) * math.sin(dist_rad) * math.cos(rumbo_rad))
    new_lon = lon_rad + math.atan2(math.sin(rumbo_rad) * math.sin(dist_rad) * math.cos(lat_rad),
                                   math.cos(dist_rad) - math.sin(lat_rad) * math.sin(new_lat))
    
    return math.degrees(new_lat), math.degrees(new_lon)