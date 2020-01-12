'use strict';

const EventEmitter = require('events');
const path = require('path');

const glfw = require('../core');


const emptyFunction = () => {};


class Window extends EventEmitter {
	
	constructor(opts = {}) {
		
		super();
		
		this._title = null;
		this._icon = null;
		this._modeCache = {};
		this._pendingKeydown = null;
		
		this._width = opts.width || 800;
		this._pxWidth = this._width;
		this._height = opts.height || 600;
		this._pxHeight = this._height;
		this._ratio = 1;
		
		this._display = opts.display;
		this._monitors = glfw.getMonitors();
		this._primaryDisplay = this._monitors.filter(d => d.is_primary)[0];
		
		this._vsync = opts.vsync ? 1 : 0; // 0 for vsync off
		this._autoIconify = opts.autoIconify === false ? false : true;
		
		const mode = opts.fullscreen ? 'fullscreen' : (opts.mode ? opts.mode : 'windowed');
		
		this._decorated = true;
		if (opts.decorated !== undefined) {
			this._decorated = opts.decorated;
		}
		
		this._msaa = opts.msaa === undefined ? 2 : opts.msaa;
		
		glfw.windowHint(glfw.RESIZABLE, glfw.TRUE);
		glfw.windowHint(glfw.VISIBLE, glfw.TRUE);
		glfw.windowHint(glfw.RED_BITS, 8);
		glfw.windowHint(glfw.GREEN_BITS, 8);
		glfw.windowHint(glfw.BLUE_BITS, 8);
		glfw.windowHint(glfw.DEPTH_BITS, 24);
		glfw.windowHint(glfw.REFRESH_RATE, 0);
		glfw.windowHint(glfw.DOUBLEBUFFER, glfw.TRUE);
		glfw.windowHint(glfw.AUTO_ICONIFY, this._autoIconify ? glfw.TRUE : glfw.FALSE);
		glfw.windowHint(glfw.DECORATED, this._decorated ? glfw.TRUE : glfw.FALSE);
		glfw.windowHint(glfw.SAMPLES, this._msaa);
		
		this._emitter = { emit: (t, e) => this.emit(t, e) };
		
		// This CREATES window, as mode switches from `undefined`
		this.mode = mode;
		
		const sizeWin = this.size;
		const sizeFB  = this.framebufferSize;
		
		this._ratio = sizeFB.width / sizeWin.width;
		
		this._pxWidth = sizeFB.width;
		this._pxHeight = sizeFB.height;
		
		this.icon = opts.icon;
		
		if ( ! opts.title ) {
			const dirpath = path.dirname(process.mainModule.filename);
			const dirname = path.basename(dirpath);
			this.title = dirname;
		} else {
			this.title = opts.title;
		}
		
		//can only be called after window creation!
		this._major = glfw.getWindowAttrib(this._window, glfw.CONTEXT_VERSION_MAJOR);
		this._minor = glfw.getWindowAttrib(this._window, glfw.CONTEXT_VERSION_MINOR);
		this._rev = glfw.getWindowAttrib(this._window, glfw.CONTEXT_REVISION);
		this._prof = glfw.getWindowAttrib(this._window, glfw.OPENGL_PROFILE);
		
		this.on('window_pos', ({ x, y }) => {
			this._x = x;
			this._y = y;
		});
		
		this.on('wresize', ({ width, height }) => {
			this._width = width;
			this._height = height;
		});
		
		this.on('resize', ({ width, height }) => {
			this._pxWidth = width;
			this._pxHeight = height;
		});
		
		this.swapBuffers();
		
		this.requestAnimationFrame = this._requestAnimationFrame.bind(this);
		this.cancelAnimationFrame = this._cancelAnimationFrame.bind(this);
		
		this.event = null;
		
	}
	
	
	get ratio() { return this._ratio; }
	get devicePixelRatio() { return this._ratio; }
	
	
	get handle() { return this._window; }
	
	
	getCurrentMonitor() {
		
		if ( ! this._window ) {
			return this._primaryDisplay;
		}
		
		let bestoverlap = 0;
		let bestmonitor = null;
		
		const { x: wx, y: wy } = this.pos;
		const { width: ww, height: wh } = this.size;
		
		this._monitors = glfw.getMonitors();
		
		this._monitors.forEach(monitor => {
			const { width: mw, height: mh } = monitor;
			const { pos_x: mx, pos_y: my } = monitor;
			let overlap = (
				Math.max(0, Math.min(wx + ww, mx + mw) - Math.max(wx, mx)) *
				Math.max(0, Math.min(wy + wh, my + mh) - Math.max(wy, my))
			);
			if (bestoverlap < overlap) {
				bestoverlap = overlap;
				bestmonitor = monitor;
			}
		});
		
		return bestmonitor;
		
	}
	
	
	get mode() { return this._mode; }
	set mode(v) {
		
		if (this._mode === v) {
			return;
		}
		
		const currentMonitor = this.getCurrentMonitor();
		
		const prevMode = this._mode;
		this._mode = v;
		
		if (this._window) {
			
			// Fullscreen can't be hidden (uh-oh)
			if (prevMode === 'fullscreen') {
				this.destroy();
				this._modeCache[prevMode] = null;
			} else {
				this.hide();
			}
			
		}
		
		if (this._monitors.length < 1) {
			throw new Error('No suitable display found for a new GLFW Window.');
		}
		
		this._display = this._monitors.indexOf(currentMonitor);
		
		if (this._mode === 'windowed') {
			
			this._x = this._prevX || this._x;
			this._y = this._prevY || this._y;
			this._width = this._prevWidth || this._width;
			this._height = this._prevHeight || this._height;
			this._decorated = this._prevDecorated || this._decorated;
			delete this._prevX;
			delete this._prevY;
			delete this._prevWidth;
			delete this._prevHeight;
			delete this._prevDecorated;
			
		} else {
			
			if (
				this._width !== currentMonitor.width ||
				this._height !== currentMonitor.height
			) {
				this._prevX = this._x;
				this._prevY = this._y;
				this._prevWidth = this._width;
				this._prevHeight = this._height;
				this._x = currentMonitor.pos_x;
				this._y = currentMonitor.pos_y;
				this._width = currentMonitor.width;
				this._height = currentMonitor.height;
			}
			
		}
		
		if ( ! this._modeCache[this._mode] ) {
			
			this._create();
			this._modeCache[this._mode] = this._window;
			
		} else {
			
			this._window = this._modeCache[this._mode];
			this.show();
			
		}
		
		if (this._mode === 'windowed') {
			
			if (this._x && this._y) {
				glfw.setWindowPos(this._window, this._x, this._y);
			}
			
		} else if (this._mode === 'borderless') {
			
			const monitor = this._monitors[this._display];
			glfw.setWindowPos(this._window, monitor.pos_x, monitor.pos_y);
			glfw.setWindowSize(this._window, monitor.width, monitor.height);
			
		}
		
		this.makeCurrent();
		
		// Vertical sync (on cards that support it)
		glfw.swapInterval(this._vsync);
		
		this._pxWidth = this._width * this._ratio;
		this._pxHeight = this._height * this._ratio;
		
		this.emit('resize', { width: this._pxWidth, height: this._pxHeight });
		
	}
	
	
	get width() { return this._pxWidth; }
	get height() { return this._pxHeight; }
	
