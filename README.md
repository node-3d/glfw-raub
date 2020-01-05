# GLFW for Node.js

This is a part of [Node3D](https://github.com/node-3d) project.

[![NPM](https://nodei.co/npm/glfw-raub.png?compact=true)](https://www.npmjs.com/package/glfw-raub)

[![Build Status](https://api.travis-ci.com/node-3d/glfw-raub.svg?branch=master)](https://travis-ci.com/node-3d/glfw-raub)
[![CodeFactor](https://www.codefactor.io/repository/github/node-3d/glfw-raub/badge)](https://www.codefactor.io/repository/github/node-3d/glfw-raub)

> npm i glfw-raub


## Synopsis

**Node.js** addon with **GLFW3** bindings.

> Note: this **addon uses N-API**, and therefore is ABI-compatible across different
Node.js versions. Addon binaries are precompiled and **there is no compilation**
step during the `npm i` command.

![Example](examples/screenshot.jpg)

* **GLFW** version **3.3** backend.
* Exposes low-level **GLFW** interface.
* Multiple windows for a single **Node.js** process.
* Able to switch to fullscreen and back.
* Has `Document` class, capable of tricking other libs, as if we are in a browser.


## Usage

This is a rather low level interface, where most of the stuff is directly reflecting
GLFW interfaces. Do not expect much. See [GLFW Docs](http://www.glfw.org/docs/latest/group__window.html)
for useful info on what it does and doesn't.

As per this lib, 3 entities are exported: GLFW itself, and `Window` and `Document` classes.

```js
const glfw = require('glfw-raub');
const { Window, Document } = glfw;
```

Here `glfw` is a low level interface container, where all `glfw*` functions are accessible as
`glfw.*`. E.g. `glfwSetWindowTitle` -> `glfw.setWindowTitle`.

`glfw.createWindow(w, h, emitter, title, display)` - this function differs from GLFW Docs
signature due to JS specifics. Here `emitter` is any object having **BOUND** `emit()` method.
It will be used to transmit GLFW events.

Classes `Window` and `Document` are created for convenience and documented in more detail below.

----------


### class Window

`Window` is high level js-wrapper around the above functions, which helps in managing window
instances. It also extends
[EventEmitter](https://nodejs.org/docs/latest-v12.x/api/events.html#events_events).

**Constructor:**

* `Window({ title, width, height, display, vsync, mode, autoIconify, msaa })`
	* `string title $PWD` - window title, takes current directory as default.
	* `number width 800` - window initial width.
	* `number height 600` - window initial height.
	* `number display undefined` - display id to open window on a specific display.
	* `boolean vsync false` - if vsync should be used.
	* `boolean fullscreen false` - if the window is fullscreen, takes presedence over `mode`.
	* `string mode 'windowed'` - one of `'windowed', 'borderless', 'fullscreen'`.
	* `boolean autoIconify true` - if fullscreen windows should iconify automatically on focus loss.
	* `number msaa 2` - multisample antialiasing level.
	* `Image icon null` - winodw icon.
	* `boolean decorated true` - if window has borders (use `false` for borderless fullscreen).


**Properties:**

* `get number ratio 1` - the ratio between physical and logical pixels, e.g `2` for Retina.
* `get number devicePixelRatio 1` - alias for `ratio`.
* `get number handle` - window pointer.
* `get/set string mode 'windowed'` - one of `'windowed', 'borderless', 'fullscreen'`. Here
`'borderless'` emulates fullscreen by a frameless, screen-sized window.
When this property is changed, a new window is created and the old is hidden.
* `get/set number width` - width in LOGICAL pixels, i.e. for Retina twice the window size.
* `get/set number height` - height in LOGICAL pixels, i.e. for Retina twice the window size.
* `get/set number offsetWidth` - alias for `width`.
* `get/set number offsetHeight` - alias for `height`.
* `get/set number w` - alias for `width`.
* `get/set number h` - alias for `height`.
* `get/set [width, height] wh` - an Array, containing LOGICAL width and height.
* `get/set { width, height } pxSize` - an Object, containing LOGICAL width and height.
* `get/set number innerWidth` - alias for `width`.
* `get/set number innerHeight` - alias for `height`.
* `get/set number clientWidth` - alias for `width`.
* `get/set number clientHeight` - alias for `height`.
* `get/set onkeydown` - alias for `.on('keydown', cb)`. Getter returns an Array of callbacks.
* `get/set onkeyup` - alias for `.on('keyup', cb)`. Getter returns an Array of callbacks.
* `get/set onmousedown` - alias for `.on('mousedown', cb)`. Getter returns an Array of callbacks.
* `get/set onmouseup` - alias for `.on('mouseup', cb)`. Getter returns an Array of callbacks.
* `get/set onwheel` - alias for `.on('wheel', cb)`. Getter returns an Array of callbacks.
* `get/set onmousewheel` - alias for `.on('mousewheel', cb)`. Getter returns an Array of callbacks.
* `get/set onresize` - alias for `.on('resize', cb)`. Getter returns an Array of callbacks.
* `get/set { width, height } size` - an Object, containing PHYSICAL width and height of the window.
* `get number scrollX` - always 0.
* `get number scrollY` - always 0.
* `get/set string title` - window title.
* `get/set Image{width, height, Buffer data, ?noflip} icon null` - window icon in RGBA format. Consider
using [this Image implementation](https://github.com/node-3d/image-raub). The given image is
vertically flipped if `noflip` is not set to `true`. Also see [this example](examples/icon.js).
* `get number msaa` - number of msaa samples.
* `get string version` - OpenGL vendor info.
* `get/set bool shouldClose` - if window is going to be closed.
* `get number platformWindow` - window HWND pointer.
* `get number platformContext` - OpenGL context handle.
* `get/set { x, y } pos` - an Object, containing the window position coordinates.
* `get/set number x` - window position X-coordinate on the screen.
* `get/set number y` - window position Y-coordinate on the screen.
* `get { width, height } framebufferSize` - the size of allocated framebuffer, same as `pxSize`.
* `get number currentContext` - which OpenGL context is now current.
* `get/set { x, y } cursorPos` - an Object, containing the cursor position coordinates.

---

**Methods:**

* `Monitor getCurrentMonitor()` - get a monitor having the most overlap with this window.
`Monitor` struct is described below.
* `Rect getBoundingClientRect()` - gets a
[browserlike](https://developer.mozilla.org/en-US/docs/Web/API/Element/getBoundingClientRect)
rect of this window.
* `number getKey(number key)` - get
[key](https://www.glfw.org/docs/latest/group__keys.html) state (`GLFW_PRESS/GLFW_RELEASE`).
* `number getMouseButton(number button)` - get mouse
[button](https://www.glfw.org/docs/latest/group__buttons.html) state (`GLFW_PRESS/GLFW_RELEASE`).
* `number getWindowAttrib(number attrib)` - get window
[attribute](https://www.glfw.org/docs/latest/window_guide.html#window_attribs).
* `setInputMode(number mode, number value)` - set input mode
[option](https://www.glfw.org/docs/latest/group__input.html#gaa92336e173da9c8834558b54ee80563b).
* `swapBuffers()` - swaps the front and back buffers of the window.
* `makeCurrent()` - make this window's GL context current.
* `destroy()` - destroy the GLFW window.
* `iconify()` - minimize the window.
* `restore()` - restore the window if it was previously iconified or maximized.
* `hide()` - hide the window.
* `show()` - show the window, if it is hidden.
* `on(string type, func cb)` - subscribe an event listener. See
[EventEmitter](https://nodejs.org/docs/latest-v12.x/api/events.html#events_events)
* `emit(string type, Object data)` - emit an event on behalf of this window.
* `dispatchEvent({ string type, ... })` - alias for `emit`, `type` is expected inside the event object.
* `addEventListener(string type, func cb)` - alias for `on`.
* `removeEventListener(string type, func cb)` - alia for `removeListener`.
* `number requestAnimationFrame(cb)` - **BOUND** `requestAnimationFrame` method, returns id.
* `cancelAnimationFrame(id)` - **BOUND** `cancelAnimationFrame` method. Cancels by id.

```
Monitor = {
	bool is_primary // is this screen primary
	string name // screen name
	number pos_x // global position x of the screen
	number pos_y // global position y of the screen
	number width_mm // screen width in mm
	number height_mm // screen height in mm
	number width // screen width
	number height // screen height
	number rate // refresh rate
	modes: [{
		number width // screen width
		number height // screen height
		number rate // refresh rate
	}]
}
```

---

**Events:**

* `'blur'` - window [focus lost](https://developer.mozilla.org/en-US/docs/Web/Events/blur)
* `'click'` - mouse button [clicked](https://developer.mozilla.org/en-US/docs/Web/Events/click)
* `'drop'` - drag-[dropped](https://developer.mozilla.org/en-US/docs/Web/Events/drop) some files on the window
* `'focus'` - window [focus gained](https://developer.mozilla.org/en-US/docs/Web/Events/focus)
* `'focusin'` - window [focus gained](https://developer.mozilla.org/en-US/docs/Web/Events/focusin)
* `'focusout'` - window [focus lost](https://developer.mozilla.org/en-US/docs/Web/Events/focusout)
* `'resize'` - render-surface resized (values in pixels) `{ width, height }`
* `'iconifiy'` - window was iconified
* `'keydown'` - keyboard [key down](https://developer.mozilla.org/en-US/docs/Web/Events/keydown)
* `'keyup'` - keyboard [key up](https://developer.mozilla.org/en-US/docs/Web/Events/keyup)
* `'mousedown'` - mouse [button down](https://developer.mozilla.org/en-US/docs/Web/Events/mousedown)
* `'mouseenter'` - mouse [entered](https://developer.mozilla.org/en-US/docs/Web/Events/mouseenter) the window
* `'mouseleave'` - mouse [left](https://developer.mozilla.org/en-US/docs/Web/Events/mouseleave) the window
* `'mouseup'` - mouse [button up](https://developer.mozilla.org/en-US/docs/Web/Events/mouseup)
* `'quit'` - window closed
* `'refresh'` - window needs to be redrawn
* `'wresize'` - window frame resized (NOT really pixels) `{ width, height }`
* `'wheel'` - mouse [wheel rotation](https://developer.mozilla.org/en-US/docs/Web/Events/wheel)
* `'move'` - window moved `{ x, y }`

> Note: `keypress` event is not supported.


----------

### class Document

`Document` extends `Window` to provide an additional web-style compatibility layer.
As name suggests, objects of such class will mimic the behavior and properties of
your typical browser `window.document`. But also it is a `Window`, at the same time.
And it is incomplete at this point: you still have to provide an `Image` class of
your choice and WebGL context (implementation). Two static methods are designated
for this:

* static setImage(Image) - for example,
[this Image implementation](https://github.com/raub/node-image)
is designed to fit perfectly. Also sets `global.HTMLImageElement`.

* static setWebgl(webgl) - for example,
[this WebGL implementation](https://github.com/raub/node-webgl)
is designed to fit perfectly.


**Constructor:**

* `Document({ ...<Window opts>, ignoreQuit, autoFullscreen })`
	* `boolean ignoreQuit false` - if the window should ignore the default quit signals, e.g. ESC key.
	* `boolean autoFullscreen false` - if window has default key handlers for fullscreen:
		* CTRL+F - borderless fullscreen window.
		* CTRL+ALT+F - real, exclusive fullscreen mode.
		* CTRL+SHIFT+F - back to windowed.


**Properties:**

* `get body` - returns `this`.
* `get style` - mimic web-element `style` property.
* `get context` - returns `Document.webgl`, set through `Document.setWebgl`.


**Methods:**

* `getContext()` - returns `Document.webgl`, set through `Document.setWebgl`.
* `getElementById(id)` - returns `this`.
* `getElementsByTagName(tag)` - if contains 'canvas', returns `this`, otherwise `null`.
* `createElementNS(_0, name)` - returns the result of `createElement(name)`.
* `createElement(name)` - for `'canvas'` returns `this` for *the first call*,
then returns new instances of canvas-like object capable of using 2d or 3d context.
This is done for some web APIs like three.js, which create additional canvases.
For `'image'` returns `new Document.Image`, set through `Document.setImage`.
