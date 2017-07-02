{
  'targets': [
    {
      'target_name': 'libsystem',
      'type': 'static_library',

      'dependencies': [
        'libhal',
        'libenv',
        'liblpx',

        'libgeometry',
        'librtxmath',
        'libentities',

        'libcomm',
        'libloc',

        'libogl-lab',
        #'libocl-lab',

        'libvision',
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
        'src/tuum_addons.cpp',

        'src/tuum_physics.cpp',
        'src/tuum_visioning.cpp',
        'src/tuum_localizer.cpp',
        'src/tuum_system.cpp',
        'src/tuum_perception.cpp',
        'src/tuum_logmgr.cpp',

        'src/syscore/MotionData.cpp',

        'src/protocols/tuum_VisionProtocol.cpp',
        'src/protocols/tuum_NaviProtocol.cpp'
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
