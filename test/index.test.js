'use strict';


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
			instance.destroy();
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
			instance.destroy();
		},
	},
	
};


describe('GLFW', () => {
	
	it('exports an object', () => {
		expect(typeof glfw).toBe('object');
	});
	
	
	Object.keys(classes).forEach(
		c => {
			it(`${c} is exported`, () => {
				expect(glfw).toHaveProperty(c);
			});
		}
	);
	
	Object.keys(classes).forEach(c => describe(c, () => {
		const current = classes[c];
		let instance = null;
		
		beforeAll(() => {
			instance = current.create();;
		});
		
		afterAll(() => {
			current.destroy();
		});
		
		it('can be created', () => {
			expect(instance).toBeInstanceOf(glfw[c]);
		});
		
		
		current.props.forEach(prop => {
			it(`#${prop} property exposed`, () => {
				expect(instance).toHaveProperty(prop);
			});
		});
		
		current.methods.forEach(method => {
			it(`#${method}() method exposed`, () => {
				expect(typeof instance[method]).toBe('function');
			});
		});
	}));
	
});
