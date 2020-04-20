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
	* `boolean osxCore false` - use OSX core profile 3.2.0 context. Works only for OSX.
		This can also be replaced with a bunch of manual `glfw.windowHint` calls on any OS.
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

---

### glfw - low level

```js
const glfw = require('glfw-raub');
```

A full list of exported symbols:

* `glfw.hideConsole()` - **Windows ONLY** hides the console window, but only in case
this console window is property of this process. For instance this works if you use
`pkg` module to bundle your app, and then doubleclick the EXE. But if you are running
a command like `node script.js`, then this won't hide the window. **It's safe to call
this function on all platforms, but it will be ignored, unless the platform is Windows**.
* `glfw.showConsole()` - `glfwShowConsole()` - **Windows ONLY** shows the console window
if it was previously hidden with `glfw.hideConsole()`.
* `glfw.testScene()` - draws a test scene, used in examples here.
* `glfw.testJoystick()` - draws a test scene, that reacts to a joystick.


* `glfw.init()` - `glfwInit()`.
* `glfw.initHint()` - `glfwInitHint()`.
* `glfw.terminate()` - `glfwTerminate()`.
* `glfw.getVersion()` - `glfwGetVersion()`.
* `glfw.getVersionString()` - `glfwGetVersionString()`.
* `glfw.getError()` - `glfwGetError()`.
* `glfw.getTime()` - `glfwGetTime()`.
* `glfw.setTime()` - `glfwSetTime()`.
* `glfw.getMonitors()` - `glfwGetMonitors()`.
* `glfw.getPrimaryMonitor()` - `glfwGetPrimaryMonitor()`.
* `glfw.windowHint()` - `glfwWindowHint()`.
* `glfw.windowHintString()` - `glfwWindowHintString()`.
* `glfw.defaultWindowHints()` - `glfwDefaultWindowHints()`.
* `glfw.joystickPresent()` - `glfwJoystickPresent()`.
* `glfw.getJoystickAxes()` - `glfwGetJoystickAxes()`.
* `glfw.getJoystickButtons()` - `glfwGetJoystickButtons()`.
* `glfw.getJoystickName()` - `glfwGetJoystickName()`.
* `glfw.createWindow()` - `glfwCreateWindow()`.
* `glfw.destroyWindow()` - `glfwDestroyWindow()`.
* `glfw.setWindowTitle()` - `glfwSetWindowTitle()`.
* `glfw.setWindowIcon()` - `glfwSetWindowIcon()`.
* `glfw.getWindowSize()` - `glfwGetWindowSize()`.
* `glfw.getWindowFrameSize()` - `glfwGetWindowFrameSize()`.
* `glfw.getWindowContentScale()` - `glfwGetWindowContentScale()`.
* `glfw.setWindowSize()` - `glfwSetWindowSize()`.
* `glfw.setWindowSizeLimits()` - `glfwSetWindowSizeLimits()`.
* `glfw.setWindowAspectRatio()` - `glfwSetWindowAspectRatio()`.
* `glfw.setWindowPos()` - `glfwSetWindowPos()`.
* `glfw.getWindowPos()` - `glfwGetWindowPos()`.
* `glfw.getWindowOpacity()` - `glfwGetWindowOpacity()`.
* `glfw.setWindowOpacity()` - `glfwSetWindowOpacity()`.
* `glfw.maximizeWindow()` - `glfwMaximizeWindow()`.
* `glfw.focusWindow()` - `glfwFocusWindow()`.
* `glfw.requestWindowAttention()` - `glfwRequestWindowAttention()`.
* `glfw.getWindowMonitor()` - `glfwGetWindowMonitor()`.
* `glfw.getFramebufferSize()` - `glfwGetFramebufferSize()`.
* `glfw.iconifyWindow()` - `glfwIconifyWindow()`.
* `glfw.restoreWindow()` - `glfwRestoreWindow()`.
* `glfw.hideWindow()` - `glfwHideWindow()`.
* `glfw.showWindow()` - `glfwShowWindow()`.
* `glfw.windowShouldClose()` - `glfwWindowShouldClose()`.
* `glfw.setWindowShouldClose()` - `glfwSetWindowShouldClose()`.
* `glfw.getWindowAttrib()` - `glfwGetWindowAttrib()`.
* `glfw.setWindowAttrib()` - `glfwSetWindowAttrib()`.
* `glfw.setInputMode()` - `glfwSetInputMode()`.
* `glfw.getInputMode()` - `glfwGetInputMode()`.
* `glfw.pollEvents()` - `glfwPollEvents()`.
* `glfw.waitEvents()` - `glfwWaitEvents()`.
* `glfw.waitEventsTimeout()` - `glfwWaitEventsTimeout()`.
* `glfw.postEmptyEvent()` - `glfwPostEmptyEvent()`.
* `glfw.getKey()` - `glfwGetKey()`.
* `glfw.getMouseButton()` - `glfwGetMouseButton()`.
* `glfw.getCursorPos()` - `glfwGetCursorPos()`.
* `glfw.setCursorPos()` - `glfwSetCursorPos()`.
* `glfw.makeContextCurrent()` - `glfwMakeContextCurrent()`.
* `glfw.getCurrentContext()` - `glfwGetCurrentContext()`.
* `glfw.swapBuffers()` - `glfwSwapBuffers()`.
* `glfw.swapInterval()` - `glfwSwapInterval()`.
* `glfw.extensionSupported()` - `glfwExtensionSupported()`.
* `glfw.rawMouseMotionSupported()` - `glfwRawMouseMotionSupported()`.
* `glfw.getKeyName()` - `glfwGetKeyName()`.
* `glfw.getKeyScancode()` - `glfwGetKeyScancode()`.
* `glfw.getJoystickHats()` - `glfwGetJoystickHats()`.
* `glfw.joystickIsGamepad()` - `glfwJoystickIsGamepad()`.
* `glfw.updateGamepadMappings()` - `glfwUpdateGamepadMappings()`.
* `glfw.getGamepadName()` - `glfwGetGamepadName()`.
* `glfw.getGamepadState()` - `glfwGetGamepadState()`.
* `glfw.setClipboardString()` - `glfwSetClipboardString()`.
* `glfw.getClipboardString()` - `glfwGetClipboardString()`.
* `glfw.getTimerValue()` - `glfwGetTimerValue()`.
* `glfw.getTimerFrequency()` - `glfwGetTimerFrequency()`.
* `glfw.platformWindow()` - `glfwPlatformWindow()`.
* `glfw.platformContext()` - `glfwPlatformContext()`.
* `glfw.getJoystickGUID()` - `glfwGetJoystickGUID()`.
* `glfw.VERSION_MAJOR` - `GLFW_VERSION_MAJOR`.
* `glfw.VERSION_MINOR` - `GLFW_VERSION_MINOR`.
* `glfw.VERSION_REVISION` - `GLFW_VERSION_REVISION`.
* `glfw.TRUE` - `GLFW_TRUE`.
* `glfw.FALSE` - `GLFW_FALSE`.
* `glfw.RELEASE` - `GLFW_RELEASE`.
* `glfw.PRESS` - `GLFW_PRESS`.
* `glfw.REPEAT` - `GLFW_REPEAT`.
* `glfw.HAT_CENTERED` - `GLFW_HAT_CENTERED`.
* `glfw.HAT_UP` - `GLFW_HAT_UP`.
* `glfw.HAT_RIGHT` - `GLFW_HAT_RIGHT`.
* `glfw.HAT_DOWN` - `GLFW_HAT_DOWN`.
* `glfw.HAT_LEFT` - `GLFW_HAT_LEFT`.
* `glfw.HAT_RIGHT_UP` - `GLFW_HAT_RIGHT_UP`.
* `glfw.HAT_RIGHT_DOWN` - `GLFW_HAT_RIGHT_DOWN`.
* `glfw.HAT_LEFT_UP` - `GLFW_HAT_LEFT_UP`.
* `glfw.HAT_LEFT_DOWN` - `GLFW_HAT_LEFT_DOWN`.
* `glfw.KEY_UNKNOWN` - `GLFW_KEY_UNKNOWN`.
* `glfw.KEY_SPACE` - `GLFW_KEY_SPACE`.
* `glfw.KEY_APOSTROPHE` - `GLFW_KEY_APOSTROPHE`.
* `glfw.KEY_COMMA` - `GLFW_KEY_COMMA`.
* `glfw.KEY_MINUS` - `GLFW_KEY_MINUS`.
* `glfw.KEY_PERIOD` - `GLFW_KEY_PERIOD`.
* `glfw.KEY_SLASH` - `GLFW_KEY_SLASH`.
* `glfw.KEY_0` - `GLFW_KEY_0`.
* `glfw.KEY_1` - `GLFW_KEY_1`.
* `glfw.KEY_2` - `GLFW_KEY_2`.
* `glfw.KEY_3` - `GLFW_KEY_3`.
* `glfw.KEY_4` - `GLFW_KEY_4`.
* `glfw.KEY_5` - `GLFW_KEY_5`.
* `glfw.KEY_6` - `GLFW_KEY_6`.
* `glfw.KEY_7` - `GLFW_KEY_7`.
* `glfw.KEY_8` - `GLFW_KEY_8`.
* `glfw.KEY_9` - `GLFW_KEY_9`.
* `glfw.KEY_SEMICOLON` - `GLFW_KEY_SEMICOLON`.
* `glfw.KEY_EQUAL` - `GLFW_KEY_EQUAL`.
* `glfw.KEY_A` - `GLFW_KEY_A`.
* `glfw.KEY_B` - `GLFW_KEY_B`.
* `glfw.KEY_C` - `GLFW_KEY_C`.
* `glfw.KEY_D` - `GLFW_KEY_D`.
* `glfw.KEY_E` - `GLFW_KEY_E`.
* `glfw.KEY_F` - `GLFW_KEY_F`.
* `glfw.KEY_G` - `GLFW_KEY_G`.
* `glfw.KEY_H` - `GLFW_KEY_H`.
* `glfw.KEY_I` - `GLFW_KEY_I`.
* `glfw.KEY_J` - `GLFW_KEY_J`.
* `glfw.KEY_K` - `GLFW_KEY_K`.
* `glfw.KEY_L` - `GLFW_KEY_L`.
* `glfw.KEY_M` - `GLFW_KEY_M`.
* `glfw.KEY_N` - `GLFW_KEY_N`.
* `glfw.KEY_O` - `GLFW_KEY_O`.
* `glfw.KEY_P` - `GLFW_KEY_P`.
* `glfw.KEY_Q` - `GLFW_KEY_Q`.
* `glfw.KEY_R` - `GLFW_KEY_R`.
* `glfw.KEY_S` - `GLFW_KEY_S`.
* `glfw.KEY_T` - `GLFW_KEY_T`.
* `glfw.KEY_U` - `GLFW_KEY_U`.
* `glfw.KEY_V` - `GLFW_KEY_V`.
* `glfw.KEY_W` - `GLFW_KEY_W`.
* `glfw.KEY_X` - `GLFW_KEY_X`.
* `glfw.KEY_Y` - `GLFW_KEY_Y`.
* `glfw.KEY_Z` - `GLFW_KEY_Z`.
* `glfw.KEY_LEFT_BRACKET` - `GLFW_KEY_LEFT_BRACKET`.
* `glfw.KEY_BACKSLASH` - `GLFW_KEY_BACKSLASH`.
* `glfw.KEY_RIGHT_BRACKET` - `GLFW_KEY_RIGHT_BRACKET`.
* `glfw.KEY_GRAVE_ACCENT` - `GLFW_KEY_GRAVE_ACCENT`.
* `glfw.KEY_WORLD_1` - `GLFW_KEY_WORLD_1`.
* `glfw.KEY_WORLD_2` - `GLFW_KEY_WORLD_2`.
* `glfw.KEY_ESCAPE` - `GLFW_KEY_ESCAPE`.
* `glfw.KEY_ENTER` - `GLFW_KEY_ENTER`.
* `glfw.KEY_TAB` - `GLFW_KEY_TAB`.
* `glfw.KEY_BACKSPACE` - `GLFW_KEY_BACKSPACE`.
* `glfw.KEY_INSERT` - `GLFW_KEY_INSERT`.
* `glfw.KEY_DELETE` - `GLFW_KEY_DELETE`.
* `glfw.KEY_RIGHT` - `GLFW_KEY_RIGHT`.
* `glfw.KEY_LEFT` - `GLFW_KEY_LEFT`.
* `glfw.KEY_DOWN` - `GLFW_KEY_DOWN`.
* `glfw.KEY_UP` - `GLFW_KEY_UP`.
* `glfw.KEY_PAGE_UP` - `GLFW_KEY_PAGE_UP`.
* `glfw.KEY_PAGE_DOWN` - `GLFW_KEY_PAGE_DOWN`.
* `glfw.KEY_HOME` - `GLFW_KEY_HOME`.
* `glfw.KEY_END` - `GLFW_KEY_END`.
* `glfw.KEY_CAPS_LOCK` - `GLFW_KEY_CAPS_LOCK`.
* `glfw.KEY_SCROLL_LOCK` - `GLFW_KEY_SCROLL_LOCK`.
* `glfw.KEY_NUM_LOCK` - `GLFW_KEY_NUM_LOCK`.
* `glfw.KEY_PRINT_SCREEN` - `GLFW_KEY_PRINT_SCREEN`.
* `glfw.KEY_PAUSE` - `GLFW_KEY_PAUSE`.
* `glfw.KEY_F1` - `GLFW_KEY_F1`.
* `glfw.KEY_F2` - `GLFW_KEY_F2`.
* `glfw.KEY_F3` - `GLFW_KEY_F3`.
* `glfw.KEY_F4` - `GLFW_KEY_F4`.
* `glfw.KEY_F5` - `GLFW_KEY_F5`.
* `glfw.KEY_F6` - `GLFW_KEY_F6`.
* `glfw.KEY_F7` - `GLFW_KEY_F7`.
* `glfw.KEY_F8` - `GLFW_KEY_F8`.
* `glfw.KEY_F9` - `GLFW_KEY_F9`.
* `glfw.KEY_F10` - `GLFW_KEY_F10`.
* `glfw.KEY_F11` - `GLFW_KEY_F11`.
* `glfw.KEY_F12` - `GLFW_KEY_F12`.
* `glfw.KEY_F13` - `GLFW_KEY_F13`.
* `glfw.KEY_F14` - `GLFW_KEY_F14`.
* `glfw.KEY_F15` - `GLFW_KEY_F15`.
* `glfw.KEY_F16` - `GLFW_KEY_F16`.
* `glfw.KEY_F17` - `GLFW_KEY_F17`.
* `glfw.KEY_F18` - `GLFW_KEY_F18`.
* `glfw.KEY_F19` - `GLFW_KEY_F19`.
* `glfw.KEY_F20` - `GLFW_KEY_F20`.
* `glfw.KEY_F21` - `GLFW_KEY_F21`.
* `glfw.KEY_F22` - `GLFW_KEY_F22`.
* `glfw.KEY_F23` - `GLFW_KEY_F23`.
* `glfw.KEY_F24` - `GLFW_KEY_F24`.
* `glfw.KEY_F25` - `GLFW_KEY_F25`.
* `glfw.KEY_KP_0` - `GLFW_KEY_KP_0`.
* `glfw.KEY_KP_1` - `GLFW_KEY_KP_1`.
* `glfw.KEY_KP_2` - `GLFW_KEY_KP_2`.
* `glfw.KEY_KP_3` - `GLFW_KEY_KP_3`.
* `glfw.KEY_KP_4` - `GLFW_KEY_KP_4`.
* `glfw.KEY_KP_5` - `GLFW_KEY_KP_5`.
* `glfw.KEY_KP_6` - `GLFW_KEY_KP_6`.
* `glfw.KEY_KP_7` - `GLFW_KEY_KP_7`.
* `glfw.KEY_KP_8` - `GLFW_KEY_KP_8`.
* `glfw.KEY_KP_9` - `GLFW_KEY_KP_9`.
* `glfw.KEY_KP_DECIMAL` - `GLFW_KEY_KP_DECIMAL`.
* `glfw.KEY_KP_DIVIDE` - `GLFW_KEY_KP_DIVIDE`.
* `glfw.KEY_KP_MULTIPLY` - `GLFW_KEY_KP_MULTIPLY`.
* `glfw.KEY_KP_SUBTRACT` - `GLFW_KEY_KP_SUBTRACT`.
* `glfw.KEY_KP_ADD` - `GLFW_KEY_KP_ADD`.
* `glfw.KEY_KP_ENTER` - `GLFW_KEY_KP_ENTER`.
* `glfw.KEY_KP_EQUAL` - `GLFW_KEY_KP_EQUAL`.
* `glfw.KEY_LEFT_SHIFT` - `GLFW_KEY_LEFT_SHIFT`.
* `glfw.KEY_LEFT_CONTROL` - `GLFW_KEY_LEFT_CONTROL`.
* `glfw.KEY_LEFT_ALT` - `GLFW_KEY_LEFT_ALT`.
* `glfw.KEY_LEFT_SUPER` - `GLFW_KEY_LEFT_SUPER`.
* `glfw.KEY_RIGHT_SHIFT` - `GLFW_KEY_RIGHT_SHIFT`.
* `glfw.KEY_RIGHT_CONTROL` - `GLFW_KEY_RIGHT_CONTROL`.
* `glfw.KEY_RIGHT_ALT` - `GLFW_KEY_RIGHT_ALT`.
* `glfw.KEY_RIGHT_SUPER` - `GLFW_KEY_RIGHT_SUPER`.
* `glfw.KEY_MENU` - `GLFW_KEY_MENU`.
* `glfw.KEY_LAST` - `GLFW_KEY_LAST`.
* `glfw.MOD_SHIFT` - `GLFW_MOD_SHIFT`.
* `glfw.MOD_CONTROL` - `GLFW_MOD_CONTROL`.
* `glfw.MOD_ALT` - `GLFW_MOD_ALT`.
* `glfw.MOD_SUPER` - `GLFW_MOD_SUPER`.
* `glfw.MOD_CAPS_LOCK` - `GLFW_MOD_CAPS_LOCK`.
* `glfw.MOD_NUM_LOCK` - `GLFW_MOD_NUM_LOCK`.
* `glfw.MOUSE_BUTTON_1` - `GLFW_MOUSE_BUTTON_1`.
* `glfw.MOUSE_BUTTON_2` - `GLFW_MOUSE_BUTTON_2`.
* `glfw.MOUSE_BUTTON_3` - `GLFW_MOUSE_BUTTON_3`.
* `glfw.MOUSE_BUTTON_4` - `GLFW_MOUSE_BUTTON_4`.
* `glfw.MOUSE_BUTTON_5` - `GLFW_MOUSE_BUTTON_5`.
* `glfw.MOUSE_BUTTON_6` - `GLFW_MOUSE_BUTTON_6`.
* `glfw.MOUSE_BUTTON_7` - `GLFW_MOUSE_BUTTON_7`.
* `glfw.MOUSE_BUTTON_8` - `GLFW_MOUSE_BUTTON_8`.
* `glfw.MOUSE_BUTTON_LAST` - `GLFW_MOUSE_BUTTON_LAST`.
* `glfw.MOUSE_BUTTON_LEFT` - `GLFW_MOUSE_BUTTON_LEFT`.
* `glfw.MOUSE_BUTTON_RIGHT` - `GLFW_MOUSE_BUTTON_RIGHT`.
* `glfw.MOUSE_BUTTON_MIDDLE` - `GLFW_MOUSE_BUTTON_MIDDLE`.
* `glfw.JOYSTICK_1` - `GLFW_JOYSTICK_1`.
* `glfw.JOYSTICK_2` - `GLFW_JOYSTICK_2`.
* `glfw.JOYSTICK_3` - `GLFW_JOYSTICK_3`.
* `glfw.JOYSTICK_4` - `GLFW_JOYSTICK_4`.
* `glfw.JOYSTICK_5` - `GLFW_JOYSTICK_5`.
* `glfw.JOYSTICK_6` - `GLFW_JOYSTICK_6`.
* `glfw.JOYSTICK_7` - `GLFW_JOYSTICK_7`.
* `glfw.JOYSTICK_8` - `GLFW_JOYSTICK_8`.
* `glfw.JOYSTICK_9` - `GLFW_JOYSTICK_9`.
* `glfw.JOYSTICK_10` - `GLFW_JOYSTICK_10`.
* `glfw.JOYSTICK_11` - `GLFW_JOYSTICK_11`.
* `glfw.JOYSTICK_12` - `GLFW_JOYSTICK_12`.
* `glfw.JOYSTICK_13` - `GLFW_JOYSTICK_13`.
* `glfw.JOYSTICK_14` - `GLFW_JOYSTICK_14`.
* `glfw.JOYSTICK_15` - `GLFW_JOYSTICK_15`.
* `glfw.JOYSTICK_16` - `GLFW_JOYSTICK_16`.
* `glfw.JOYSTICK_LAST` - `GLFW_JOYSTICK_LAST`.
* `glfw.GAMEPAD_BUTTON_A` - `GLFW_GAMEPAD_BUTTON_A`.
* `glfw.GAMEPAD_BUTTON_B` - `GLFW_GAMEPAD_BUTTON_B`.
* `glfw.GAMEPAD_BUTTON_X` - `GLFW_GAMEPAD_BUTTON_X`.
* `glfw.GAMEPAD_BUTTON_Y` - `GLFW_GAMEPAD_BUTTON_Y`.
* `glfw.GAMEPAD_BUTTON_LEFT_BUMPER` - `GLFW_GAMEPAD_BUTTON_LEFT_BUMPER`.
* `glfw.GAMEPAD_BUTTON_RIGHT_BUMPER` - `GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER`.
* `glfw.GAMEPAD_BUTTON_BACK` - `GLFW_GAMEPAD_BUTTON_BACK`.
* `glfw.GAMEPAD_BUTTON_START` - `GLFW_GAMEPAD_BUTTON_START`.
* `glfw.GAMEPAD_BUTTON_GUIDE` - `GLFW_GAMEPAD_BUTTON_GUIDE`.
* `glfw.GAMEPAD_BUTTON_LEFT_THUMB` - `GLFW_GAMEPAD_BUTTON_LEFT_THUMB`.
* `glfw.GAMEPAD_BUTTON_RIGHT_THUMB` - `GLFW_GAMEPAD_BUTTON_RIGHT_THUMB`.
* `glfw.GAMEPAD_BUTTON_DPAD_UP` - `GLFW_GAMEPAD_BUTTON_DPAD_UP`.
* `glfw.GAMEPAD_BUTTON_DPAD_RIGHT` - `GLFW_GAMEPAD_BUTTON_DPAD_RIGHT`.
* `glfw.GAMEPAD_BUTTON_DPAD_DOWN` - `GLFW_GAMEPAD_BUTTON_DPAD_DOWN`.
* `glfw.GAMEPAD_BUTTON_DPAD_LEFT` - `GLFW_GAMEPAD_BUTTON_DPAD_LEFT`.
* `glfw.GAMEPAD_BUTTON_LAST` - `GLFW_GAMEPAD_BUTTON_LAST`.
* `glfw.GAMEPAD_BUTTON_CROSS` - `GLFW_GAMEPAD_BUTTON_CROSS`.
* `glfw.GAMEPAD_BUTTON_CIRCLE` - `GLFW_GAMEPAD_BUTTON_CIRCLE`.
* `glfw.GAMEPAD_BUTTON_SQUARE` - `GLFW_GAMEPAD_BUTTON_SQUARE`.
* `glfw.GAMEPAD_BUTTON_TRIANGLE` - `GLFW_GAMEPAD_BUTTON_TRIANGLE`.
* `glfw.GAMEPAD_AXIS_LEFT_X` - `GLFW_GAMEPAD_AXIS_LEFT_X`.
* `glfw.GAMEPAD_AXIS_LEFT_Y` - `GLFW_GAMEPAD_AXIS_LEFT_Y`.
* `glfw.GAMEPAD_AXIS_RIGHT_X` - `GLFW_GAMEPAD_AXIS_RIGHT_X`.
* `glfw.GAMEPAD_AXIS_RIGHT_Y` - `GLFW_GAMEPAD_AXIS_RIGHT_Y`.
* `glfw.GAMEPAD_AXIS_LEFT_TRIGGER` - `GLFW_GAMEPAD_AXIS_LEFT_TRIGGER`.
* `glfw.GAMEPAD_AXIS_RIGHT_TRIGGER` - `GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER`.
* `glfw.GAMEPAD_AXIS_LAST` - `GLFW_GAMEPAD_AXIS_LAST`.
* `glfw.NO_ERROR` - `GLFW_NO_ERROR`.
* `glfw.NOT_INITIALIZED` - `GLFW_NOT_INITIALIZED`.
* `glfw.NO_CURRENT_CONTEXT` - `GLFW_NO_CURRENT_CONTEXT`.
* `glfw.INVALID_ENUM` - `GLFW_INVALID_ENUM`.
* `glfw.INVALID_VALUE` - `GLFW_INVALID_VALUE`.
* `glfw.OUT_OF_MEMORY` - `GLFW_OUT_OF_MEMORY`.
* `glfw.API_UNAVAILABLE` - `GLFW_API_UNAVAILABLE`.
* `glfw.VERSION_UNAVAILABLE` - `GLFW_VERSION_UNAVAILABLE`.
* `glfw.PLATFORM_ERROR` - `GLFW_PLATFORM_ERROR`.
* `glfw.FORMAT_UNAVAILABLE` - `GLFW_FORMAT_UNAVAILABLE`.
* `glfw.NO_WINDOW_CONTEXT` - `GLFW_NO_WINDOW_CONTEXT`.
* `glfw.FOCUSED` - `GLFW_FOCUSED`.
* `glfw.ICONIFIED` - `GLFW_ICONIFIED`.
* `glfw.RESIZABLE` - `GLFW_RESIZABLE`.
* `glfw.VISIBLE` - `GLFW_VISIBLE`.
* `glfw.DECORATED` - `GLFW_DECORATED`.
* `glfw.AUTO_ICONIFY` - `GLFW_AUTO_ICONIFY`.
* `glfw.FLOATING` - `GLFW_FLOATING`.
* `glfw.MAXIMIZED` - `GLFW_MAXIMIZED`.
* `glfw.CENTER_CURSOR` - `GLFW_CENTER_CURSOR`.
* `glfw.TRANSPARENT_FRAMEBUFFER` - `GLFW_TRANSPARENT_FRAMEBUFFER`.
* `glfw.HOVERED` - `GLFW_HOVERED`.
* `glfw.FOCUS_ON_SHOW` - `GLFW_FOCUS_ON_SHOW`.
* `glfw.RED_BITS` - `GLFW_RED_BITS`.
* `glfw.GREEN_BITS` - `GLFW_GREEN_BITS`.
* `glfw.BLUE_BITS` - `GLFW_BLUE_BITS`.
* `glfw.ALPHA_BITS` - `GLFW_ALPHA_BITS`.
* `glfw.DEPTH_BITS` - `GLFW_DEPTH_BITS`.
* `glfw.STENCIL_BITS` - `GLFW_STENCIL_BITS`.
* `glfw.ACCUM_RED_BITS` - `GLFW_ACCUM_RED_BITS`.
* `glfw.ACCUM_GREEN_BITS` - `GLFW_ACCUM_GREEN_BITS`.
* `glfw.ACCUM_BLUE_BITS` - `GLFW_ACCUM_BLUE_BITS`.
* `glfw.ACCUM_ALPHA_BITS` - `GLFW_ACCUM_ALPHA_BITS`.
* `glfw.AUX_BUFFERS` - `GLFW_AUX_BUFFERS`.
* `glfw.STEREO` - `GLFW_STEREO`.
* `glfw.SAMPLES` - `GLFW_SAMPLES`.
* `glfw.SRGB_CAPABLE` - `GLFW_SRGB_CAPABLE`.
* `glfw.REFRESH_RATE` - `GLFW_REFRESH_RATE`.
* `glfw.DOUBLEBUFFER` - `GLFW_DOUBLEBUFFER`.
* `glfw.CLIENT_API` - `GLFW_CLIENT_API`.
* `glfw.CONTEXT_VERSION_MAJOR` - `GLFW_CONTEXT_VERSION_MAJOR`.
* `glfw.CONTEXT_VERSION_MINOR` - `GLFW_CONTEXT_VERSION_MINOR`.
* `glfw.CONTEXT_REVISION` - `GLFW_CONTEXT_REVISION`.
* `glfw.CONTEXT_ROBUSTNESS` - `GLFW_CONTEXT_ROBUSTNESS`.
* `glfw.OPENGL_FORWARD_COMPAT` - `GLFW_OPENGL_FORWARD_COMPAT`.
* `glfw.OPENGL_DEBUG_CONTEXT` - `GLFW_OPENGL_DEBUG_CONTEXT`.
* `glfw.OPENGL_PROFILE` - `GLFW_OPENGL_PROFILE`.
* `glfw.CONTEXT_RELEASE_BEHAVIOR` - `GLFW_CONTEXT_RELEASE_BEHAVIOR`.
* `glfw.CONTEXT_NO_ERROR` - `GLFW_CONTEXT_NO_ERROR`.
* `glfw.CONTEXT_CREATION_API` - `GLFW_CONTEXT_CREATION_API`.
* `glfw.SCALE_TO_MONITOR` - `GLFW_SCALE_TO_MONITOR`.
* `glfw.COCOA_RETINA_FRAMEBUFFER` - `GLFW_COCOA_RETINA_FRAMEBUFFER`.
* `glfw.COCOA_FRAME_NAME` - `GLFW_COCOA_FRAME_NAME`.
* `glfw.COCOA_GRAPHICS_SWITCHING` - `GLFW_COCOA_GRAPHICS_SWITCHING`.
* `glfw.X11_CLASS_NAME` - `GLFW_X11_CLASS_NAME`.
* `glfw.X11_INSTANCE_NAME` - `GLFW_X11_INSTANCE_NAME`.
* `glfw.NO_API` - `GLFW_NO_API`.
* `glfw.OPENGL_API` - `GLFW_OPENGL_API`.
* `glfw.OPENGL_ES_API` - `GLFW_OPENGL_ES_API`.
* `glfw.NO_ROBUSTNESS` - `GLFW_NO_ROBUSTNESS`.
* `glfw.NO_RESET_NOTIFICATION` - `GLFW_NO_RESET_NOTIFICATION`.
* `glfw.LOSE_CONTEXT_ON_RESET` - `GLFW_LOSE_CONTEXT_ON_RESET`.
* `glfw.OPENGL_ANY_PROFILE` - `GLFW_OPENGL_ANY_PROFILE`.
* `glfw.OPENGL_CORE_PROFILE` - `GLFW_OPENGL_CORE_PROFILE`.
* `glfw.OPENGL_COMPAT_PROFILE` - `GLFW_OPENGL_COMPAT_PROFILE`.
* `glfw.CURSOR` - `GLFW_CURSOR`.
* `glfw.STICKY_KEYS` - `GLFW_STICKY_KEYS`.
* `glfw.STICKY_MOUSE_BUTTONS` - `GLFW_STICKY_MOUSE_BUTTONS`.
* `glfw.LOCK_KEY_MODS` - `GLFW_LOCK_KEY_MODS`.
* `glfw.RAW_MOUSE_MOTION` - `GLFW_RAW_MOUSE_MOTION`.
* `glfw.CURSOR_NORMAL` - `GLFW_CURSOR_NORMAL`.
* `glfw.CURSOR_HIDDEN` - `GLFW_CURSOR_HIDDEN`.
* `glfw.CURSOR_DISABLED` - `GLFW_CURSOR_DISABLED`.
* `glfw.ANY_RELEASE_BEHAVIOR` - `GLFW_ANY_RELEASE_BEHAVIOR`.
* `glfw.RELEASE_BEHAVIOR_FLUSH` - `GLFW_RELEASE_BEHAVIOR_FLUSH`.
* `glfw.RELEASE_BEHAVIOR_NONE` - `GLFW_RELEASE_BEHAVIOR_NONE`.
* `glfw.NATIVE_CONTEXT_API` - `GLFW_NATIVE_CONTEXT_API`.
* `glfw.EGL_CONTEXT_API` - `GLFW_EGL_CONTEXT_API`.
* `glfw.OSMESA_CONTEXT_API` - `GLFW_OSMESA_CONTEXT_API`.
* `glfw.ARROW_CURSOR` - `GLFW_ARROW_CURSOR`.
* `glfw.IBEAM_CURSOR` - `GLFW_IBEAM_CURSOR`.
* `glfw.CROSSHAIR_CURSOR` - `GLFW_CROSSHAIR_CURSOR`.
* `glfw.HAND_CURSOR` - `GLFW_HAND_CURSOR`.
* `glfw.HRESIZE_CURSOR` - `GLFW_HRESIZE_CURSOR`.
* `glfw.VRESIZE_CURSOR` - `GLFW_VRESIZE_CURSOR`.
* `glfw.CONNECTED` - `GLFW_CONNECTED`.
* `glfw.DISCONNECTED` - `GLFW_DISCONNECTED`.
* `glfw.JOYSTICK_HAT_BUTTONS` - `GLFW_JOYSTICK_HAT_BUTTONS`.
* `glfw.COCOA_CHDIR_RESOURCES` - `GLFW_COCOA_CHDIR_RESOURCES`.
* `glfw.COCOA_MENUBAR` - `GLFW_COCOA_MENUBAR`.
* `glfw.DONT_CARE` - `GLFW_DONT_CARE`.
