import { EventEmitter } from 'events';

declare module "glfw-raub" {
	type TWindowMode = 'windowed' | 'borderless' | 'fullscreen';
	
	type TSize = Readonly<{ width: number; height: number }>;
	
	type TPos = Readonly<{ x: number; y: number }>;
	
	type TRect = TSize & TPos & {
		left: number;
		top: number;
		right: number;
		bottom: number;
	};
	
	type TImage = TSize & Readonly<{
		data: Buffer;
		noflip?: boolean;
	}>;
	
	type TMonitorMode = TSize & Readonly<{
		rate: number; // refresh rate
	}>;
	
	type TMonitor = TMonitorMode & {
		is_primary: boolean; // is this screen primary
		name: string; // screen name
		pos_x: number; // global position x of the screen
		pos_y: number; // global position y of the screen
		width_mm: number; // screen width in mm
		height_mm: number; // screen height in mm
		modes: ReadonlyArray<TMonitorMode>;
	};
	
	type TEvent = {
		type: string;
		[key: string]: any;
	};
	
	type TMouseEvent = TEvent & {
		buttons: number;
		clientX: number;
		clientY: number;
		pageX: number;
		pageY: number;
		x: number;
		y: number;
		shiftKey: boolean;
		ctrlKey: boolean;
		altKey: boolean;
		metaKey: boolean;
	};
	
	type TMouseMoveEvent = TMouseEvent & {
		movementX: number;
		movementY: number;
	};
	
	type TMouseButtonEvent = TMouseEvent & {
		button: number;
		which: number;
	};
	
	type TMouseScrollEvent = TMouseEvent & {
		deltaX: number;
		deltaY: number;
		deltaZ: number;
		wheelDeltaX: number;
		wheelDeltaY: number;
		wheelDelta: number;
	};
	
	type TJoystickEvent = TEvent & {
		id: number;
		event: number;
	};
	
	type TKeyEvent = TEvent & {
		repeat: boolean;
		altKey: boolean;
		ctrlKey: boolean;
		metaKey: boolean;
		shiftKey: boolean;
		code: string | null;
		key: string | null;
		which: number;
		charCode: number;
	};
	
	type TDropEvent = TEvent & {
		dataTransfer: Readonly<{ [key: string]: never }>
		dropEffect: 'none';
		effectAllowed: 'all';
		files: ReadonlyArray<string>;
		items: ReadonlyArray<string>;
		types: ReadonlyArray<never>;
	};
	
	type TIconifyEvent = TEvent & {
		iconified: boolean;
	};
	
	type TPosEvent = TEvent & TPos;
	
	type TSizeEvent = TEvent & TSize;
	
	type TEventCb<T extends TEvent> = (event: T) => (void | boolean);
	
	type TCbField<T extends TEvent> = TEventCb<T> | ReadonlyArray<TEventCb<T>>;
	
	type TWindowOpts = Readonly<Partial<{
		/** Major OpenGL version to be used. Default is 2. */
		major: number;
		/** Minor OpenGL version to be used. Default is 1. */
		minor: number;
		/** Window title, takes current directory as default. Default is $PWD. */
		width: number;
		/** Window initial width. Default is 800. */
		height: number;
		/** Window initial height. Default is 600. */
		display: number;
		/** Display id to open window on a specific display. Default is undefined. */
		vsync: boolean;
		/** If vsync should be used. Default is false. */
		autoIconify: boolean;
		/** If the window is fullscreen, takes presedence over `mode`. Default is false. */
		fullscreen: boolean;
		/** One of `'windowed', 'borderless', 'fullscreen'`. Default is 'windowed'. */
		mode: TWindowMode;
		/** If fullscreen windows should iconify automatically on focus loss. Default is true. */
		decorated: boolean;
		/** Multisample antialiasing level. Default is 2. */
		msaa: number;
		/** Winodw icon. Default is null. */
		icon: TImage;
		/** If window has borders (use `false` for borderless fullscreen). Default is true. */
		title: string;
	}>>;

	type TWindowPtr = number;
	
	/** GLFW Window API wrapper
	 * Window is a higher-level js-wrapper for GLFW API.
	 * It helps managing window instances. It also extends
	 * EventEmitter to provide event-handling.
	*/
	class Window extends EventEmitter {
		constructor(opts?: TWindowOpts);
		
