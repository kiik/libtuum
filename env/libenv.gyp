{
  'targets': [
    {
      'target_name': 'libenv',
      'type': 'static_library',

      'dependencies': [

      ],

      'defines': [

      ],

      'includes': [
        '../build/common.gypi'
      ],

      'cflags': [
        '--std=c++11'
      ],

      'include_dirs': [
        'inc',
      ],
      'sources': [
        'src/platform.cpp',

        'src/tuum_communication.cpp',
        'src/tuum_configuration.cpp',
        'src/tuum_logger.cpp',
        'src/tuum_platform.cpp',
        'src/tuum_buff.cpp',

        'src/tuum_fs.cpp',
        'src/fs/tuum_file.cpp',
        'src/fs/tuum_dbfile.cpp',

        'src/ProgramModule.cpp',
        'src/ModuleNet.cpp',
        'src/ResourceNetwork.cpp',

        'src/strms/DataStream.cpp',
        'src/strms/ImageStream.cpp',
      ],

      'direct_dependent_settings': {
        'include_dirs': ['inc'],
      },
      'link_settings': {
        'libraries': ['-lenv'],
      },
    },
  ],
}
