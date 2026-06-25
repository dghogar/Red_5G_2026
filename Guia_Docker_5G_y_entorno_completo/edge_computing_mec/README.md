# Despliegue de la Plataforma Edge Computing (ETSI MEC) y Descubrimiento de Servicios

## 1. Introducción y arquitectura del entorno

Este directorio contiene la implementación completa de la infraestructura de **Computación Perimetral Multi-Acceso (MEC)**, diseñada para integrarse con una red **5G Standalone (SA)**. El objetivo principal de este módulo es acercar los servicios computacionales al usuario final, minimizando la latencia y dotando a la red de un mecanismo inteligente de descubrimiento de servicios y enrutamiento dinámico.

Ante la complejidad de implementar la función **EASDF** (*Edge Application Server Discovery Function*, 3GPP Release 17) de forma puramente nativa en núcleos *open source* como **Open5GS**, se ha diseñado una arquitectura híbrida **Push & Proxy** orientada a eventos. Esta solución utiliza el plano de control definido por los estándares **ETSI MEC** y emula el comportamiento del plano de datos en el equipo cliente mediante técnicas de intercepción DNS y **Redes Definidas por Software (SDN)**.

La plataforma se apoya en dos pilares normativos del ETSI:

- **ETSI GS MEC 011 (App Enablement API)**: actúa como el catálogo central o registro de servicios donde las aplicaciones Edge se inscriben e inyectan sus reglas DNS.
- **ETSI GS MEC 012 (Radio Network Information API - RNIS)**: funciona como el componente de inteligencia de red, extrayendo información sobre la conexión física de los equipos de usuario (**UE**) para desencadenar el envío proactivo del catálogo de servicios.

---

## 2. Estructura del repositorio y componentes clave

El ecosistema MEC se divide modularmente en las siguientes carpetas y ficheros principales:

- **`docker-compose.yml`**: manifiesto principal que orquesta los contenedores base de la plataforma MEC, incluyendo el servidor **MEP MEC011**, el servidor **RNIS MEC012** y el **proxy inverso NGINX**.
- **`011/tfg_mec_project/`**:
  - **`mep_server/`**: contiene el código autogenerado mediante OpenAPI del servidor MEC011. Se ha incorporado una base de datos en memoria para registrar aplicaciones y reglas DNS, por ejemplo en `application_support_controller.py`.
  - **`mep_app/`**: alberga los microservicios Edge, como `mi_app_edge_v4.py`. Estos scripts interactúan con el MEC011 para darse de alta mediante `POST` e inyectar su dominio mediante `PUT` al arrancar, capturando señales del sistema como `SIGINT` para un apagado limpio.
- **`012/rnis_server/`**: contiene la API estandarizada de información de radio.
- **`edge_router_controller.py`**: orquestador SDN de NGINX. Actúa como el cerebro del enrutamiento, monitoriza el MEC011 y, al detectar nuevas reglas DNS, reescribe dinámicamente `nginx.conf` y reinicia el proxy en caliente sin corte de servicio.
- **`rnis_mec012.py`** o **`rnis_eventos.py`**: controlador reactivo que recibe eventos de conexión desde la pasarela (**eUPF**), consulta las aplicaciones activas y realiza un **push HTTP POST** al dispositivo móvil.
- **`ue_client/`**: simuladores de interacción para el plano de datos del cliente. Incluye simuladores para entornos Linux nativos (`simulador_ue_web.py`) y entornos restringidos como Android mediante Termux (`simulador_edge_proxy.py`).

---

## 3. Requisitos previos

Antes de desplegar la plataforma, verifica que el entorno cumple los siguientes requisitos:

### Entorno de red

La máquina designada como servidor Edge (**DN/GW**) debe tener:

- conectividad L3 hacia la red del **eUPF**
- visibilidad hacia las IPs asignadas a los equipos de usuario, por ejemplo la subred `10.47.0.0/16`

### Dependencias base

- **Python 3.10+**
- **pip**
- **Docker**
- **Docker Compose**

### Librerías Python

Instala las dependencias indicadas en `requirements.txt`. Si necesitas hacerlo manualmente:

```bash
pip install flask requests urllib3
```

---

## 4. Instrucciones de despliegue y puesta en marcha

El despliegue debe seguir un orden estricto para garantizar que la plataforma de control esté operativa antes de que el tráfico empiece a fluir.

### Paso 1: Inicialización de la infraestructura Core MEC