	set width(v) {
		if (this._pxWidth === v) {
			return;
		}
		this._width = Math.floor(v / this._ratio);
		this._pxWidth = v;
		glfw.setWindowSize(this._window, this._width, this._height);
	}
	set height(v) {
		if (this._pxHeight === v) {
			return;
		}
		this._height = Math.floor(v / this._ratio);
		this._pxHeight = v;
		glfw.setWindowSize(this._window, this._width, this._height);
	}
	
	get offsetWidth() { return this._pxWidth; }
	set offsetWidth(v) { this.width = v; }
	get offsetHeight() { return this._pxHeight; }
	set offsetHeight(v) { this.height = v; }
	
	get w() { return this.width; }
	set w(v) { this.width = v; }
	get h() { return this.height; }
	set h(v) { this.height = v; }
	get wh() { return [this.width, this.height]; }
	set wh([width, height]) {
		if (this._pxWidth === width && this._pxHeight === height) {
			return;
		}
		this._width = Math.floor(width / this._ratio);
		this._pxWidth = width;
		this._height = Math.floor(height / this._ratio);
		this._pxHeight = height;
		glfw.setWindowSize(this._window, this._width, this._height);
	}
	get pxSize() {
		const size = glfw.getFramebufferSize(this._window);
		this._pxWidth = size.width;
		this._pxHeight = size.height;
		this._width = this._pxWidth / this._ratio;
		this._height = this._pxHeight / this._ratio;
		return size;
	}
	
