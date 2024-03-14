#ifndef COLLECTIONSHAPES_H
#define COLLECTIONSHAPES_H

#include <AIS_InteractiveObject.hxx>

///
/// \brief Класс реализующий связь между AIS моделями и математичкской моделью.
/// ОЯвляется контейниром
///
class CollectionShapes : public NCollection_Vector<Handle(AIS_InteractiveObject)>
{
public:

    struct Segment
    {
        Handle(AIS_InteractiveObject) object;
        gp_Trsf coorsys;
    };


    int find(Handle(AIS_InteractiveObject)& object);

    void AddSegment(Handle(AIS_InteractiveObject)& AISsegment, gp_Trsf coordsys);
    void AddSegment(Segment& segment);

    Segment& operator[](size_t i);


    operator NCollection_Vector<Handle(AIS_InteractiveObject)>();

private:
    std::vector<Handle(AIS_InteractiveObject)> _data;

};

#endif // COLLECTIONSHAPES_H
