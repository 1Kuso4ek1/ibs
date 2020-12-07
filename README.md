# ibs
IBS is the simplest build system for linux
## Changelog
#### 07.12.20
> Now you can write instruction files for windows and linux.
> Now ibs will ask you to install gcc if you on windows (only manual installation)
## Building 
```
git clone https://github.com/1Kuso4ek1/ibs
cd ibs/src
g++ main.cpp -o ibs
```
## Using
To build and run example programm you need to copy all files from "example" folder to "src", then execute this commands:
```
./ibs building.ibs libsinstall.ibs
./pong
```
Now you can play simple pong. :)
## Build file
Let's open example/building.ibs
```
files
{
pong.cpp
}
name
{
pong
}
libs
{
sfml-graphics
sfml-window
sfml-system
sfml-audio
}
```
In "files" you can write all your project files, in "name" you can write any word, it will be the name of your program, in libs you can write all libraries, which you use.
## Instructions file
Let's open example/libsinstall.ibs
```
linux {
sudo apt install libsfml-dev
}
```
In "linux" you can write any linux commands, in "windows" (I was too lazy to write installing commands for windows :D) you can write any windows commands. For example, you can use instructions file for installing libraries.
P. S. This is an optional argument