	set pxSize({ width, height }) {
		this.wh = [width, height];
	}
	
	get innerWidth() { return this.width; }
	set innerWidth(v) { this.width = v; }
	get innerHeight() { return this.height; }
	set innerHeight(v) { this.height = v; }
	
	get clientWidth() { return this.width; }
	set clientWidth(v) { this.width = v; }
	get clientHeight() { return this.height; }
	set clientHeight(v) { this.height = v; }
	
	get onkeydown() { return this.listeners('keydown'); }
	set onkeydown(cb) { this.on('keydown', cb); }
	
	get onkeyup() { return this.listeners('keyup'); }
	set onkeyup(cb) { this.on('keyup', cb); }
	
	get onmousedown() { return this.listeners('mousedown'); }
	set onmousedown(cb) { this.on('mousedown', cb); }
	
	get onmouseup() { return this.listeners('mouseup'); }
	set onmouseup(cb) { this.on('mouseup', cb); }
	
	get onwheel() { return this.listeners('wheel'); }
	set onwheel(cb) { this.on('wheel', cb); }
	
	get onmousewheel() { return this.listeners('mousewheel'); }
	set onmousewheel(cb) { this.on('mousewheel', cb); }
	
	get onresize() { return this.listeners('resize'); }
	set onresize(cb) { this.on('resize', cb); }
	
	getBoundingClientRect() {
		return {
			x: 0,
			y: 0,
			width: this._pxWidth,
			height: this._pxHeight,
			left: 0,
			top: 0,
			right: this._pxWidth,
			bottom: this._pxHeight,
		};
	}
	
	get size() {
		const size = glfw.getWindowSize(this._window);
		this._width = size.width;
		this._height = size.height;
		this._pxWidth = size.width * this._ratio;
		this._pxHeight = size.height * this._ratio;
		return size;
	}
	
	set size({ width, height }) {
		if (this._width === width && this._height === height) {
			return;
		}
		this._width = width;
		this._height = height;
		this._pxWidth = width * this._ratio;
		this._pxHeight = height * this._ratio;
		glfw.setWindowSize(this._window, width, height);
	}
	
	
	get scrollX() { return 0; }
	get scrollY() { return 0; }
	
	get title() { return this._title; }
	set title(v) {
		this._title = v || 'Untitled';
		glfw.setWindowTitle(this._window, this._title);
	}
	
	
	get icon() { return this._icon; }
	set icon(v) {
		if ( ! (v && typeof v === 'object') ) {
			this._icon = null;
			return;
		}
		this._icon = v;
		glfw.setWindowIcon(this._window, this._icon);
	}
	
	
	get msaa() { return this._msaa; }
	
	get version() {
		return `GL ${this._major}.${this._minor}.${this._rev} Profile: ${this._prof}`;
	}
	
	get shouldClose() { return glfw.windowShouldClose(this._window); }
	set shouldClose(v) { glfw.setWindowShouldClose(v); }
	
	get platformWindow() { return glfw.platformWindow(this._window); }
	get platformContext() { return glfw.platformContext(this._window); }
	
	
	get x() { return this._x; }
	set x(v) {
		if (this._x === v) {
			return;
		}
		this._x = v;
		glfw.setWindowPos(this._window, this._x, this._y);
	}
	
	get y() { return this._y; }
	set y(v) {
		if (this._y === v) {
			return;
		}
		this._y = v;
		glfw.setWindowPos(this._window, this._x, this._y);
	}
	
