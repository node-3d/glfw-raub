# GLFW for Node.js

This is a part of [Node3D](https://github.com/node-3d) project.

[![NPM](https://badge.fury.io/js/glfw-raub.svg)](https://badge.fury.io/js/glfw-raub)
[![ESLint](https://github.com/node-3d/glfw-raub/actions/workflows/eslint.yml/badge.svg)](https://github.com/node-3d/glfw-raub/actions/workflows/eslint.yml)
[![Test](https://github.com/node-3d/glfw-raub/actions/workflows/test.yml/badge.svg)](https://github.com/node-3d/glfw-raub/actions/workflows/test.yml)
[![Cpplint](https://github.com/node-3d/glfw-raub/actions/workflows/cpplint.yml/badge.svg)](https://github.com/node-3d/glfw-raub/actions/workflows/cpplint.yml)

```console
npm i -s glfw-raub
```

**Node.js** addon with **GLFW3** bindings.

![Example](examples/screenshot.jpg)

* **GLFW** version **3.4.0** backend.
* Exposes low-level **GLFW** interface.
* Multiple windows for a single **Node.js** process.
* Able to switch to fullscreen and back.
* Has `Window` class, simplifying low-level interactions.
* Has `Document` class, capable of tricking other libs, as if we are in a browser.

```js
import glfw from 'glfw-raub';
const { Window } = glfw;

const wnd = new Window({ title: 'GLFW Test', vsync: true });

wnd.loop(() => {
	if (wnd.shouldClose || wnd.getKey(glfw.KEY_ESCAPE)) {
		process.exit(0);
		return;
	}
	
	glfw.testScene(wnd.width, wnd.height);
});
```

> Note: this **addon uses N-API**, and therefore is ABI-compatible across different
Node.js versions. Addon binaries are precompiled and **there is no compilation**
step during the `npm i` command.


## GLFW

This is a low-level interface, where most of the stuff is directly reflecting
GLFW API. GLFW **does NOT EXPOSE** OpenGL commands, it only [controls the window-related
setup and resources](http://www.glfw.org/docs/latest/group__window.html).
Aside from several additional features, this addon directly exposes the GLFW API to JS. E.g.:

```cpp
DBG_EXPORT JS_METHOD(pollEvents) {
	glfwPollEvents();
	RET_GLFW_VOID;
}
```

Nothing is added between you and GLFW, unless necessary or explicitly mentioned.

* All `glfw*` functions are accessible as
    `glfw.*`. E.g. `glfwPollEvents` -> `glfw.pollEvents`.
* All `GLFW_*` constants are accessible as
    `glfw.*`. E.g. `GLFW_TRUE` -> `glfw.TRUE`.


See [this example](/examples/vulkan.mjs) for raw GLFW calls.

See [TS declarations](/index.d.ts) for more details.

----------


### class Window

```js
const { Window } = glfw;
const wnd = new Window({ title: 'GLFW Test', vsync: true });
```

This class helps managing window objects and their events. It can also switch between
fullscreen, borderless and windowed modes.

The first window creates an additional invisible root-window for context sharing.
(so that you can also close any window and still keep the root context).
The platform context (pointers/handles) for sharing may be obtained when necessary.

See [TS declarations](/index.d.ts) for more details.

----------

### class Document

```js
const { Document } = glfw;
const doc = new Document({ title: 'GLFW Test', vsync: true });
```

Document inherits from `Window` and has the same features in general.
It exposes additional APIs to mimic the content of web `document`.
There are some tricks to provide WebGL libraries with necessary environment.
Document is specifically designed for compatibility with [three.js](https://threejs.org/).
Other web libraries may work too, but may require additional tweaking.

See [TS declarations](/index.d.ts) for more details.

----------

### Extras

* `glfw.hideConsole(): void` - tries to hide the console window on Windows.
* `glfw.showConsole(): void` - shows the console window if it has been hidden.
* `glfw.drawWindow(w: number, cb: (dateNow: number) => void): void` - this is a shortcut
    to call `pollEvents`, then `cb`, and then `swapBuffers`, where you only supply `cb`
    and C++ side does the rest.
* `glfw.platformDevice(w: number): number` - returns the window HDC on Windows,
    or whatever is similar on other systems.
* `glfw.platformWindow(w: number): number` - returns the window HWND on Windows,
    or whatever is similar on other systems.
* `glfw.platformContext(w: number): number` - returns the window WGL Context on Windows,
    or whatever is similar on other systems.