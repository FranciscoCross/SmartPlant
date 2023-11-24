# SmartPlant
Este proyecto consiste en brindar un conjunto de servicos **DevSecOps** para aplicar al desarrollo en proyectos **Internet of Things (IoT)**, en este caso la aplicación se trata de un sistema de monitoreo de variables aptas para cultivar.

Este repositorio incluye el código y las herramientas el desarrollo en un dispositivo **ESP32**, en concreto se trabajó con la placa de desarrollo **NodeMCU32s**.

Los servicios corren en un servidor **Raspberry Pi 4**, y su repositorio es: **Repositorio Servidor**

## Guía de Configuración
A continuación se describe cómo partir de este proyecto para iniciar su propio proyecto IoT con el dispositivo **NodeMCU32s**. Se recomienda trabajar con el entorno de desarrollo **Visual Studio Code** y la extensión **PlatformIO** para compilar y testear en el dispositivo.

### Fork del Repositorio

El primer paso es forkear (bifurcar o "copiar") este repositorio en su cuenta de GitHub.

![Fork](/docs/GithubFork.jpg)

Una vez listo el fork, clone el repositorio, preferentemente usando SSH.

### ESP32 Conexión USB
Para cargar código al ESP32 considere:
- Si está usando Windows o una máquina virtual, se necesita presionar el botón BOOT mientras el programa está siendo cargado, de otro modo el sistema avisará con un error que indica que la placa no está en modo de flasheo.
- Se recomienda usar Linux, ya que no se necesita presionar el botón BOOT, pero es necesario darle permisos de escritura al puerto USB, de preferencia al USB 0.
- Para darle permisos de escritura a los puertos USB: (Reinicio necesario)
```
sudo usermod -a -G dialout $USER
```

### Variables de Entorno
Las variables de entorno requeridas para compilar el binario son:
- MQTT_SERV: Dirección Web del Broker MQTT donde se enviarán los datos (Ver **Repositorio Servidor**)
- WMAP_PASS: Contraseña del Access Point provisorio que se dispone para configurar las credenciales con el smartphone.

Estas variables deben ser configuradas en la computadora que compila el código o en los secretos de GitHub para el compilado automático con el Workflow.

### Configuración de Credenciales con WiFiManager
Las credenciales se configuran haciendo uso de un teléfono celular. Una vez cargado el código del proyecto en la placa ESP32, ésta actúa como Access Point emitiendo una red WiFi con el nombre **PowerPotConfigAP** y la contraseña indicada en la variable de entorno WMAP_PASS. Las credenciales que se deben configurar son:
- WiFi Nombre de Red
- WiFi Contraseña
- MQTT Usuario
- MQTT Contraseña
- ESP32-ID: Identificador de la Placa ESP32 para distinguirlas en caso de tener varias.

Las credenciales se eliminan si la placa no consigue conectarse en 120 segundos. (Puede modificarse en /include/param.hpp -> CONFIG_TIMEOUT)




--------------------

## Workflow
Para desencadenar el workflow se necesita pushear cambios en las carpetas: **/src/**, **/include/** y **/test/**. Nuestra recomendación es cambiar únicamente la frecuencia de parpadeo del LED, y dejar el resto del código como está. Cambie el define DELAY de la línea 7 de **/include/blink.h** a un valor que pueda distinguir fácilmente en el parpadeo del LED de la placa. Por ejemplo, alterne entre 200 y 2000 ms de DELAY. Después de pushear los cambios, el código va a compilarse, testearse y cargarse en la placa automáticamente. Esta es la ventaja de DevOps, ahorrar tiempo y evitar cometer errores.

### Tests
Hay 2 tests simples que se realizan antes de cargar el código a la placa. El primero es chequear si el valor de DELAY es mayor o igual a 50 ms, y el segundo es si el valor de DELAY es menor o igual a 5000 ms.
Si se elije un valor fuera del **rango válido de DELAY (50 - 5000ms)**, los tests fallarán y **el código no se cargará en la placa**, simulando una prevención de posibles daños.


## Referencias
- [PlatformIO](https://platformio.org)