# modulos/video_server.py
from flask import Flask, request, jsonify
from flask_cors import CORS
import threading
import asyncio
from aiortc import RTCPeerConnection, RTCSessionDescription
from modulos.camera_stream import UGVVideoTrack
from modulos.logger import log
import logging

log_flask = logging.getLogger('werkzeug')
log_flask.setLevel(logging.ERROR)

app = Flask(__name__)
CORS(app)

cam_streamer_ref = None
pcs = set()
webrtc_loop = asyncio.new_event_loop()

def start_webrtc_loop(loop):
    asyncio.set_event_loop(loop)
    loop.run_forever()

threading.Thread(target=start_webrtc_loop, args=(webrtc_loop,), daemon=True).start()

async def procesar_oferta_webrtc(params):
    offer = RTCSessionDescription(sdp=params["sdp"], type=params["type"])
    pc = RTCPeerConnection()
    pcs.add(pc)

    @pc.on("connectionstatechange")
    async def on_connectionstatechange():
        if pc.connectionState in ["failed", "closed", "disconnected"]:
            await pc.close()
            pcs.discard(pc)

    if cam_streamer_ref:
        pc.addTrack(UGVVideoTrack(cam_streamer_ref))

    await pc.setRemoteDescription(offer)
    answer = await pc.createAnswer()
    await pc.setLocalDescription(answer)

    return {"sdp": pc.localDescription.sdp, "type": pc.localDescription.type}

@app.route('/offer', methods=['POST'])
def offer():
    params = request.json
    future = asyncio.run_coroutine_threadsafe(procesar_oferta_webrtc(params), webrtc_loop)
    return jsonify(future.result())

def iniciar_micro_servidor(camara):
    global cam_streamer_ref
    cam_streamer_ref = camara
    log.info("[VÍDEO] 🎥 Micro-peaje WebRTC iniciado en puerto 5000.")
    app.run(host='0.0.0.0', port=5000, debug=False, use_reloader=False)