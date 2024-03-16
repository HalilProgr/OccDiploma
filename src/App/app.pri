CASROOT = $${IMPORT_PATH}/OpenCASCADE-7.7.0-vc14-64/opencascade-7.7.0
DESTDIR = $${BIN_PATH}/

INCLUDEPATH += $${CASROOT}/inc #Подключение *.h файлов библиотеки Open Cascade
INCLUDEPATH += $${IMPORT_PATH}/orocos_kdl/include
INCLUDEPATH += $${IMPORT_PATH}/Eigen3/include/eigen3
LIBS += -L$${CASROOT}/win64/vc14/lib/ #Подключение пути до статических библиотек *.lib Open Cascade


LIBS += -L&&{IMPORT_PATH}/orocos_kdl/lib



LIBS += -lC:/Qtprogect/OccDiploma/import/orocos_kdl/lib/orocos-kdl
#LIBS += -lC:/Qtprogect/OccDiploma/import/orocos_kdl/lib/orocos-kdld

# opengl32 and user32 libs required respectively for wglGetCurrentDC() and WindowFromDC()
win32:LIBS += -lopengl32 -luser32

LIBS += -lTKernel -lTKGeomBase -lTKGeomAlgo -lTKG2d -lTKV3d -lTKG3d  -lTKHLR -lTKService -lTKMath -lTKBRep -lTKTopAlgo -lTKOpenGl -lTKPrim -lTKShHealing -lTKMesh
LIBS += -lTKPrim
LIBS += -lTKernel -lTKMath -lTKTopAlgo -lTKService
LIBS += -lTKG2d -lTKG3d -lTKV3d -lTKOpenGl
LIBS += -lTKBRep -lTKXSBase -lTKGeomBase
LIBS += -lTKMeshVS -lTKXSDRAW
LIBS += -lTKLCAF -lTKXCAF -lTKCAF
LIBS += -lTKCDF -lTKBin -lTKBinL -lTKBinXCAF -lTKXml -lTKXmlL -lTKXmlXCAF

# Opencascade
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
