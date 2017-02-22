{
  'targets': [
    {
      'target_name': 'libocl-lab',
      'type': 'static_library',

      'dependencies': [
        'libenv',
        'liblpx',
      ],

      'cflags': [
        '--std=c++11',
      ],
      'include_dirs': [
        'inc',
      ],
      'sources': [
        'src/tuum_ocl.cpp',

        'src/core/clab_types.cpp',
        'src/core/clab_reader.cpp',
        'src/core/clab_parser.cpp',

        'src/sys/clab_pass.cpp',
        'src/sys/clab_kernel.cpp',
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
          '-lOpenCL',
        ],
      },
    },
  ],
}
