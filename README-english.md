# Pipeline-ESP32
This project is intended to create a DevOps Pipeline for Internet of Things devices, in this case an ESP32 Board. The main objective is to have a workflow with a set of actions to trigger when a repository push includes changes in the source code directory.

## Getting Started
### Fork Repository
To get started you need to fork this repository on your own GitHub account, because you will need to set up an access token for the runner permissions.

![Fork](/docs/GithubFork.jpg)

Once forked clone the repository, preferably using SSH.

### Docker Compose
Next step is installing Docker Compose. It is highly recommended to use Linux, although you can also use Windows. In Ubuntu based distributions you can install it with the command:
```
sudo apt install docker-compose
```
For Windows (or other OS) please refer to the [Docker Documentation](https://docs.docker.com/desktop/windows/install).

### Credentials
For security reasons you have to set up a GitHub Access Token, which grants you permissions over your GitHub account to allow a self-hosted runner that we will use to run the pipeline jobs.
Create a file called ".env" in your repository root folder. Note that this file is included in the .gitignore, because your credentials shouldn't be shared. In the file you should add:
```
RUNNER_REPOSITORY_URL=URL of your Forked Repository
GITHUB_ACCESS_TOKEN=Your Token
```

![DevSettings](/docs/GithubDevSettings.jpg)

To create a new GitHub Access Token, go to Settings, Developer Settings (at the bottom), Personal Access Tokens, and click on **Generate new token**. You will get a long sequence of characters, copy it and paste it on the .env file as indicated before. You won't be able to retrieve this token, so if you lose your .env file and need the token again, you will need to create a new one and replace it.

While generating a new token, you have to choose which permissions to give to the token, make sure to check:
- **repo**
- **workflow**

![AccessToken](/docs/GithubAccessToken.jpg)

### ESP32 USB Connection
To upload code to the ESP32 board consider:
- If you are using Windows or a virtual machine, it is needed to press the BOOT button whilst the program is being uploaded, otherwise the system will have an error telling you the board is not in flash mode.
- It is highly recommended to use Linux, you don't need to press the button, but it is needed to give write permissions to the USB port, preferably the USB 0.
- To give permissions to the USB ports: (Reboot needed)
```
sudo usermod -a -G dialout $USER
```

### Self-Hosted Containerized Runner
A Runner is a server which runs the actions (or jobs) included in the workflow of this repository, found in **.github/workflows** folder. A self-hosted runner means it will run on your computer, containerized because we will use a Docker container to keep all the dependencies as simple as possible.
To start the runner, open a terminal on the root of the repository and run this command:
```
sudo docker-compose up
```
The initialization of the container will take about 4 minutes. In case of a misconfiguration with the access token, the runner will tell you there is an error with the GitHub authentication.

## Workflow
To trigger the workflow you need to push changes in the folders: **/src/**, **/include/** and **/test/**. Our recommendation is to change only the LED blink frequency, and keep the code as it is. Change the define DELAY in line 7 of **/include/blink.h** to a noticeable value you can distinguish in the LED on the board. For example, toggle between 200 and 2000 ms of DELAY. After pushing the changes, the code will compile, test and upload to the board automatically. This is the advantage of DevOps, saving time and being less error prone.

### Tests
There are 2 simple tests made before uploading the code to the board. First test is if the DELAY value is greater or equal than 50 ms, and the second tests if the DELAY value is less or equal than 5000 ms.
If you choose a value outside the **valid DELAY range (50 - 5000 ms)**, the tests will fail and **the code will not be uploaded to the board**, simulating a possible damage prevention.

## References
- [Docker](https://www.docker.com)
- [TCardonne GitHub Runner](https://registry.hub.docker.com/r/tcardonne/github-runner)
- [PlatformIO](https://platformio.org)