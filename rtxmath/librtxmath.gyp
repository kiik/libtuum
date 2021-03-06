{
  'targets': [
    {
      'target_name': 'librtxmath',
      'type': 'static_library',

      'dependencies': [
        'libenv',
        'libgeometry',
      ],

      'cflags': [
        '--std=c++11'
      ],
      'include_dirs': [
	'inc'
      ],
      'sources': [
        'src/__future__.cpp',
        'src/Transform.cpp',
        'src/PID.cpp',
      ],

      'all_dependent_settings': {
        'include_dirs': ['inc']
      },

      'link_settings': {
        'libraries': ['-lrtxmath']
      },
    },
  ],
}
