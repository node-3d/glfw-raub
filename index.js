'use strict';

// Add deps dll dirs
require('node-deps-opengl-raub');

const glfw = require('./binary/glfw');

const Window = require('./js/window');


// Initialize GLFW
if ( ! glfw.Init() ) {
	throw new Error('Failed to initialize GLFW');
}

// OpenGL window default hints
glfw.DefaultWindowHints();


module.exports = Object.assign(glfw, { Window });
