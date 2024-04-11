#ifndef DOCUMENT_COMMON_OVERVIEW_H
#define DOCUMENT_COMMON_OVERVIEW_H

#include <Standard_WarningsDisable.hxx>
#include <QObject>
#include <QList>
#include <Standard_WarningsRestore.hxx>

#include <AIS_InteractiveContext.hxx>
#include <V3d_Viewer.hxx>
// Тест манипулятора написанного мною ! //
#include <src/common/RobotManipulator.h>
#include <src/data/CollectionShapes.h>


namespace App
{
    namespace Common
    {

    //! Implements visualization of samples content
    class DocumentCommon : public QObject
    {
        Q_OBJECT
    public:

        DocumentCommon(QWidget* );
        ~DocumentCommon();

        const Handle(AIS_InteractiveContext)& GetContext();
        const Handle(V3d_Viewer)& GetViewer();

        void SetViewer (const Handle(V3d_Viewer)& theViewer) { myViewer = theViewer; }

        ///
        /// \brief Clear
        ///
        void Clear();
        bool IsEmpty() const { return myContextIsEmpty; }

        void DislayAll();

        ///
        /// \brief ActivateManipulator
        /// \param obj
        ///
        void ActivateManipulator(Handle(AIS_InteractiveObject) object);
        void DeactivateManipulator();

        void AddDynamicObjects(std::shared_ptr<App::Data::CollectionShapes> collection);

    private:
        Handle(V3d_Viewer) Viewer ();

    public:
        Handle(App::Common::RobotManipulator) myManipulator;

    private:
        std::vector<std::shared_ptr<App::Data::CollectionShapes>> dynamicObjects;

        Handle(V3d_Viewer)             myViewer;
        Handle(AIS_InteractiveContext) myContext;

        bool myContextIsEmpty;
        bool ObjectActiveted;
    };

}}

#endif