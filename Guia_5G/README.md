# Configuración de Open5GS como núcleo 5GC Standalone (SA)
## Introducción
Open5GS es una implementación en C del núcleo 4G/5G (EPC y 5GC) que soporta tanto redes LTE como NR en modos NSA y SA. En este repositorio se describe cómo desplegar Open5GS como núcleo 5G Standalone (5GC SA) sobre Ubuntu 22.04, utilizando MongoDB como base de datos de suscriptores y la WebUI oficial para su gestión.[^1][^2][^3][^4]
## Arquitectura de Open5GS
La arquitectura de Open5GS separa plano de control y plano de usuario (CUPS), e integra tanto funciones 4G como 5G.[^3][^5]



En modo 5G SA las funciones principales son:

- **AMF (Access and Mobility Management Function)**: gestiona registro, movilidad y autenticación inicial del UE, y termina la interfaz N2 frente al gNB.[^5][^3]
- **SMF (Session Management Function)**: establece y gestiona sesiones PDU, coordina la asignación de direcciones IP y programa el UPF a través de N4 (PFCP).[^6][^5]
- **UPF (User Plane Function)**: reenvía el tráfico de usuario entre el gNB (N3) y la red de datos (N6), aplicando reglas de encaminamiento y NAT si es necesario.[^3][^5]
- **UDM/UDR/PCF/NRF/AUSF**: funciones de base de datos de suscriptores, políticas, descubrimiento de funciones de red y autenticación, interconectadas mediante la Service Based Interface (SBI, interfaces N8, N10, N12, N13, N15, N36).[^5][^3]

Principales interfaces lógicas mostradas en el diagrama:

- **N1**: NAS entre UE y AMF, transportado sobre N2/RRC desde el gNB.[^5]
- **N2**: señalización NGAP entre gNB y AMF.[^5]
- **N3**: plano de usuario GTP-U entre gNB y UPF.[^5]
- **N4**: PFCP entre SMF y UPF para control de plano de usuario.[^6][^5]
- **N6**: conexión del UPF hacia la red de datos/WAN (por ejemplo, Internet).[^3][^5]

En un despliegue simple todo el plano de control y de usuario puede residir en una única máquina virtual o servidor físico.[^3]
## Requisitos previos
- Ubuntu Server 22.04 x86_64 con acceso a Internet y privilegios `sudo`.
- Sin otros servicios que usen MongoDB o el puerto 3000 (reservado para la WebUI).
- Opcionalmente, una segunda interfaz de red dedicada al tráfico 5G (subred de `ogstun`).

> Nota: A partir de Ubuntu 22.04 algunos paquetes en el PPA `open5gs/latest` pueden no estar firmados correctamente; se recomienda usar siempre `apt` con la opción `-y` y verificar el estado de los servicios tras la instalación.[^1][^7]
## Instalación de MongoDB (base de datos de suscriptores)
MongoDB almacena la información de suscriptores (IMSI, claves, DNN/APN, QoS) utilizada por las funciones UDM/UDR de Open5GS.[^3]

```bash
# Actualizar índices y dependencias básicas
sudo apt update
sudo apt install -y gnupg

# Importar la clave pública de MongoDB 8.0
curl -fsSL https://pgp.mongodb.com/server-8.0.asc \
  | sudo gpg -o /usr/share/keyrings/mongodb-server-8.0.gpg --dearmor

# Añadir el repositorio oficial de MongoDB 8.0 para Ubuntu 22.04 (jammy)
echo "deb [ arch=amd64,arm64 signed-by=/usr/share/keyrings/mongodb-server-8.0.gpg ] \\
  https://repo.mongodb.org/apt/ubuntu jammy/mongodb-org/8.0 multiverse" \
  | sudo tee /etc/apt/sources.list.d/mongodb-org-8.0.list

# Instalar MongoDB
sudo apt update
sudo apt install -y mongodb-org

# Iniciar y habilitar el servicio
sudo systemctl start mongod
sudo systemctl enable mongod

# (Opcional) Verificar el estado
sudo systemctl status mongod --no-pager
```
## Instalación de Open5GS
Open5GS proporciona paquetes precompilados para Ubuntu mediante el PPA `open5gs/latest`. La siguiente secuencia instala todos los NFs del core (4G/5G) en un solo servidor.[^1]

