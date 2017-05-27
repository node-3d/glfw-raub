{
	'variables': {
		'platform'      : '<(OS)',
		'opengl_root'   : '<!(node -e "console.log(require(\'node-deps-opengl-raub\').root)")',
		'opengl_include': '<(opengl_root)/include',
		'opengl_bin'    : '<!(node -e "console.log(require(\'node-deps-opengl-raub\').bin)")',
	},
	'conditions': [
		['platform == "mac"', { 'variables': { 'platform': 'darwin' } }],
		['platform == "win"', { 'variables': { 'platform': 'windows'  } }],
	],
	'targets': [
		{
			'target_name': 'glfw',
			'defines': [ 'VERSION=0.4.6' ],
			'sources': [ 'src/glfw.cc' ],
			'include_dirs': [
				'<!(node -e "require(\'nan\')")',
				'<(opengl_include)',
			],
			'library_dirs': [ '<(opengl_bin)' ],
			'conditions': [
				[
					'OS=="linux"',
					{
						'libraries': [
							'<!@(pkg-config --libs glfw3 glew)',
							'-lXrandr','-lXinerama','-lXxf86vm','-lXcursor','-lXi',
							'-lrt','-lm'
						]
					}
				],
				[
					'OS=="mac"',
					{
						'include_dirs': [ '<!@(pkg-config glfw3 glew --cflags-only-I | sed s/-I//g)' ],
						'libraries'   : [ '<!@(pkg-config --libs glfw3 glew)', '-framework OpenGL' ],
						'library_dirs': [ '/usr/local/lib' ],
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
			'target_name'  : 'copy_binary',
			'type'         : 'none',
			'dependencies' : ['glfw'],
			'copies'       : [
				{
					'destination' : '<(module_root_dir)/bin_<(platform)',
					'conditions'  : [
						[
							'OS=="linux"',
							{ 'files' : [] }
						],
						[
							'OS=="mac"',
							{ 'files' : [] }
						],
						[
							'OS=="win"',
							{ 'files' : [ '<(module_root_dir)/build/Release/glfw.node' ] },
						],
					]
				}
			],
		},
		
		{
			'target_name'  : 'remove_extras',
			'type'         : 'none',
			'dependencies' : ['copy_binary'],
			'actions'      : [
				{
					'action_name' : 'action_remove1',
					'inputs'      : ['build/Release/glfw.*'],
					'outputs'     : ['build'],
					'conditions'  : [
						[ 'OS=="linux"', { 'action' : [ 'rm -rf <@(_inputs)' ] } ],
						[ 'OS=="mac"'  , { 'action' : [ 'rm -rf <@(_inputs)' ] } ],
						[ 'OS=="win"'  , { 'action' : [ '<(module_root_dir)/_del', '<@(_inputs)' ] } ],
					],
				},
				{
					'action_name' : 'action_remove2',
					'inputs'      : ['build/Release/obj/glfw/*.obj'],
					'outputs'     : ['build'],
					'conditions'  : [
						[ 'OS=="linux"', { 'action' : [ 'rm -rf <@(_inputs)' ] } ],
						[ 'OS=="mac"'  , { 'action' : [ 'rm -rf <@(_inputs)' ] } ],
						[ 'OS=="win"'  , { 'action' : [ '<(module_root_dir)/_del', '<@(_inputs)' ] } ],
					],
				},
				{
					'action_name' : 'action_remove3',
					'inputs'      : ['build/Release/obj/glfw/*.pdb'],
					'outputs'     : ['build'],
					'conditions'  : [
						[ 'OS=="linux"', { 'action' : [ 'rm -rf <@(_inputs)' ] } ],
						[ 'OS=="mac"'  , { 'action' : [ 'rm -rf <@(_inputs)' ] } ],
						[ 'OS=="win"'  , { 'action' : [ '<(module_root_dir)/_del', '<@(_inputs)' ] } ],
					],
				},
			],
		},
		
	]
}
