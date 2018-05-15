'use strict';

// Add deps dll dirs
require('deps-opengl-raub');

const { binPath } = require('addon-tools-raub');

const core = require(`./${binPath}/glfw`);


// Initialize GLFW
if ( ! core.init() ) {
	throw new Error('Failed to initialize GLFW');
}

// OpenGL window default hints
core.defaultWindowHints();


module.exports = core;
