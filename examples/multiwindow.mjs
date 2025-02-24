import glfw from '../index.js';
const { Window } = glfw;


const windows = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9].map((i) => {
	const w = new Window({ title: `GLFW Multiwindow ${i}`, vsync: false, width: 200, height: 200 });
	const render = () => {
		w.makeCurrent();
		glfw.testScene(w.width, w.height);
	};
	return { w, render };
});

// testing events
windows.forEach(({ w }, i) => {
	w.on('mousemove', (e) => console.log(`[#${i + 1} mousemove] ${e.x}, ${e.y}`));
});


let prevTime = Date.now();
let frames = 0;
let events = 0;


const loopFunc = () => {
	for (let i = 0; i < windows.length; i++) {
		const { w, render } = windows[i];
		if (w.shouldClose || w.getKey(glfw.KEY_ESCAPE)) {
			process.exit(0);
			return;
		}
		w.drawWindow(render);
	}
	
	setImmediate(loopFunc);
	
	frames++;
	const time = Date.now();
	if (time >= prevTime + 2000) {
		console.log(
			'FPS:', Math.floor((frames * 1000) / (time - prevTime)),
			'Events:', events,
		);
		prevTime = time;
		frames = 0;
		events = 0;
	}
};
setImmediate(loopFunc);
