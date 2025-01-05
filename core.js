'use strict';

require('segfault-raub');

// Add deps dll dirs
require('deps-opengl-raub');

const { getBin, createLogger } = require('addon-tools-raub');

createLogger({ name: 'glfw' });

const core = require(`./${getBin()}/glfw.node`);


// Initialize GLFW
if (!global.__isGlfwInited) {
	if (!core.init()) {
		throw new Error('Failed to initialize GLFW');
	}
	
	// OpenGL window default hints
	core.defaultWindowHints();
	global.__isGlfwInited = true;
}

module.exports = core;
