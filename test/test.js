'use strict';

const { expect } = require('chai');

const glfw = require('glfw-raub');


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
	},
	
};


describe('GLFW', () => {
	
	it('exports an object', () => {
		expect(glfw).to.be.an('object');
	});
	
	
	Object.keys(classes).forEach(
		c => {
			it(`${c} is exported`, () => {
				expect(glfw).to.respondTo(c);
			});
		}
	);
	
	Object.keys(classes).forEach(c => describe(c, () => {
		
		const current = classes[c];
		const instance = current.create();
		
		it('can be created', () => {
			expect(instance).to.be.an.instanceOf(glfw[c]);
		});
		
		
		current.props.forEach(prop => {
			it(`#${prop} property exposed`, () => {
				expect(instance).to.have.property(prop);
			});
		});
		
		current.methods.forEach(method => {
			it(`#${method}() method exposed`, () => {
				expect(instance).to.respondTo(method);
			});
		});
		
	}));
	
});
