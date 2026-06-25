# Despliegue de la Arquitectura de Monitorización y Observabilidad 5G (NOC)

## 1. Introducción y paradigma de observabilidad

Este directorio contiene los ficheros de configuración y manifiestos de orquestación necesarios para desplegar un **Centro de Operaciones de Red (NOC)** de nivel profesional sobre la infraestructura **5G Standalone** y la plataforma **MEC (Multi-Access Edge Computing)**.

En arquitecturas distribuidas basadas en servicios (**SBA**), la simple validación de conectividad resulta insuficiente. Es necesario observar con precisión el comportamiento de la red, los cuellos de botella y el cumplimiento de las políticas de **Calidad de Servicio (QoS)** y **Network Slicing**.

Para ello, se ha diseñado un ecosistema de monitorización holístico compuesto por cuatro pilares fundamentales:

- **Métricas y consumo de recursos**: **Prometheus** para recolección y **cAdvisor** para monitorización de contenedores a nivel de kernel.
- **Agregación centralizada de registros**: **Grafana Loki** como base de datos documental y **Promtail** como agente recolector pasivo.
- **Trazabilidad distribuida**: **Jaeger** y **Grafana Beyla**, utilizando tecnología **eBPF** para inspeccionar transacciones de red sin modificar el código fuente de las funciones de red (**NFs**).
- **Medición pasiva de QoS end-to-end**: **Qosium**, para la obtención de métricas de latencia y *jitter* unidireccional con precisión de microsegundos.

---

## 2. Topología de despliegue

Dada la naturaleza descentralizada de la maqueta (**Core** frente a **Edge**), los agentes de monitorización se han segmentado estratégicamente para operar en sus respectivos nodos sin saturar el plano de control.

### Estructura de directorios

- **`values.json`**: archivo de exportación que contiene la estructura paramétrica y el diseño de los dashboards personalizados para Grafana.
- **`edge-monitor/`**: agentes para la **eUPF**.
  - **`docker-compose.yaml`**: manifiesto para instanciar **cAdvisor** y **Grafana Beyla**.
  - **`promtail-edge.yaml`**: configuración del agente Promtail para leer los logs de la eUPF y enviarlos al nodo central de **Loki**.
- **`edge-monitor_dngw/`**: agentes para la **Pasarela de Datos / Servidor MEC**.
  - **`docker-compose.yaml`** y **`promtail-edge.yaml`**: manifiestos análogos adaptados para monitorizar los microservicios de la plataforma Edge (**MEC011**, **MEC012**, **NGINX**) y las aplicaciones finales.

> **Nota:** Los motores centrales, es decir, **Grafana**, **Prometheus** y **Loki**, se despliegan de forma unificada en el nodo principal (**Core**) mediante el archivo `sa-deploy.yaml` central.

---

## 3. Requisitos previos y restricciones técnicas

Antes de iniciar el despliegue, asegúrate de cumplir los siguientes requisitos.

### Sincronización de relojes estricta (PTP)

Para que la correlación de trazas de **Jaeger** y las mediciones de latencia de **Qosium** sean válidas, todos los nodos físicos deben estar sincronizados temporalmente. Para ello se requiere la instalación del demonio `ptpd`.

### Privilegios del kernel para eBPF

**Grafana Beyla** necesita capacidades avanzadas para inyectar sondas en el núcleo del sistema operativo. Por ello, los ficheros `docker-compose.yaml` de este directorio otorgan explícitamente permisos como:

- `privileged: true`
- `pid: "host"`
- `network_mode: "host"`
- montaje del volumen `/sys/kernel/debug`

### Variables de entorno y conectividad

Asegúrate de que las máquinas perimetrales tengan conectividad IP y rutas correctas hacia el nodo **Core**, en particular hacia:

- **Loki** en el puerto `3100`
- **Jaeger / OTLP** en el puerto `4317`

---

## 4. Instrucciones de despliegue

### Paso 1: Sincronización temporal mediante PTP

Antes de levantar cualquier sonda, es fundamental alinear los relojes de las máquinas físicas operando en modo multicast.

#### En la máquina Core (maestro)

```bash
sudo apt install ptpd
sudo ptpd --masteronly --interface <interfaz_red_local> --verbose
```

#### En las máquinas Edge y UEs (esclavos)

```bash
sudo apt install ptpd
sudo ptpd --slaveonly --interface <interfaz_red_local> --verbose
```

### Paso 2: Despliegue de agentes perimetrales

Inicia los agentes de recolección en los nodos periféricos. Estos servicios operan de manera pasiva y no deberían interrumpir el tráfico de red de las aplicaciones.

#### En el nodo eUPF

```bash
cd edge-monitor/
# Edita promtail-edge.yaml si fuera necesario para apuntar a la IP del Core (Loki)
sudo docker compose up -d
```

#### En el nodo DN/GW (plataforma MEC)

```bash
cd edge-monitor_dngw/
# Edita promtail-edge.yaml correspondientemente
sudo docker compose up -d
```

Para verificar el estado de los contenedores:

```bash
sudo docker ps
```

### Paso 3: Importación de dashboards en Grafana

Una vez la telemetría esté fluyendo hacia el nodo central:

1. Accede a la interfaz web de Grafana en `http://<IP_CORE>:3000`.
2. Dirígete a **Dashboards > Import**.
3. Carga el contenido del fichero `values.json` incluido en este directorio.

Grafana auto-configurará paneles de correlación integrando en una misma vista:

- consumo de **CPU/RAM** vía **cAdvisor**
- volumen de tráfico encapsulado **GTP-U** vía **Prometheus**
- trazas de logs del sistema vía **Loki/Promtail**

---

## 5. Explicación técnica de las configuraciones

### Lectura out-of-band con Promtail

La recolección de logs se ha diseñado para no interactuar directamente con el motor de Docker. **Promtail** monta en modo lectura (`:ro`) los directorios locales donde el software escribe los archivos de registro, por ejemplo `/var/log/...`.

Esto garantiza un nivel de intrusión prácticamente nulo en el ciclo de vida de los contenedores 5G.

### Auto-instrumentación con eBPF

En los archivos Docker Compose del Edge, la directiva `BEYLA_EXECUTABLE_NAME` se ajusta dinámicamente para interceptar las llamadas al sistema del binario de la **eUPF** o del servidor web monitorizado. De este modo, es posible construir el diagrama de cascada en **Jaeger** sin inyectar librerías de observabilidad (**SDKs**) dentro de las aplicaciones perimetrales.

---

## 6. Notas operativas

- Verifica siempre la sincronización temporal antes de analizar latencias o trazas distribuidas.
- Comprueba que los puertos `3100` y `4317` son accesibles desde los nodos Edge hacia el Core.
- Si Beyla no genera trazas, revisa primero los privilegios del contenedor y el montaje de `/sys/kernel/debug`.
- Si Promtail no envía logs, valida rutas locales, permisos de lectura y la URL configurada del servicio Loki.
- Mantén `values.json` versionado junto al repositorio para poder reconstruir rápidamente los dashboards tras una reinstalación de Grafana.
