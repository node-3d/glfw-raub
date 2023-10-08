# GLFW for Node.js

This is a part of [Node3D](https://github.com/node-3d) project.

[![NPM](https://badge.fury.io/js/glfw-raub.svg)](https://badge.fury.io/js/glfw-raub)
[![CodeFactor](https://www.codefactor.io/repository/github/node-3d/glfw-raub/badge)](https://www.codefactor.io/repository/github/node-3d/glfw-raub)

```
npm i glfw-raub
```

**Node.js** addon with **GLFW3** bindings.

> Note: this **addon uses N-API**, and therefore is ABI-compatible across different
Node.js versions. Addon binaries are precompiled and **there is no compilation**
step during the `npm i` command.

![Example](examples/screenshot.jpg)

* **GLFW** version **3.3.4** backend.
* Exposes low-level **GLFW** interface.
* Multiple windows for a single **Node.js** process.
* Able to switch to fullscreen and back.
* Has `Window` class, simplifying low-level interactions.
* Has `Document` class, capable of tricking other libs, as if we are in a browser.


## GLFW

This is a low-level interface, where most of the stuff is directly reflecting
GLFW API. It **does NOT EXPOSE** OpenGL commands.
See [GLFW Docs](http://www.glfw.org/docs/latest/group__window.html)
for useful info on what it does and doesn't.

```js
const glfw = require('glfw-raub');
```

Here `glfw` is an API container, where all `glfw*` functions are accessible as
`glfw.*`. E.g. `glfwSetWindowTitle` -> `glfw.setWindowTitle`.

See [TypeSctipt definitions](/index.d.ts) for more details.

----------


### class Window

```js
const { Window } = glfw;
```

This class helps managing window objects and their events. It can also switch between
fullscreen, borderless and windowed modes.

See [TypeSctipt definitions](/index.d.ts) for more details.

----------

### class Document

```js
const { Document } = glfw;
```

It can be used to facilitate the environment for other
JS libraries, such as [three.js](https://threejs.org/).

See [TypeSctipt definitions](/index.d.ts) for more details.