```bash
# Añadir el PPA oficial de Open5GS
sudo add-apt-repository -y ppa:open5gs/latest
sudo apt update

# Instalar Open5GS (todas las funciones de red)
sudo apt install -y open5gs

# Comprobar el estado de los servicios
sudo systemctl status open5gs-* --no-pager

# (Opcional) Listar todos los servicios instalados
systemctl list-units --type=service | grep open5gs
```

En Ubuntu 22.04 puede ser necesario forzar `-y` en `apt install` para evitar bloqueos debidos a paquetes no firmados o dependencias adicionales.[^7]
## Instalación de Node.js y Open5GS WebUI
La WebUI permite gestionar suscriptores y parámetros del core desde un navegador. Para su instalación se requiere previamente Node.js (se recomienda una versión LTS reciente, como la 18 o 20). La siguiente secuencia usa el repositorio oficial de Nodesource para Node.js 20.[^8][^4]

```bash
# Dependencias para Node.js
sudo apt update
sudo apt install -y ca-certificates curl gnupg
sudo mkdir -p /etc/apt/keyrings

# Clave GPG de Nodesource
curl -fsSL https://deb.nodesource.com/gpgkey/nodesource-repo.gpg.key \
  | sudo gpg --dearmor -o /etc/apt/keyrings/nodesource.gpg

# Crear el repositorio para Node.js 20
NODE_MAJOR=20
echo "deb [signed-by=/etc/apt/keyrings/nodesource.gpg] \
  https://deb.nodesource.com/node_${NODE_MAJOR}.x nodistro main" \
  | sudo tee /etc/apt/sources.list.d/nodesource.list

# Instalar Node.js
sudo apt update
sudo apt install -y nodejs
```

Instalación de la WebUI de Open5GS:

```bash
# Instalar WebUI
curl -fsSL https://open5gs.org/open5gs/assets/webui/install | sudo -E bash -

# Verificar servicios
sudo systemctl status mongod --no-pager
sudo systemctl status open5gs-webui.service --no-pager
```

Para permitir acceso remoto a la WebUI, editar la unidad `systemd`:

```bash
sudo nano /lib/systemd/system/open5gs-webui.service
```

Añadir o modificar las líneas de entorno dentro de la sección `[Service]`:

```ini
Environment=HOSTNAME=0.0.0.0
Environment=PORT=3000   # o el puerto que se desee
```

Aplicar los cambios y reiniciar el servicio:

```bash
sudo systemctl daemon-reload
sudo systemctl restart open5gs-webui
sudo systemctl status open5gs-webui --no-pager
```

Con `systemctl status open5gs-webui.service` se puede ver en qué puerto y dirección escucha la WebUI.[^8]

Credenciales por defecto:

- Usuario: `admin`
- Contraseña: `1423`
## Habilitar rc.local y servicio auxiliar
Para automatizar reglas de enrutamiento, NAT y firewall es útil restaurarlas en cada arranque mediante `/etc/rc.local`.

```bash
# Descargar unidad de systemd para rc-local
sudo wget -P /etc/systemd/system/ \
  https://raw.githubusercontent.com/catinello/rc-local.service/master/rc-local.service

sudo chmod +x /etc/systemd/system/rc-local.service

# Crear el script rc.local
sudo touch /etc/rc.local
sudo chmod +x /etc/rc.local

# (Opcional) Habilitar el servicio
sudo systemctl enable rc-local
sudo systemctl start rc-local
sudo systemctl status rc-local --no-pager
```

Dentro de `/etc/rc.local` se pueden colocar las reglas de `iptables`, activación de `ip_forward`, etc., terminando siempre con `exit 0`.
## Configuración básica de Open5GS
Los ficheros de configuración de Open5GS se encuentran en `/etc/open5gs` y los registros en `/var/log/open5gs`.[^1][^3]