		/** The ratio between physical and logical pixels, e.g 2 for Retina. Default is 1.*/
		readonly ratio: number;
		
		/** Alias for ratio. */
		readonly devicePixelRatio: number;
		
		/** GLFW window pointer. Literally, the pointer from C++. */
		readonly handle: number;
		
		/** Always 0 */
		readonly scrollX: number;
		
		/** Always 0 */
		readonly scrollY: number;
		
		/** Number of msaa samples. */
		readonly msaa: number;
		
		/** OpenGL vendor info. */
		readonly version: string;
		
		/** The size of allocated framebuffer, same as pxSize. */
		readonly framebufferSize: Readonly<{ width: number; height: number }>;
		
		/** Which OpenGL context is now current. */
		readonly currentContext: number;
		
		/** Window display mode. Default is 'windowed'.
		 * One of 'windowed', 'borderless', 'fullscreen'.
		 * Here 'borderless' emulates fullscreen by a frameless, screen-sized window.
		 * When this property is changed, a new window is created and the old is hidden.
		*/
		mode: TWindowMode;
		
		/** Width in LOGICAL pixels.
		 * For Retina - twice the window size.
		*/
		width: number;
		
		/** Height in LOGICAL pixels.
		 * For Retina - twice the window size.
		*/
		height: number;
		
		/** Alias for width. */
		offsetWidth: number;
		
		/** Alias for height. */
		offsetHeight: number;
		
		/** Alias for width. */
		w: number;
		
		/** Alias for height. */
		h: number;
		
		/** An Array, containing LOGICAL width and height. */
		wh: [number, number];
		
		/** An Object, containing LOGICAL width and height. */
		pxSize: TSize;
		
		/** Alias for width. */
		innerWidth: number;
		
		/** Alias for height. */
		innerHeight: number;
		
		/** Alias for width. */
		clientWidth: number;
		
		/** Alias for height. */
		clientHeight: number;
		
		/** Alias for .on('keydown', cb).
		 * Setter adds a new callback.
		 * Getter returns an Array of currently existing callbacks.
		*/
		onkeydown: TCbField<TKeyEvent>;
		
		/** Alias for .on('keyup', cb).
		 * Setter adds a new callback.
		 * Getter returns an Array of currently existing callbacks.
		*/
		onkeyup: TCbField<TKeyEvent>;
		
		/** Alias for .on('mousedown', cb).
		 * Setter adds a new callback.
		 * Getter returns an Array of currently existing callbacks.
		*/
		onmousedown: TCbField<TMouseButtonEvent>;
		
		/** Alias for .on('mouseup', cb).
		 * Setter adds a new callback.
		 * Getter returns an Array of currently existing callbacks.
		*/
		onmouseup: TCbField<TMouseButtonEvent>;
		
		/** Alias for .on('wheel', cb).
		 * Setter adds a new callback.
		 * Getter returns an Array of currently existing callbacks.
		*/
		onwheel: TCbField<TMouseScrollEvent>;
		
		/** Alias for .on('mousewheel', cb).
		 * Setter adds a new callback.
		 * Getter returns an Array of currently existing callbacks.
		*/
		onmousewheel: TCbField<TMouseScrollEvent>;
		
		/** Alias for .on('resize', cb).
		 * Setter adds a new callback.
		 * Getter returns an Array of currently existing callbacks.
		*/
		onresize: TCbField<TSizeEvent>;
		
		/** An Object, containing PHYSICAL width and height of the window. */
		size: TSize;
		
		/** Alias for .on('keydown', cb).
		 * Setter adds a new callback.
		 * Getter returns an Array of currently existing callbacks.
		*/
		title: string;
		
		/** Window icon in RGBA format.
		 * Consider using image-raub Image implementation.
		 * The given image is vertically flipped if noflip is not set to true.
		 * @see https://github.com/node-3d/image-raub
		 * @see https://github.com/node-3d/glfw-raub/examples/icon.js
		 * */
		icon: TImage;
		
		/** If the window is going to be closed. */
		shouldClose: boolean;
		
