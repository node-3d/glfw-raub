'use strict';


const assert = require('node:assert').strict;
const { describe, it, before, after } = require('node:test');
const { getPlatform } = require('addon-tools-raub');

const platform = getPlatform();
if (platform !== 'linux') {
	global.__isGlfwInited = true;
}

const glfw = require('..');


const classes = {
	Window: {
		create() {
			return new glfw.Window();
		},
		props: [
			'handle','width','height','w','h','wh','size','title','icon',
			'msaa','version','shouldClose','platformWindow','platformContext',
			'pos','framebufferSize','currentContext','cursorPos',
		],
		methods: [
			'getKey','getMouseButton','getWindowAttrib','setInputMode','swapBuffers',
			'makeCurrent','destroy','iconify','restore','hide','show',
		],
		destroy(instance) {
			if (instance) {
				instance.destroy();
			}
		},
	},
	
	Document: {
		create() {
			return new glfw.Document();
		},
		props: [
			'body','ratio','devicePixelRatio','innerWidth','innerHeight',
			'clientWidth','clientHeight','onkeydown','onkeyup','style','context',
		],
		methods: [
			'getContext','getElementById','getElementsByTagName',
			'createElementNS','createElement','dispatchEvent',
			'addEventListener','removeEventListener','requestAnimationFrame',
		],
		destroy(instance) {
			if (instance) {
				instance.destroy();
			}
		},
	},
};


describe('GLFW', () => {
	it('exports an object', () => {
		assert.strictEqual(typeof glfw, 'object');
	});
	
	
	Object.keys(classes).forEach((c) => {
		it(`${c} is exported`, () => {
			assert.ok(glfw[c] !== undefined);
		});
	});
	
	if (getPlatform() !== 'linux') {
		return;
	}
	
	Object.keys(classes).forEach((c) => describe(c, () => {
		const current = classes[c];
		let instance = null;
		
		before(() => {
			instance = current.create();
		});
		
		after(() => {
			current.destroy(instance);
		});
		
		it('can be created', () => {
			assert.ok(instance instanceof glfw[c]);
		});
		
		current.props.forEach((prop) => {
			it(`#${prop} property exposed`, () => {
				assert.ok(instance[prop] !== undefined);
			});
		});
		
		current.methods.forEach((method) => {
			it(`#${method}() method exposed`, () => {
				assert.strictEqual(typeof instance[method], 'function');
			});
		});
	}));
});
