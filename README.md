# GLFW for Node.js

GLFW 3 crossplatform addon with minimized dependencies.

* IMPORTANT: as of 0.7.0 all methods start lowercase, like in WebGL. This breaks compatibility
of any old code, that would call `glfw.method()`. If you want to upgrade, consider the following
search pattern regexp: `glfw\.\w+\(`. Any modern text editor should help to easily handle this.


## Install

```
npm i -s node-glfw-raub
```

Note: as this is a compiled addon, compilation tools must be in place on your system.
Such as MSVS13 for Windows, where `npm install --global windows-build-tools` most probably helps.


## Use

This is a rather low level interface, where most of the stuff is directly reflecting
GLFW interfaces. Do not expect much. See [GLFW Docs](http://www.glfw.org/docs/latest/group__window.html)
for useful info on what it does and doesn't.

As per this lib, 2 entities are exported.

```js
const glfw = require('node-glfw-raub');
const { Window } = glfw;
```

Here `glfw` is low level interface container, where all `glfw*` functions are accessible as
`glfw.*`. E.g. `glfwSetWindowTitle` -> `glfw.SetWindowTitle`.

`glfw.createWindow(w, h, emitter, title, display)` - this function differs from GLFW Docs
signature due to JS specifics. Here `emitter` is any object having `emit()` method
**AS OWN PROPERTY**, specifically not prototype property. It will be used to transmit
glfw events.


----------

### class Window

`Window` is higher level js-wrapper around the above functions, which helps in managing window
instances. It basically has all the functionality where in GLFW Docs `window` parameter
is mentioned. E.g. `glfwSetWindowTitle (window, title)` -> `window.title = title`.

There are few simple rules for the above transformation to become intuitive:

* API is available if it has `window` parameter.
* All props start lowercase.
* Word "Window" is omitted.
* Whatever could have a `get/set` interface is made so.


Constructor:

* `Window({ title, width, height, display, vsync, fullscreen, msaa })`
	* `string title $PWD` - window title, takes current directory as default.
	* `number width 800` - window initial width.
	* `number height 600` - window initial height.
	* `number display undefined` - display id to open window on a specific display.
	* `boolean vsync false` - if vsync should be used.
	* `boolean fullscreen false` - if fullscreen should be used.
	* `number msaa 2` - multisample antialiasing level.
	* `boolean decorated true` - if window has borders (use `false` for borderless fullscreen).


Properties:

* `get number handle` - window pointer.
* `get string version` - OpenGL vendor info.
* `get number platformWindow` - window HWND pointer.
* `get number platformContext` - OpenGL context handle.
* `get {width, height} framebufferSize` - the size of allocated framebuffer.
* `get number currentContext` - what GLFW window is now current.
* `get number samples` - number of msaa samples passed to the constructor.

---

* `get/set number width|w` - window width.
* `get/set number height|h` - window height.
* `get/set [width, height] wh` - window width and height.
* `get/set {width, height} size` - window width and height.
* `get/set string title` - window title.
* `get/set boolean shouldClose` - if window is going to be closed.
* `get/set {x, y} pos` - where window is on the screen.
* `get/set {x, y} cursorPos` - where mouse is relative to the window.

---

* `getKey(number key)` - `glfw.getKey(window, key)`.
* `getMouseButton(number button)` - `glfw.getMouseButton(window, button)`.
* `getWindowAttrib(number attrib)` - `glfw.getWindowAttrib(window, attrib)`.
* `setInputMode(number mode)` - `glfw.setInputMode(window, mode)`.
* `swapBuffers()` - `glfw.swapBuffers(window)`.
* `makeCurrent()` - `glfw.makeContextCurrent(window)`.
* `destroy()` - `glfw.destroyWindow(window)`.
* `iconify()` - `glfw.iconifyWindow(window)`.
* `restore()` - `glfw.restoreWindow(window)`.
* `hide()` - `glfw.hideWindow(window)`.
* `show()` - `glfw.showWindow(window)`.
* `on(string type, function cb)` - listen for window events.


## GLFW events:

* `'window_pos'` - window moved
* `'resize'` - window frame resized
* `'framebuffer_resize'` - render-surface resized
* `'drop'` - drag-dropped some files on the window
* `'quit'` - window closed
* `'refresh'` - window needs to be redrawn
* `'iconified'` - window was iconified
* `'focused'` - focus gained/lost
* `'keyup'` - keyboard key up
* `'keydown'` - keyboard key down
* `'keypress'` - keyboard key pressed
* `'mousemove'` - mouse moved
* `'mouseenter'` - mouse entered/left the window
* `'mousedown'` - mouse button down
* `'mouseup'` - mouse button up
* `'click'` - mouse button clicked
* `'mousewheel'` - mouse wheel rotation
