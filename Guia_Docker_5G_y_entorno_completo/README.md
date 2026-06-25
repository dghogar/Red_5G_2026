# 🐳 Guía de Despliegue: Core 5G, Edge Computing, Monitorización y UGV

Este directorio (`Guia_Docker_5G_y_entorno_completo`) conforma el núcleo virtualizado del Trabajo de Fin de Grado. Aquí se concentra toda la orquestación basada en contenedores (**Docker**) necesaria para levantar la red 5G Standalone, los servicios de Edge Computing (**MEC**), el sistema de observabilidad y el caso de uso práctico con el vehículo autónomo.

A diferencia del despliegue centrado puramente en la radio, esta guía proporciona el ecosistema completo para habilitar capacidades avanzadas como **Network Slicing** y **eBPF/XDP** en el plano de usuario.

---

## 📂 Mapa del directorio

El entorno está modularizado en las siguientes carpetas y archivos clave:

- **`ARQ_entorno.jpg` / `arq_interfaces_open5gs.jpeg`**: esquemas visuales y diagramas de arquitectura de red que detallan las interfaces SBA del núcleo Open5GS y la topología física/lógica del despliegue.
- **`core_5G_docker/`**: archivos `docker-compose.yaml` y configuraciones `.env` para instanciar las funciones del plano de control del núcleo 5G (AMF, SMF, UDM, UDR, NRF, PCF, etc.) basadas en Open5GS y donde se encuentra centralizado las metricas monitorizadas existentes.
- **`eUPF/`**: despliegue especializado de la *User Plane Function* utilizando la tecnología eBPF/XDP (`edgecomllc/eupf`). Incluye la configuración que permite a esta función procesar paquetes directamente en el kernel de Linux para lograr latencias ultrabajas.
- **`edge_computing_mec/`**: implementación en Python de las APIs del estándar ETSI MEC. Incluye el servidor MEC011 (registro de apps), MEC012 (RNIS / notificaciones de radio), el orquestador dinámico y el proxy inverso NGINX que enruta el tráfico Edge.
- **`monitorizacion/`**: stack completo de observabilidad. Incluye Prometheus (recolección de métricas), Grafana (dashboards visuales), cAdvisor (rendimiento de contenedores) y Promtail/Loki para la trazabilidad centralizada de los logs del sistema 5G. (situados en eUPF y DN/GW)
- **`ran_srsran/`**: configuraciones avanzadas (archivos `.conf` y `.yml`) para levantar la estación base gNodeB física utilizando radios definidas por software (SDR) mediante srsRAN.
- **`ueransim/`**: entorno de simulación de gNodeB y equipos de usuario (UEs). Vital para probar escenarios de **Network Slicing** (múltiples UEs conectándose a distintas subredes) sin depender del hardware de radio físico.
- **`scripts_modem_sim/`**: conjunto de scripts necesarios situados en el portatil conectado a través del modem (GUIA conexion GUIA_5G) donde recibe lista de aplicaciones operativas.
- **`UGV_BEAST/`**: contiene el servidor web de baja latencia y el script controlador del robot físico (Vehículo Terrestre No Tripulado). Representa el **caso de uso final**: teleoperación asíncrona y transmisión de vídeo en tiempo real aprovechando el Edge Computing para evadir el CG-NAT y la latencia de las redes públicas.

> **Nota:** Debido a su volumen, el código fuente completo de `UGV_BEAST/` puede no detallarse en los anexos en bruto de la memoria, aunque la carpeta forme parte del entorno funcional del proyecto.

---

## 🚀 Requisitos previos del host

Antes de levantar la infraestructura, el servidor anfitrión (Linux/Ubuntu) debe estar preparado para soportar el enrutamiento de paquetes y los permisos de eBPF.

### Habilitar IP forwarding

Es obligatorio para que los contenedores del Core y la eUPF puedan enrutar el tráfico hacia Internet o hacia la red MEC local.

