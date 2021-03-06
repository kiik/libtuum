{
  'targets': [
    {
      'target_name': 'libsystem',
      'type': 'static_library',

      'dependencies': [
        'libhal',
        'libenv',
        'liblpx',
        'libloc',
        'libvision',
        'libgeometry',
        'librtxmath',
        'libentities',
      ],

      'cflags': [
        '--std=c++11'
      ],
      'include_dirs': [
	'inc'
      ],
      'sources': [
        'src/tuum_localization.cpp',
        'src/tuum_navigation.cpp',
        'src/tuum_motion.cpp',
        'src/tuum_context.cpp',

        'src/tuum_physics.cpp',
        'src/tuum_visioning.cpp',
        'src/tuum_system.cpp',

        'src/syscore/MotionData.cpp',
      ],


      'direct_dependent_settings': {
        'include_dirs': ['inc']
      },

      'link_settings': {
        'libraries': ['-lsystem']
      },
    },
  ],
}
