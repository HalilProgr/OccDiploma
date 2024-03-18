#ifndef COLLECTIONSHAPES_H
#define COLLECTIONSHAPES_H

#include "Segment.h"
#include "algorithms/kinematic.h"
// временные #include для загрузки объектов. Далее метод init будет удалён.
#include <STEPCAFControl_Reader.hxx>
#include <STEPCAFControl_Controller.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <AIS_Shape.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <TDF_ChildIterator.hxx>
#include <Prs3d_LineAspect.hxx>
//


/// TODO
/// возможно следует сделать класс шаблонным, где параметр шаблона - класс реализующий кинематику,
/// пока не решил
///
/// Внесём несколько уточнений, под звеньями будем считать все звеняь отличные от базы!
/// База то неподвижна - вот и нечего её дергать
///
///
namespace App
{
    namespace Data
    {

    ///
    /// \brief CollectionShapes класс реализующий связь между AIS моделями и математичкской моделью.
    /// Является контейниром AIS_Shape и их положения. Адаптер между кинемтаикой и отображением.
    ///
    class CollectionShapes
    {
    public:
        CollectionShapes();

        ///
        /// \brief Временный метод.
        ///  уёдет в ридер.
        Handle( XCAFDoc_ColorTool) aColorTool;
        Handle( XCAFDoc_ShapeTool) aShapeTool;
        ///
        void Init();
        std::vector<Handle (AIS_InteractiveObject)> ReadFile(const std::string& theStepName);
        void Visit(const TDF_Label& theLabel, std::vector<Handle (AIS_InteractiveObject)>& outVec); // Почему такое название ?!
        ///
        ///

        std::vector<std::shared_ptr<Segment>>& GetSegmnets() {return _data;}
        std::shared_ptr<Segment> GetSegment(Handle(AIS_InteractiveObject) obj);

        ///
        /// \brief SetPosition
        /// \param pos
        ///
        void MoveTCP(gp_Trsf pos);
        void MoveSegment(int numberSegment, double newPosition);

        ///
        /// \brief SetSegments
        /// \param segments
        ///
        void SetSegments(std::vector<Segment> segments);
        //void AddSegment(Handle(AIS_InteractiveObject)& AISsegment, gp_Trsf coordsys);
        //void AddSegment(Segment& segment);

        ///
        /// \brief IsEmpty
        /// \return
        ///
        bool IsEmpty();

    private:
        void SetActualPosition();

    private:
        bool _displaed;

        Kinematic::kinematic _kinematic; // кинематика робота.
        std::vector<std::shared_ptr<Segment>> _data; // думаю лучше хранить сегменты в shared_ptr
         // удобное отображение элементов в OCC формате, необходимо для отображения элементов в Document
        NCollection_Vector<Handle(AIS_InteractiveObject)> _view;

        gp_Trsf _basePosition;
    };

}}

#endif // COLLECTIONSHAPES_H
