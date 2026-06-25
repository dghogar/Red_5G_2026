"""
PROYECTO: Red de Conectividad de Drones Terrestres (UGV) en Situaciones de Emergencia.
MÓDULO: Pasarela de Vídeo Asíncrona (video_gateway.py)
DESCRIPCIÓN: Aísla la carga de red del streaming de vídeo. Recibe el tráfico 
             UDP del dron y lo retransmite eficientemente a los clientes web
             conectados mediante WebSockets puros.
"""

import asyncio
import websockets
import socket
import threading

clientes_video = set()

async def despachador_web(websocket):
    """Mantiene la conexión abierta con los clientes web que visualizan el vídeo."""
    clientes_video.add(websocket)
    try:
        await websocket.wait_closed()
    finally:
        clientes_video.remove(websocket)

async def receptor_udp_video():
    """
    Receptor de alto rendimiento. Lee los datagramas UDP (MPEG-TS) provenientes
    de la red MANET y los envía de forma asíncrona a todos los navegadores conectados.
    """
    loop = asyncio.get_running_loop()
    sock_vid = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock_vid.bind(("0.0.0.0", 5000))
    sock_vid.setblocking(False)
    
    while True:
        data = await loop.sock_recv(sock_vid, 65535)
        if clientes_video:
            # Multidifusión a todos los iframes activos
            await asyncio.gather(*[cliente.send(data) for cliente in clientes_video], return_exceptions=True)

async def motor_video():
    """Levanta el servidor de WebSockets en el puerto 5001."""
    async with websockets.serve(despachador_web, "0.0.0.0", 5001):
        await receptor_udp_video()

def arrancar_bucle_video():
    """Punto de entrada bloqueante para el hilo asíncrono."""
    asyncio.run(motor_video())

def iniciar_gateway():
    """Función de inicialización que será llamada desde estacion_tierra.py"""
    threading.Thread(target=arrancar_bucle_video, daemon=True).start()
    print("[VÍDEO] Video Gateway independiente iniciado (UDP:5000 -> WS:5001)")