En un despliegue sólo-5G (SA) los ficheros mínimos a revisar son:

- `nrf.yaml`: configuración del Network Repository Function (descubrimiento de NFs, dirección IP/puerto de servicio SBI).
- `amf.yaml`: PLMN, TAC, listas de gNBs permitidos y parámetros de acceso.
- `smf.yaml`: DNN/APN soportados, subredes para asignación de IP, conexión N4 hacia el UPF.
- `upf.yaml`: interfaz GTP-U (`ogstun`), subredes internas (por ejemplo `10.45.0.0/16` y `2001:db8:cafe::/48`) y ruta hacia N6.

Aunque el escenario se centre en 5G SA, también se instalan otros ficheros como `udm.yaml`, `udr.yaml`, `pcf.yaml`, `bsf.yaml` y `nssf.yaml`, que deben ser coherentes con el PLMN y DNN configurados.[^3]

Para pruebas de laboratorio se recomienda usar el PLMN **001/01**, reservado para test y que evita conflictos regulatorios.[^3]
## Ejemplo de parámetros de suscriptor
La información del suscriptor se introduce desde la WebUI en la sección **Subscriber**. Los campos típicos son:

- IMSI: `001010000052121`
- SUBSCRIBER KEY (K): `6fdca27c74c6d7798e063dd62d413d59`
- AMF (Authentication Management Field): `9000`
- USIM TYPE: `OPc`
- OPERATOR KEY (OPc/OP): `364de2e1b0f38521aa01233f07b3f761`
- UE-AMBR DL: `1` Gbps
- UE-AMBR UL: `1` Gbps

Estos parámetros deben coincidir exactamente con la configuración de la SIM/UE (por ejemplo, un módem Telit FN990 con SIM sysmocom-V2) para que la autenticación 5G-AKA sea correcta.[^3]
## Scripts útiles
Se recomienda añadir dos scripts sencillos en el repositorio:

- `ver_apn.sh`: consulta la base de datos MongoDB de Open5GS para mostrar los DNN/APN configurados para un IMSI concreto.
- `restartOpen5GS.sh`: reinicia de forma ordenada todos los servicios de Open5GS (`systemctl restart open5gs-*`).

Estos scripts pueden invocarse desde `/usr/local/bin` o desde el propio repositorio, y facilitan las tareas de operación diaria.
## Configuración de red, encaminamiento y NAT
Para que los UEs obtengan conectividad a Internet a través del UPF es necesario habilitar el reenvío de paquetes (IPv4/IPv6) y configurar reglas de NAT sobre la interfaz `ogstun`.
### Activar IPv4/IPv6 forwarding
```bash
sudo sysctl -w net.ipv4.ip_forward=1
sudo sysctl -w net.ipv6.conf.all.forwarding=1
```

Estas mismas líneas pueden añadirse a `/etc/rc.local` o a `/etc/sysctl.conf` para que sean persistentes tras reinicio.[^3]
### Reglas de NAT
Suponiendo que Open5GS asigne IPs a UEs en `10.45.0.0/16` (IPv4) y `2001:db8:cafe::/48` (IPv6) a través de `ogstun`, se pueden aplicar las siguientes reglas de `iptables`/`ip6tables`:

```bash
# NAT IPv4 para UEs
sudo iptables -t nat -A POSTROUTING -s 10.45.0.0/16 ! -o ogstun -j MASQUERADE

# NAT IPv6 para UEs
sudo ip6tables -t nat -A POSTROUTING -s 2001:db8:cafe::/48 ! -o ogstun -j MASQUERADE
```
### Configuración del firewall (UFW)
Si se está usando UFW como cortafuegos sencillo:

```bash
sudo ufw status      # Comprobar estado (por defecto puede estar "active")

# Deshabilitar completamente UFW (opción simple para laboratorio)
sudo ufw disable
sudo ufw status      # Debería mostrar "inactive"
```

Opcionalmente se pueden añadir reglas más finas con `iptables`:

