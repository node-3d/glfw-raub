'use strict';

const glfw = require('../');
const { Window } = glfw;


const w1 = new Window({ title: 'GLFW Simple Test 1' });

// 'blur' - window focus lost
// 'click' - mouse button clicked
// 'drop' - drag-dropped some files on the window
// 'focus' - window focus gained
// 'focusin' - window focus gained
// 'focusout' - window focus lost
// 'resize' - render-surface resized
// 'iconifiy' - window was iconified
// 'keydown' - keyboard key down
// 'keyup' - keyboard key up
// 'mousedown' - mouse button down
// 'mouseenter' - mouse entered the window
// 'mouseleave' - mouse left the window
// 'mouseup' - mouse button up
// 'quit' - window closed
// 'refresh' - window really needs to be redrawn
// 'wresize' - window frame resized
// 'wheel' - mouse wheel rotation
// 'move' - window moved


// testing events
// w1.on('mousemove', e => console.log('[mousemove]', e));
w1.on('blur', e => { e.target = {}; console.log('[blur]', e); });
w1.on('click', e => { e.target = {}; console.log('[click]', e); });
w1.on('drop', e => { e.target = {}; console.log('[drop]', e); });
w1.on('focus', e => { e.target = {}; console.log('[focus]', e); });
w1.on('focusin', e => { e.target = {}; console.log('[focusin]', e); });
w1.on('focusout', e => { e.target = {}; console.log('[focusout]', e); });
w1.on('resize', e => { e.target = {}; console.log('[resize]', e); });
w1.on('iconifiy', e => { e.target = {}; console.log('[iconifiy]', e); });
w1.on('keydown', e => { e.target = {}; console.log('[keydown]', e); });
w1.on('keyup', e => { e.target = {}; console.log('[keyup]', e); });
w1.on('mousedown', e => { e.target = {}; console.log('[mousedown]', e); });
w1.on('mouseenter', e => { e.target = {}; console.log('[mouseenter]', e); });
w1.on('mouseleave', e => { e.target = {}; console.log('[mouseleave]', e); });
w1.on('mouseup', e => { e.target = {}; console.log('[mouseup]', e); });
w1.on('quit', e => { e.target = {}; console.log('[quit]', e); });
w1.on('refresh', e => { e.target = {}; console.log('[refresh]', e); });
w1.on('wresize', e => { e.target = {}; console.log('[wresize]', e); });
w1.on('wheel', e => { e.target = {}; console.log('[wheel]', e); });
w1.on('move', e => { e.target = {}; console.log('[move]', e); });


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
