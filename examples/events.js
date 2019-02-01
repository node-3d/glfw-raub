'use strict';

const util = require('util');

const glfw = require('../');
const { Window } = glfw;


const w1 = new Window({ title: 'GLFW Simple Test 1' });

// 'click' - mouse button clicked
// 'drop' - drag-dropped some files on the window
// 'focused' - focus gained/lost
// 'framebuffer_resize' - render-surface resized
// 'iconified' - window was iconified
// 'keydown' - keyboard key down
// 'keyup' - keyboard key up
// 'mousedown' - mouse button down
// 'mouseenter' - mouse entered/left the window
// 'mousemove' - mouse moved
// 'mouseup' - mouse button up
// 'wheel' - mouse wheel rotation
// 'quit' - window closed
// 'refresh' - window sure needs to be redrawn
// 'resize' - window frame resized
// 'window_pos' - window moved


// testing events
w1.on('click', e => console.log('[click]', e));
w1.on('drop', e => console.log('[drop]', e));
w1.on('focused', e => console.log('[focused]', e));
w1.on('framebuffer_resize', e => console.log('[framebuffer_resize]', e));
w1.on('iconified', e => console.log('[iconified]', e));
w1.on('keydown', e => console.log('[keydown]', e));
w1.on('keyup', e => console.log('[keyup]', e));
w1.on('mousedown', e => console.log('[mousedown]', e));
w1.on('mouseenter', e => console.log('[mouseenter]', e));
w1.on('mousemove', e => console.log('[mousemove]', e));
w1.on('mouseup', e => console.log('[mouseup]', e));
w1.on('wheel', e => console.log('[wheel]', e));
w1.on('quit', e => console.log('[quit]', e));
w1.on('refresh', e => console.log('[refresh]', e));
w1.on('resize', e => console.log('[resize]', e));
w1.on('window_pos', e => console.log('[window_pos]', e));



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
