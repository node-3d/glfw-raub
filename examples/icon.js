'use strict';

const glfw = require('../');
const { Window } = glfw;


const w1 = new Window({ title: 'GLFW Simple Test 1' });

const ICON_SIZE = 32;

const iconData = [];
for (let y = 0; y < ICON_SIZE; y++) {
	for (let x = 0; x < ICON_SIZE; x++) {
		const r = 255 * (x / (ICON_SIZE - 1));
		const g = 255 * (y / (ICON_SIZE - 1));
		const b = 255 * (1 - r * g);
		iconData.push(r, g, b, 255);
	}
}

const icon = {
	width  : ICON_SIZE,
	height : ICON_SIZE,
	data   : Buffer.from(iconData),
	noflip : true,
};

w1.icon = icon;


const draw = () => {
	
	w1.makeCurrent();
	const wsize1 = w1.framebufferSize;
	glfw.testScene(wsize1.width, wsize1.height);
	w1.swapBuffers();
	
	glfw.pollEvents();
	
};


const animate = () => {
	
	if ( ! (
		w1.shouldClose ||
		w1.getKey(glfw.KEY_ESCAPE)
	) ) {
		
		draw();
		setTimeout(animate, 16);
		
	} else {
		// Close OpenGL window and terminate GLFW
		w1.destroy();
		
		glfw.terminate();
		
		process.exit(0);
	}
	
};


animate();
