'use strict';


const { getPlatform } = require('addon-tools-raub');

const platform = getPlatform();
if (platform !== 'linux') {
	global.__isGlfwInited = true;
}

module.exports = async () => require('..');
