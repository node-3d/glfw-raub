'use strict';

const glfw = require('../core');
const Window  = require('./window');


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
	
	static exit() {
		process.exit(0);
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
			this.on('keydown', e => e.keyCode === ESC_KEY && process.exit(0));
			
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
		
		this.swapBuffers();
		
		const sizeWin = this.size;
		const sizeFB  = this.framebufferSize;
		
		this._ratio = sizeFB.width / sizeWin.width;
		
		this.requestAnimationFrame = this._requestAnimationFrame.bind(this);
		
	}
	
	
	get body() { return this; }
	
	get ratio() { return this._ratio; }
	get devicePixelRatio() { return this._ratio; }
	
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
	
	get style() {
		const that = this;
		return {
			get width() { return that.width; },
			set width(v) { that.width = parseInt(v); },
			get height() { return that.height; },
			set height(v) { that.height = parseInt(v); },
		};
	}
	
	
	get context() { return Document.webgl; }
	getContext() { return Document.webgl; }
	
	
	getElementById() { return this; }
	getElementsByTagName() { return this; }
	
	appendChild() {}
	
	
	createElementNS(_0, name) { return this.createElement(name); }
	
	createElement(name) {
		
		name = name.toLowerCase();
		
		if (name.indexOf('canvas') >= 0) {
			
			return this;
			
		} else if (name.indexOf('img') >= 0) {
			
			const img = new Document.Image();
			img.addEventListener = img.on;
			return img;
			
		}
		
		return null;
		
	}
	
	
	dispatchEvent(event) { this.emit(event.type, event); }
	
	addEventListener(name, callback) { this.on(name, callback); }
	
	removeEventListener(name, callback) { this.removeListener(name, callback); }
	
	
	_requestAnimationFrame(cb) {
		this.swapBuffers();
		glfw.pollEvents();
		setImmediate(() => cb(Date.now()));
	}
	
}


global.HTMLCanvasElement = Document;


Document.setImage(class FakeImage {
	get src() { console.error('Document.Image class not set.'); return ''; }
	set src(v) { console.error('Document.Image class not set.'); v = null; }
	get complete() { return false; }
	on() {}
	onload() {}
	onerror() {}
});

Document.setWebgl(null);


module.exports = Document;
