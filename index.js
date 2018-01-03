'use strict';

// Add deps dll dirs
require('node-deps-opengl-raub');

const EventEmitter = require('events');

const glfw = require('./binary/glfw');


const events = new EventEmitter();

events.emit = (type, event) => {
	
	if (type !== 'quit') {
		event.preventDefault  = () => {};
		event.stopPropagation = () => {};
	}
	
	events.listeners(type).forEach(listener => listener(event));
	
};


Object.defineProperty(glfw, 'events', {
	get          : function () { return events; },
	enumerable   : true,
	configurable : true
});


module.exports = glfw;
