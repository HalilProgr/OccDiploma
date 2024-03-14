#ifndef DOCUMENT_COMMON_OVERVIEW_H
#define DOCUMENT_COMMON_OVERVIEW_H

#include <Standard_WarningsDisable.hxx>
#include <QObject>
#include <QList>
#include <Standard_WarningsRestore.hxx>

#include <AIS_InteractiveContext.hxx>
#include <V3d_Viewer.hxx>
#include <AIS_Manipulator.hxx>

class ApplicationCommonWindow;

//! Implements visualization of samples content
class DocumentCommon : public QObject
{
    Q_OBJECT
public:

    DocumentCommon(QWidget* );
    ~DocumentCommon();

    const Handle(AIS_InteractiveContext)& getContext() { return myContext; }

    const Handle(V3d_Viewer)& getViewer() { return myViewer; }

    void setViewer (const Handle(V3d_Viewer)& theViewer) { myViewer = theViewer; }
    void SetObjects(const NCollection_Vector<Handle(AIS_InteractiveObject)>& theObjects,
                    Standard_Boolean theDisplayShaded = Standard_False);
    void Clear();
    bool IsEmpty() const { return myContextIsEmpty; }

    void ActivateObject(Handle(AIS_InteractiveObject) object);
    void DeactivateObject();

    bool minupulateActive() { return ObjectActiveted;};

    void setMousePosition(int anXPix, int anYPix, Handle(V3d_View) view);
signals:
    void selectionChanged();
    void sendCloseDocument( DocumentCommon* );

private:
    bool isDynamicObject(Handle(AIS_InteractiveObject) object);

    Handle(V3d_Viewer) Viewer ();

private:
    Handle(AIS_Manipulator) myManipulator;

    Handle(V3d_Viewer)             myViewer;
    Handle(AIS_InteractiveContext) myContext;
    bool myContextIsEmpty;

    bool ObjectActiveted;
};

#endif
