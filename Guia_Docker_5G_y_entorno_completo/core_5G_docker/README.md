# Despliegue del Core 5G Standalone (SA) - Open5GS Contenedorizado

## 1. Introducción y arquitectura del entorno

Este repositorio contiene la infraestructura y los ficheros de configuración necesarios para el despliegue del núcleo de red 5G (**5G Core** o **5GC**) en su modalidad **Standalone (SA)**. La arquitectura implementada se basa en **Open5GS**, una solución de código abierto altamente modular que sigue el paradigma de la **Arquitectura Basada en Servicios (SBA)** estandarizado por el **3GPP**.

Para garantizar la portabilidad, el aislamiento y la escalabilidad del entorno, las **Funciones de Red (NFs)** —tales como el **AMF** (gestión de acceso y movilidad), **SMF** (gestión de sesiones) y **UPF** (plano de usuario)— han sido desplegadas íntegramente a través de contenedores **Docker**. Este enfoque no solo simplifica la orquestación del escenario, sino que permite una integración limpia con la red de acceso radio (**RAN**) y el ecosistema de computación perimetral (**Edge Computing**).

---

## 2. Requisitos previos y preparación del host

Antes de iniciar el despliegue del núcleo de red, es imperativo acondicionar la máquina anfitriona (**host físico**) para garantizar el correcto enrutamiento de los paquetes y el rendimiento exigido por los protocolos de telecomunicaciones.

### 2.1. Dependencias de software

- Motor de **Docker** y **Docker Compose** actualizados.
- Entorno **GNU/Linux** (recomendado **Ubuntu 20.04 LTS** o **22.04 LTS**).

### 2.2. Acondicionamiento del sistema y enrutamiento

Para que las Funciones del Plano de Usuario (**UPF**) puedan cursar tráfico desde los Equipos de Usuario (**UE**) hacia las Redes de Datos (**DN/Internet**), el núcleo del sistema operativo debe permitir el reenvío de paquetes y la traducción de direcciones de red (**NAT**).

Ejecuta los siguientes comandos con privilegios de administrador:

```bash
# Deshabilitar el firewall para evitar el bloqueo del tráfico GTP-U y de señalización
sudo ufw disable

# Habilitar el reenvío de paquetes (IP forwarding) en el kernel
sudo sysctl -w net.ipv4.ip_forward=1
sudo sysctl -w net.ipv6.conf.all.forwarding=1

# Maximizar el rendimiento de la CPU (evitando la latencia por gestión de energía)
echo performance | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor

# Reglas NAT para enmascarar el tráfico saliente desde la subred de los UEs (ej. 10.45.0.0/16)
sudo iptables -t nat -A POSTROUTING -s 10.45.0.0/16 ! -o ogstun -j MASQUERADE
```

**Nota:** si previamente se ha instalado Open5GS directamente sobre el sistema (*bare-metal*), es indispensable detener sus servicios utilizando el script proporcionado (`stopOpen5GS_local.sh`) y liberar los puertos con:

```bash
sudo fuser -k 9090/tcp && sudo fuser -k 3000/tcp
```

Esto evita conflictos de direccionamiento con los contenedores.

---

## 3. Estructura de ficheros y configuración central

El despliegue está orquestado principalmente por dos ficheros clave situados en el directorio raíz del entorno:

### `.env` - Variables de entorno globales

Actúa como el centro neurálgico de la configuración. Define:

- El direccionamiento IP de cada contenedor dentro de la subred aislada de Docker.
- Las credenciales por defecto.
- Los identificadores de la red móvil (**MCC** y **MNC**).
- Las llaves criptográficas de las tarjetas **SIM**.

Su modificación permite adaptar rápidamente la maqueta sin alterar el código fuente de los contenedores.

### `sa-deploy.yaml` - Manifiesto de Docker Compose

Archivo que define la topología de la red. Orquesta la instanciación de:

- Las bases de datos (**MongoDB**).
- La Interfaz Gráfica de Gestión (**WebUI**).
- Todas las **NFs** del **5G Core**.

Además, dentro de las carpetas específicas, por ejemplo `amf/`, `smf/` y `upf/`, se encuentran los archivos `.yaml` propios de Open5GS, donde se declaran las políticas de enrutamiento y las capacidades de segmentación.

