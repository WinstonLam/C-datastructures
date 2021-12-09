<!-- Made by: Jorrit Stutterheim -->

# Introduction

Build and debug you C applications from VSCode on your mac laptop using 'devcontainers'.

Provided files:
- Dockerfile
- .devcontainer.json (keep in mind hidden file, see the dot in front)

.vscode folder containing: (keep in mind hidden folder, see the dot in front)
- launch.json
- tasks.json
- settings.json
- this readme :)

# How does it work

A docker container with all required C tools and visual studio extensions will be mounted by VSCode. If all setting files are configured correctly debugging should work out of the box.

With current provided settings it will always build the file you have open in your editor, so if there are dependencies that need to be build specify these in .vscode/tasks.json. More details below.

# Step-by-step guide

## Mounting the container
- Build docker image locally by doing something like this:
```
docker build -t gdb-cpp-image .
```

- Place the .devcontainer.json file in the root of your repository
- Place the .vscode folder in the root of your repository
- Open your repository in VSCode
- Open the remote container with the VSCode 'remote-container Open Workspace in container' command
- This should open the container for your current session and install all required extensions, this might take a minute.
- In the bottom left of your screen you should now see that you're working on a container

## Debugging

Provided files are configured for debugging heap.c in assignment 4. How this works is:
- Open heap.c
- Create a main function:
```
int main(){
    printf("Hoi\n");
    printf("Alweer hoi\n");
}
```
- set a breakpoint at one of those lines
- Press debug (F5)

- Have a look in 'tasks.json', there you can see:
```
"${file}",
"array.c",
```
Which means it build the local file and need to build array.c as well for this to work. (So you might need to change this in later assignments.)

- Have a look in launch.json, there you can add arguments that will be passed to your program. For example:
```
"args": ["<" "tests/opgave_voorbeeld_duration.txt"]
```
or just nothing:
```
"args": []
```


