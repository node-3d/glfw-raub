{
	'variables': {
		'rm'             : '<!(node -e "require(\'addon-tools-raub\').rm()")',
		'cp'             : '<!(node -e "require(\'addon-tools-raub\').cp()")',
		'mkdir'          : '<!(node -e "require(\'addon-tools-raub\').mkdir()")',
		'binary'         : '<!(node -e "require(\'addon-tools-raub\').bin()")',
		'opengl_include' : '<!(node -e "require(\'deps-opengl-raub\').include()")',
		'opengl_bin'     : '<!(node -e "require(\'deps-opengl-raub\').bin()")',
	},
	'targets': [
		{
			'target_name': 'glfw',
			'sources': [ 'cpp/bindings.cpp', 'cpp/events.cpp', 'cpp/glfw.cpp' ],
			'include_dirs': [
				'<!@(node -e "require(\'addon-tools-raub\').include()")',
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
						],
						'defines': [
							'__APPLE__'
						]
					}
				],
				[
					'OS=="win"',
					{
						'libraries': [ 'glfw3dll.lib', 'opengl32.lib' ],
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
				'action': ['<(mkdir)', '-p', '<(binary)']
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
				'action'      : ['<(cp)', 'build/Release/glfw.node', '<(binary)/glfw.node'],
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
				'conditions'  : [[
					# IF WINDOWS
					'OS=="win"',
					{ 'action' : [
						'<(rm)',
						'<(module_root_dir)/build/Release/glfw.*',
						'<(module_root_dir)/build/Release/obj/glfw/*.*'
					] },
					# ELSE
					{ 'action' : [
						'<(rm)',
						'<(module_root_dir)/build/Release/obj.target/glfw/cpp/bindings.o',
						'<(module_root_dir)/build/Release/obj.target/glfw/cpp/events.o',
						'<(module_root_dir)/build/Release/obj.target/glfw/cpp/glfw.o',
						'<(module_root_dir)/build/Release/glfw.node'
					] }
				]],
			}],
		},
	]
}
