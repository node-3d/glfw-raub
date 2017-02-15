{
	'variables': {
		'platform'      : '<(OS)',
		'opengl_root'   : '<!(node -e "console.log(require(\'node-deps-opengl-raub\').root)")',
		'opengl_include': '<(opengl_root)/include',
		'opengl_bin'    : '<!(node -e "console.log(require(\'node-deps-opengl-raub\').bin)")',
	},
	'conditions': [
		['platform == "mac"', { 'variables': { 'platform': 'darwin' } }],
		['platform == "win"', { 'variables': { 'platform': 'win32'  } }],
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
						'msvs_version'  : '2013',
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
			'target_name': 'copy_binary',
			'type':'none',
			'dependencies' : ['glfw'],
			'destination': '<(module_root_dir)/bin_<(platform)',
			'conditions': [
				[
					'OS=="linux"',
					{
						'copies': [
							{
								'files': [
									'<(module_root_dir)/build/Release/glfw.node',
								]
							}
						]
					}
				],
				[
					'OS=="mac"',
					{
						'copies': [
							{
								'files': [
									'<(module_root_dir)/build/Release/glfw.node',
								]
							}
						]
					}
				],
				[
					'OS=="win"',
					{
						'copies': [
							{
								'files': [
									'<(module_root_dir)/build/Release/glfw.node',
								]
							}
						]
					},
				],
				
			],
			
		}
	]
}
