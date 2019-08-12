'use strict';

// Add deps dll dirs
require('deps-opengl-raub');

const { bin } = require('addon-tools-raub');

const core = require(`./${bin}/glfw`);


// Initialize GLFW
if ( ! core.init() ) {
	throw new Error('Failed to initialize GLFW');
}

// OpenGL window default hints
core.defaultWindowHints();

module.exports = core;
