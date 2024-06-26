'use strict';

const glfw = require('../core');
const Window = require('./window');
const {
	emptyFunction, ESC_KEY, F_KEY,
} = require('./constants');


class Document extends Window {
	static setImage(Image) {
		this.Image = Image;
		global.HTMLImageElement = Image;
	}
	
	
	static setWebgl(webgl) {
		this.webgl = webgl;
		this.isWebglInited = false;
	}
	
	
	constructor(opts = {}) {
		super(opts);
		
		if (Document.webgl && !Document.isWebglInited) {
			try {
				if (typeof Document.webgl.init === 'function') {
					Document.webgl.init();
				}
			} catch (_e) {
				console.warn('WebGL `init()` call failed, but it may still work.');
			}
			Document.isWebglInited = true;
		}
		if (Document.webgl) {
			Document.webgl.canvas = this;
		}
		
		this.on('mousedown', (e) => { this.emit('pointerdown', e); });
		this.on('mouseup', (e) => { this.emit('pointerup', e); });
		this.on('mousemove', (e) => { this.emit('pointermove', e); });
		
		if (!opts.ignoreQuit) {
			const isUnix = process.platform !== 'win32';
			if ( isUnix && process.listeners('SIGINT').indexOf(Document.exit) < 0 ) {
				process.on('SIGINT', Document.exit);
			}
			
			this.on('quit', () => process.exit(0));
			
			if (opts.autoEsc) {
				this.on('keydown', (e) => e.keyCode === ESC_KEY && process.exit(0));
			}
		}
		
		if (opts.autoFullscreen) {
			this.on('keydown', (e) => {
				if (e.keyCode === F_KEY && e.ctrlKey && e.shiftKey) {
					this.mode = 'windowed';
				} else if (e.keyCode === F_KEY && e.ctrlKey && e.altKey) {
					this.mode = 'fullscreen';
				} else if (e.keyCode === F_KEY && e.ctrlKey) {
					this.mode = 'borderless';
				}
			});
		}
	}
	
	setPointerCapture = () => {
		this.setInputMode(glfw.CURSOR, glfw.CURSOR_DISABLED);
	};
	
	releasePointerCapture = () => {
		this.setInputMode(glfw.CURSOR, glfw.CURSOR_NORMAL);
	};
	
	makeCurrent() {
		if (Document.webgl) {
			Document.webgl.canvas = this;
		}
		super.makeCurrent();
	}
	
	
	get body() { return this; }
	
	
	get style() {
		const that = this;
		return {
			get width() { return that.innerWidth; },
			set width(v) { that.width = parseInt(v) * that.devicePixelRatio; },
			get height() { return that.innetHeight; },
			set height(v) { that.height = parseInt(v) * that.devicePixelRatio; },
		};
	}
	
	
	get context() { return Document.webgl; }
	
	getContext(kind) {
		return kind === '2d' ? new Document.Image() : Document.webgl;
	}
	
	
	getRootNode() { return this; }
	
	getElementById() { return this; }
	
	getElementsByTagName() { return [this]; }
	
	appendChild() {}
	
	
	createElementNS(_0, name) { return this.createElement(name); }
	
	
	createElement(name) {
		name = name.toLowerCase();
		
		if (name.indexOf('img') >= 0) {
			return new Document.Image();
		}
		
		if (name.indexOf('canvas') >= 0) {
			if (!this._isCanvasRequested) {
				this._isCanvasRequested = true;
				return this;
			}
			
			const that = this;
			
			return {
				_ctx: null,
				width: this.width,
				height: this.height,
				
				getContext(kind) {
					this._ctx = that.getContext(kind);
					return this._ctx;
				},
				
				get data() {
					return this._ctx && this._ctx.data;
				},
				
				onkeydown: emptyFunction,
				onkeyup: emptyFunction,
				onmousedown: emptyFunction,
				onmouseup: emptyFunction,
				onwheel: emptyFunction,
				onmousewheel: emptyFunction,
				onresize: emptyFunction,
				
				dispatchEvent: emptyFunction,
				addEventListener: emptyFunction,
				removeEventListener: emptyFunction,
			};
		}
		
		return null;
	}
}


global.HTMLCanvasElement = Document;


Document.setImage(class FakeImage {
	get src() {
		console.error('Document.Image class not set.');
		return '';
	}
	set src(v) {
		console.error('Document.Image class not set.');
		v = null;
	}
	get complete() { return false; }
	on() {}
	onload() {}
	onerror() {}
});

Document.setWebgl(null);


module.exports = Document;
