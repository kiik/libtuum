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
        'src/core/clab_types.cpp',
        'src/core/clab_lang.cpp',
        'src/core/clab_reader.cpp',
        'src/core/clab_parser.cpp',
        'src/core/clab_modules.cpp',

        'src/sys/clab_properties.cpp',
        'src/sys/clab_procedure.cpp',
        'src/sys/clab_kernel.cpp',
        'src/sys/clab_pipeline.cpp',
        'src/sys/clab_script.cpp',

        'src/tuum_ocl_ppl.cpp',
        'src/tuum_ocl.cpp',
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
