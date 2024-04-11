DESTDIR = $${BIN_PATH}/

# include headers
INCLUDEPATH += $${IMPORT_PATH}/OpenCASCADE-7.7.0-vc14-64/opencascade-7.7.0/inc
INCLUDEPATH += $${IMPORT_PATH}/orocos_kdl/include
INCLUDEPATH += $${IMPORT_PATH}/Eigen3/include/eigen3
# include path to myLib
INCLUDEPATH += $${LIB_PATH}


#include libs path
LIBS += -L$${IMPORT_PATH}/OpenCASCADE-7.7.0-vc14-64/opencascade-7.7.0/win64/vc14/lib/ #Подключение пути до статических библиотек *.lib Open Cascade
LIBS += -L$${IMPORT_PATH}/orocos_kdl/lib/
LIBS += -L$${LIBS_PATH}/

# MyLib
LIBS += -lMyLib$${LIB_SUFFIX}

# KDL
LIBS += -lorocos-kdl$${LIB_SUFFIX}

#opengl32 and user32 libs required respectively for wglGetCurrentDC() and WindowFromDC()
win32:LIBS += -lopengl32 -luser32

# Opencascade
LIBS += -lTKGeomAlgo  -lTKHLR -lTKShHealing -lTKMesh
LIBS += -lTKPrim
LIBS += -lTKernel -lTKMath -lTKTopAlgo -lTKService
LIBS += -lTKG2d -lTKG3d -lTKV3d -lTKOpenGl
LIBS += -lTKBRep -lTKXSBase -lTKGeomBase
LIBS += -lTKMeshVS -lTKXSDRAW
LIBS += -lTKLCAF -lTKXCAF -lTKCAF
LIBS += -lTKCDF -lTKBin -lTKBinL -lTKBinXCAF -lTKXml -lTKXmlL -lTKXmlXCAF
# -- IGES support
LIBS += -lTKIGES
# -- STEP support
LIBS += -lTKSTEP -lTKXDESTEP -lTKXDEIGES
# -- STL support
LIBS += -lTKSTL
# -- OBJ/glTF support

linux-g++: QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/../../lib.$${OS_SUFFIX}/
