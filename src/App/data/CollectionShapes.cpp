#include "CollectionShapes.hpp"


int CollectionShapes::find(opencascade::handle<AIS_InteractiveObject>& object)
{
    int count = 0;
    for(auto iter = begin(); iter != end(); iter++, count++)
        if(*iter == object)
            return count;

    return -1;
}
