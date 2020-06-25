# Visualisation

## What Does It Do?
Simple C++ data visualisation tool.

## Why?
It's useful if you want to visualise large data quickly, and to do so without interrupting program flow at a minimal setup complexity.

While there are many existing visualisation tools, being able to customise how the data is interpretted and displayed is important.

## Dependencies (x64-windows):
* SDL2
* glew
* OpenGL
* stb [(simple but robust image loader)](https://github.com/nothings/stb)
* SimpleIni

## Build
*The intended architecture is 64 bits*

### 1. Visualisation.lib
* Build the static library from 'Visualisation/'

### 2. Visualisation.exe
* Build the executable from 'Display/' and 'Visualisation.lib'

### 3. Running
Add the absolute path of 'Visualisation.exe' to 'VisualisationConfig.ini' and place everything in the same directory

```path=C:/your/path/to/Visualisation.exe```
