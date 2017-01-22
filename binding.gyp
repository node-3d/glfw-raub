{
  'variables': {
    'platform': '<(OS)',
  },
  'conditions': [
    # Replace gyp platform with node platform, blech
    ['platform == "mac"', {'variables': {
      'platform': 'darwin',
    }}],
    ['platform == "win"', {'variables': {'platform': 'win32'}}],
  ],
  'targets': [
    {
      'target_name': 'glfw',
      'defines': [
        'VERSION=0.4.6',
      ],
      'sources': [
        'src/glfw.cc'
      ],
      'include_dirs': [
        "<!(node -e \"require('nan')\")",
        './deps/include',
      ],
      'conditions': [
        ['OS=="linux"', {
          'libraries': [
            '<!@(pkg-config --libs glfw3 glew)',
            '-lXrandr','-lXinerama','-lXxf86vm','-lXcursor','-lXi',
            '-lrt','-lm'
            ]
        }],
        ['OS=="mac"', {
          'include_dirs': [ '<!@(pkg-config glfw3 glew --cflags-only-I | sed s/-I//g)'],
          'libraries': [ '<!@(pkg-config --libs glfw3 glew)', '-framework OpenGL'],
          'library_dirs': ['/usr/local/lib'],
        }],
        ['OS=="win"', {
            'include_dirs': [
              './deps/include',
              ],
            'library_dirs': [
              './deps/windows/lib/<(target_arch)',
              ],
            'libraries': [
              'FreeImage.lib',
              'glfw3dll.lib',
              'glew32.lib',
              'opengl32.lib'
              ],
            'defines' : [
              'WIN32_LEAN_AND_MEAN',
              'VC_EXTRALEAN'
            ],
            'msvs_version' : '2013',
            'msvs_settings' : {
              'VCCLCompilerTool' : {
                'AdditionalOptions' : ['/O2','/Oy','/GL','/GF','/Gm-','/EHsc','/MT','/GS','/Gy','/GR-','/Gd']
              },
              'VCLinkerTool' : {
                'AdditionalOptions' : ['/OPT:REF','/OPT:ICF','/LTCG']
              },
            },
          },
        ],
      ],
    }
  ]
}
