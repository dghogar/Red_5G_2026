# 📱 Guía de despliegue: Pasarela móvil 5G (UE) y control del UGV BEAST

Este documento detalla los pasos necesarios para configurar un teléfono móvil Android comercial para que actúe como puente (**gateway**) entre una red **5G Standalone (SA)** privada y el vehículo terrestre no tripulado **UGV BEAST**.

---

## 📋 Índice

- [Fase 1: Conexión a la red 5G privada](#fase-1-conexión-a-la-red-5g-privada)
- [Fase 2: Entorno de terminal móvil (Termux)](#fase-2-entorno-de-terminal-móvil-termux)
- [Fase 3: Conectividad con el UGV BEAST (USB Tethering)](#fase-3-conectividad-con-el-ugv-beast-usb-tethering)
- [Fase 4: Enrutamiento de tráfico (Socat)](#fase-4-enrutamiento-de-tráfico-socat)
- [Fase 5: Proxy inverso (NGINX) y visualización](#fase-5-proxy-inverso-nginx-y-visualización)

---

## 📡 Fase 1: Conexión a la red 5G privada

Para evitar que el teléfono intente conectarse a redes comerciales o desvíe tráfico, debemos forzar su conexión exclusiva a nuestro Core **Open5GS** y **srsRAN**.

### Configuración del APN

1. Ve a **Ajustes > Redes móviles > Nombres de puntos de acceso (APN)**.
2. Crea un nuevo APN con los datos configurados en tu UPF de Open5GS. Por defecto suele ser `internet` o el MCC/MNC de tu red de laboratorio.
3. Guarda y selecciona este APN.

### Forzar modo NR only (5G exclusivo)

1. Descarga e instala una aplicación de gestión de red oculta desde la Play Store, como **Force 5G Only**.
2. Abre la app y accede a la configuración avanzada de telefonía.
3. En el menú desplegable **Tipo de red preferida**, selecciona obligatoriamente **NR only**. Esto evita que el móvil salte a 4G/LTE.

### Aislamiento de red

- Desactiva la interfaz Wi-Fi.
- Comprueba que tienes conexión 5G y verifica la IP que te ha dado el Core, por ejemplo `10.47.0.9`.

---

## 💻 Fase 2: Entorno de terminal móvil (Termux)

Para convertir el móvil en un nodo de red avanzado, necesitamos una terminal Linux embebida.

### Instalación

Descarga **Termux**. Se recomienda usar la tienda **F-Droid** en lugar de Google Play, ya que la versión de Play Store está obsoleta.

### Actualización del sistema base

```bash
pkg update && pkg upgrade
```

### Instalación de paquetes clave

Instala las herramientas que utilizaremos para red, SSH y el servidor web:

```bash
pkg install socat openssh nmap nginx net-tools
```

---

## 🔌 Fase 3: Conectividad con el UGV BEAST (USB Tethering)

El robot y el móvil deben comunicarse físicamente sin depender de redes inalámbricas externas.

### Conexión física

Conecta el teléfono móvil a la Raspberry Pi del UGV BEAST mediante un cable USB de datos.

### Activación del anclaje USB

1. En el móvil, ve a **Ajustes > Conexiones > Conexión compartida y módem**.
2. Activa **Anclaje de red por USB** (*USB Tethering*).
3. Android creará una subred privada local exclusiva para el cable.

### Detección de IPs en Termux

Abre Termux y descubre la IP que Android se ha asignado a sí mismo en el cable:

```bash
ifconfig
```

Busca la interfaz `rndis0` o `usb0`. Supongamos que la IP del móvil es `10.233.35.1`.

Descubre la IP que Android le ha dado a la Raspberry Pi del UGV escaneando la red:

```bash
nmap -sn 10.233.35.0/24
```

Supongamos que la IP del robot es `10.233.35.145`.

---

## 🔀 Fase 4: Enrutamiento de tráfico (Socat)

Dado que la Raspberry Pi está detrás del NAT del móvil, usamos `socat` para crear túneles UDP transparentes entre el cable y la antena 5G.

En Termux, ejecuta estos tres comandos en segundo plano, adaptando las IPs a las tuyas:

### Autopista de telemetría

```bash
socat UDP4-LISTEN:65432,bind=10.233.35.1,fork UDP4:192.168.0.120:65432 &
```

### Autopista de vídeo

```bash
socat UDP4-LISTEN:5000,bind=10.233.35.1,fork UDP4:192.168.0.120:5000 &
```

### Autopista de comandos

```bash
socat UDP4-LISTEN:65432,bind=10.47.0.9,fork UDP4:10.233.35.145:65432 &
```

---

## 🌐 Fase 5: Proxy inverso (NGINX) y visualización

Para poder visualizar la interfaz de control de la Estación de Tierra directamente desde el navegador del móvil y evitar problemas de CORS o puertos al acceder a la red 5G, configuramos **NGINX** en Termux como proxy inverso.

### 1. Configuración de NGINX en Termux

El archivo de configuración de NGINX en Termux se encuentra en una ruta específica. Vamos a editarlo:

```bash
nano /data/data/com.termux/files/usr/etc/nginx/nginx.conf
```

Modifica el bloque `server` para que escuche en el puerto `8080` del teléfono y redirija el tráfico HTTP y los WebSockets hacia el servidor Edge (`192.168.0.120`):

```nginx
worker_processes  1;

events {
    worker_connections  1024;
}

http {
    include       mime.types;
    default_type  application/octet-stream;
    sendfile        on;
    keepalive_timeout  65;

    server {
        listen       8080;
        server_name  localhost;

        # Redirigir tráfico web normal a la Estación de Tierra (Flask)
        location / {
            proxy_pass http://192.168.0.120:5000;
            proxy_set_header Host $host;
            proxy_set_header X-Real-IP $remote_addr;
        }

        # Redirigir el tráfico de WebSockets (SocketIO y vídeo jsmpeg)
        location /socket.io/ {
            proxy_pass http://192.168.0.120:5000;
            proxy_http_version 1.1;
            proxy_set_header Upgrade $http_upgrade;
            proxy_set_header Connection "upgrade";
        }
    }
}
```

Guarda el archivo con `Ctrl + O`, confirma con `Enter` y sal de nano con `Ctrl + X`.

### 2. Arrancar NGINX

En la consola de Termux, inicia el servidor web ejecutando:

```bash
nginx
```

Si necesitas recargar la configuración en el futuro:

```bash
nginx -s reload
```

### 3. Acceso desde el navegador móvil

Abre tu navegador de confianza en el móvil Android. Se recomienda **Google Chrome** por su compatibilidad con WebSockets y decodificación por hardware para vídeo.

En la barra de direcciones, escribe:

```text
http://localhost:8080
```

**Resultado:** NGINX interceptará la petición, la enrutará por la antena 5G hasta el servidor MEC (`192.168.0.120`), y devolverá el panel de control táctico (**GCS**) adaptado a la pantalla del móvil.

---

## Notas operativas

- Mantén Wi-Fi desactivado durante las pruebas para garantizar que todo el tráfico use la red 5G.
- Comprueba que la IP del móvil en `usb0` o `rndis0` coincide con la usada en `socat`.
- Si el navegador no carga el panel, revisa la configuración de NGINX y que el servidor Edge esté accesible en `192.168.0.120:5000`.
- Si no recibes vídeo o telemetría, valida que los puertos y las IPs coinciden en móvil, UGV y servidor MEC.