	get pos() {
		const pos = glfw.getWindowPos(this._window);
		this._x = pos.x;
		this._y = pos.y;
		return pos;
	}
	set pos({ x, y }) {
		if (this._x === x && this._y === y) {
			return;
		}
		this._x = x;
		this._y = y;
		glfw.setWindowPos(this._window, x, y);
	}
	
	
	get framebufferSize() { return glfw.getFramebufferSize(this._window); }
	
	get currentContext() { return glfw.getCurrentContext(this._window); }
	
	get cursorPos() { return glfw.getCursorPos(this._window); }
	set cursorPos({ x, y }) { glfw.setCursorPos(this._window, x, y); }
	
	
	getKey(key) { return glfw.getKey(this._window, key); }
	
	getMouseButton(button) { return glfw.getMouseButton(this._window, button); }
	
	getWindowAttrib(attrib) { return glfw.getWindowAttrib(this._window, attrib); }
	
	setInputMode(mode, value) { glfw.setInputMode(this._window, mode, value); }
	
	swapBuffers() { glfw.swapBuffers(this._window); }
	
	makeCurrent() { glfw.makeContextCurrent(this._window); }
	
	destroy() { glfw.destroyWindow(this._window); }
	
	iconify() { glfw.iconifyWindow(this._window); }
	
	restore() { glfw.restoreWindow(this._window); }
	
	hide() { glfw.hideWindow(this._window); }
	
	show() { glfw.showWindow(this._window); }
	
	
	emit(type, event) {
		
		if (type === 'keydown' || type === 'keyup') {
			const glfwCode = event.which;
			event.which = Window.extraCodes[glfwCode] || glfwCode;
			event.keyCode = event.which;
			event.key = (
				event.charCode && String.fromCharCode(event.charCode) ||
				event.code ||
				Window.keyNames[glfwCode] ||
				'?'
			);
			event.code = (
				Window.codeNames[glfwCode] ||
				(event.code && `Key${event.code.toUpperCase()}`) ||
				'UNKNOWN'
			);
		}
		
		event.target = this;
		event.preventDefault = emptyFunction;
		event.stopPropagation = emptyFunction;
		
		this.makeCurrent();
		
		this.event = event;
		super.emit(type, event);
		this.event = null;
		
	}
	
	
	// Create a new window according to the current 'mode'
	_create() {
		
		if (this._mode === 'windowed') {
			
			glfw.windowHint(glfw.DECORATED, this._decorated ? glfw.TRUE : glfw.FALSE);
			this._window = glfw.createWindow(
				this._width,
				this._height,
				this._emitter,
				this._title
			);
			
		} else if (this._mode === 'borderless') {
			
			this._prevDecorated = this._decorated;
			this._decorated = false;
			
			glfw.windowHint(glfw.DECORATED, glfw.FALSE);
			
			this._window = glfw.createWindow(
				this._width,
				this._height,
				this._emitter,
				this._title
			);
			
		} else if (this._mode === 'fullscreen') {
			
			this._adjustFullscreen();
			
			this._window = glfw.createWindow(
				this._width,
				this._height,
				this._emitter,
				this._title,
				this._display
			);
			
		} else {
			
			throw new Error(`Not supported display mode: '${this._mode}'.`);
			
		}
		
		if ( ! this._window ) {
			throw new Error('Failed to open a new GLFW Window');
		}
		
	}
	
	_requestAnimationFrame(cb) {
		this.swapBuffers();
		glfw.pollEvents();
		return setImmediate(() => cb(Date.now()));
	}
	
	_cancelAnimationFrame(id) { clearImmediate(id); }
	
	dispatchEvent(event) { this.emit(event.type, event); }
	
	addEventListener(name, callback) { this.on(name, callback); }
	
	removeEventListener(name, callback) { this.removeListener(name, callback); }
	
	static exit() {
		process.exit(0);
	}
	
	// ----- Fullscreen mode helpers
	
