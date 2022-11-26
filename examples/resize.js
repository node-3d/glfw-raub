'use strict';

const glfw = require('../');
const { Window } = glfw;


const w1 = new Window({ title: 'GLFW Simple Test 1', vsync: true });

// testing events
w1.on('keydown', () => w1.height += 10);
w1.on('mousedown', () => w1.width += 10);

console.log('Press a key to resize Height.\nClick mouse to resize Width.');
w1.on(
	'resize',
	({ width, height}) => console.log('Resized to', width, 'x', height)
);

const draw = () => {
	const wsize1 = w1.framebufferSize;
	glfw.testScene(wsize1.width, wsize1.height);
	w1.swapBuffers();
	
	glfw.pollEvents();
};


const close = () => {
	// Close the window and terminate GLFW
	w1.destroy();
	glfw.terminate();
	process.exit(0);
};


const animate = () => {
	if (w1.shouldClose || w1.getKey(glfw.KEY_ESCAPE)) {
		close();
		return;
	}
	
	draw();
	setTimeout(animate, 16);
};


animate();
