'use strict';

const glfw = require('./core');

const Window = require('./js/window');
const Document = require('./js/document');


// Initialize GLFW
if ( ! glfw.init() ) {
	throw new Error('Failed to initialize GLFW');
}

// OpenGL window default hints
glfw.defaultWindowHints();


module.exports = Object.assign(glfw, { Window, Document });
