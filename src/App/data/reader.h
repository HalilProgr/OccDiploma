#ifndef READER_H
#define READER_H

#include <AIS_InteractiveObject.hxx>

class Reader
{
public:
    Reader();

    Handle(AIS_InteractiveObject) Readstepfile(const std::string& theStepName);
};

#endif // READER_H
