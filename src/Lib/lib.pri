DESTDIR = $${LIBS_PATH}/
win32: DLLDESTDIR = $${BIN_PATH}/
VERSION = 0.0.1

INCLUDEPATH += $${IMPORT_PATH}/OpenCASCADE-7.7.0-vc14-64/opencascade-7.7.0/inc #Подключение *.h файлов библиотеки Open Cascade
INCLUDEPATH += $${IMPORT_PATH}/orocos_kdl/include
INCLUDEPATH += $${IMPORT_PATH}/Eigen3/include/eigen3
