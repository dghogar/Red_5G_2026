\# 🐳 Guía de Despliegue: Core 5G, Edge Computing, Monitorización y UGV

Este directorio (`Guia\_Docker\_5G\_y\_entorno\_completo`) conforma el núcleo virtualizado del Trabajo de Fin de Grado. Aquí se concentra toda la orquestación basada en contenedores (Docker) necesaria para levantar la red 5G Standalone, los servicios de Edge Computing (MEC), el sistema de observabilidad y el caso de uso práctico con el vehículo autónomo.

A diferencia del despliegue centrado puramente en la radio, esta guía proporciona el ecosistema completo para habilitar capacidades avanzadas como \*\*Network Slicing\*\* y \*\*eBPF/XDP\*\* en el plano de usuario.

\---

\## 📂 Mapa del Directorio

El entorno está modularizado en las siguientes carpetas y archivos clave:

* 🗺️ \*\*`ARQ\_entorno.jpg` / `arq\_interfaces\_open5gs.jpeg`\*\*: Esquemas visuales y diagramas de arquitectura de red que detallan las interfaces SBA del núcleo Open5GS y la topología física/lógica del despliegue.
* 🧠 \*\*`core\_5G\_docker/`\*\*: Archivos `docker-compose.yaml` y configuraciones `.env` para instanciar las funciones del plano de control del núcleo 5G (AMF, SMF, UDM, UDR, NRF, PCF, etc.) basadas en Open5GS.
* ⚡ \*\*`eUPF/`\*\*: Despliegue especializado de la \*User Plane Function\* utilizando la tecnología eBPF/XDP (`edgecomllc/eupf`). Incluye la configuración que permite a esta función procesar paquetes directamente en el kernel de Linux para lograr latencias ultrabajas.
* ☁️ \*\*`edge\_computing\_mec/`\*\*: Implementación en Python de las APIs del estándar ETSI MEC. Incluye el servidor MEC011 (registro de apps), MEC012 (RNIS / notificaciones de radio), el orquestador dinámico y el proxy inverso NGINX que enruta el tráfico Edge.
* 📊 \*\*`monitorizacion/`\*\*: Stack completo de observabilidad. Incluye Prometheus (recolección de métricas), Grafana (dashboards visuales), cAdvisor (rendimiento de contenedores) y Promtail/Loki para la trazabilidad centralizada de los logs del sistema 5G.
* 📡 \*\*`ran\_srsran/`\*\*: Configuraciones avanzadas (archivos `.conf` y `.yml`) para levantar la estación base gNodeB física utilizando radios definidas por software (SDR) mediante srsRAN.
* 💻 \*\*`ueransim/`\*\*: Entorno de simulación de gNodeB y Equipos de Usuario (UEs). Vital para probar escenarios de \*\*Network Slicing\*\* (múltiples UEs conectándose a distintas subredes) sin depender del hardware de radio físico.
* 📲 \*\*`scripts\_modem\_sim/`\*\*: Conjunto de herramientas, comandos AT/QMI y rutinas de `pySim` destinadas al aprovisionamiento criptográfico de las tarjetas SIM físicas y la configuración del módem M.2.
* 🤖 \*\*`UGV\_BEAST/`\*\*: \*[Nota: Debido a su volumen, el código fuente completo no se detalla en los anexos en bruto]\*. Esta carpeta contiene el servidor web de baja latencia y el script controlador del robot físico (Vehículo Terrestre No Tripulado). Representa el \*\*caso de uso final\*\*: teleoperación asíncrona y transmisión de vídeo en tiempo real aprovechando el Edge Computing para evadir el CG-NAT y la latencia de las redes públicas.

\---

\## 🚀 Requisitos Previos del Host

Antes de levantar la infraestructura, el servidor anfitrión (Linux/Ubuntu) debe estar preparado para soportar el enrutamiento de paquetes y los permisos de eBPF:

1. \*\*Habilitar IP Forwarding:\*\* Obligatorio para que los contenedores del Core y la eUPF puedan enrutar el tráfico hacia Internet o hacia la red MEC local.

\```bash

sudo sysctl -w net.ipv4.ip\_forward=1

Permisos eBPF: El despliegue de la eUPF requiere montar el sistema de archivos bpf y permisos de administrador de red. Esto ya está contemplado en su respectivo docker-compose.yaml mediante privileged: true y los volúmenes /sys/fs/bpf.

🛠️ Flujo de Despliegue Recomendado

Para evitar condiciones de carrera (servicios intentando conectar a bases de datos o funciones de red no inicializadas), se recomienda el siguiente orden de ejecución:

1. Despliegue del Core 5G y eUPF

Levantar las bases de datos (MongoDB) y el plano de control (AMF, SMF...), seguido del plano de usuario de alto rendimiento:

Bash

cd core\_5G\_docker

sudo docker compose -f sa-deploy.yaml up -d

(Nota: Si se están probando escenarios de Network Slicing, utilizar los yaml específicos que levantan múltiples instancias de SMF/UPF).

1. Monitorización y Observabilidad

Para comenzar a recopilar métricas desde el minuto cero:

Bash

cd monitorizacion

sudo docker compose up -d

Acceso a Grafana: http://<IP\_HOST>:3000

1. Edge Computing (MEC)

Inicie el entorno perimetral que vigilará la conexión de los usuarios y habilitará los servicios locales:

Bash

cd edge\_computing\_mec

sudo docker compose up -d

python3 edge\_router\_controller.py

1. Conexión de la Radio y UEs

Dependiendo de si se realiza una prueba física o simulada:

Simulada: Navegar a ueransim/ y levantar primero la nr\_gnb, seguida de los nr\_ue.

Física: Navegar a ran\_srsran/, levantar el contenedor de la antena y encender el módem físico configurado con los scripts de scripts\_modem\_sim/.

1. Activación del UGV BEAST (Caso de Uso)

Una vez el módem integrado en el robot adquiera una IP del túnel 5G (ej. 10.47.0.X), inicie el servidor de control en la carpeta UGV\_BEAST/. El sistema MEC detectará la presencia del robot y actualizará el DNS de los clientes (UEs) conectados para permitir la teleoperación y recepción de vídeo con latencias mínimas.


\*\*\*

\### Notas sobre el diseño de este README:

1. \*\*Explicación del UGV:\*\* Está destacado como el \*\*caso de uso final\*\* del proyecto. La mención que he puesto (\*"[Nota: Debido a su volumen, el código fuente completo no se detalla en los anexos en bruto]"\*) justifica perfectamente ante el tribunal por qué esa carpeta concreta existe en las capturas pero no se detalla línea a línea en el anexo de código de tu memoria.
1. \*\*Orden lógico:\*\* El "Flujo de Despliegue Recomendado" explica el porqué de cada componente y de cada proceso
