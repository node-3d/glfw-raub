{
	'variables': {
		'bin': '<!(node -p "require(\'addon-tools-raub\').getBin()")',
		'gl_include': '<!(node -p "require(\'deps-opengl-raub\').include")',
		'gl_bin': '<!(node -p "require(\'deps-opengl-raub\').bin")',
	},
	'targets': [{
		'target_name': 'glfw',
		'includes': ['../node_modules/addon-tools-raub/utils/common.gypi'],
		'sources': [
			'cpp/bindings.cpp',
		],
		'include_dirs': [
			'<(gl_include)',
			'<!@(node -p "require(\'addon-tools-raub\').getInclude()")',
			'include',
		],
		'library_dirs': ['<(gl_bin)'],
		'conditions': [
			['OS=="linux"', {
				'libraries': [
					"-Wl,-rpath,'$$ORIGIN'",
					"-Wl,-rpath,'$$ORIGIN/../node_modules/deps-opengl-raub/<(bin)'",
					"-Wl,-rpath,'$$ORIGIN/../../deps-opengl-raub/<(bin)'",
					'<(gl_bin)/libglfw.so.3',
					'<(gl_bin)/libGL.so',
					'<(gl_bin)/libXrandr.so',
				],
			}],
			['OS=="mac"', {
				'libraries': [
					'-Wl,-rpath,@loader_path',
					'-Wl,-rpath,@loader_path/../node_modules/deps-opengl-raub/<(bin)',
					'-Wl,-rpath,@loader_path/../../deps-opengl-raub/<(bin)',
					'<(gl_bin)/glfw.dylib',
				],
			}],
			['OS=="win"', {
				'libraries': ['glfw3dll.lib', 'opengl32.lib'],
			}],
		],
	}],
}
