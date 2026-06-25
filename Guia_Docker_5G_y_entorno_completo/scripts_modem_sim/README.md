# Despliegue del Cliente UE (Módem 5G) - Emulación de Enrutamiento DNS y Descubrimiento MEC

## 1. Introducción y propósito del módulo

Este directorio alberga los scripts operativos diseñados para ejecutarse en el equipo anfitrión (ordenador personal) que proporciona conectividad física a la red 5G mediante el módem industrial, por ejemplo **Telit Wireless FN990A28**.

En el marco de la arquitectura híbrida **Push & Proxy** diseñada para este proyecto, el **Equipo de Usuario (UE)** actúa como un ente pasivo y reactivo. En lugar de emplear un modelo de sondeo continuo (*polling*) que degradaría la eficiencia energética y espectral, este módulo instancia un servidor de escucha local. Su objetivo es recibir notificaciones asíncronas (*Push*) directamente desde la inteligencia de red (**MEC012/RNIS**) en el momento exacto en que el dispositivo establece la sesión PDU o cuando varían los servicios perimetrales disponibles.

---

## 2. Estructura de ficheros y lógica de funcionamiento

El directorio `ue_client` contiene las herramientas que actúan como la terminación del plano de control en el extremo del cliente:

- **`simulador_ue.py`**: implementación base mediante interfaz de línea de comandos (**CLI**). Muestra por consola los eventos de red recibidos.
- **`simulador_ue_web.py`**: implementación principal y recomendada. Integra un servidor web (**Flask**) que proporciona un *dashboard* visual interactivo para el usuario final, a la vez que gestiona en segundo plano las reglas de enrutamiento.

### 2.1. Mecanismo de intercepción transparente (simulación de EASDF)

Para que el usuario pueda acceder a los servicios perimetrales utilizando nombres de dominio, por ejemplo `http://video3.edge.tfg`, sin requerir un servidor DNS centralizado ni la función nativa **EASDF** del núcleo **3GPP Release 17**, los scripts implementan un mecanismo de inyección local:

1. El script recibe el catálogo JSON mediante `POST /notificacion_edge` con las direcciones IP del nodo Edge y los dominios de las aplicaciones.
2. Inmediatamente invoca la función `sincronizar_hosts()`, que edita dinámicamente el archivo `/etc/hosts` del sistema operativo Linux.
3. Las reglas inyectadas se etiquetan con un marcador seguro `# --- EDGE COMPUTING TFG ---` para poder ser purgadas instantáneamente si la red notifica que una aplicación perimetral ha sido apagada o si el UE abandona la celda.

---

## 3. Requisitos previos del sistema

### Conectividad 5G

El módem Telit debe estar conectado y haber negociado con éxito una sesión PDU, con un túnel GTP activo y una IP válida asignada por la eUPF, por ejemplo en la subred `10.47.0.x`.

### Entorno Python 3

```bash
sudo apt update
sudo apt install python3 python3-pip
```

### Dependencias del script

```bash
pip3 install flask requests
```

---

## 4. Instrucciones de despliegue y ejecución

La ejecución de este módulo es el último eslabón en la cadena de demostración del ecosistema MEC.

> **Importante:** el script debe ejecutarse con privilegios de administrador (`sudo`). Esta elevación de privilegios es estrictamente necesaria, ya que el proceso en segundo plano debe contar con permisos de escritura sobre el fichero de sistema `/etc/hosts` para aplicar las políticas de enrutamiento de red.

```bash
cd scripts_modem_sim/ue_client/

# Ejecutar el simulador con interfaz gráfica web
sudo python3 simulador_ue_web.py
```

Al ejecutarse, la terminal indicará que el servidor se encuentra a la escucha en el puerto `9090` y accesible en `0.0.0.0`.

---

## 5. Validación práctica y experiencia de usuario

Para validar la correcta orquestación de la arquitectura MEC, el usuario debe realizar los siguientes pasos:

1. **Apertura del panel de control**: manteniendo el script en ejecución, abre un navegador web estándar, por ejemplo Google Chrome o Firefox, y accede a `http://localhost:9090`.
2. **Estado inicial**: el panel mostrará la confirmación de `5G Listo`, pero indicará que está a la espera de notificaciones de la red.
3. **Detonante en el servidor Edge**: en cuanto el administrador de la red despliegue una nueva aplicación, por ejemplo `mi_app_edge_v4.py`, el servidor MEC011 la registrará y el RNIS enviará el *push* a la IP de este módem.
4. **Refresco dinámico**: sin necesidad de recargar la página manualmente, el *dashboard* web del portátil hará una transición en tiempo real, mostrando una **tarjeta de aplicación** con un enlace directo azul, por ejemplo `Abrir video3.edge.tfg`.
5. **Consumo del servicio**: al hacer clic en el enlace, el navegador resolverá el dominio de manera transparente hacia la IP de la plataforma perimetral (**DN/GW**), certificando que la intercepción a nivel de sistema operativo y el descubrimiento de red asíncrono han funcionado con éxito.

---

## 6. Manejo de errores comunes

- **`Permission denied: '/etc/hosts'`**: ocurre si el script se lanzó sin `sudo`. El proceso detendrá su ejecución para salvaguardar la integridad de la prueba.
- **Dominio no carga en el navegador por caché DNS**: si tras hacer clic en la aplicación MEC el navegador muestra un error de resolución, es posible que haya retenido una consulta DNS fallida previa. Se recomienda realizar las pruebas en modo incógnito o purgar la caché DNS interna del navegador, por ejemplo en Chrome mediante `chrome://net-internals/#dns`.