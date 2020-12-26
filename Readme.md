# Table data reader

The project has been created for one of my friends to help him fetch data from the application that doesn't have any functions to export data and presents data at a custom grid widget and so data can't be read with WINAPI functions.

### Prerequisites

The development may be performed in these two environments:    
- Linux    
- Windows    

Prerequisites for Linux environment (Ubuntu 20.04 package list):    
- g++ 9    
- cmake    
    - dpkg-dev (required to build DEB-package only)    
    - cppcheck (required to static analysis only)    
- libtesseract-dev    
    - libopencv-dev    
    - tesseract-ocr-eng    
    - tesseract-ocr-rus    
- googletest    
    - googletest-tools    
    - libgtest-dev

Prerequisites for Windows environment:
- CMake 3.16+    
- Tesseract 4.1    
    - tiff 4.1.0    
        - libpng 1.6.37    
        - zlib 1.2.11    
        - jpeg 9d    
    - OpenCV 4.1    
        - laptonica 1.74.4    
- GoogleTest 1.10    
- cppcheck (only if you want to make static code analysis)    
- NSIS 3.6 (only if you want to make package)

Visual Studio Code extensions:    
- ms-vscode.cpptools    
- ms-vscode.cmake-tools    
- dbaeumer.vscode-eslint
    
### Installing

Development under Linux performs using Visual Studio Code with the 'Remote Containers' extension.
The environment is specified at .devcontainer/{devcontainer.json,Dockerfile}.    

Development under Windows performs using Visual Studio Code, MinGW-w64-v7.0.0. To build the project under Windows you have to build (with MinGW) or install the libraries/packages accounted in the ['Prerequisites'](#Prerequisites) section.    

The great start point to build Tesseract with MinGW is [here](https://github.com/ghimiredhikura/Install-OpenCV-with-Tesseract-Windows.git).
A step by step series of examples that tell you how to get a development env running.

The working Windows batch file to setup environment for Visual Studio Code looks like:
```
set MSYS2_PATH_TYPE=inherit
set PATH=D:\bin\cmake\bin;D:\bin\mingw64\mingw64\bin;D:\bin\nsis-3.06.1;D:\bin\git\bin;%PATH%
set OpenCV_DIR=D:\bin\OpenCV-4.1.1-MinGW
set GTEST_DIR=D:\bin\gtest
set Tesseract_DIR=D:\bin\tesseract-mingw\cmake
set Leptonica_DIR=D:\bin\tesseract-mingw\cmake
set TABREAD_EXTERNAL=D:/bin/tabread-external
```
The 'TABREAD_EXTERNAL' environment variable should point to the directory with Tesseract and MinGW shared libraries and data necessary for tabread executable.
The directory structure is listed below:
```
.
+-- bin
|   +-- tessdata
|       +-- eng.traineddata
|       +-- equ.traineddata
|       +-- rus.traineddata
+-- libgcc_s_seh-1.dll
+-- libleptonica-1.74.4.dll
+-- liblzma-5.dll
+-- libopencv_core411.dll
+-- libopencv_imgcodecs411.dll
+-- libopencv_imgproc411.dll
+-- libpng16.dll
+-- libstdc++-6.dll
+-- libtesseract41.dll
+-- libtiff.dll
+-- libwinpthread-1.dll
+-- libzstd.dll
```

To check application, build it with Visual Studio Code an then run the command from the build directory:
```
tabread/tabread -i=../data/000.png -o=output.txt
```
to run with single threaded mode OCR or run:
```
tabread/tabread -i=../data/000.png -o=output.txt -th
```
for multithreaded (parrallel execution policy) OCR.    

You may configure and then run 'package' target from the 'workspaceFolder' to generate installation bundle:
```
make -C build package
```
under Linux environment or
```
mingw-make -C package
```
under Windows environment.

## Running the tests

Run tests from Visual Studio Code:    
1. Open 'Command Palette' (Ctrl-Shift-P).
2. Type and then select 'CMake: Run Tests'.

## Deployment

1. Installation at Linux environment:    
```
dpkg -i tabread_1.0.0_amd64.deb
```
2. Installation at Windows environment:    
Execute the installation bundle 'tabread-X.Y.Z.exe', where X.Y.Z is a package version.

## Further work

1. Make GUI application (select widget/appliction or file, progress, etc.).
2. Read appliction's and filters' parameters from configuration file.
3. Improve grid detection algorithms.
4. Multiline text recognition.