	_areaDiff(mode) {
		return Math.abs(mode.width * mode.height - this._width * this._height);
	}
	
	
	_sizeEqual(mode) {
		return mode.width === this._width && mode.height === this._height;
	}
	
	
	_sortByAreaDiff(modes) {
		const sorted = modes.sort((a, b) => this._areaDiff(a) - this._areaDiff(b));
		const best = this._areaDiff(sorted[0]);
		return sorted.filter(mode => this._areaDiff(mode) === best);
	}
	
	
	_sortByRate(modes) {
		return modes.sort((a, b) => this._areaDiff(a) - this._areaDiff(b));
	}
	
	
	_adjustFullscreen() {
		
		const mode = (() => {
			const modes = this._monitors[this._display].modes;
			const exact = modes.filter(mode => this._sizeEqual(mode));
			const chosen = (exact.length ? exact : this._sortByAreaDiff(modes));
			return chosen.sort((a, b) => b.rate - a.rate)[0];
		})();
		
		this._width = mode.width;
		this._height = mode.height;
		
		glfw.windowHint(glfw.REFRESH_RATE, mode.rate);
		
	}
	
}


Window.keyNames = {
	[glfw.KEY_BACKSPACE]     : 'Backspace',
	[glfw.KEY_CAPS_LOCK]     : 'CapsLock',
	[glfw.KEY_DELETE]        : 'Delete',
	[glfw.KEY_END]           : 'End',
	[glfw.KEY_ENTER]         : 'Enter',
	[glfw.KEY_ESCAPE]        : 'Escape',
	[glfw.KEY_F1]            : 'F1',
	[glfw.KEY_F2]            : 'F2',
	[glfw.KEY_F3]            : 'F3',
	[glfw.KEY_F4]            : 'F4',
	[glfw.KEY_F5]            : 'F5',
	[glfw.KEY_F6]            : 'F6',
	[glfw.KEY_F7]            : 'F7',
	[glfw.KEY_F8]            : 'F8',
	[glfw.KEY_F9]            : 'F9',
	[glfw.KEY_F10]           : 'F10',
	[glfw.KEY_F11]           : 'F11',
	[glfw.KEY_F12]           : 'F12',
	[glfw.KEY_F13]           : 'F13',
	[glfw.KEY_F14]           : 'F14',
	[glfw.KEY_F15]           : 'F15',
	[glfw.KEY_F16]           : 'F16',
	[glfw.KEY_F17]           : 'F17',
	[glfw.KEY_F18]           : 'F18',
	[glfw.KEY_F19]           : 'F19',
	[glfw.KEY_F20]           : 'F20',
	[glfw.KEY_F21]           : 'F21',
	[glfw.KEY_F22]           : 'F22',
	[glfw.KEY_F23]           : 'F23',
	[glfw.KEY_F24]           : 'F24',
	[glfw.KEY_F25]           : 'F25',
	[glfw.KEY_HOME]          : 'Home',
	[glfw.KEY_INSERT]        : 'Insert',
	[glfw.KEY_KP_ENTER]      : 'Enter',
	[glfw.KEY_KP_DECIMAL]    : 'Delete',
	[glfw.KEY_KP_EQUAL]      : 'Equal',
	[glfw.KEY_LEFT]          : 'ArrowLeft',
	[glfw.KEY_UP]            : 'ArrowUp',
	[glfw.KEY_RIGHT]         : 'ArrowRight',
	[glfw.KEY_DOWN]          : 'ArrowDown',
	[glfw.KEY_LEFT_ALT]      : 'Alt',
	[glfw.KEY_RIGHT_ALT]     : 'Alt',
	[glfw.KEY_LEFT_BRACKET]  : 'LeftBracket',
	[glfw.KEY_RIGHT_BRACKET] : 'RightBracket',
	[glfw.KEY_LEFT_CONTROL]  : 'Control',
	[glfw.KEY_RIGHT_CONTROL] : 'Control',
	[glfw.KEY_LEFT_SHIFT]    : 'Shift',
	[glfw.KEY_RIGHT_SHIFT]   : 'Shift',
	[glfw.KEY_LEFT_SUPER]    : 'Meta',
	[glfw.KEY_RIGHT_SUPER]   : 'Meta',
	[glfw.KEY_MENU]          : 'ContextMenu',
	[glfw.KEY_NUM_LOCK]      : 'NumLock',
	[glfw.KEY_PAGE_UP]       : 'PageUp',
	[glfw.KEY_PAGE_DOWN]     : 'PageDown',
	[glfw.KEY_PAUSE]         : 'Pause',
	[glfw.KEY_PRINT_SCREEN]  : 'PrintScreen',
	[glfw.KEY_SCROLL_LOCK]   : 'ScrollLock',
	[glfw.KEY_SPACE]         : ' ',
	[glfw.KEY_TAB]           : 'Tab',
};

