{
  'targets': [
    {
      'target_name': 'libogl-lab',
      'type': 'static_library',

      'dependencies': [
        'libenv',
        'libhal',
      ],

      'cflags': [
        '--std=c++11',
      ],
      'include_dirs': [
        'inc',
        '/usr/local/include/glip-lib',
      ],
      'sources': [
        'src/tuum_ogl.cpp',
      ],


      'direct_dependent_settings': {
        'include_dirs': [
          'inc',
        ],
        'cflags': [
        ],
      },

      'link_settings': {
        'libraries': [
          '-lglip',
          '<!@(pkg-config --libs gl)',
          '<!@(pkg-config --libs glfw3)',
          '<!@(pkg-config --libs glew)',
        ],
      },
    },
  ],
}
