'use strict';

const glfw = require('./core');

const Window = require('./js/window');
const Document = require('./js/document');
const {
	keyNames, codeNames, extraCodes,
} = require('./js/constants');

module.exports = Object.assign(glfw, {
	Window, Document, keyNames, codeNames, extraCodes,
});
