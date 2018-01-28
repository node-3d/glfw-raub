'use strict';

const util = require('util');

const glfw = require('.');
const { Window } = glfw;


const w1 = new Window({ title: 'GLFW Simple Test 1' });
const w2 = new Window({ title: 'GLFW Simple Test 2' });


const iconData = [];
for (let i = 0; i < 18 * 18 * 4; i++) {
	iconData.push(Math.floor(256 * Math.random()));
}
const icon = {
	width  : 18,
	height : 18,
	data: Buffer.from(iconData),
}

w1.icon = icon;

iconData.splice(0, iconData.length);
for (let i = 0; i < 18 * 18 * 4; i++) {
	iconData.push(Math.floor(256 * Math.random()));
}
icon.data = Buffer.from(iconData);

w2.icon = icon;


// testing events
w1.on('keydown', e => console.log(`[#1 keydown] ${util.inspect(e)}`));
w1.on('keydown', e => w1.height += 10);
w1.on('mousedown', e => console.log(`[#1 mousedown] ${util.inspect(e)}`));
w1.on('mousedown', e => w1.width += 10);
w1.on('mousemove', e => console.log(`[#1 mousemove] ${e.x}, ${e.y}`));
w1.on('mousewheel', e => console.log(`[#1 mousewheel] ${e.position}`));
w1.on('resize', e => console.log(`[#1 resize] ${e.width}, ${e.height}`));

w2.on('keydown', e => console.log(`[#2 keydown] ${util.inspect(e)}`));
w2.on('keydown', e => w2.height -= 10);
w2.on('mousedown', e => console.log(`[#2 mousedown] ${util.inspect(e)}`));
w2.on('mousedown', e => w2.width -= 10);
w2.on('mousemove', e => console.log(`[#2 mousemove] ${e.x}, ${e.y}`));
w2.on('mousewheel', e => console.log(`[#2 mousewheel] ${e.position}`));
w2.on('resize', e => console.log(`[#2 resize] ${e.width}, ${e.height}`));

console.log(w1.version);


const draw = () => {
	
	w1.makeCurrent();
	const wsize1 = w1.framebufferSize;
	glfw.testScene(wsize1.width, wsize1.height);
	w1.swapBuffers();
	
	w2.makeCurrent();
	const wsize2 = w2.framebufferSize;
	glfw.testScene(wsize2.width, wsize2.height);
	w2.swapBuffers();
	
	glfw.pollEvents();
	
};


const animate = () => {
	
	if ( ! (
		w1.shouldClose || w2.shouldClose ||
		w1.getKey(glfw.KEY_ESCAPE) || w2.getKey(glfw.KEY_ESCAPE)
	) ) {
		
		draw();
		setTimeout(animate, 16);
		
	} else {
		// Close OpenGL window and terminate GLFW
		w1.destroy();
		w2.destroy();
		
		glfw.terminate();
		
		process.exit(0);
	}
	
};


animate();
