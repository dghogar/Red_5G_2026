# Despliegue de la Función de Plano de Usuario Perimetral (eUPF) con Aceleración eBPF/XDP

## 1. Introducción y arquitectura

Este directorio contiene la infraestructura, la configuración y las rutinas de despliegue de la **Edge User Plane Function (eUPF)**. A diferencia del **UPF** central tradicional, este componente se despliega en el perímetro de la red (**Edge**) con el objetivo de habilitar el **Local Breakout (LBO)**, derivando el tráfico de datos de los equipos de usuario (**UE**) directamente hacia las aplicaciones **MEC (Multi-Access Edge Computing)** para garantizar una latencia ultrabaja.

### El paradigma eBPF y XDP

El principal hito tecnológico de este nodo radica en la adopción del software `edgecomllc/eupf`, que evita el uso de interfaces de red virtuales tradicionales (**TUN/TAP**) del núcleo Linux. En su lugar, emplea **eBPF (Extended Berkeley Packet Filter)** y **XDP (eXpress Data Path)**.

Esta tecnología inyecta la lógica de procesamiento **GTP-U** directamente en el controlador de la tarjeta de red (**NIC**) física. Los paquetes son interceptados, desencapsulados y enrutados en la propia cola de recepción (**Ingress/RX**) en cuestión de microsegundos, antes de que el sistema operativo asigne memoria para ellos, logrando un rendimiento superior y un consumo mínimo de recursos (**CPU/RAM**).

---

## 2. Requisitos previos y restricciones topológicas

Antes del despliegue, asegúrate de cumplir las siguientes condiciones:

- **Kernel Linux** compatible con **eBPF/XDP**; se recomienda **Ubuntu 20.04 LTS** o superior.
- **Docker** y **Docker Compose** actualizados para la orquestación del contenedor.
- **Topología DN/GW externa** correctamente diseñada.

### Restricción clave de la topología

Debido a la naturaleza asimétrica de **XDP**, que opera exclusivamente en la cola **RX**, el tráfico de respuesta (**downlink**) generado localmente en la máquina de la eUPF por la cola **TX** no es interceptado por el programa eBPF. Como consecuencia, ese tráfico no se encapsularía en **GTP** y terminaría descartado.

Por ello, es estrictamente necesario que las aplicaciones Edge se alojen en una máquina independiente que actúe como **Red de Datos / Pasarela (DN/GW)**, forzando que el tráfico de retorno entre físicamente por el enlace de red hacia la tarjeta de la eUPF.

---

## 3. Estructura de ficheros

Este entorno se apoya en los siguientes archivos principales:

- **`docker-compose.yaml`**: manifiesto de orquestación que define la instanciación de la eUPF.
- **`.env`**: archivo de variables globales. Especifica el direccionamiento IP de la eUPF, del **SMF** del Core y el nombre de la interfaz física de red (`UPF_INTERFACE_NAME`, por ejemplo `eno1`).
- **`eupf_init.sh`**: script de inicialización y punto de entrada del contenedor.
- **`log_watcher.py`**: agente de monitorización pasiva, pieza clave de la arquitectura orientada a eventos para la integración con la plataforma **MEC**.

---

## 4. Decisiones de ingeniería aplicadas

Para garantizar la viabilidad del entorno en este laboratorio, se han aplicado varias decisiones de diseño reflejadas en la configuración.

### Evasión del NAT en PFCP

En despliegues iniciales usando redes `bridge` de Docker, la traducción de direcciones (**SNAT**) enmascaraba la IP de origen de la eUPF en el puerto UDP `8805`. Esto provocaba que el **SMF** central rechazara los mensajes de latido (**Heartbeats**), generando bucles de desconexión continuos con estado **PFCP de-associated**.

Para evitarlo, el archivo `docker-compose.yaml` opera en modo:

```yaml
network_mode: "host"
```

De esta forma se preserva la IP física transparente del nodo perimetral.

### Asignación autónoma de F-TEID

La eUPF se ha configurado para generar sus propios identificadores **F-TEID (Fully Qualified Tunnel Endpoint Identifier)**, notificando al **SMF** que debe delegar esta función en el nodo perimetral.

---

## 5. Instrucciones de despliegue

### Paso 1: Acondicionamiento del host

Habilita el *forwarding* en el sistema operativo y optimiza el rendimiento del procesador para evitar cuellos de botella en **XDP**:

```bash
sudo sysctl -w net.ipv4.ip_forward=1
echo performance | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor
```

Si quieres verificar la interfaz física disponible en el host:

```bash
ip a
```

### Paso 2: Parametrización del entorno

Edita el fichero `.env` para reflejar la topología física real. Presta especial atención a estas variables:

- **`UPF_INTERFACE_NAME`**: nombre de la tarjeta de red física, por ejemplo `eth0` o `eno1`, donde el programa eBPF realizará el *hook*.
- **`UPF_IP`** y **`UPF_ADVERTISE_IP`**: dirección IP física de la máquina eUPF.
- **`SMF_IP`**: dirección IP de la máquina que aloja el **Core 5G**.

### Paso 3: Inicialización

Despliega el contenedor en segundo plano:

```bash
sudo docker compose up -d
```

Para verificar el correcto establecimiento de la asociación **PFCP**, consulta los logs del contenedor:

```bash
sudo docker logs -f eupf-edge
```

Salida esperada:

```text
INFO: PFCP associated [IP_DEL_SMF]:8805
```

---

## 6. Integración MEC: agente `log_watcher.py`

Para evitar técnicas ineficientes como el sondeo activo (**ICMP polling**) en la detección de nuevos dispositivos, se ha diseñado el script `log_watcher.py`.

Este microservicio se ejecuta nativamente en el host de la eUPF y aplica un análisis en tiempo real sobre la salida estándar del contenedor. Cuando intercepta un evento de control **PFCP** (*Session Establishment Request*) y detecta la cadena `UE IPv4 Address:`, extrae la IP asignada al móvil y lanza un **webhook asíncrono (HTTP POST)** hacia el servicio **RNIS (MEC012)** alojado en la **DN/GW**.

### Ejecución del agente

Ejecuta el script en una sesión paralela o mediante `nohup`, `tmux` o `screen`:

```bash
python3 log_watcher.py
```

---

## 7. Enrutamiento estático externo

Para que la topología **MEC** funcione correctamente, recuerda configurar las tablas de rutas tanto en la máquina central (**Core**) como en la pasarela de servicios (**DN/GW**).

En la máquina **DN/GW**, se debe indicar que la subred de los terminales, por ejemplo `10.47.0.0/16`, se alcanza a través de la IP física de este nodo eUPF:

```bash
# Comando a ejecutar en la máquina DN/GW externa
sudo ip route add 10.47.0.0/16 via <IP_FISICA_DE_ESTA_EUPF>
```

Puedes comprobar la tabla de rutas actual con:

```bash
ip route
```

---

## 8. Notas operativas

- Ejecuta `log_watcher.py` en paralelo al contenedor para no perder eventos PFCP de establecimiento de sesión.
- Si la asociación PFCP no se mantiene, revisa primero `SMF_IP`, el modo `host` en Docker y la accesibilidad UDP al puerto `8805`.
- Verifica que la interfaz indicada en `UPF_INTERFACE_NAME` sea la NIC física real y no una interfaz virtual.
- Recuerda que alojar las aplicaciones Edge en la propia máquina de la eUPF rompe el flujo de retorno esperado por la aceleración XDP.
