# Despliegue de Misión Crítica: Teleoperación del Vehículo Autónomo Terrestre (UGV BEAST) sobre 5G y MEC

## 1. Introducción y arquitectura del caso de uso

Este directorio contiene el código fuente, los archivos de configuración y los recursos estáticos necesarios para la operación telemétrica y cinemática del robot terrestre **UGV BEAST** (*Unmanned Ground Vehicle*). El ecosistema de software se compone de dos grandes bloques arquitectónicos:

### Estación de control terrestre (GCS / servidor Edge)

Implementada en la pasarela perimetral (**DN/GW**). Proporciona una interfaz gráfica de usuario (**GUI**) mediante tecnologías web (**HTML/JS/Leaflet**) y actúa como el cerebro táctico, gestionando el enrutamiento de vídeo de baja latencia mediante el microservicio `video_gateway.py` y la telemetría bidireccional vía **WebSockets**.

### Nodo robótico terrestre (UGV BEAST)

El vehículo autónomo actúa como cliente dentro de la red 5G. Su unidad de procesamiento embebida (**Raspberry Pi**) captura flujos de vídeo y datos geoespaciales (**GPS/IMU/LiDAR**), transmitiéndolos hacia la infraestructura Edge, al mismo tiempo que recibe y ejecuta comandos cinemáticos de actuación.

La validación de este caso de uso demuestra de manera empírica las ventajas operativas de la integración entre las redes **5G Privadas** y la **Computación Perimetral (MEC)**, concretamente en la drástica reducción de latencia, crítica para la teleoperación, y la mitigación de los problemas de enrutamiento asociados al **Carrier-Grade NAT (CG-NAT)**.

---

## 2. Topología de conectividad móvil: pasarela lógica

Dadas las restricciones físicas del chasis del robot y la necesidad de emplear dispositivos **COTS (Commercial Off-The-Shelf)** para el acceso a la red, se ha implementado un esquema de conectividad basado en una pasarela lógica, con un **smartphone Android** actuando como UE.

Para que la Raspberry Pi del UGV BEAST pueda establecer comunicación bidireccional con el entorno 5G a través del dispositivo móvil, se requiere la configuración de un túnel **USB Tethering** y la herramienta `socat` para la retransmisión de puertos.

### 2.1. Preparación de la interfaz virtual en el UGV

El robot debe reconocer la interfaz virtual creada al conectar el teléfono inteligente mediante USB y asignarse una dirección IP estática dentro de la subred del terminal.

En la Raspberry Pi, edita el fichero de configuración DHCP:

```bash
sudo nano /etc/dhcpcd.conf
```

Añade la siguiente configuración:

```conf
interface usb0
static ip_address=192.168.42.10/24
static routers=192.168.42.129
static domain_name_servers=8.8.8.8
```

> **Nota:** la dirección del router `192.168.42.129` es la IP estándar asignada por los sistemas Android a la pasarela de anclaje USB.

### 2.2. Enrutamiento inverso en el terminal móvil

Para que el tráfico originado en el servidor Edge alcance los puertos de escucha de la Raspberry Pi a través de la interfaz WAN 5G del móvil, se requiere la utilidad `socat`.

Instala la consola de emulación **Termux** en el dispositivo Android y después instala `socat`:

```bash
pkg update && pkg upgrade
pkg install socat
```

Ejecuta la regla de redirección de puertos. El siguiente comando instruye al móvil para que escuche en su puerto `5000` (interfaz 5G) y reenvíe el tráfico hacia la IP de la Raspberry Pi en el túnel USB:



```bash
#Scripts a modificar su direccion IP y direcciones IP que debemos de utilizar para poder intercomunicar el servidor-UGV_BEAST a traves del movil conectado al UGV_BEAST
#TAMBIEN ES POSIBLE QUE LA UTILIZACION DE DIRECCIONES IP ESTATICAS FALLE Y HAYA QUE MODIFICARLA CADA VEZ QUE CONECTAMOS EL MOVIL AL UGV BEAST A TRAVES DEL USB
#IP_ANDROID_ROBOT = camera_stream, manet_node
#IP_ANDROID_5G = estacion_tierra

socat UDP4-LISTEN:65432,bind=<IP_ANDROID_ROBOT>,fork UDP4:192.168.0.120:65432 & # Escucha UDP (telemetria) USB y lo envía a Servidor Web
socat UDP4-LISTEN:5000,bind=<IP_ANDROID_ROBOT>,fork UDP4:192.168.0.120:5000& # Escucha UDP (camara) USB y lo envía a Servidor Web 
socat UDP4-LISTEN:65432,bind=<IP_ANDROID_5G>,fork UDP4:<IP_ROBOT>:65432 & & # Escucha cara red 5G móvil y las manda por cable al robot 
```

Este procedimiento debe replicarse para cualquier puerto adicional requerido para la transmisión de vídeo o telemetría.

---

