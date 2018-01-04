{
	'variables': {
		'opengl_root'   : '<!(node -e "console.log(require(\'node-deps-opengl-raub\').root)")',
		'opengl_include': '<(opengl_root)/include',
		'opengl_bin'    : '<!(node -e "console.log(require(\'node-deps-opengl-raub\').bin)")',
	},
	'targets': [
		{
			'target_name': 'glfw',
			'defines': [ 'VERSION=0.4.6' ],
			'sources': [ 'src/glfw.cpp' ],
			'include_dirs': [
				'<!(node -e "require(\'nan\')")',
				'<(opengl_include)',
				'<!(node -e "require(\'node-addon-tools-raub\')")',
			],
			'library_dirs': [ '<(opengl_bin)' ],
			'conditions': [
				[
					'OS=="linux"',
					{
						'libraries': [
							'-Wl,-rpath,<(opengl_bin)',
							'<(opengl_bin)/libfreeimage.so',
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
							'<(opengl_bin)/freeimage.dylib',
							'<(opengl_bin)/glfw.dylib',
							'<(opengl_bin)/glew.dylib'
						],
					}
				],
				[
					'OS=="win"',
					{
						'libraries': [ 'FreeImage.lib', 'glfw3dll.lib', 'glew32.lib', 'opengl32.lib' ],
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
						'<(module_root_dir)/_rd "<(module_root_dir)/binary" && ' +
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
				'outputs'     : ['src'],
				'conditions'  : [
					[ 'OS=="linux"', { 'action' : [
						'rm',
						'<(module_root_dir)/build/Release/obj.target/glfw/src/glfw.o',
						'<(module_root_dir)/build/Release/obj.target/glfw.node',
						'<(module_root_dir)/build/Release/glfw.node'
					] } ],
					[ 'OS=="mac"', { 'action' : [
						'rm',
						'<(module_root_dir)/build/Release/obj.target/glfw/src/glfw.o',
						'<(module_root_dir)/build/Release/glfw.node'
					] } ],
					[ 'OS=="win"', { 'action' : [
						'<(module_root_dir)/_del "<(module_root_dir)/build/Release/glfw.*" && ' +
						'<(module_root_dir)/_del "<(module_root_dir)/build/Release/obj/glfw/*.*"'
					] } ],
				],
			}],
		},
	]
}
