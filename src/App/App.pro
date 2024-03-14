QT       +=widgets core gui opengl openglwidgets

TARGET = App
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
        algorithms/kinematic.cpp \
        data/CollectionShapes.cpp \
        data/reader.cpp \
        main.cpp\
        mainwindow.cpp \
        view/DocumentCommon.cpp \
        view/OcctGlTools.cpp \
        view/OcctQtViewer.cpp

HEADERS  += \
        algorithms/kinematic.h \
        data/CollectionShapes.hpp \
        data/reader.h \
        mainwindow.hpp \
        view/DocumentCommon.hpp \
        view/OcctGlTools.h \
        view/OcctQtViewer.h


FORMS    += mainwindow.ui

RESOURCES += \ # папка, содержащая ресурсы проекта(иконки)
          .qrc \

include( ../../common.pri )
include( ./app.pri )

#LIBS += -lMyLib$${LIB_SUFFIX}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