En la máquina que actúa como servidor Edge (**DN/GW**), levanta los contenedores base definidos en el archivo Compose. Esto iniciará las APIs **MEC011**, **MEC012** y el servicio **NGINX** inicialmente vacío.

```bash
cd edge_computing_mec/APIs_MEC/
sudo docker compose up --build -d
```

Para comprobar el estado de los contenedores:

```bash
sudo docker ps
```

### Paso 2: Ejecución de los controladores lógicos

Estos scripts actúan como *daemons* en segundo plano, enlazando la lógica de los microservicios. Se recomienda abrir terminales independientes o utilizar herramientas como `tmux` o `screen`.

```bash
# 1. Levantar el orquestador dinámico de NGINX
sudo python3 edge_router_controller.py

# 2. Levantar el controlador de eventos RNIS (webhook listener en el puerto 6000)
python3 rnis_mec012.py
```

### Paso 3: Activación del sensor en la eUPF

Para evitar arquitecturas de *polling* que degradan el espectro, se emplea una arquitectura 100% orientada a eventos. En la máquina donde reside el **eUPF**, ejecuta el script vigía. Este proceso analiza en tiempo real los mensajes **PFCP Session Establishment Request** extraídos de los logs de Docker y notifica al RNIS.

```bash
python3 log_watcher.py
```

### Paso 4: Despliegue de una aplicación Edge

Enciende manualmente uno de los microservicios Edge. La aplicación se registrará automáticamente en el **MEC011**, y el orquestador detectará el cambio para actualizar NGINX de forma transparente.

```bash
python3 011/tfg_mec_project/mep_app/mi_app_edge_v3.py
```

### Paso 5: Configuración de los equipos de usuario (UE)

Dependiendo del entorno del usuario, el procesamiento del *push* y la resolución DNS cambian.

#### Opción A: Cliente nativo Linux

El script recibe el *push* con el catálogo y modifica el archivo `/etc/hosts` con privilegios de administrador, logrando una interceptación DNS a nivel de sistema operativo.

```bash
cd ue_client/
sudo python3 simulador_ue_web.py
```

Accede a la siguiente URL en el navegador para visualizar el dashboard interactivo:

```text
http://localhost:9090
```

#### Opción B: Cliente Android mediante Termux

Debido a las restricciones de seguridad de Android, que impiden modificar `/etc/hosts` sin acceso *root*, se utiliza un servidor proxy local que intercepta el tráfico HTTP.

1. Instala **Termux** en Android.
2. Instala Python dentro de Termux:

```bash
pkg install python
```

3. Transfiere y ejecuta el script:

```bash
python simulador_edge_proxy.py
```

4. En los ajustes de **APN** de la conexión 5G de Android, establece el proxy manual a `127.0.0.1` y puerto `8080`.

También pueden utilizarse extensiones como **SwitchyOmega** en navegadores compatibles.

Accede después a:

```text
http://127.0.0.1:9090
```

---

## 5. Resumen del flujo de interacción

Una vez completados los pasos anteriores, la dinámica de la red operará de la siguiente manera:

1. El módem 5G establece conexión con el núcleo **Open5GS**.
2. El **eUPF** enruta la **PDU Session** y asigna la IP, por ejemplo `10.47.0.X`.
3. El sensor `log_watcher.py` detecta el evento instantáneamente y lo envía al **RNIS**.
4. El RNIS consulta al **MEC011**, encapsula la IP en una estructura de datos estándar ETSI (`AssociateId`, `type=1`) y realiza un *push* silencioso hacia el puerto `9090` del UE.
5. El dashboard del usuario se actualiza automáticamente mostrando el botón de acceso directo a la aplicación, por ejemplo `video3.edge.tfg`.
6. El usuario consume el servicio redirigido a través de la infraestructura local, demostrando la eficacia del **Edge Computing** en la minimización de la latencia y la optimización del enrutamiento.

---

## 6. Notas operativas

- Ejecuta los controladores en terminales separadas para simplificar la depuración.
- Verifica que el proxy NGINX puede recargarse sin errores tras cada modificación dinámica del archivo `nginx.conf`.
- Comprueba que la máquina Edge tiene alcance IP real hacia la subred de UEs.
- Si el *push* no llega al cliente, revisa el puerto `9090`, las reglas de firewall y la conectividad entre el RNIS y el dispositivo final.
- En pruebas con Android, recuerda que la interceptación DNS no es nativa y depende del proxy configurado en el APN o navegador.
