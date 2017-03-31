/usr/lib/x86_64-linux-gnu/qt5/bin/rcc -no-compress -name qml qml.qrc -o ../inc/qrc_qml.h
touch ../src/qtgui.cpp

moc ../inc/vstream/QSVideoRenderer.hpp -o ../src/vstream/moc_QSVideoRenderer.cpp
moc ../inc/vstream/VideoPlayer.hpp -o ../src/vstream/moc_VideoPlayer.cpp


moc ../inc/TuumScene.hpp -o ../src/moc_TuumScene.cpp
moc ../inc/VisionResponse.hpp -o ../src/moc_VisionResponse.cpp
