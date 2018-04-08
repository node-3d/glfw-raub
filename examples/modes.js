'use strict';

const util = require('util');

const glfw = require('../');
const { Document } = glfw;


const doc = new Document({ title: 'GLFW Simple Test 1', autoFullscreen: true });

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
