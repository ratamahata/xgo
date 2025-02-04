# XgO

![image](img/go4jigular.jpg)

The aim of the project is to add multi-platform UI for [X&O game](https://github.com/ratamahata/xo)
based on [Fyne](http://fyne.io) toolkit,<br/>
pretty much on it's [tic tac toe](https://github.com/fyne-io/examples/tree/develop/tictactoe) example

![image](img/itsfynecg.jpg)

To install,
1. Make sure you have all required instruments installed on your system:
- [GoLang](https://go.dev/doc/install)
- [SWIG](https://www.swig.org/download.html) (used to [incorporate C/C++ code into GO Lang](https://www.swig.org/Doc4.3/Go.html))
- [GIT](https://git-scm.com/downloads)

 Also, make sure all the commands are available on your PATH:

```console
 go version
 ```
 ```console
 swig -version
 ```
 ```console
 git version
```
 
2. Clone the repo and run, or build:

```console
git clone https://github.com/ratamahata/xgo
```
 ```console
 cd xgo
```
 ```console
go run .
```

The last command will build to the temporary folder and run from there. 
To build the executable for your OS in current folder, you may use:

```console
go build .
```
3. Enjoy.

![image](img/rstraight.jpg)
