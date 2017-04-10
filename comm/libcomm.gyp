{
  'targets': [
    {
      'target_name': 'libcomm',
      'type': 'static_library',
      'defines': [

      ],
      'dependencies': [
        'libenv',
        'libhal',
      ],

      'includes': [
        '../build/common.gypi'
      ],
      'variables': {
        'pkg-config': 'pkg-config'
      },

      'cflags': [
        '--std=c++11',
      ],
      'include_dirs': [
        'inc',
      ],
     'sources': [
        'src/tuum_comm.cpp',
        #'src/tuum_http.cpp',
        'src/tuum_wsproto.cpp',

        'src/WSContext.cpp',
        'src/wsocs/WebSocketServer.cpp',

        'src/wsocs/serving.cpp',
      ],

      'direct_dependent_settings': {
        'include_dirs': ['inc']
      },
    },
  ],
}
