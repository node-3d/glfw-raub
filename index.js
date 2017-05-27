'use strict';

require('node-deps-opengl-raub');


const platformPaths = {
	win32  : './bin_windows/glfw',
	linux  : './bin_linux/glfw'  ,
	darwin : './bin_darwin/glfw' ,
};

const binDir  = platformPaths[process.platform];

const glfw = require(binDir);

module.exports = glfw;


// Easy event emitter based event loop.  Started automatically when the first
// listener is added.

const EventEmitter = require('events');
const events = new EventEmitter();

events.emit = function() {
	
	const args = Array.prototype.slice.call(arguments);
	const evt  = args[1]; // args[1] is the event, args[0] is the type of event
	
	if(args[0] !== 'quit') {
		evt.preventDefault  = function () {};
		evt.stopPropagation = function () {};
	}
	
	events.listeners(args[0]).forEach(function(listener) {
		listener(args[1]);
	});
	
};


Object.defineProperty(glfw, 'events', {
	get: function () { return events; },
	enumerable  : true,
	configurable: true
});