---

## 4. Gestión de Network Slicing y calidad de servicio

Uno de los pilares de este despliegue es la capacidad de ofrecer segmentación de red (**Network Slicing**). La configuración se aborda desde dos frentes:

### Configuración estructural

En los ficheros `amf.yaml` y `smf.yaml` se definen las porciones de red permitidas (**S-NSSAI**), combinando el Tipo de Servicio (**SST**, por ejemplo `1` para eMBB, `2` para URLLC) con su Diferenciador (**SD**).

Simultáneamente, el `smf.yaml` vincula estos identificadores a Nombres de Red de Datos (**DNN/APN**) específicos, tales como `internet` o `mec-local`.

### Configuración de suscriptores

El aprovisionamiento de las tarjetas **SIM** y las políticas de Calidad de Servicio (**QoS**) se gestionan a través de la Interfaz Web. Desde esta plataforma se asignan los perfiles **5QI** (*5G QoS Identifier*), garantizando el ancho de banda (**GBR/MBR**) y los niveles de prioridad y retención (**ARP**) para cada sesión PDU.

---

## 5. Puesta en marcha del entorno

Una vez ajustadas las variables en el archivo `.env`, el despliegue del núcleo se realiza mediante un único comando que descarga las imágenes preconstruidas y levanta los servicios en segundo plano:

```bash
# Despliegue de la arquitectura Standalone
sudo docker compose -f sa-deploy.yaml up -d
```

Para verificar que todos los servicios han iniciado correctamente y no existen caídas por errores de configuración, se pueden monitorizar los registros:

```bash
# Verificación de logs de una función específica (ejemplo: AMF)
sudo docker logs amf --tail=50
```

El acceso al Panel de Gestión de Suscriptores (**WebUI**) se realiza a través del navegador apuntando a la dirección del host definida en el despliegue, generalmente `http://<DOCKER_HOST_IP>:9999` o puerto `3000`, utilizando las credenciales por defecto:

```text
admin / 1423
```

---

## 6. Observabilidad y monitorización integral

Dada la complejidad inherente a las arquitecturas 5G distribuidas, se ha integrado de forma nativa un ecosistema de monitorización avanzado, adherido directamente al manifiesto `sa-deploy.yaml`, con el fin de obtener un control completo sobre el rendimiento y la estabilidad del sistema.

### Prometheus y cAdvisor

Encargados de la recolección activa de métricas. Mediante la `infoAPI` nativa de Open5GS en el puerto `9090`, extraen datos vitales como el número de UEs conectados, flujos GTP activos y consumo de recursos (**CPU/RAM**) de cada contenedor.

### Grafana

Interfaz de visualización de datos unificada, disponible en el puerto `3000`. Se han diseñado dashboards personalizados que correlacionan la carga del sistema con el rendimiento de la red.

### Loki y Promtail

Promtail actúa como agente pasivo leyendo el directorio mapeado localmente (`./log:/var/log/open5gs`), enviando las trazas de texto a Loki. Esto permite auditar la señalización **NGAP/PFCP** y diagnosticar errores desde el propio panel de Grafana sin interactuar con la consola de los contenedores.

### Jaeger

Herramienta de trazabilidad distribuida que mapea el recorrido temporal de la señalización a través de la arquitectura SBA del núcleo, crucial para medir retardos en el establecimiento de sesiones.

---

## 7. Detención y limpieza del escenario

Para detener el núcleo de red de forma segura, liberando las interfaces de red virtuales y el direccionamiento asignado, se ejecutará la directiva de apagado de Compose:

```bash
# Detención de la infraestructura y eliminación de contenedores temporales
sudo docker compose -f sa-deploy.yaml down

# (Opcional) Limpieza de datos persistentes o errores de caché en contenedores concretos
sudo docker compose -f sa-deploy.yaml rm -f smf upf
```

---

## 8. Contexto del proyecto

Este documento forma parte de los anexos y la fundamentación técnica del Trabajo Fin de Grado enfocado en el despliegue de soluciones de **Edge Computing** y **Network Slicing** sobre infraestructura 5G privada.