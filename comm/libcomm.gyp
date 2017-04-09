{
  'targets': [
    {
      'target_name': 'libcomm',
      'type': 'static_library',
      'defines': [

      ],
      'dependencies': [
        'libenv',

        #TODO: Remove extra dependencies
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
        'src/wsocs/protocols.cpp',
        'src/wsocs/serving.cpp',
        'src/wsocs/WebSocketServer.cpp',

        'src/tuum_http.cpp',
        'src/tuum_wsproto.cpp',
        'src/tuum_comm.cpp',

        'src/WSContext.cpp',
      ],

      'direct_dependent_settings': {
        'include_dirs': ['inc']
      },
    },
  ],
}
