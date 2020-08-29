'use strict';

const Window = require('./window');


const ESC_KEY = 27;
const F_KEY = 70;


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
		
		if (Document.webgl && ! Document.isWebglInited) {
			if (typeof Document.webgl.init === 'function') {
				Document.webgl.init();
			}
			Document.isWebglInited = true;
		}
		
		if ( ! opts.ignoreQuit ) {
			
			const isUnix = process.platform !== 'win32';
			if ( isUnix && process.listeners('SIGINT').indexOf(Document.exit) < 0 ) {
				process.on('SIGINT', Document.exit);
			}
			
			this.on('quit', () => process.exit(0));
			
			if ( opts.autoEsc ) {
				this.on('keydown', e => e.keyCode === ESC_KEY && process.exit(0));
			}
			
		}
		
		if (opts.autoFullscreen) {
			
			this.on('keydown', e => {
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
	
	
	getElementById() { return this; }
	
	getElementsByTagName() { return this; }
	
	appendChild() {}
	
	
	createElementNS(_0, name) { return this.createElement(name); }
	
	createElement(name) {
		
		name = name.toLowerCase();
		
		if (name.indexOf('canvas') >= 0) {
			
			if ( ! this._isCanvasRequested ) {
				this._isCanvasRequested = true;
				return this;
			}
			
			const that = this;
			
			return {
				_ctx   : null,
				width  : this.width,
				height : this.height,
				
				getContext(kind) {
					this._ctx = that.getContext(kind);
					return this._ctx;
				},
				
				get data() {
					return this._ctx && this._ctx.data;
				},
				
				onkeydown    : () => {},
				onkeyup      : () => {},
				onmousedown  : () => {},
				onmouseup    : () => {},
				onwheel      : () => {},
				onmousewheel : () => {},
				onresize     : () => {},
				
				dispatchEvent       : () => {},
				addEventListener    : () => {},
				removeEventListener : () => {},
				
			};
			
		} else if (name.indexOf('img') >= 0) {
			
			return new Document.Image();
			
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