## 3. Estructura de ficheros del ecosistema de software

El entorno de software está altamente modularizado. A continuación, se detallan los ficheros críticos.

### 3.1. Núcleo de control y comunicaciones

- **`estacion_tierra.py`**: script principal de la GCS. Instancia el servidor Flask en el puerto `80`, orquesta las comunicaciones bidireccionales mediante **Flask-SocketIO** y sirve el dashboard táctico.
- **`video_gateway.py`**: microservicio de pasarela de vídeo. Desacopla la ingesta intensiva de datagramas UDP (**MPEG-TS**) provenientes de la cámara del robot y los retransmite asíncronamente a los clientes web conectados mediante **WebSockets**, evitando el bloqueo del bucle principal de control.
- **`app_ugv.py`**: script residente en la Raspberry Pi del UGV. Orquesta la inicialización de los sensores, captura el flujo de vídeo y ejecuta las primitivas de movimiento.

### 3.2. Configuraciones dinámicas

- **`config.yaml`**: archivo centralizado de parámetros. Define las direcciones IP, puertos de comunicación, *timeouts* y parámetros de calibración cinemática, como límites de velocidad y offsets del servo.
- **`config.py`**: gestor de estado en memoria que inicializa la configuración predeterminada en caso de ausencia del fichero YAML.

### 3.3. Módulos de procesamiento y frontend

- **`/modulos/`**: contiene la lógica de negocio subdividida. Ejemplos: `robot_ctrl.py` para la interfaz con los controladores de motores HAT, `video_server.py` para el *pipeline* GStreamer y `gps_imu.py` para la fusión de datos sensoriales.
- **`/js/` y `/css/`**: capa de presentación visual (*frontend*). El fichero `script_main.js` es fundamental, ya que maneja la descompresión del vídeo en el navegador con `jsmpeg.min.js`, gestiona los *inputs* del usuario por teclado o joystick y actualiza el mapa interactivo con **Leaflet**.

---

## 4. Instrucciones de despliegue operativo

Para la correcta validación del caso de uso, se debe seguir un orden estricto de encendido que garantice el establecimiento del canal de radio y la disponibilidad de los servicios lógicos.

### Fase 1: levantamiento de la estación terrestre

En la máquina de la infraestructura MEC, por ejemplo `192.168.0.120`, inicializa los componentes del servidor. El script `estacion_tierra.py` invocará automáticamente al microservicio `video_gateway.py`.

```bash
cd 'Prueba 9'
# Se recomienda usar un entorno virtual con las dependencias instaladas
python3 estacion_tierra.py
```

Verifica que la consola reporte que la **Estación de Tierra Activa** y la **Pasarela de Vídeo** están operativas.

### Fase 2: establecimiento del enlace 5G y configuración de pasarela

1. Conecta el dispositivo móvil Android al UGV BEAST mediante un cable USB.
2. Activa la opción **Anclaje de red por USB (USB Tethering)** en los ajustes del terminal móvil.
3. Asegúrate de que el dispositivo ha negociado correctamente una **PDU Session** en el núcleo Open5GS y dispone de una IP en el plano de usuario, por ejemplo `10.45.0.2`.
4. Abre Termux en el móvil y lanza el *socket forwarding*:

```bash
socat TCP4-LISTEN:5000,fork TCP4:192.168.42.10:5000
```

### Fase 3: arranque de los sistemas del UGV

Una vez establecida la ruta de red, ingresa mediante SSH a la Raspberry Pi del UGV o automatiza su arranque mediante un demonio `systemd`:

```bash
# Conectarse a la Raspberry Pi (contraseña: ws)
ssh ws@192.168.42.10

# Navegar al directorio de trabajo e inicializar el agente robótico
cd 'Prueba 9'
python3 app_ugv.py
```

### Fase 4: operación táctica y visualización

Accede a la interfaz de usuario desde un equipo autorizado dentro de la red, por ejemplo un ordenador portátil cliente, abriendo el navegador web en la dirección IP del servidor Edge, por ejemplo `http://192.168.0.120/`.

Desde el dashboard podrás confirmar el flujo ininterrumpido de vídeo, la recepción de datos telemétricos, como posición, velocidad y estado de sensores, y ejercer el control cinemático sobre el robot, validando de facto los bajísimos índices de latencia garantizados por el paradigma **Edge Computing**.

---

## 5. Notas operativas

- Verifica que el módem o smartphone Android ha obtenido conectividad 5G antes de iniciar la redirección de puertos.
- Si el tráfico no alcanza la Raspberry Pi, comprueba la IP de `usb0`, la regla de `socat` y la conectividad entre DN/GW y el robot.
- Mantén separados el servicio de vídeo y el bucle de control para evitar bloqueos por carga de red.
- Si el dashboard no responde, revisa el puerto `80`, la ejecución de `estacion_tierra.py` y el estado del microservicio `video_gateway.py`.