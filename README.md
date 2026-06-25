# # Despliegue de Network Slicing y Edge Computing en una maqueta 5G

Este repositorio contiene todo el código, los archivos de configuración y los scripts desarrollados para el Trabajo de Fin de Grado (TFG) en Ingeniería de Tecnologías de Telecomunicación por la Universidad de Valladolid (UVA).

El objetivo principal de este proyecto es el diseño, despliegue y validación de una maqueta de red móvil **5G Standalone (SA)**. Sobre esta arquitectura se implementan conceptos avanzados de telecomunicaciones de nueva generación, específicamente **Network Slicing** y **Multi-Access Edge Computing (MEC)**, optimizando el rendimiento del plano de usuario mediante tecnologías como eBPF/XDP (eUPF).

## 🗂️ Estructura del Repositorio

El proyecto está dividido en dos directorios principales, separando claramente la red de acceso por radio (RAN) del núcleo de la red virtualizado (5G Core) y los servicios adicionales.

### 1. `Guia_5G/` (Acceso por Radio y UE)
Este directorio contiene los recursos y configuraciones necesarios para levantar la red de acceso 5G en un entorno local. Está enfocado en la simulación y despliegue físico/virtual de la radio.
* **RAN (Radio Access Network):** Configuraciones para el despliegue de la gNodeB (ej. mediante srsRAN).
* **Modem / Equipo de Usuario (UE):** Scripts y parámetros para inicializar el cliente que se conecta a la red 5G y consume los servicios.

### 2. `Guia_Docker_5G_y_entorno_completo/` (5G Core, MEC y Monitorización)
Este directorio aloja el corazón virtualizado del proyecto. Aquí se define la topología de la red central y los servicios de valor añadido desplegados mediante contenedores.
* **5G Core:** Orquestación en Docker de las funciones de red (AMF, SMF, UPF, UDM, etc.) basadas en Open5GS.
* **Edge Computing (MEC) & eUPF:** Scripts y despliegues para el enrutamiento local del tráfico y la implementación de la *eBPF User Plane Function* para optimizar el procesamiento de paquetes.
* **Network Slicing:** Archivos de configuración para la segmentación de la red y la asignación de tráfico según los requerimientos del servicio.
* **Monitorización:** Despliegue integrado de herramientas de observabilidad (Prometheus, Grafana, cAdvisor) para extraer y visualizar métricas de red y rendimiento en tiempo real.

## 🛠️ Tecnologías Principales
* **Core 5G:** Open5GS
* **RAN & UE:** srsRAN 
* **Virtualización:** Docker & Docker Compose
* **Optimización de Red:** eBPF / XDP (eUPF)
* **Monitorización:** Prometheus, Grafana, Loki

---
**Autor:** [Diego García Hernández]  
**Tutor:** Juan Carlos Aguado Manzano  
**Titulación:** Grado en Ingeniería de Tecnologías de Telecomunicación (Mención en Telemática)  
**Universidad:** Universidad de Valladolid (UVA) - 2026