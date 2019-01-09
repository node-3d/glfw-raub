'use strict';

const glfw = require('../');
const { Window } = glfw;


const w1 = new Window({ title: 'GLFW Simple Test 1' });


const iconData = [];
for (let i = 0; i < 18 * 18 * 4; i++) {
	iconData.push(Math.floor(256 * Math.random()));
}
const icon = {
	width  : 18,
	height : 18,
	data: Buffer.from(iconData),
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
