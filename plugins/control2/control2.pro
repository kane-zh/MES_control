TEMPLATE        = lib           #表示这个makefile是一个lib的makefile
CONFIG         += plugin        #应用程序是一个插件
CONFIG         += resources_big
QT             += widgets
QT             += core gui quick quickwidgets
TARGET          = control2      #生成插件的名称
win32:CONFIG(release, debug|release): DESTDIR         = ../../mainApp/release/plugins/libs
else:win32:CONFIG(debug, debug|release): DESTDIR         = ../../mainApp/debug/plugins/libs
else:unix: DESTDIR         = ../../mainApp/plugins/libs


DISTFILES += \
    AppleTVInput.qml \
    FpsDisplay.qml \
    HomeForm.ui.qml \
    InfoSheet.qml \
    Page1Form.ui.qml \
    Page2Form.ui.qml \
    Planet.qml \
    PlanetButton.qml \
    PlanetEffect.qml \
    PlanetFrameGraph.qml \
    PlanetMaterial.qml \
    PlanetsLight.qml \
    PlanetsMain.qml \
    Ring.qml \
    ShadowEffect.qml \
    SolarSystem.qml \
    StyledSlider.qml \
    SunEffect.qml \
    images/earth.png \
    images/jupiter.png \
    images/mars.png \
    images/mercury.png \
    images/neptune.png \
    images/saturn.png \
    images/sun.png \
    images/uranus.png \
    images/venus.png \
    main.qml \
    meshes/ring.obj \
    meshes/starfield.obj \
    planets.js \
    programmer.json

FORMS += \
    mainform.ui

HEADERS += \
    mainform.h

SOURCES += \
    mainform.cpp

RESOURCES += \
    123.qrc \
    123.qrc \
    planets-qml-images.qrc \
    planets-qml.qrc

SUBDIRS += \
    planets-qml.pro
