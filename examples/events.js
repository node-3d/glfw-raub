'use strict';

const util = require('util');

const glfw = require('../');
const { Window } = glfw;


const w1 = new Window({ title: 'GLFW Simple Test 1' });

// 'blur' - window focus lost
// 'click' - mouse button clicked
// 'drop' - drag-dropped some files on the window
// 'focus' - window focus gained
// 'focusin' - window focus gained
// 'focusout' - window focus lost
// 'fbresize' - render-surface resized
// 'iconifiy' - window was iconified
// 'keydown' - keyboard key down
// 'keyup' - keyboard key up
// 'mousedown' - mouse button down
// 'mouseenter' - mouse entered the window
// 'mouseleave' - mouse left the window
// 'mouseup' - mouse button up
// 'quit' - window closed
// 'refresh' - window really needs to be redrawn
// 'resize' - window frame resized
// 'wheel' - mouse wheel rotation
// 'move' - window moved


// testing events
// w1.on('mousemove', e => console.log('[mousemove]', e));
w1.on('blur', e => console.log('[blur]', e));
w1.on('click', e => console.log('[click]', e));
w1.on('drop', e => console.log('[drop]', e));
w1.on('focus', e => console.log('[focus]', e));
w1.on('focusin', e => console.log('[focusin]', e));
w1.on('focusout', e => console.log('[focusout]', e));
w1.on('fbresize', e => console.log('[fbresize]', e));
w1.on('iconifiy', e => console.log('[iconifiy]', e));
w1.on('keydown', e => console.log('[keydown]', e));
w1.on('keyup', e => console.log('[keyup]', e));
w1.on('mousedown', e => console.log('[mousedown]', e));
w1.on('mouseenter', e => console.log('[mouseenter]', e));
w1.on('mouseleave', e => console.log('[mouseleave]', e));
w1.on('mouseup', e => console.log('[mouseup]', e));
w1.on('quit', e => console.log('[quit]', e));
w1.on('refresh', e => console.log('[refresh]', e));
w1.on('resize', e => console.log('[resize]', e));
w1.on('wheel', e => console.log('[wheel]', e));
w1.on('move', e => console.log('[move]', e));


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
