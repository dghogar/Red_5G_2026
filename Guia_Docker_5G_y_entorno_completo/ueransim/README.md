# Despliegue de la Red de Acceso y Equipos de Usuario Simulados (UERANSIM)

## 1. Introducción y propósito del módulo

Este directorio contiene los manifiestos de orquestación y ficheros de configuración necesarios para el despliegue de **UERANSIM**, un simulador de código abierto de última generación que emula tanto la estación base 5G (**gNodeB**) como los equipos de usuario (**UE**) en arquitecturas **Standalone (SA)**.

En el marco de este proyecto de investigación, UERANSIM cumple un rol crítico que el hardware comercial (**COTS UE** o módems industriales) no puede satisfacer plenamente: la validación determinista del **Network Slicing** y de múltiples flujos **QoS**. A diferencia de los módems físicos, que dependen de políticas **URSP (UE Route Selection Policy)** dictadas por el núcleo para enrutar el tráfico, UERANSIM permite forzar a nivel de configuración la petición explícita de un Slice específico (**SST/SD**) y conectarse simultáneamente a múltiples Redes de Datos (**DNN/APN**), tales como `internet` y `mec-local`.

Este aislamiento permite generar cargas de tráfico sintéticas y evaluar de forma empírica cómo el **5G Core (Open5GS)** y el **Plano de Usuario Perimetral (eUPF)** aplican las reglas de **Calidad de Servicio (PCC Rules)** a cada segmento de red.

---

## 2. Topología y estructura de ficheros

La emulación se ha contenedorizado mediante **Docker** para asegurar la portabilidad y permitir el escalado horizontal, es decir, el lanzamiento de múltiples UEs.

### Manifiestos de despliegue

- **`nr-gnb.yaml`**: orquesta el contenedor de la estación base simulada.
- **`nr-ue.yaml` / `nr-ue2.yaml`**: orquestan los contenedores de los equipos de usuario, permitiendo instanciar terminales aislados que atacarán distintos Slices.

### Ficheros de configuración interna

- **`ueransim-gnb.yaml`**: configuración de la capa de acceso. Define el código de área de seguimiento (**TAC**), los identificadores **PLMN (MCC/MNC)** y las interfaces de comunicación **NGAP** hacia el **AMF** y **GTP-U** hacia el **UPF/eUPF**.
- **`ueransim-ue[n].yaml`**: perfiles criptográficos y lógicos de los UEs. Contienen el **IMSI**, las claves de autenticación (**K**, **OPc**) y la configuración de las sesiones PDU, incluyendo el APN solicitado y los atributos `sst/sd`.

Las variables sensibles, como las credenciales de la SIM simulada y las IPs de la infraestructura, se inyectan dinámicamente en tiempo de ejecución a través del archivo global `.env` heredado del entorno Core.

---

## 3. Requisitos previos y acondicionamiento del host

Dado que UERANSIM creará interfaces de red virtuales, por ejemplo `uesimtun0`, para canalizar el tráfico de datos hacia el túnel 5G, la máquina anfitriona debe permitir el enrutamiento IP y no enmascarar el tráfico intercontenedor.

Ejecuta en la máquina anfitriona del simulador (**RAN**):

```bash
# Habilitar el reenvío de paquetes IPv4
sudo sysctl -w net.ipv4.ip_forward=1

# Deshabilitar el filtrado de ruta inversa (RP Filter) para evitar caídas asimétricas
sudo sysctl -w net.ipv4.conf.all.rp_filter=0
sudo sysctl -w net.ipv4.conf.default.rp_filter=0

# Asegurar que el tráfico generado en las interfaces virtuales no es bloqueado por Docker
sudo iptables -I DOCKER-USER -j ACCEPT
```

### Nota para Edge Computing

Si el gNB simulado se comunica con una eUPF externa, es imperativo asegurar que el host cuenta con las rutas estáticas correctas hacia la subred del plano de usuario, por ejemplo `10.47.0.0/16`.

---

## 4. Instrucciones de despliegue

Para evitar desincronizaciones temporales, que derivan en la liberación del contexto del UE por expiración de temporizadores en el AMF, se debe respetar el siguiente orden de arranque.

### Paso 1: Inicialización de la estación base (gNB)

Se procede a levantar la antena simulada en segundo plano:

```bash
sudo docker compose -f nr-gnb.yaml up -d
```

Se puede verificar que el gNB ha establecido el túnel **SCTP** con el AMF mediante la inspección de los registros:

```bash
sudo docker logs nr_gnb
```

Salida esperada:

```text
NGAP connection established with AMF
```

### Paso 2: Conexión de los equipos de usuario (UE)

Una vez la RAN está operativa, se instancian los dispositivos móviles:

```bash
# Levantar el UE 1 (ej. configurado para Slice eMBB - APN internet)
sudo docker compose -f nr-ue.yaml up -d

# Levantar el UE 2 (ej. configurado para Slice URLLC - APN mec-local)
sudo docker compose -f nr-ue2.yaml up -d
```

---

## 5. Enrutamiento del tráfico y pruebas de concepto

El éxito del registro de un UE en UERANSIM genera una nueva interfaz de red en el contenedor, habitualmente `uesimtun0`. Sin embargo, para forzar que el tráfico, como `ping`, `iperf3` o `HTTP`, fluya obligatoriamente a través de la infraestructura 5G hacia la aplicación Edge, se deben inyectar rutas estáticas dentro del contenedor del UE.

### 5.1. Configuración de tablas de enrutamiento en el UE

Accede a la consola interactiva del UE simulado:

```bash
sudo docker exec -it nr_ue1 bash
```

Una vez dentro, dirige el tráfico hacia la interfaz del túnel 5G:

```bash
# Enrutar el tráfico destinado a la red MEC (ej. 192.168.0.x) por el túnel
ip route add 192.168.0.0/16 dev uesimtun0

# (Opcional) Forzar todo el tráfico de salida por el túnel 5G
ip route add 0.0.0.0/1 dev uesimtun0
```

### 5.2. Generación de tráfico y validación de QoS

Para validar que el Slicing y la QoS configurada en el Core se están aplicando correctamente, se recomienda utilizar herramientas de medición activa desde el contenedor del UE:

```bash
# Prueba de conectividad básica hacia la pasarela Edge (DN/GW)
ping 192.168.0.120

# Inyección de carga de tráfico para comprobar los límites MBR configurados
iperf3 -c 192.168.0.120 -p 5001 --bind <IP_ASIGNADA_AL_UE>
```

---

## 6. Gestión avanzada y diagnóstico (CLI)

UERANSIM incorpora una herramienta de gestión por línea de comandos (**nr-cli**) que resulta invaluable para auditar el estado de la conexión radio y desencadenar eventos de movilidad o desconexión.

Para acceder a las métricas del terminal:

```bash
sudo docker exec -it nr_ue1 bash
cd /UERANSIM/build

# Listar todos los UEs activos e información de su IMSI
./nr-cli --dump

# Entrar en el modo interactivo para un UE específico
./nr-cli imsi-001011234567895
```

Desde este shell interactivo, se pueden ejecutar comandos como `status` para ver los portadores de acceso radio y las PDU Sessions, o `deregister switch-off` para simular un apagado ordenado del terminal y verificar cómo el Core 5G libera los recursos.