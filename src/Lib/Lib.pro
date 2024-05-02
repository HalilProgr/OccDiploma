include( ../../common.pri )
include( ./lib.pri )


QT += core gui widgets core gui opengl openglwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
# название файла нашей статической библиотеки

TARGET = MyLib$${LIB_SUFFIX}
TEMPLATE = lib
CONFIG += staticlib

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
            src/algorithms/kinematic.cpp \
            src/common/Reader.cpp \
            src/common/RobotManipulator.cpp \
            src/common/Tools.cpp \
            src/data/Manipulator.cpp \
            src/data/Segment.cpp \
            src/common/DocumentCommon.cpp \
            src/data/Tool.cpp \
            src/view/OcctGlTools.cpp \
            src/view/OcctQtViewer.cpp

HEADERS += \
           src/algorithms/ikinematic.h \
           src/algorithms/kinematic.h \
           src/common/DocumentCommon.h \
           src/common/Reader.h \
           src/data/CommonType.h \
           src/common/RobotManipulator.h \
           src/data/Manipulator.h \
           src/data/Segment.h \
           src/data/Tool.h \
           src/view/OcctGlTools.h \
           src/view/OcctQtViewer.h \
           src/common/Tools.h

win32 {
    QMAKE_TARGET_PRODUCT = My Lib
    QMAKE_TARGET_DESCRIPTION = It is my library
}



