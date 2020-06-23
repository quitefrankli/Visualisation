# Visualisation

## What Does It Do?
Visualises data graphically

## Why?
It's convenient to visualise large data without sacrificing speed or interrupting program flow at a minimal setup complexity

## Dependencies (x64-windows):
* SDL2
* glew
* OpenGL
* stb [Simple but robust image loader](https://github.com/nothings/stb)
* SimpleIni

## Installation
This is intended to be built on Windows in 64bit, but 32bit will also work provided the dependencies are also in 32bit.
Build 'Visualisation.lib' from 'Visualisation/' and 'Visualisation.exe' from 'Display/' and the previously built static library
After editing the two, add the absolute path of 'Visualisation.exe' to 'VisualisationConfig.ini'
```path=C:/your/path/to/Visualisation.exe```
Include the header and don't forget to link 'Visualisation.lib'