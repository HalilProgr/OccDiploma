#ifndef _VIEW_H_
#define _VIEW_H_

// подключение *.h файлов библиотеки Qt
#include <QOpenGLWidget>
#include <QRubberBand>
#include <QTimer>

// подключение *.h файлов библиотеки OpenCascade
#include <AIS_InteractiveContext.hxx>

#include <AIS_Shape.hxx>

#include <AIS_ViewController.hxx>
#include <ViewerTest_EventManager.hxx>

class QMenu;
class QRubberBand;

class View : public QWidget
{
    Q_OBJECT

public:
    // объявление перечисления описывающие состояние мыши
    enum CurrentAction3d
    {
        CurAction3d_Nothing,
        CurAction3d_DynamicZooming,
        CurAction3d_WindowZooming,
        CurAction3d_DynamicPanning,
        CurAction3d_GlobalPanning,
        CurAction3d_DynamicRotation
    };

public:
    View(QWidget* parent);

    const Handle(AIS_InteractiveContext)& getContext() const;
signals:
    void selectionChanged(void);

public slots:
    // операции для изменения представления
    void pan(void);
    void fitAll(void);
    void reset(void);
    void zoom(void);
    void rotate(void);

protected:
    virtual QPaintEngine* paintEngine() const;

    // Методы перерисовки и масштабирования
    virtual void paintEvent(QPaintEvent* theEvent);
    virtual void resizeEvent(QResizeEvent* theEvent);

    // Методы связанные с мышью
    virtual void mousePressEvent(QMouseEvent* theEvent);
    virtual void mouseReleaseEvent(QMouseEvent* theEvent);
    virtual void mouseMoveEvent(QMouseEvent * theEvent);
    virtual void wheelEvent(QWheelEvent * theEvent);

    // Методы связанные с нажатием на клавиши мыши
    virtual void onLButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onMButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onRButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onMouseWheel(const int theFlags, const QPoint theDelta, const QPointF thePoint);
    virtual void onLButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onMButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onRButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onMouseMove(const int theFlags, const QPoint thePoint);

protected:
    // Метод инициализации главного окна отображения
    void init(void);
    // Перемещение по методы осуществляющие перемешение по сцене
    void popup(const int x, const int y);
    void dragEvent(const int x, const int y);
    void inputEvent(const int x, const int y);
    void moveEvent(const int x, const int y);
    void multiMoveEvent(const int x, const int y);
    void multiDragEvent(const int x, const int y);
    void multiInputEvent(const int x, const int y);
    void drawRubberBand(const int minX, const int minY, const int maxX, const int maxY);
    void panByMiddleButton(const QPoint& thePoint);

private:
    void InitGeom();

    // V3d_Viewer - класс обёртка над графическим драйвером OpenGL
    Handle(V3d_Viewer) myViewer;

    // V3d_View - Окно просмотра созданное V3d_Viewer
    Handle(V3d_View) myView;

    // AIS_InteractiveContext - класс отвечающий за визуализацию 3D объектов
    Handle(AIS_InteractiveContext) myContext;

    // Standard_Integer переопределенное int в библиотеке OCCT
    // в переменных описанных ниже сохраняются координаты мыши
    Standard_Integer myXmin;
    Standard_Integer myYmin;
    Standard_Integer myZmin;
    Standard_Integer myXmax;
    Standard_Integer myYmax;
    Standard_Integer myZmax;

    // Переменная описывающаяю текущий режим мыши
    CurrentAction3d myCurrentMode;

    Standard_Boolean myDegenerateModeIsOn;

    // Объект отображающий прямоугольник выделения
    QRubberBand* myRectBand;

    gp_Pnt getTrueMousePosition(const QPoint point);
    gp_Pnt convert2DPointTo3DPoint(Standard_Real x, Standard_Real y, Handle(V3d_View) aView);

    Handle(AIS_ViewController) theViewCtrl;

};

#endif // _VIEW_H_
