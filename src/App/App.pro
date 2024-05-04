include( ../../common.pri )
include( ./app.pri )

QT       +=widgets core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = App
TEMPLATE = app

SOURCES += \
        main.cpp\
        mainwindow.cpp

HEADERS  += \
        $${LIB_PATH}/src/algorithms/ikinematic.h \
        $${LIB_PATH}/src/algorithms/kinematic.h \
        $${LIB_PATH}/src/data/CommonType.h \
        $${LIB_PATH}/src/data/Segment.h \
        $${LIB_PATH}/src/common/RobotManipulator.h \
        $${LIB_PATH}/src/common/DocumentCommon.h \
        $${LIB_PATH}/src/common/Tools.h \
        $${LIB_PATH}/src/common/Reader.h \
        $${LIB_PATH}/src/view/OcctGlTools.h \
        $${LIB_PATH}/src/view/OcctQtViewer.h \
        mainwindow.h

#RESOURCES += \ # папка, содержащая ресурсы проекта(иконки)
#         .qrc \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
