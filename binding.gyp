{
	'variables': {
		'_del'           : '<!(node -e "console.log(require(\'node-addon-tools-raub\')._del)")',
		'_rd'            : '<!(node -e "console.log(require(\'node-addon-tools-raub\')._rd)")',
		'opengl_include' : '<!(node -e "console.log(require(\'node-deps-opengl-raub\').include)")',
		'opengl_bin'     : '<!(node -e "console.log(require(\'node-deps-opengl-raub\').bin)")',
	},
	'targets': [
		{
			'target_name': 'glfw',
			'defines': [ 'VERSION=0.4.6' ],
			'sources': [ 'cpp/glfw.cpp' ],
			'include_dirs': [
				'<!(node -e "require(\'node-addon-tools-raub\').printNan()")',
				'<!(node -e "console.log(require(\'node-addon-tools-raub\').include)")',
				'<(opengl_include)',
			],
			'library_dirs': [ '<(opengl_bin)' ],
			'conditions': [
				[
					'OS=="linux"',
					{
						'libraries': [
							'-Wl,-rpath,<(opengl_bin)',
							'<(opengl_bin)/libglfw.so.3',
							'<(opengl_bin)/libGLEW.so.2.0',
							'<(opengl_bin)/libGL.so',
							'<(opengl_bin)/libXrandr.so',
						],
					}
				],
				[
					'OS=="mac"',
					{
						'libraries': [
							'-Wl,-rpath,<(opengl_bin)',
							'<(opengl_bin)/glfw.dylib',
							'<(opengl_bin)/glew.dylib'
						],
					}
				],
				[
					'OS=="win"',
					{
						'libraries': [ 'glfw3dll.lib', 'glew32.lib', 'opengl32.lib' ],
						'defines' : [
							'WIN32_LEAN_AND_MEAN',
							'VC_EXTRALEAN'
						],
						'msvs_settings' : {
							'VCCLCompilerTool' : {
								'AdditionalOptions' : [
									'/O2','/Oy','/GL','/GF','/Gm-',
									'/EHsc','/MT','/GS','/Gy','/GR-','/Gd',
								]
							},
							'VCLinkerTool' : {
								'AdditionalOptions' : ['/OPT:REF','/OPT:ICF','/LTCG']
							},
						},
					},
				],
			],
		},
		{
			'target_name'  : 'make_directory',
			'type'         : 'none',
			'dependencies' : ['glfw'],
			'actions'      : [{
				'action_name' : 'Directory created.',
				'inputs'      : [],
				'outputs'     : ['build'],
				'conditions'  : [
					[ 'OS=="linux"', { 'action': ['mkdir', '-p', 'binary'] } ],
					[ 'OS=="mac"', { 'action': ['mkdir', '-p', 'binary'] } ],
					[ 'OS=="win"', { 'action': [
						'<(_rd) "<(module_root_dir)/binary" && ' +
						'md "<(module_root_dir)/binary"'
					] } ],
				],
			}],
		},
		{
			'target_name'  : 'copy_binary',
			'type'         : 'none',
			'dependencies' : ['make_directory'],
			'actions'      : [{
				'action_name' : 'Module copied.',
				'inputs'      : [],
				'outputs'     : ['binary'],
				'conditions'  : [
					[ 'OS=="linux"', { 'action' : [
						'cp',
						'<(module_root_dir)/build/Release/glfw.node',
						'<(module_root_dir)/binary/glfw.node'
					] } ],
					[ 'OS=="mac"', { 'action' : [
						'cp',
						'<(module_root_dir)/build/Release/glfw.node',
						'<(module_root_dir)/binary/glfw.node'
					] } ],
					[ 'OS=="win"', { 'action' : [
						'copy "<(module_root_dir)/build/Release/glfw.node"' +
						' "<(module_root_dir)/binary/glfw.node"'
					] } ],
				],
			}],
		},
		{
			'target_name'  : 'remove_extras',
			'type'         : 'none',
			'dependencies' : ['copy_binary'],
			'actions'      : [{
				'action_name' : 'Build intermediates removed.',
				'inputs'      : [],
				'outputs'     : ['cpp'],
				'conditions'  : [
					[ 'OS=="linux"', { 'action' : [
						'rm',
						'<(module_root_dir)/build/Release/obj.target/glfw/cpp/glfw.o',
						'<(module_root_dir)/build/Release/obj.target/glfw.node',
						'<(module_root_dir)/build/Release/glfw.node'
					] } ],
					[ 'OS=="mac"', { 'action' : [
						'rm',
						'<(module_root_dir)/build/Release/obj.target/glfw/cpp/glfw.o',
						'<(module_root_dir)/build/Release/glfw.node'
					] } ],
					[ 'OS=="win"', { 'action' : [
						'<(_del) "<(module_root_dir)/build/Release/glfw.*" && ' +
						'<(_del) "<(module_root_dir)/build/Release/obj/glfw/*.*"'
					] } ],
				],
			}],
		},
	]
}