		/** Window position X-coordinate on the screen. */
		x: number;
		
		/** Window position Y-coordinate on the screen. */
		y: number;
		
		/** An Object, containing the window position coordinates. */
		pos: TPos;
		
		/** An Object, containing the cursor position coordinates. */
		cursorPos: TPos;
		
		/** Get a monitor having the most overlap with this window. */
		getCurrentMonitor(): TMonitor;
		
		/** Gets a browserlike rect of this window.
		 * @see https://developer.mozilla.org/en-US/docs/Web/API/Element/getBoundingClientRect
		*/
		getBoundingClientRect(): TRect;
		
		/** Get key state (GLFW_PRESS/GLFW_RELEASE).
		 * @see https://www.glfw.org/docs/latest/group__keys.html
		*/
		getKey(): number;
		
		/** Get mouse button state (GLFW_PRESS/GLFW_RELEASE).
		 * @see https://www.glfw.org/docs/latest/group__buttons.html
		*/
		getMouseButton(): number;
		
		/** Get window attribute.
		 * @see https://www.glfw.org/docs/latest/window_guide.html#window_attribs
		*/
		getWindowAttrib(): number;
		
		/** Set input mode option. */
		setInputMode(mode: number): void;
		
		/** Swaps the front and back buffers of the window. */
		swapBuffers(): void;
		
		/** Make this window's GL context current. */
		makeCurrent(): void;
		
		/** Destroy the GLFW window. */
		destroy(): void;
		
		/** Minimize the window. */
		iconify(): void;
		
		/** Restore the window if it was previously iconified or maximized. */
		restore(): void;
		
		/** Hide the window. */
		hide(): void;
		
		/** Show the window, if it is hidden. */
		show(): void;
		
		/** Emit an event on behalf of this window. */
		emit(name: string, event: TEvent): boolean;
		
		/** Add event listener.
		 * 'blur' - window [focus lost](https://developer.mozilla.org/en-US/docs/Web/Events/blur)
		 * 'click' - mouse button [clicked](https://developer.mozilla.org/en-US/docs/Web/Events/click)
		 * 'drop' - drag-[dropped](https://developer.mozilla.org/en-US/docs/Web/Events/drop) some files on the window
		 * 'focus' - window [focus gained](https://developer.mozilla.org/en-US/docs/Web/Events/focus)
		 * 'focusin' - window [focus gained](https://developer.mozilla.org/en-US/docs/Web/Events/focusin)
		 * 'focusout' - window [focus lost](https://developer.mozilla.org/en-US/docs/Web/Events/focusout)
		 * 'resize' - render-surface resized (values in pixels) `{ width, height }`
		 * 'iconifiy' - window was iconified
		 * 'keydown' - keyboard [key down](https://developer.mozilla.org/en-US/docs/Web/Events/keydown)
		 * 'keyup' - keyboard [key up](https://developer.mozilla.org/en-US/docs/Web/Events/keyup)
		 * 'mousedown' - mouse [button down](https://developer.mozilla.org/en-US/docs/Web/Events/mousedown)
		 * 'mouseenter' - mouse [entered](https://developer.mozilla.org/en-US/docs/Web/Events/mouseenter) the window
		 * 'mouseleave' - mouse [left](https://developer.mozilla.org/en-US/docs/Web/Events/mouseleave) the window
		 * 'mouseup' - mouse [button up](https://developer.mozilla.org/en-US/docs/Web/Events/mouseup)
		 * 'quit' - window closed
		 * 'refresh' - window needs to be redrawn
		 * 'wresize' - window frame resized (NOT really pixels) `{ width, height }`
		 * 'wheel' - mouse [wheel rotation](https://developer.mozilla.org/en-US/docs/Web/Events/wheel)
		 * 'move' - window moved `{ x, y }`
		 * Note: `keypress` event is not supported.
		*/
		on(name: string, cb: (event: TEvent) => (void | boolean)): this;
		
		/** Alias for emit, type is expected inside the event object. */
		dispatchEvent(event: TEvent): void;
		
		/** Alias for on. */
		addEventListener(name: string): void;
		
		/** Alia for removeListener. */
		removeEventListener(name: string): void;
		
