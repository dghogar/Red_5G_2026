# Despliegue de la Red de Acceso Radio (RAN) - srsRAN Project y SDR

## 1. Introducción y arquitectura del nodo de acceso

Este directorio almacena los archivos de configuración y la parametrización necesaria para la instanciación de la estación base 5G (**gNB**) utilizando la plataforma de código abierto **srsRAN Project**.

En la arquitectura **5G Standalone (SA)** desplegada, el gNB actúa como el puente crítico entre el espectro radioeléctrico (plano radio) y el núcleo de la red (**5G Core - Open5GS**). Su función principal es gestionar la capa de acceso radioeléctrico, estableciendo el canal de señalización **NGAP** mediante el protocolo **SCTP** con la **AMF**, y orquestando los túneles **GTP-U** para el tráfico de datos hacia la **UPF**.

> **Nota importante:** Este documento aborda la configuración y ejecución operativa del gNB. La guía exhaustiva de compilación desde código fuente, resolución de dependencias, parcheo de las librerías SoapySDR y configuración del hardware BladeRF se encuentra detallada en el directorio principal `GUIA_5G`.

---

## 2. Requisitos previos del sistema anfitrión

Para garantizar la estabilidad del enlace radioeléctrico y evitar desbordamientos de búfer en el procesamiento de la señal en banda base, el equipo anfitrión debe cumplir con los siguientes ajustes a nivel de sistema operativo antes de lanzar el proceso del gNB.

```bash
# Maximizar la frecuencia del procesador para evitar cuellos de botella en la capa PHY
echo performance | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor

# Deshabilitar el firewall para permitir el tráfico SCTP y UDP
sudo ufw disable
```

### Hardware requerido

- **Radio Definida por Software (SDR)** soportada.
- En este laboratorio se emplea la plataforma **Nuand BladeRF xA9**, acoplada a través de la capa de abstracción **SoapySDR**.

---

## 3. Estructura y análisis de la configuración (`gnb_custom_cell_properties.yml`)

El corazón del despliegue recae sobre el fichero de configuración YAML principal, por ejemplo `gnb_custom_cell_properties.yml`. Este archivo debe modificarse meticulosamente para coincidir con la topología del Core 5G. A continuación, se explican los bloques más relevantes.

### 3.1. Enlace con el 5G Core (AMF y UPF)

El gNB debe conocer cómo alcanzar el plano de control del núcleo.

- **`amf`**: define la dirección IP del AMF (`addr`) y el puerto de escucha SCTP, por defecto `38412`.
- **`bind_addr`**: define la IP local de la máquina del gNB utilizada para la comunicación N2 (señalización) y N3 (datos GTP-U).

### 3.2. Identidad de red y segmentación (Network Slicing)

Para que los equipos de usuario (**UE**) reconozcan la celda como parte de la red privada y puedan acceder a los servicios MEC, es vital alinear la identidad celular con la base de datos de Open5GS.

- **`mcc / mnc`**: código de país y operador de la red móvil, por ejemplo `001` y `01` para redes privadas de prueba.
- **`tac`**: *Tracking Area Code*, define el área de localización.
- **`tai_slice_support_list`**: lista que declara los segmentos lógicos de red (*Slices*) soportados por la antena. Se define mediante el tipo de servicio (**SST**, por ejemplo `1` para eMBB) y el diferenciador (**SD**). Si un UE solicita un Slice no listado aquí, la conexión será rechazada.

### 3.3. Configuración de radiofrecuencia (RU SDR)

El bloque `ru_sdr` parametriza la interacción directa con el hardware físico.

- **`device_args`**: argumentos de inicialización. Para BladeRF bajo SoapySDR, suele definirse como `type=soapy,driver=bladerf`.
- **`band` y `arfcn`**: definen la banda 5G NR, por ejemplo `n78`, y el canal de radiofrecuencia (ARFCN) para la transmisión.
- **Ganancias (`tx_gain`, `rx_gain`)**: deben ajustarse empíricamente en función de la proximidad de los dispositivos para evitar la saturación del receptor (*clipping*) o la pérdida de señal (*SNR* pobre).

---

## 4. Puesta en marcha del gNB

Una vez conectado el SDR por USB y verificada su detección mediante utilidades nativas, por ejemplo `lsusb` o `bladeRF-cli -e 'version'`, el gNB se inicializa invocando el binario compilado de srsRAN y pasándole como argumento el fichero de configuración:

```bash
# Iniciar la estación base 5G
sudo gnb -c gnb_custom_cell_properties.yml
```

### Comprobación de éxito

Al ejecutarse, la consola mostrará la inicialización de las capas **PHY**, **MAC**, **RLC**, **PDCP** y **RRC**. El indicador crítico de éxito es la aparición del mensaje `NGAP connection established` con el AMF, lo que certifica que el plano de control entre la RAN y el Core está plenamente operativo.

---

## 5. Diagnóstico de rendimiento y monitorización física

El despliegue con hardware SDR de alto rendimiento requiere una monitorización continua para descartar anomalías de procesamiento en tiempo real, como errores *late*.

### Indicadores físicos: LEDs BladeRF

En la plataforma **Nuand BladeRF**, el comportamiento de los diodos emisores de luz ofrece un diagnóstico instantáneo de la salud del enlace.

- **LED 1 (D11 - Actividad)**: parpadeo periódico indicando carga correcta de la FPGA.
- **LED 2 (D12 - TX Underflow)**: si se ilumina o parpadea intensamente, el host no está enviando las muestras digitales al SDR a la velocidad requerida. Indica un cuello de botella en la CPU anfitriona.
- **LED 3 (D13 - RX Overflow)**: si se activa, el receptor SDR está capturando más señales de las que el bus USB o el host es capaz de procesar.

### Alertas en consola: logs de srsRAN

Es habitual en entornos de laboratorio observar mensajes como `[W] Real-time failure in RF: overflow` o `Downlink data late`. Si estos avisos son esporádicos, el impacto en la QoS será marginal. Si son continuos y provocan desconexiones bajo la causa `MAC max KOs reached`, se recomienda revisar la política de escalado de la CPU, reducir el ancho de banda del canal (PRBs) o ajustar los parámetros de ganancia.