'use strict';

require('segfault-raub');

// Add deps dll dirs
require('deps-opengl-raub');

const { getBin } = require('addon-tools-raub');

const core = require(`./${getBin()}/glfw`);


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
