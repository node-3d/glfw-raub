'use strict';

// Add deps dll dirs
require('node-deps-opengl-raub');

const EventEmitter = require('events');

const platformPaths = {
	win32  : './bin_windows/glfw',
	linux  : './bin_linux/glfw'  ,
	darwin : './bin_darwin/glfw' ,
};

const binDir = platformPaths[process.platform];

const glfw = require(binDir);


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
