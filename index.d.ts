import { EventEmitter } from 'events';

declare module "glfw-raub" {
	// This nested namespace 'glfw' will merge with the enclosing 
	// declared namespace 'glfw'.
	// https://www.typescriptlang.org/docs/handbook/declaration-merging.html
	namespace glfw {
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
			[key: string]: unknown;
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

		type TWindowOpts = Readonly<{
			major?: number;
			minor?: number;
			width?: number;
			height?: number;
			display?: number;
			vsync?: boolean;
			autoIconify?: boolean;
			fullscreen?: boolean;
			mode?: TWindowMode;
			decorated?: boolean;
			msaa?: number;
			icon?: TImage;
			title?: string;
		}>;

		type TWindowPtr = number;

		class Window extends EventEmitter {
			constructor(opts?: TWindowOpts);
			
			readonly ratio: number;
			readonly devicePixelRatio: number;
			readonly handle: number;
			readonly scrollX: number;
			readonly scrollY: number;
			readonly msaa: number;
			readonly version: string;
			readonly framebufferSize: Readonly<{ width: number; height: number }>;
			readonly currentContext: number;
			
			mode: TWindowMode;
			width: number;
			height: number;
			offsetWidth: number;
			offsetHeight: number;
			w: number;
			h: number;
			wh: [number, number];
			pxSize: TSize;
			innerWidth: number;
			innerHeight: number;
			clientWidth: number;
			clientHeight: number;
			onkeydown: TEventCb<TKeyEvent> | ReadonlyArray<TEventCb<TKeyEvent>>;
			onkeyup: TEventCb<TKeyEvent> | ReadonlyArray<TEventCb<TKeyEvent>>;
			onmousedown: TEventCb<TMouseButtonEvent> | ReadonlyArray<TEventCb<TMouseButtonEvent>>;
			onmouseup: TEventCb<TMouseButtonEvent> | ReadonlyArray<TEventCb<TMouseButtonEvent>>;
			onwheel: TEventCb<TMouseScrollEvent> | ReadonlyArray<TEventCb<TMouseScrollEvent>>;
			onmousewheel: TEventCb<TMouseScrollEvent> | ReadonlyArray<TEventCb<TMouseScrollEvent>>;
			onresize: TEventCb<TSizeEvent> | ReadonlyArray<TEventCb<TSizeEvent>>;
			size: TSize;
			title: string;
			icon: TImage;
			shouldClose: boolean;
			x: number;
			y: number;
			pos: TPos;
			cursorPos: TPos;
			
			getCurrentMonitor(): TMonitor;
			getBoundingClientRect(): TRect;
			getKey(): number;
			getMouseButton(): number;
			getWindowAttrib(): number;
			setInputMode(mode: number): void;
			swapBuffers(): void;
			makeCurrent(): void;
			destroy(): void;
			iconify(): void;
			restore(): void;
			hide(): void;
			show(): void;
			emit(name: string, event: TEvent): boolean;
			dispatchEvent(event: TEvent): void;
			addEventListener(name: string): void;
			removeEventListener(name: string): void;
		}
		
		
		type TDocumentOpts = TWindowOpts & Readonly<{
			ignoreQuit: boolean;
			autoFullscreen: boolean;
		}>;
		
		class Document extends Window {
			static setImage(Image: unknown): void;
			static setWebgl(Webgl: unknown): void;
			
			constructor(opts?: TDocumentOpts);
			
			readonly body: Document;
			readonly style: TSize;
			readonly context: Readonly<{ [key: string]: unknown }>;
			
			getContext(kind: string): Document | Readonly<{ [key: string]: unknown }>;
			getElementById(id: string): Document;
			getElementsByTagName(tag: string): ReadonlyArray<Document>;
			appendChild(): void;
			createElementNS(unused: unknown, tag: string): void;
			createElement(tag: string): void;
		}

		
		const hideConsole: () => void;
		const showConsole: () => void;
		const testScene: () => void;
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
	
	export = glfw;
}