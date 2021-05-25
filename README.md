# crudeimg
Very simple Linux image viewer. Written in c++ using SFML library. It is very small and fast, but it's only features are displaying, moving and zooming images

### Dependencies:
  - SFML 2.5

### Installation:
  1. Download main.cpp and build it.
     - g++ -c main.cpp -o crudeimg.o
     - g++ crudeimg.o -o crudeimg -lsfml-graphics -lsfml-window -lsfml-system -O3
  2. Copy binary file to /usr/bin (optional)
     - sudo cp crudeimg /usr/bin/crudeimg
  
### Usage:</br>
crudeimg <path_to_image>

exaple:
crudeimg ~/img.png

### Key Bindings:
  - Left Shift + R - Set image scale to make it fit window size.
  - Left Shift + O - Set image scale to 1.0.

### Supported image formats:
png, bmp, jpg, psd