```bash
# Aceptar tráfico entrante en la interfaz GTP-U del UPF (ogstun)
sudo iptables -I INPUT -i ogstun -j ACCEPT

# Bloquear tráfico de los UEs hacia el propio host UPF
sudo iptables -I INPUT -s 10.45.0.0/16 -j DROP
sudo ip6tables -I INPUT -s 2001:db8:cafe::/48 -j DROP

# Bloquear tráfico de UEs hacia otras VNFs (sustituir x.x.x.x/y por la red a proteger)
sudo iptables -I FORWARD -s 10.45.0.0/16 -d x.x.x.x/y -j DROP
```

Todas estas reglas pueden integrarse en `/etc/rc.local` para aplicarse automáticamente al arrancar el sistema.
## Acceso y pruebas con el RAN/UE
Una vez configurado el core, se puede conectar un gNB o simulador (por ejemplo UERANSIM) al AMF usando la interfaz N2 y N3.[^9][^10]

Pasos típicos de prueba:

1. Registrar el gNB con el PLMN `001/01`, TAC y direcciones IP que coincidan con `amf.yaml`.
2. Encender el UE (módem o teléfono) con la SIM configurada.
3. Verificar en los logs de `open5gs-amfd` y `open5gs-smfd` que se establece el registro y la sesión PDU.
4. Hacer ping a una IP pública (por ejemplo `8.8.8.8`) desde el UE para confirmar que el tráfico sale por el UPF hacia Internet.
## Referencias
- Guía Quick Start y documentación oficial de Open5GS.[^1][^3]
- Diagrama oficial de arquitectura Open5GS.[^5]
- Discusiones de instalación y uso de WebUI en el repositorio Open5GS.[^8][^11]
- Ejemplos de despliegues 5GC SA con UERANSIM y Open5GS.[^9][^12]

---

## Conexión del módem Telit FN990A28

El módem **Telit Wireless Connection FN990A28** se utiliza como UE 5G para la comunicación **gNB–CORE**.

### Preparación del SDK y librerías

```bash
cd ~/telit_qmi_sdk-1.11.17-0
find . -name "libtelitSDK.so" -o -name "libtqcm.so" # DONDE SE ENCUENTRA LA LIBRERIA

sudo cp ~/telit_qmi_sdk-1.11.17-0/telit_qmi_sdk/lib/libtelitSDK.so /usr/local/lib/
sudo cp ~/telit_qmi_sdk-1.11.17-0/telit_qmi_sdk/lib/libtqcm.so     /usr/local/lib/
echo "/usr/local/lib" | sudo tee /etc/ld.so.conf.d/telit.conf
sudo ldconfig
```
MUY IMPORTANTE: PARAR NETWORK MANAGER
```bash
sudo systemctl stop NetworkManager
sudo systemctl disable NetworkManager
sudo systemctl status NetworkManager
```
Comandos AT para verificación del módem

| Comando     | Descripción                                            |
| ----------- | ------------------------------------------------------ |
| AT          | Debe responder OK, confirma comunicación con el módem. |
| ATI         | Muestra información del módem.                         |
| AT+CPIN?    | Verifica si la SIM está lista (READY).                 |
| AT+CREG?    | Verifica registro en red (0,1 o 0,5).                  |
| AT+CGDCONT? | Verifica perfil PDP/DNN configurado (0-10 aceptable).  |
| AT+CSQ      | Fuerza de señal (>10 aceptable).                       |
| AT+CGATT?   | ¿Está enganchado a la red? Debe responder 1.           |
| AT+CGACT?   | ¿Está activado el contexto de datos?                   |
| AT+CNMP=71  | Fuerza el módem para 5G NR.                            |

Consola serie:
```bash
sudo minicom -D /dev/ttyUSB2 -b 115200
```

Configuración con qmicli
```bash
# Forzar 5G NR
sudo qmicli -d /dev/cdc-wdm0 --nas-set-system-selection-preference="5gnr"

# Ver todos los perfiles 3GPP (había hasta 26, por eso fallaba)
sudo qmicli -d /dev/cdc-wdm0 --wds-get-profile-list=3gpp

# Borrar perfiles innecesarios
sudo qmicli -d /dev/cdc-wdm0 --wds-delete-profile=3gpp,26
```
ESTABLECER CONEXIÓN gNB–CORE PRIMERO
IMPORTANTE: Primero conexión gNB–CORE, después enchufar USB módem , esperar a primera conexión-desconexión y comandos (sino ERROR).