Window.codeNames = {
	...Window.keyNames,
	[glfw.KEY_EQUAL]         : 'Equal',
	[glfw.KEY_COMMA]         : 'Comma',
	[glfw.KEY_MINUS]         : 'Minus',
	[glfw.KEY_PERIOD]        : 'Period',
	[glfw.KEY_SEMICOLON]     : 'Semicolon',
	[glfw.KEY_SLASH]         : 'Slash',
	[glfw.KEY_APOSTROPHE]    : 'Apostrophe',
	[glfw.KEY_BACKSLASH]     : 'Backslash',
	[glfw.KEY_SPACE]         : 'Space',
	[glfw.KEY_GRAVE_ACCENT]  : 'Backquote',
	[glfw.KEY_LEFT_ALT]      : 'AltLeft',
	[glfw.KEY_RIGHT_ALT]     : 'AltRight',
	[glfw.KEY_LEFT_CONTROL]  : 'ControlLeft',
	[glfw.KEY_RIGHT_CONTROL] : 'ControlRight',
	[glfw.KEY_LEFT_SHIFT]    : 'ShiftLeft',
	[glfw.KEY_RIGHT_SHIFT]   : 'ShiftRight',
	[glfw.KEY_LEFT_SUPER]    : 'MetaLeft',
	[glfw.KEY_RIGHT_SUPER]   : 'MetaRight',
	[glfw.KEY_LEFT]          : 'ArrowLeft',
	[glfw.KEY_UP]            : 'ArrowUp',
	[glfw.KEY_RIGHT]         : 'ArrowRight',
	[glfw.KEY_DOWN]          : 'ArrowDown',
	[glfw.KEY_0]             : 'Digit0',
	[glfw.KEY_1]             : 'Digit1',
	[glfw.KEY_2]             : 'Digit2',
	[glfw.KEY_3]             : 'Digit3',
	[glfw.KEY_4]             : 'Digit4',
	[glfw.KEY_5]             : 'Digit5',
	[glfw.KEY_6]             : 'Digit6',
	[glfw.KEY_7]             : 'Digit7',
	[glfw.KEY_8]             : 'Digit8',
	[glfw.KEY_9]             : 'Digit9',
	[glfw.KEY_KP_0]          : 'Numpad0',
	[glfw.KEY_KP_1]          : 'Numpad1',
	[glfw.KEY_KP_2]          : 'Numpad2',
	[glfw.KEY_KP_3]          : 'Numpad3',
	[glfw.KEY_KP_4]          : 'Numpad4',
	[glfw.KEY_KP_5]          : 'Numpad5',
	[glfw.KEY_KP_6]          : 'Numpad6',
	[glfw.KEY_KP_7]          : 'Numpad7',
	[glfw.KEY_KP_8]          : 'Numpad8',
	[glfw.KEY_KP_9]          : 'Numpad9',
	[glfw.KEY_KP_ADD]        : 'NumpadAdd',
	[glfw.KEY_KP_DECIMAL]    : 'NumpadDecimal',
	[glfw.KEY_KP_DIVIDE]     : 'NumpadDivide',
	[glfw.KEY_KP_ENTER]      : 'NumpadEnter',
	[glfw.KEY_KP_EQUAL]      : 'Equal',
	[glfw.KEY_KP_MULTIPLY]   : 'NumpadMultiply',
	[glfw.KEY_KP_SUBTRACT]   : 'NumpadSubtract',
};


