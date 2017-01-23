{
	'variables': {
		'platform' : '<(OS)',
		'deps_root': '<!(node -e "console.log(require(\'node-deps-opengl-raub\'))")',
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
				'./deps/include',
			],
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
						'include_dirs': [ '<(deps_root)/include' ],
						'library_dirs': [ '<(deps_root)/bin_<(platform)' ],
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
			'conditions': [
				[
					'OS=="linux"',
					{
						'copies': [
							{
								'destination': '<(module_root_dir)/bin_linux',
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
								'destination': '<(module_root_dir)/bin_darwin',
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
								'destination': '<(module_root_dir)/bin_win32',
								'files': [
									'<(module_root_dir)/build/Release/glfw.node',
									'<(deps_root)/bin_<(platform)/FreeImage.dll',
									'<(deps_root)/bin_<(platform)/glew32.dll',
									'<(deps_root)/bin_<(platform)/glfw3.dll',
								]
							}
						]
					},
				],
				
			],
			
		}
	]
}