```bash
cd ~/telit_qmi_sdk-1.11.17-0/telit_qmi_sdk-1.11.17-0/telit_qmi_sdk/tqcm/examples/data_connection_example

make clean
make

sudo ./telit_data_connection -c data_connection.conf
```
Verificación con iperf3 y ping
```bash
#CORE (servidor):
iperf3 -s -p 5201

#Módem (cliente):
iperf3 -c 10.45.0.1 -p 5201

#Ping:
ping 8.8.8.8
```


## References

1. [open5gs/docs/_docs/guide/01-quickstart.md at main · open5gs/open5gs](https://github.com/open5gs/open5gs/blob/main/docs/_docs/guide/01-quickstart.md?plain=1) - Open5GS is a C-language Open Source implementation for 5G Core and EPC, i.e. the core network of LTE...

2. [Open5GS](https://github.com/open5gs) - Open5GS is a C-language Open Source implementation of 5G Core and EPC, i.e. the core network of NR/L...

3. [5G: Open5GS: Quick Start](http://www.onnocenter.or.id/wiki/index.php/5G:_Open5GS:_Quick_Start)

4. [Open5gs: Install WebUI Open5gs](http://onnocenter.or.id/wiki/index.php/Open5gs:_Install_WebUI_Open5gs)

5. [[PDF] N26 N11 N2 - Open5GS](https://open5gs.org/open5gs/assets/Open5GS-Diagram.pdf) - CUPS userplane server can be co-located at the 4G/ 5G NSA/ 5G SA. BBU/ vBBU/ vCU (whatever form of B...

6. [Diameter Interfaces (Gx, Gy, S6b) | open5gs/open5gs | DeepWiki](https://deepwiki.com/open5gs/open5gs/4.2-sgw-upgw-u) - This page describes the Open5GS SMF/PGW-C component located in `src/smf/`. It covers the component's...

7. [Problems with the quickstart guide · Issue #549 · open5gs/open5gs](https://github.com/open5gs/open5gs/issues/549) - Hi, I am doing my final degree thesis about 4G virtual mobile network and I need help to advance. I ...

8. [I am new to Open5gs, please help on webUI installation · open5gs open5gs · Discussion #1955](https://github.com/open5gs/open5gs/discussions/1955) - curl -fsSL {{ site.url }}{{ site.baseurl }}/assets/webui/install | sudo -E bash - in above command, ...

9. [GitHub - s5uishida/open5gs_5gc_ueransim_vpp_upf_dpdk_sample_config: Open5GS 5GC & UERANSIM UE / RAN Sample Configuration - UPG-VPP(VPP/DPDK UPF)](https://github.com/s5uishida/open5gs_5gc_ueransim_vpp_upf_dpdk_sample_config) - Open5GS 5GC & UERANSIM UE / RAN Sample Configuration - UPG-VPP(DPDK/VPP UPF) - s5uishida/open5gs_5gc...

10. [Building a Personal Open-Source 5G Network](https://nuradioconcepts.io/2023/12/13/building-a-personal-open-source-5g-network/) - There are a few things that I configure to make starting and stopping the 5GC more optimal. When Ope...

11. [failure to install and activate open5gs webUI according to QuickInstall · Issue #971 · open5gs/open5gs](https://github.com/open5gs/open5gs/issues/971) - Following the QuickStart guide the Open5Gs itself is installed properly. However the webUI fails to ...

12. [Step-by-Step Guide to Configuring a 5G SA Network with Open5GS ...](https://ernie55ernie.github.io/python/2023/03/02/step-by-step-guide-to-configuring-a-5g-sa-network-with-open5gs-and-mec-on-virtual-machines.html) - In this blog post, we will embark on a comprehensive journey to set up a Standalone (SA) 5G network ...

