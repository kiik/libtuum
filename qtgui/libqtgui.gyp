{
  'targets': [
    {
      'target_name': 'libqtgui',
      'type': 'static_library',

      'dependencies': [
        'libenv',
        'libentities',
        'libhal',
        'libsystem',
        'liblpx'
      ],

      'cflags': [
        '--std=c++11',
        '-fPIC',
        '<!@(pkg-config --cflags Qt5Quick)',
        '<!@(pkg-config --cflags assimp)',
      ],
      'include_dirs': [
        'inc'
      ],
      'sources': [
        'src/3dcore/TSEntity.cpp',
        'src/3dcore/TSMaterial.cpp',
        'src/3dcore/TSMesh.cpp',
        'src/3dcore/TSShader.cpp',

        'src/components/TSCamera.cpp',
        'src/components/TSDemoComponent.cpp',
        'src/components/TSMeshFilter.cpp',
        'src/components/TSMeshRenderer.cpp',
        'src/components/TSTransform.cpp',

        'src/materials/YUV2RGBMaterial.cpp',
        'src/textures/VideoTexture.cpp',

        'src/utils/ObjLoader.cpp',
        'src/utils/ShaderLdr.cpp',

        'src/vstream/QSVideoRenderer.cpp',
        'src/vstream/VideoPlayer.cpp',
        'src/vstream/moc_QSVideoRenderer.cpp',
        'src/vstream/moc_VideoPlayer.cpp',

        'src/TuumScene.cpp',
        'src/VisionResponse.cpp',
        'src/moc_TuumScene.cpp',
        'src/moc_VisionResponse.cpp',

        'src/TSEntityManager.cpp',
        'src/TSResourceManager.cpp',

        'src/TuumUI.cpp',

        'src/rtxgui.cpp',
      ],

      'all_dependent_settings': {
        'include_dirs': ['inc']
      },

      'link_settings': {
        'libraries': [
            '<!@(pkg-config --libs Qt5Quick)',
            '<!@(pkg-config --libs assimp)',
        ]
      },
    },
  ],
}
