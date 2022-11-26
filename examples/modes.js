'use strict';

const glfw = require('../');
const { Document } = glfw;

let mode = 'windowed';

if (process.argv.includes('--fullscreen')) {
	mode = 'fullscreen';
} else if (process.argv.includes('--borderless')) {
	mode = 'borderless';
}

const doc = new Document({ title: 'GLFW Simple Test 1', mode, vsync: true });

doc.on(
	'resize',
	({ width, height}) => console.log('Resized to', width, 'x', height)
);

const F_KEY = 70;

doc.on('keydown', e => {
	
	if (e.keyCode === F_KEY && e.ctrlKey && e.shiftKey) {
		doc.mode = 'windowed';
	} else if (e.keyCode === F_KEY && e.ctrlKey && e.altKey) {
		doc.mode = 'fullscreen';
	} else if (e.keyCode === F_KEY && e.ctrlKey) {
		doc.mode = 'borderless';
	} else {
		return;
	}
	
});


const draw = () => {
	const wsize1 = doc.framebufferSize;
	glfw.testScene(wsize1.width, wsize1.height);
	doc.swapBuffers();
	
	glfw.pollEvents();
};


const close = () => {
	// Close OpenGL window and terminate GLFW
	doc.destroy();
	glfw.terminate();
	process.exit(0);
};


const animate = () => {
	if (doc.shouldClose) {
		close();
		return;
	}
	
	draw();
	setTimeout(animate, 16);
};


animate();
