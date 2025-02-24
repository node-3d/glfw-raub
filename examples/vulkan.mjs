import glfw from '../index.js';
import EventEmitter from 'node:events';


const isVulkanSupported = glfw.vulkanSupported();
console.log('Vulkan is', isVulkanSupported ? 'supported.' : 'unsupported.');

let instancePtr = null;

if (isVulkanSupported) {
	const vkExt = glfw.getRequiredInstanceExtensions();
	console.log('Vulkan extensions:', vkExt);
	
	instancePtr = glfw.vulkanCreateInstance('vulkan-example', vkExt);
	console.log('Created instance:', instancePtr);
}

const emitterObj = new EventEmitter();
const emitter = { emit: (t, e) => emitterObj.emit(t, e) };

glfw.windowHint(glfw.RESIZABLE, glfw.TRUE);
glfw.windowHint(glfw.VISIBLE, glfw.TRUE);
glfw.windowHint(glfw.AUTO_ICONIFY, glfw.FALSE);
glfw.windowHint(glfw.DECORATED, glfw.TRUE);

glfw.windowHint(glfw.CLIENT_API, glfw.NO_API);
const windowPtr = glfw.createWindow(
	800,
	600,
	emitter,
	'vulkan example',
	null,
	true,
);
glfw.windowHint(glfw.CLIENT_API, glfw.OPENGL_API);

let deviceInfo = null;

if (instancePtr) {
	const surfacePtr = glfw.createWindowSurface(instancePtr, windowPtr);
	console.log('Created surface:', surfacePtr);
	
	deviceInfo = glfw.vulkanCreateDevice(instancePtr, windowPtr);
	console.log('Created device:', deviceInfo);
	
	const isSupported = glfw.getPhysicalDevicePresentationSupport(
		instancePtr, deviceInfo.physicalDevice, deviceInfo.queueFamily,
	);
	console.log('Presentation supported:', isSupported);
	
	const vkGetDeviceProcAddrPtr = glfw.getInstanceProcAddress(instancePtr, 'vkGetDeviceProcAddr');
	console.log('Got `vkGetDeviceProcAddr`:', vkGetDeviceProcAddrPtr);
}

const draw = () => {
	glfw.pollEvents();
};


const close = () => {
	if (deviceInfo) {
		glfw.vulkanDestroyDevice(instancePtr, deviceInfo.device);
		console.log('Deleted device.');
	}
	
	// Close the window and terminate GLFW
	glfw.destroyWindow(windowPtr);
	
	if (instancePtr) {
		glfw.vulkanDestroyInstance(instancePtr);
		console.log('Deleted instance.');
	}
	
	glfw.terminate();
	process.exit(0);
};


const loopFunc = () => {
	const shouldClose = glfw.windowShouldClose(windowPtr);
	const isEscPressed = glfw.getKey(windowPtr, glfw.KEY_ESCAPE);
	
	if (shouldClose || isEscPressed) {
		close();
		return;
	}
	
	draw();
	setImmediate(loopFunc);
};
setImmediate(loopFunc);
