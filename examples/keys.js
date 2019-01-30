'use strict';

const util = require('util');

const glfw = require('../');
const { Window } = glfw;


const w1 = new Window({ title: 'GLFW Simple Test 1' });

// testing events
w1.on('keydown', e => console.log(`[#1 keydown] ${util.inspect(e)}`));
w1.on('char', e => {
	console.log(`[#1 char] ${util.inspect(e)}`);
	console.log(String.fromCharCode(e.charCode));
});


console.log(w1.version);


const draw = () => {
	
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
