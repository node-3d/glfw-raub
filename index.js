'use strict';

const glfw = require('./core');

const Window = require('./js/window');
const Document = require('./js/document');


module.exports = Object.assign(glfw, { Window, Document });
