#ifndef _OcctQtViewer_HeaderFile
#define _OcctQtViewer_HeaderFile

#include <Standard_WarningsDisable.hxx>
#include <QOpenGLWidget>

#include <AIS_InteractiveContext.hxx>
#include <AIS_ViewController.hxx>
#include <V3d_View.hxx>

#include "src/common/DocumentCommon.h"

class AIS_ViewCube;

//! OCCT 3D View.
class OcctQtViewer : public QOpenGLWidget, public AIS_ViewController
{
    Q_OBJECT
public:

    //! Main constructor.
    OcctQtViewer (Lib::Common::DocumentCommon* document, QWidget* theParent = nullptr);

    //! Destructor.
    virtual ~OcctQtViewer();

    //! Return View.
    const Handle(V3d_View)& View() const { return myView; }

    //! Return OpenGL info.
    const QString& getGlInfo() const { return myGlInfo; }

    //! Minial widget size.
    virtual QSize minimumSizeHint() const override { return QSize(720, 480); }

    //! Default widget size.
    virtual QSize sizeHint()        const override { return QSize(720, 480); }

public:

    //! Handle subview focus change.
    virtual void OnSubviewChanged(const Handle(AIS_InteractiveContext)&,
                                  const Handle(V3d_View)&,
                                  const Handle(V3d_View)& theNewView) override;

protected: // OpenGL events

    virtual void initializeGL() override;
    virtual void paintGL() override;

protected: // user input events

    virtual void closeEvent       (QCloseEvent*  theEvent) override;
    virtual void keyPressEvent    (QKeyEvent*    theEvent) override;
    virtual void mousePressEvent  (QMouseEvent*  theEvent) override;
    virtual void mouseReleaseEvent(QMouseEvent*  theEvent) override;
    virtual void mouseMoveEvent   (QMouseEvent*  theEvent) override;
    virtual void wheelEvent       (QWheelEvent*  theEvent) override;

private:

    //! Dump OpenGL info.
    void dumpGlInfo (bool theIsBasic, bool theToPrint);

    //! Request widget paintGL() event.
    void updateView();

    //! Handle view redraw.
    virtual void handleViewRedraw (const Handle(AIS_InteractiveContext)& theCtx,
                                  const Handle(V3d_View)& theView) override;

private:
    Lib::Common::DocumentCommon* document;

    Handle(V3d_View)               myView;
    Handle(AIS_ViewCube)           myViewCube;

    Handle(V3d_View)               myFocusView;

    Handle(AIS_InteractiveObject) m_Selected;

    QString myGlInfo;
    bool myIsCoreProfile;
};

#endif // _OcctQtViewer_HeaderFile
