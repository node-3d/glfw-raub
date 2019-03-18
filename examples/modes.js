'use strict';

const glfw = require('../');
const { Document } = glfw;


const doc = new Document({ title: 'GLFW Simple Test 1', autoFullscreen: true });

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


const animate = () => {
	
	if ( ! doc.shouldClose ) {
		
		draw();
		setTimeout(animate, 16);
		
	} else {
		// Close OpenGL window and terminate GLFW
		doc.destroy();
		
		glfw.terminate();
		
		process.exit(0);
	}
	
};


animate();
