# GLUT_RUN

Open this project in VS Code and use the dev container to build and run GLUT programs inside Docker.

Create the devcontainer folder and files on mac/Linux:

```bash
mkdir -p .devcontainer
touch .devcontainer/devcontainer.json
touch .devcontainer/Dockerfile
touch .devcontainer/start-vnc.sh
```

Create the same files in Windows PowerShell:

```powershell
New-Item -ItemType Directory -Force .devcontainer
New-Item -ItemType File -Force .devcontainer/devcontainer.json
New-Item -ItemType File -Force .devcontainer/Dockerfile
New-Item -ItemType File -Force .devcontainer/start-vnc.sh
```

Create the same files in Windows CMD:

```cmd
mkdir .devcontainer
type nul > .devcontainer\devcontainer.json
type nul > .devcontainer\Dockerfile
type nul > .devcontainer\start-vnc.sh
```


How to run it (same steps on Mac and Windows)

Install Docker Desktop and make sure it's running.
In VS Code, install the Dev Containers extension (ms-vscode-remote.remote-containers).
Create the files above in your Roshni folder.
Open the folder in VS Code → press F1 → run "Dev Containers: Reopen in Container". First time builds the image (a few minutes).
Once it reopens, a browser tab should pop to the noVNC page. If not, open http://localhost:6080/vnc.html yourself and click Connect.
In the VS Code terminal (which is now inside the container), compile and run:

   gcc main.c -o app -lglut -lGLU -lGL && ./app