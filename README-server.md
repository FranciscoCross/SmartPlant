## Para Comenzar
### Fork Repository
Lo primero que se necesita es forkear este repositorio en su cuenta de GitHub, porque necesitará un access token para darle permisos al runner.

![Fork](/docs/GithubFork.jpg)

Una vez listo el fork, clone el repositorio, preferentemente usando SSH.

### Docker Compose
El próximo paso es intalar Docker Compose. Se recomienda ampliamente usar Linux, aunque se puede usar Windows. En distribuciones basadas en Ubuntu se puede instalar con el comando:
```
sudo apt install docker-compose
```
Para Windows (u otro SO) por favor diríjase a [Docker Documentation](https://docs.docker.com/desktop/windows/install).

### Credenciales
Por motivos de seguridad debe crear un GitHub Access Token, el cual le da permisos sobre su cuenta de GitHub para permitir que un self-hosted runner pueda ejecutar las tareas (jobs) del pipeline.
Cree un archivo llamado ".env" en la carpeta raíz de su repositorio. Nótese que este archivo está incluido en el .gitignore, ya que sus credenciales no se deben compartir. En este archivo debe agregar:
```
RUNNER_REPOSITORY_URL=URL of your Forked Repository
GITHUB_ACCESS_TOKEN=Your Token
```
![DevSettings](/docs/GithubDevSettings.jpg)

Para crear un nuevo GitHub Access Token, vaya a Ajustes, Ajustes de Desarrollador (Developer Settings, abajo), Personal Access Tokens, y clickee en **Generar nuevo token**. Obtendrá una secuencia larga de caracteres, cópiela y péguela en el archivo .env en los campos indicados previamente. No podrá recuperar este token, así que si pierde su archivo .env y necesita el token, necesitará crear un nuevo token y reemplazarlo.

Al generar un nuevo token, debe elegir que permisos asignarle, asegúrese de tildar los siguientes:
- **repo**
- **workflow**

![AccessToken](/docs/GithubAccessToken.jpg)

### ESP32 USB Connection
Para cargar código al ESP32 considere:
- Si está usando Windows o una máquina virtual, se necesita presionar el botón BOOT mientras el programa está siendo cargado, de otro modo el sistema avisará con un error que indica que la placa no está en modo de flasheo.
- Se recomienda usar Linux, ya que no se necesita presionar el botón BOOT, pero es necesario darle permisos de escritura al puerto USB, de preferencia al USB 0.
- Para darle permisos de escritura a los puertos USB: (Reinicio necesario)
```
sudo usermod -a -G dialout $USER
```

### Self-Hosted Containerized Runner
Un Runner es un servidor que ejecuta las acciones (o jobs) incluídas en el workflow de este repositorio, que se encuentra en la carpeta **.github/workflow**. Un runner self-hosted significa que se ejecutará de forma local en su computadora, y conteinerizado significa que se usa un contenedor de Docker para mantener las dependencias lo más simple posible.
Para ejecutar el runner, abra una terminal en la carpeta raíz del repositorio y corra este comando:
```
sudo docker-compose up
```
La inicialización del contenedor tomará aproximadamente 4 minutos. En caso de un error de configuración con el access token, el runner lo notificará como un error de autenticación de GitHub.

## Workflow
Para desencadenar el workflow se necesita pushear cambios en las carpetas: **/src/**, **/include/** y **/test/**. Nuestra recomendación es cambiar únicamente la frecuencia de parpadeo del LED, y dejar el resto del código como está. Cambie el define DELAY de la línea 7 de **/include/blink.h** a un valor que pueda distinguir fácilmente en el parpadeo del LED de la placa. Por ejemplo, alterne entre 200 y 2000 ms de DELAY. Después de pushear los cambios, el código va a compilarse, testearse y cargarse en la placa automáticamente. Esta es la ventaja de DevOps, ahorrar tiempo y evitar cometer errores.

### Tests
Hay 2 tests simples que se realizan antes de cargar el código a la placa. El primero es chequear si el valor de DELAY es mayor o igual a 50 ms, y el segundo es si el valor de DELAY es menor o igual a 5000 ms.
Si se elije un valor fuera del **rango válido de DELAY (50 - 5000ms)**, los tests fallarán y **el código no se cargará en la placa**, simulando una prevención de posibles daños.


## Referencias
- [Docker](https://www.docker.com)
- [TCardonne GitHub Runner](https://registry.hub.docker.com/r/tcardonne/github-runner)
- [PlatformIO](https://platformio.org)