		/** BOUND requestAnimationFrame method, returns id. */
		requestAnimationFrame(cb: (dateNow: number) => void): number;
		
		/** BOUND cancelAnimationFrame method. Cancels by id. */
		cancelAnimationFrame(id: number): void;
	}
	
	
	type TDocumentOpts = TWindowOpts & Readonly<{
		/** If the window should ignore the default quit signals, e.g. ESC key. */
		ignoreQuit: boolean;
		
		/** If window has the default key handlers for fullscreen.
		 * CTRL+F - borderless fullscreen window.
		 * CTRL+ALT+F - real, exclusive fullscreen mode.
		 * CTRL+SHIFT+F - back to windowed.
		*/
		autoFullscreen: boolean;
	}>;
	
	/** Web-like Document
	 * Document extends Window to provide an additional web-style compatibility layer.
	 * As name suggests, objects of such class will mimic the behavior and properties of
	 * your typical browser window.document. But also it is a Window, at the same time.
	 * And it is incomplete at this point: you still have to provide an Image class of
	 * your choice and WebGL context (implementation).
	 * Two static methods are designated for this: setImage and setWebgl.
	*/
	class Document extends Window {
		/** Set Image implementation
		 * For example, [this Image implementation](https://github.com/raub/node-image).
		 * Also sets global.HTMLImageElement.
		*/
		static setImage(Image: unknown): void;
		
		/** Set WebGL implementation
		 * For example, [this WebGL implementation](https://github.com/raub/node-webgl).
		*/
		static setWebgl(Webgl: unknown): void;
		
		constructor(opts?: TDocumentOpts);
		
		/** Returns `this`. */
		readonly body: Document;
		
		/** Mimic web-element `style` property.
		 * But only `width` and `height` matters.
		*/
		readonly style: TSize;
		
		/** Returns `Document.webgl`, set through `Document.setWebgl`. */
		readonly context: Readonly<{ [key: string]: unknown }>;
		
		/** Returns `Document.webgl`, set through `Document.setWebgl`. */
		getContext(kind: string): Readonly<{ [key: string]: unknown }>;
		
		/** Returns `this`. */
		getElementById(id: string): Document;
		
		/** If contains 'canvas', returns `this`, otherwise `null`. */
		getElementsByTagName(tag: string): ReadonlyArray<Document>;
		
		/** Does nothing. */
		appendChild(): void;
		
		/** Returns the result of `createElement(tag)` */
		createElementNS(unused: unknown, tag: string): void;
		
		/** Fake createElement.
		 * For `'canvas'` returns `this` for *the first call*,
		 * then returns new instances of canvas-like object capable of using 2d or 3d context.
		 * This is done for some web APIs like three.js, which create additional canvases.
		 * For `'image'` returns `new Document.Image`, set through `Document.setImage`.
		*/
		createElement(tag: string): Document;
	}

	/** Hide the terminal window.
	 * **Windows ONLY** hides the console window, but only in case
	 * this console window is property of this process. For instance this works if you use
	 * `pkg` module to bundle your app, and then doubleclick the EXE. But if you are running
	 * a command, like `node script.js`, then this won't hide the window. **It's safe to call
	 * this function on all platforms, but it will be ignored, unless the platform is Windows**.
	*/
	const hideConsole: () => void;
	
	/** Show the terminal window.
	 * **Windows ONLY** shows the console window
	 * if it was previously hidden with `glfw.hideConsole()`.
	*/
	const showConsole: () => void;
	
	/** Draws a test scene, used in examples here. */
	const testScene: () => void;
	
	/** Draws a test scene, that reacts to a joystick. */
	const testJoystick: () => void;
	
	const init: () => void;
	const initHint: (hint: number, value: number) => void;
	const terminate: () => void;
	const getVersion: () => Readonly<{
		major: number;
		minor: number;
		rev: number;
	}>;
	const getVersionString: () => string;
	const getError: () => string;
	const getTime: () => number;
	const setTime: (time: number) => void;
	const getMonitors: () => ReadonlyArray<TMonitor>;
	const getPrimaryMonitor: () => TMonitor;
	const windowHint: (hint: number, value: number) => void;
	const windowHintString: (hint: number, value: string) => void;
	const defaultWindowHints: () => void;
	const joystickPresent: () => boolean;
	const getJoystickAxes: (id: number) => string;
	const getJoystickButtons: (id: number) => string;
	const getJoystickName: (id: number) => string;
	
