'use strict';

const glfw = require('../');
const { Window } = glfw;


const w1 = new Window({ title: 'GLFW Events', vsync: false });

let prevTime = Date.now();
let frames = 0;
let events = 0;

w1.on('mousemove', () => { events++; });

const loopFunc = () => {
	if (w1.shouldClose || w1.getKey(glfw.KEY_ESCAPE)) {
		process.exit(0);
		return;
	}
	
	glfw.testScene(w1.width, w1.height);
	w1.requestAnimationFrame(loopFunc);
	
	frames++;
	const time = Date.now();
	if (time >= prevTime + 1000) {
		console.log(
			'FPS:', Math.floor((frames * 1000) / (time - prevTime)),
			'Events:', events,
		);
		prevTime = time;
		frames = 0;
		events = 0;
	}
};

w1.requestAnimationFrame(loopFunc);