Window.extraCodes = {
	[glfw.KEY_APOSTROPHE]    : 222,
	[glfw.KEY_BACKSLASH]     : 220,
	[glfw.KEY_BACKSPACE]     : 8,
	[glfw.KEY_CAPS_LOCK]     : 20,
	[glfw.KEY_COMMA]         : 188,
	[glfw.KEY_DELETE]        : 46,
	[glfw.KEY_DOWN]          : 40,
	[glfw.KEY_END]           : 35,
	[glfw.KEY_ENTER]         : 13,
	[glfw.KEY_EQUAL]         : 187,
	[glfw.KEY_ESCAPE]        : 27,
	[glfw.KEY_F10]           : 121,
	[glfw.KEY_F11]           : 122,
	[glfw.KEY_F12]           : 123,
	[glfw.KEY_F13]           : 123,
	[glfw.KEY_F14]           : 123,
	[glfw.KEY_F15]           : 123,
	[glfw.KEY_F16]           : 123,
	[glfw.KEY_F17]           : 123,
	[glfw.KEY_F18]           : 123,
	[glfw.KEY_F19]           : 123,
	[glfw.KEY_F1]            : 112,
	[glfw.KEY_F20]           : 123,
	[glfw.KEY_F21]           : 123,
	[glfw.KEY_F22]           : 123,
	[glfw.KEY_F23]           : 123,
	[glfw.KEY_F24]           : 123,
	[glfw.KEY_F25]           : 123,
	[glfw.KEY_F2]            : 113,
	[glfw.KEY_F3]            : 114,
	[glfw.KEY_F4]            : 115,
	[glfw.KEY_F5]            : 116,
	[glfw.KEY_F6]            : 117,
	[glfw.KEY_F7]            : 118,
	[glfw.KEY_F8]            : 119,
	[glfw.KEY_F9]            : 120,
	[glfw.KEY_GRAVE_ACCENT]  : 192,
	[glfw.KEY_HOME]          : 36,
	[glfw.KEY_INSERT]        : 45,
	[glfw.KEY_KP_0]          : 96,
	[glfw.KEY_KP_1]          : 97,
	[glfw.KEY_KP_2]          : 98,
	[glfw.KEY_KP_3]          : 99,
	[glfw.KEY_KP_4]          : 100,
	[glfw.KEY_KP_5]          : 101,
	[glfw.KEY_KP_6]          : 102,
	[glfw.KEY_KP_7]          : 103,
	[glfw.KEY_KP_8]          : 104,
	[glfw.KEY_KP_9]          : 105,
	[glfw.KEY_KP_ADD]        : 107,
	[glfw.KEY_KP_DECIMAL]    : 110,
	[glfw.KEY_KP_DIVIDE]     : 111,
	[glfw.KEY_KP_ENTER]      : 13,
	[glfw.KEY_KP_EQUAL]      : 187,
	[glfw.KEY_KP_MULTIPLY]   : 106,
	[glfw.KEY_KP_SUBTRACT]   : 109,
	[glfw.KEY_LEFT]          : 37,
	[glfw.KEY_LEFT_ALT]      : 18,
	[glfw.KEY_LEFT_BRACKET]  : 219,
	[glfw.KEY_LEFT_CONTROL]  : 17,
	[glfw.KEY_LEFT_SHIFT]    : 16,
	[glfw.KEY_LEFT_SUPER]    : 91,
	[glfw.KEY_MENU]          : 18,
	[glfw.KEY_MINUS]         : 189,
	[glfw.KEY_NUM_LOCK]      : 144,
	[glfw.KEY_PAGE_DOWN]     : 34,
	[glfw.KEY_PAGE_UP]       : 33,
	[glfw.KEY_PAUSE]         : 19,
	[glfw.KEY_PERIOD]        : 190,
	[glfw.KEY_PRINT_SCREEN]  : 144,
	[glfw.KEY_RIGHT]         : 39,
	[glfw.KEY_RIGHT_ALT]     : 18,
	[glfw.KEY_RIGHT_BRACKET] : 221,
	[glfw.KEY_RIGHT_CONTROL] : 17,
	[glfw.KEY_RIGHT_SHIFT]   : 16,
	[glfw.KEY_RIGHT_SUPER]   : 93,
	[glfw.KEY_SCROLL_LOCK]   : 145,
	[glfw.KEY_SEMICOLON]     : 186,
	[glfw.KEY_SLASH]         : 191,
	[glfw.KEY_SPACE]         : 32,
	[glfw.KEY_TAB]           : 9,
	[glfw.KEY_UP]            : 38,
};


module.exports = Window;
