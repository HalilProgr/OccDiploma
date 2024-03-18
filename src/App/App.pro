QT       +=widgets core gui opengl openglwidgets

TARGET = App
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
        algorithms/kinematic.cpp \
        common/RobotManipulator.cpp \
        data/CollectionShapes.cpp \
        data/Segment.cpp \
        data/Reader.cpp \
        main.cpp\
        mainwindow.cpp \
        common/DocumentCommon.cpp \
        view/OcctGlTools.cpp \
        view/OcctQtViewer.cpp

HEADERS  += \
        algorithms/ikinematic.h \
        algorithms/kinematic.h \
        data/CommonType.h \
        common/RobotManipulator.h \
        data/CollectionShapes.h \
        data/Segment.h \
        data/Reader.h \
        mainwindow.hpp \
        common/DocumentCommon.hpp \
        view/OcctGlTools.h \
        view/OcctQtViewer.h \
        common/Tools.h

RESOURCES += \ # папка, содержащая ресурсы проекта(иконки)
          .qrc \

include( ../../common.pri )
include( ./app.pri )

#LIBS += -lMyLib$${LIB_SUFFIX}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
