TEMPLATE = subdirs

SUBDIRS += \
    src/MyApp \
    src/MyLib


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/src/EventManager.cpp \
    src/src/myaisshape.cpp \
    src/src/main.cpp \
    src/src/mainwindow.cpp \
    src/src/myaisowner.cpp \
    src/src/view.cpp

HEADERS += \
    src/inc/EventManager.h \
    src/inc/mainwindow.h \
    src/inc/myaisshape.h \
    src/inc/view.h \
    src/inc/ABC.h \
    src/inc/myaisowner.h

FORMS += \
    mainwindow.ui

RESOURCES += \ # папка, содержащая ресурсы проекта(иконки)
    .qrc \
    .qrc

CASROOT = "C:\\OpenCASCADE-7.7.0-vc14-64\\opencascade-7.7.0"

INCLUDEPATH += $${CASROOT}/inc #Подключение *.h файлов библиотеки Open Cascade
LIBS += -L$${CASROOT}/win64/vc14/lib/ #Подключение пути до статических библиотек *.lib Open Cascade

#Подключение библиотек Open Cascade перечисленных ниже


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