	/** Create a GLFW window.
	 * This function differs from GLFW Docs signature due to JS specifics.
	 * Here `emitter` is any object having a **BOUND** `emit()` method.
	 * It will be used to transmit GLFW events.
	 */
	const createWindow: (
		width: number,
		height: number,
		emitter: Readonly<{ emit: (name: string, event: TEvent ) => void }>,
		title: string,
		monitor: number,
	) => TWindowPtr;
	
	const destroyWindow: (window: TWindowPtr) => void;
	const setWindowTitle: (window: TWindowPtr, title: string) => void;
	const setWindowIcon: (window: TWindowPtr, icon: TImage) => void;
	const getWindowSize: (window: TWindowPtr) => TSize;
	const getWindowFrameSize: (window: TWindowPtr) => Readonly<{
		left: number;
		top: number;
		right: number;
		bottom: number;
	}>;
	const getWindowContentScale: (window: TWindowPtr) => Readonly<{
		xscale: number;
		yscale: number;
	}>;
	const setWindowSize: (window: TWindowPtr, w: number, h: number) => void;
	const setWindowSizeLimits: (
		window: TWindowPtr,
		minwidth: number,
		minheight: number,
		maxwidth: number,
		maxheight: number,
	) => void;
	const setWindowAspectRatio: (window: TWindowPtr, numer: number, denom: number) => void;
	const setWindowPos: (window: TWindowPtr, x: number, y: number) => void;
	const getWindowPos: (window: TWindowPtr) => TPos;
	const getWindowOpacity: (window: TWindowPtr) => number;
	const setWindowOpacity: (window: TWindowPtr, opacity: number) => void;
	const maximizeWindow: (window: TWindowPtr) => void;
	const focusWindow: (window: TWindowPtr) => void;
	const requestWindowAttention: (window: TWindowPtr) => void;
	const getWindowMonitor: (window: TWindowPtr) => TMonitor;
	const getFramebufferSize: (window: TWindowPtr) => TSize;
	const iconifyWindow: (window: TWindowPtr) => void;
	const restoreWindow: (window: TWindowPtr) => void;
	const hideWindow: (window: TWindowPtr) => void;
	const showWindow: (window: TWindowPtr) => void;
	const windowShouldClose: (window: TWindowPtr) => number;
	const setWindowShouldClose: (window: TWindowPtr, shouldClose: number) => void;
	const getWindowAttrib: (window: TWindowPtr) => number;
	const setWindowAttrib: (window: TWindowPtr, value: number) => void;
	const setInputMode: (window: TWindowPtr, mode: number, value: number) => void;
	const getInputMode: (window: TWindowPtr, mode: number) => number;
	const pollEvents: () => void;
	const waitEvents: () => void;
	const waitEventsTimeout: (timeout: number) => void;
	const postEmptyEvent: () => void;
	const getKey: (window: TWindowPtr, key: number) => number;
	const getMouseButton: (window: TWindowPtr, button: number) => number;
	const getCursorPos: (window: TWindowPtr, x: number, y: number) => void;
	const setCursorPos: (window: TWindowPtr) => TPos;
	const makeContextCurrent: (window: TWindowPtr) => void;
	const getCurrentContext: () => TWindowPtr;
	const swapBuffers: (window: TWindowPtr) => void;
	const swapInterval: (interval: number) => void;
	const extensionSupported: (name: string) => boolean;
	const rawMouseMotionSupported: () => boolean;
	const getKeyName: (key: number, scan: number) => string;
	const getKeyScancode: (key: number) => number;
	const getJoystickHats: (id: string) => ReadonlyArray<number>;
	const joystickIsGamepad: (id: string) => boolean;
	const updateGamepadMappings: (mappings: string) => boolean;
	const getGamepadName: (id: number) => string;
	const getGamepadState: (id: number) => Readonly<{
		buttons: ReadonlyArray<number>;
		axes: ReadonlyArray<number>;
	}>;
	const setClipboardString: (window: TWindowPtr, value: string) => void;
	const getClipboardString: (window: TWindowPtr) => string;
	const getTimerValue: () => number;
	const getTimerFrequency: () => number;
	const platformWindow: (window: TWindowPtr) => number;
	const platformContext: (window: TWindowPtr) => number;
	const platformDevice: (window: TWindowPtr) => number;
	const getJoystickGUID: (id: number) => (null | string);
	const VERSION_MAJOR: number;
	const VERSION_MINOR: number;
	const VERSION_REVISION: number;
	const TRUE: number;
	const FALSE: number;
	const RELEASE: number;
	const PRESS: number;
	const REPEAT: number;
	const HAT_CENTERED: number;
	const HAT_UP: number;
	const HAT_RIGHT: number;
	const HAT_DOWN: number;
	const HAT_LEFT: number;
	const HAT_RIGHT_UP: number;
	const HAT_RIGHT_DOWN: number;
	const HAT_LEFT_UP: number;
	const HAT_LEFT_DOWN: number;
	const KEY_UNKNOWN: number;
	const KEY_SPACE: number;
	const KEY_APOSTROPHE: number;
	const KEY_COMMA: number;
	const KEY_MINUS: number;
	const KEY_PERIOD: number;
	const KEY_SLASH: number;
	const KEY_0: number;
	const KEY_1: number;
	const KEY_2: number;
	const KEY_3: number;
	const KEY_4: number;
	const KEY_5: number;
	const KEY_6: number;
	const KEY_7: number;
	const KEY_8: number;
	const KEY_9: number;
	const KEY_SEMICOLON: number;
	const KEY_EQUAL: number;
	const KEY_A: number;
	const KEY_B: number;
	const KEY_C: number;
	const KEY_D: number;
	const KEY_E: number;
	const KEY_F: number;
	const KEY_G: number;
	const KEY_H: number;
	const KEY_I: number;
	const KEY_J: number;
	const KEY_K: number;
	const KEY_L: number;
	const KEY_M: number;
	const KEY_N: number;
	const KEY_O: number;
	const KEY_P: number;
	const KEY_Q: number;
	const KEY_R: number;
	const KEY_S: number;
	const KEY_T: number;
	const KEY_U: number;
	const KEY_V: number;
	const KEY_W: number;
	const KEY_X: number;
	const KEY_Y: number;
	const KEY_Z: number;
	const KEY_LEFT_BRACKET: number;
	const KEY_BACKSLASH: number;
	const KEY_RIGHT_BRACKET: number;
	const KEY_GRAVE_ACCENT: number;
	const KEY_WORLD_1: number;
	const KEY_WORLD_2: number;
	const KEY_ESCAPE: number;
	const KEY_ENTER: number;
	const KEY_TAB: number;
	const KEY_BACKSPACE: number;
	const KEY_INSERT: number;
	const KEY_DELETE: number;
	const KEY_RIGHT: number;
	const KEY_LEFT: number;
	const KEY_DOWN: number;
	const KEY_UP: number;
	const KEY_PAGE_UP: number;
	const KEY_PAGE_DOWN: number;
	const KEY_HOME: number;
	const KEY_END: number;
	const KEY_CAPS_LOCK: number;
	const KEY_SCROLL_LOCK: number;
	const KEY_NUM_LOCK: number;
	const KEY_PRINT_SCREEN: number;
	const KEY_PAUSE: number;
	const KEY_F1: number;
	const KEY_F2: number;
	const KEY_F3: number;
	const KEY_F4: number;
	const KEY_F5: number;
	const KEY_F6: number;
	const KEY_F7: number;
	const KEY_F8: number;
	const KEY_F9: number;
	const KEY_F10: number;
	const KEY_F11: number;
	const KEY_F12: number;
	const KEY_F13: number;
	const KEY_F14: number;
	const KEY_F15: number;
	const KEY_F16: number;
	const KEY_F17: number;
	const KEY_F18: number;
	const KEY_F19: number;
	const KEY_F20: number;
	const KEY_F21: number;
	const KEY_F22: number;
	const KEY_F23: number;
	const KEY_F24: number;
	const KEY_F25: number;
	const KEY_KP_0: number;
	const KEY_KP_1: number;
	const KEY_KP_2: number;
	const KEY_KP_3: number;
	const KEY_KP_4: number;
	const KEY_KP_5: number;
	const KEY_KP_6: number;
	const KEY_KP_7: number;
	const KEY_KP_8: number;
	const KEY_KP_9: number;
	const KEY_KP_DECIMAL: number;
	const KEY_KP_DIVIDE: number;
	const KEY_KP_MULTIPLY: number;
	const KEY_KP_SUBTRACT: number;
	const KEY_KP_ADD: number;
	const KEY_KP_ENTER: number;
	const KEY_KP_EQUAL: number;
	const KEY_LEFT_SHIFT: number;
	const KEY_LEFT_CONTROL: number;
	const KEY_LEFT_ALT: number;
	const KEY_LEFT_SUPER: number;
	const KEY_RIGHT_SHIFT: number;
	const KEY_RIGHT_CONTROL: number;
	const KEY_RIGHT_ALT: number;
	const KEY_RIGHT_SUPER: number;
	const KEY_MENU: number;
	const KEY_LAST: number;
	const MOD_SHIFT: number;
	const MOD_CONTROL: number;
	const MOD_ALT: number;
	const MOD_SUPER: number;
	const MOD_CAPS_LOCK: number;
	const MOD_NUM_LOCK: number;
	const MOUSE_BUTTON_1: number;
	const MOUSE_BUTTON_2: number;
	const MOUSE_BUTTON_3: number;
	const MOUSE_BUTTON_4: number;
	const MOUSE_BUTTON_5: number;
	const MOUSE_BUTTON_6: number;
	const MOUSE_BUTTON_7: number;
	const MOUSE_BUTTON_8: number;
	const MOUSE_BUTTON_LAST: number;
	const MOUSE_BUTTON_LEFT: number;
	const MOUSE_BUTTON_RIGHT: number;
	const MOUSE_BUTTON_MIDDLE: number;
	const JOYSTICK_1: number;
	const JOYSTICK_2: number;
	const JOYSTICK_3: number;
	const JOYSTICK_4: number;
	const JOYSTICK_5: number;
	const JOYSTICK_6: number;
	const JOYSTICK_7: number;
	const JOYSTICK_8: number;
	const JOYSTICK_9: number;
	const JOYSTICK_10: number;
	const JOYSTICK_11: number;
	const JOYSTICK_12: number;
	const JOYSTICK_13: number;
	const JOYSTICK_14: number;
	const JOYSTICK_15: number;
	const JOYSTICK_16: number;
	const JOYSTICK_LAST: number;
	const GAMEPAD_BUTTON_A: number;
	const GAMEPAD_BUTTON_B: number;
	const GAMEPAD_BUTTON_X: number;
	const GAMEPAD_BUTTON_Y: number;
	const GAMEPAD_BUTTON_LEFT_BUMPER: number;
	const GAMEPAD_BUTTON_RIGHT_BUMPER: number;
	const GAMEPAD_BUTTON_BACK: number;
	const GAMEPAD_BUTTON_START: number;
	const GAMEPAD_BUTTON_GUIDE: number;
	const GAMEPAD_BUTTON_LEFT_THUMB: number;
	const GAMEPAD_BUTTON_RIGHT_THUMB: number;
	const GAMEPAD_BUTTON_DPAD_UP: number;
	const GAMEPAD_BUTTON_DPAD_RIGHT: number;
	const GAMEPAD_BUTTON_DPAD_DOWN: number;
	const GAMEPAD_BUTTON_DPAD_LEFT: number;
	const GAMEPAD_BUTTON_LAST: number;
	const GAMEPAD_BUTTON_CROSS: number;
	const GAMEPAD_BUTTON_CIRCLE: number;
	const GAMEPAD_BUTTON_SQUARE: number;
	const GAMEPAD_BUTTON_TRIANGLE: number;
	const GAMEPAD_AXIS_LEFT_X: number;
	const GAMEPAD_AXIS_LEFT_Y: number;
	const GAMEPAD_AXIS_RIGHT_X: number;
	const GAMEPAD_AXIS_RIGHT_Y: number;
	const GAMEPAD_AXIS_LEFT_TRIGGER: number;
	const GAMEPAD_AXIS_RIGHT_TRIGGER: number;
	const GAMEPAD_AXIS_LAST: number;
	const NO_ERROR: number;
	const NOT_INITIALIZED: number;
	const NO_CURRENT_CONTEXT: number;
	const INVALID_ENUM: number;
	const INVALID_VALUE: number;
	const OUT_OF_MEMORY: number;
	const API_UNAVAILABLE: number;
	const VERSION_UNAVAILABLE: number;
	const PLATFORM_ERROR: number;
	const FORMAT_UNAVAILABLE: number;
	const NO_WINDOW_CONTEXT: number;
	const FOCUSED: number;
	const ICONIFIED: number;
	const RESIZABLE: number;
	const VISIBLE: number;
	const DECORATED: number;
	const AUTO_ICONIFY: number;
	const FLOATING: number;
	const MAXIMIZED: number;
	const CENTER_CURSOR: number;
	const TRANSPARENT_FRAMEBUFFER: number;
	const HOVERED: number;
	const FOCUS_ON_SHOW: number;
	const RED_BITS: number;
	const GREEN_BITS: number;
	const BLUE_BITS: number;
	const ALPHA_BITS: number;
	const DEPTH_BITS: number;
	const STENCIL_BITS: number;
	const ACCUM_RED_BITS: number;
	const ACCUM_GREEN_BITS: number;
	const ACCUM_BLUE_BITS: number;
	const ACCUM_ALPHA_BITS: number;
	const AUX_BUFFERS: number;
	const STEREO: number;
	const SAMPLES: number;
	const SRGB_CAPABLE: number;
	const REFRESH_RATE: number;
	const DOUBLEBUFFER: number;
	const CLIENT_API: number;
	const CONTEXT_VERSION_MAJOR: number;
	const CONTEXT_VERSION_MINOR: number;
	const CONTEXT_REVISION: number;
	const CONTEXT_ROBUSTNESS: number;
	const OPENGL_FORWARD_COMPAT: number;
	const OPENGL_DEBUG_CONTEXT: number;
	const OPENGL_PROFILE: number;
	const CONTEXT_RELEASE_BEHAVIOR: number;
	const CONTEXT_NO_ERROR: number;
	const CONTEXT_CREATION_API: number;
	const SCALE_TO_MONITOR: number;
	const COCOA_RETINA_FRAMEBUFFER: number;
	const COCOA_FRAME_NAME: number;
	const COCOA_GRAPHICS_SWITCHING: number;
	const X11_CLASS_NAME: number;
	const X11_INSTANCE_NAME: number;
	const NO_API: number;
	const OPENGL_API: number;
	const OPENGL_ES_API: number;
	const NO_ROBUSTNESS: number;
	const NO_RESET_NOTIFICATION: number;
	const LOSE_CONTEXT_ON_RESET: number;
	const OPENGL_ANY_PROFILE: number;
	const OPENGL_CORE_PROFILE: number;
	const OPENGL_COMPAT_PROFILE: number;
	const CURSOR: number;
	const STICKY_KEYS: number;
	const STICKY_MOUSE_BUTTONS: number;
	const LOCK_KEY_MODS: number;
	const RAW_MOUSE_MOTION: number;
	const CURSOR_NORMAL: number;
	const CURSOR_HIDDEN: number;
	const CURSOR_DISABLED: number;
	const ANY_RELEASE_BEHAVIOR: number;
	const RELEASE_BEHAVIOR_FLUSH: number;
	const RELEASE_BEHAVIOR_NONE: number;
	const NATIVE_CONTEXT_API: number;
	const EGL_CONTEXT_API: number;
	const OSMESA_CONTEXT_API: number;
	const ARROW_CURSOR: number;
	const IBEAM_CURSOR: number;
	const CROSSHAIR_CURSOR: number;
	const HAND_CURSOR: number;
	const HRESIZE_CURSOR: number;
	const VRESIZE_CURSOR: number;
	const CONNECTED: number;
	const DISCONNECTED: number;
	const JOYSTICK_HAT_BUTTONS: number;
	const COCOA_CHDIR_RESOURCES: number;
	const COCOA_MENUBAR: number;
	const DONT_CARE: number;
}