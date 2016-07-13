NodeJS bindings to GLFW
=======================

This projects attempts to provide platform-independent access to windowing system and input devices on desktop/laptop computers using GLFW 3 and above.

Dependencies
------------
- NodeJS
While v0.6.5+ work in many cases, some missing features for typed arrays are only available in v0.7.x. So we recommend at least v0.7.5.

- GLEW (http://glew.sourceforge.net/)
GLEW is used to find OpenGL extensions in a cross-platform manner.

- GLFW (http://www.glfw.org/)
GLFW is a simple multi-platform framework for opening a window, creating an OpenGL context, and managing input.

All of these libraries are cross-platform. node-glfw provides a Javascript wrapper to access native methods in GLFW.

Once dependent libraries are installed, do
```
npm install node-gyp
npm install --save bindings nan
node-gyp rebuild
```

Installation (Mac)
-------------------------
Use Homebrew
```
brew install pkg-config glfw3 glew
```

Installation (Linux)
-------------------------
Use apt-get or similar package manager
```
sudo apt-get install libxrandr-dev libxinerama-dev libxcursor-dev libfreeimage-dev libglew-dev libxi-dev
```

Download GLFW3 (do not use ```apt-get install libglfw-dev```, it is wrong version)
```
cd glfw
mkdir build
cd build
cmake ..
make
sudo make install
```
Installation (Windows)
----------------------

Have Visual Studio (Express version works fine) installed.
Windows dependencies are bundled with this package, so `npm install node-glfw` should work out-of-box. The binary packages for Windows on their respective web site above do work as well but you'll need to change the path in ```bindings.gyp``` to point to where you installed them, includes and libs.

Notes
-----
- node-glfw is a just a platform binding so don't expect samples here. You should install node-webgl, which contains lots of tests and examples using node-glfw features.
- run ```node test/test.js``` to see a colored triangle with mouse tracking in the command-line. This indicates all is installed correctly.


