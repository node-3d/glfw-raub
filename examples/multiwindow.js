'use strict';

const glfw = require('../');
const { Window } = glfw;


const windows = [1, 2, 3, 4, 5].map((i) => (
	new Window({ title: `GLFW Multiwindow ${i}`, vsync: true, width: 200, height: 200 })
));

// testing events
windows.forEach((w, i) => {
	w.on('mousemove', (e) => console.log(`[#${i + 1} mousemove] ${e.x}, ${e.y}`));
});


const draw = () => {
	glfw.pollEvents();
	windows.forEach((w) => {
		w.makeCurrent();
		glfw.testScene(w.width, w.height);
		w.swapBuffers();
	});
};


const loopFunc = () => {
	if (
		windows.some((w) => w.shouldClose || w.getKey(glfw.KEY_ESCAPE))
	) {
		process.exit(0);
		return;
	}
	
	draw();
	setImmediate(loopFunc);
};
setImmediate(loopFunc);
