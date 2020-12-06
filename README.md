# ibs
IBS is the simplest build system for linux
## Building 
```
git clone https://github.com/1Kuso4ek1/ibs
cd ibs/src
g++ main.cpp -o ibs
```
## Using
To build and run example programm you need to copy all files from "example" folder to "src", then execute this commands:
```
./ibs building.ibs libsinstall.ins
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
In this file you can write any linux commands, for example, you can install libraries.
P. S. This is an optional argument
