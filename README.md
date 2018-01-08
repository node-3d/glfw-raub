# GLFW for Node.js

GLFW 3 crossplatform addon with minimized dependencies.


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

`glfw.CreateWindow(w, h, emitter, title, display)` - this function differs from GLFW Docs
signature due to JS specifics. Here `emitter` is any object having `emit()` method
**AS OWN PROPERTY**, specifically not prototype property. It will be used to transmit
glfw events.

----------

`Window` is higher level js-wrapper around the above functions, which helps in managing window
instances. It basically has all the functionality where in GLFW Docs `window` parameter
is mentioned. E.g. `glfwSetWindowTitle (window, title)` -> `window.title = title`.

There are few simple rules for the above transformation to become intuitive:

* API is available if it has `window` parameter.
* All props start lowercase.
* Word "Window" is omitted.
* Whatever can have a `get/set` interface is done so.

Without further ado:

* get `handle` - window pointer as `int`
* get `version` - OpenGL vendor info as `string`
* get `platformWindow` - window HWND pointer as `int`
* get `platformContext` - OpenGL context handle as `int`
* get `framebufferSize` - the size of allocated framebuffer as `{width, height}`
* get `currentContext` - what GLFW window is now current as `int`

---

* get/set `width`|`w` - window width as `int`
* get/set `height`|`h` - window height as `int`
* get/set `wh` - window width and height as `[width, height]`
* get/set `size` - window width and height as `{width, height}`
* get/set `title` - window title as `string`
* get/set `shouldClose` - if window is going to be closed as `bool`
* get/set `pos` - where window is on the screen as `{x, y}`
* get/set `cursorPos` - where mouse is relative to the window as `{x, y}`

---

* `getKey(key)` - glfw.GetKey(window, key)
* `getMouseButton(button)` - glfw.GetMouseButton(window, button)
* `getWindowAttrib(attrib)` - glfw.GetWindowAttrib(window, attrib)
* `setInputMode(mode)` - glfw.SetInputMode(window, mode)
* `swapBuffers()` - glfw.SwapBuffers(window)
* `makeCurrent()` - glfw.MakeContextCurrent(window)
* `destroy()` - glfw.DestroyWindow(window)
* `iconify()` - glfw.IconifyWindow(window)
* `restore()` - glfw.RestoreWindow(window)
* `hide()` - glfw.HideWindow(window)
* `show()` - glfw.ShowWindow(window)
* `on(type, cb)` - listen for window events

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