```bash
sudo sysctl -w net.ipv4.ip_forward=1
```

Para hacerlo persistente tras reiniciar:

```bash
echo 'net.ipv4.ip_forward=1' | sudo tee -a /etc/sysctl.conf
sudo sysctl -p
```

### Permisos eBPF

El despliegue de la eUPF requiere montar el sistema de archivos BPF y permisos de administrador de red. Esto normalmente ya queda contemplado en su correspondiente `docker-compose.yaml` mediante opciones como:

- `privileged: true`
- montaje de `/sys/fs/bpf`
- capacidades de red elevadas

---

## 🛠️ Flujo de despliegue recomendado

Para evitar condiciones de carrera, como servicios intentando conectar a bases de datos o funciones de red aún no inicializadas, se recomienda el siguiente orden de ejecución.

### 1. Despliegue del Core 5G y eUPF

Levantar primero las bases de datos (MongoDB) y el plano de control (AMF, SMF, etc.), seguido del plano de usuario de alto rendimiento.

```bash
cd core_5G_docker
sudo docker compose -f sa-deploy.yaml up -d
```

> **Nota:** Si se están probando escenarios de **Network Slicing**, utilizar los archivos YAML específicos que levanten múltiples instancias de SMF/UPF.

### 2. Monitorización y observabilidad

Iniciar la monitorización desde el minuto cero permite capturar métricas, consumo de recursos y logs desde el arranque del sistema.

```bash
cd monitorizacion
sudo docker compose up -d
```

Acceso a Grafana:

```text
http://<IP_HOST>:3000
```

### 3. Edge Computing (MEC)

Iniciar el entorno perimetral que detectará la conexión de usuarios y habilitará los servicios locales en el borde de red.

```bash
cd edge_computing_mec
sudo docker compose up -d
python3 edge_router_controller.py
```

### 4. Conexión de la radio y UEs

Dependiendo del tipo de prueba, el despliegue continúa de forma distinta:

#### Opción A: prueba simulada

```bash
cd ueransim
# Levantar primero el gNodeB simulado
# Después, iniciar uno o varios UEs
```

Secuencia recomendada:

- arrancar `nr-gnb`
- validar registro contra el Core
- arrancar `nr-ue` para cada UE de prueba

#### Opción B: prueba física

```bash
cd ran_srsran
# Levantar el contenedor o servicio asociado a la gNodeB física
# Encender y validar el módem físico aprovisionado previamente
```

Además, debe haberse preparado el módem y la SIM usando los recursos de `scripts_modem_sim/`.

### 5. Activación del UGV BEAST

Una vez el módem integrado en el robot obtenga una IP del túnel 5G (por ejemplo, `10.47.0.X`), iniciar el servidor de control en la carpeta `UGV_BEAST/`. El sistema MEC detectará la presencia del robot y actualizará el DNS de los clientes (UEs) conectados para permitir la teleoperación y la recepción de vídeo con latencias mínimas.

---

## 📌 Resumen operativo

Orden recomendado de arranque:

1. `core_5G_docker/`
2. `monitorizacion/`
3. `edge_computing_mec/`
4. `ueransim/` o `ran_srsran/`
5. `UGV_BEAST/`

Este orden minimiza errores de inicialización y facilita el diagnóstico de incidencias en una arquitectura distribuida con múltiples dependencias.

---

## 📝 Notas de diseño del README

- **Explicación del UGV:** `UGV_BEAST/` aparece destacado como el **caso de uso final** del proyecto. Esto ayuda a justificar ante el tribunal por qué esa carpeta existe en capturas o en la estructura del repositorio aunque su código no se detalle línea a línea en los anexos.
- **Orden lógico:** el flujo de despliegue recomendado no solo indica qué arrancar, sino también el porqué de cada componente y la dependencia funcional entre procesos.
- **Formato GitHub:** se han adaptado títulos, listas, bloques de código, notas y rutas para que GitHub renderice correctamente el documento como `README.md`.
