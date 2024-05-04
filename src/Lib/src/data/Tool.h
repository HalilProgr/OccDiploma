#ifndef TOOL_H
#define TOOL_H
#include "src/data/CommonType.h"
#include <AIS_InteractiveObject.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax2.hxx>


namespace Lib
{
    namespace Data
    {

        class Tool
        {
        public:

            Tool(gp_Pnt startPnt, gp_Ax2 axis,
                 double toollenght, double coneheight,
                 double conebottomdiameter,double conetopdiameter);

            ///
            /// \brief IsPart
            ///
            bool IsPart (Handle(AIS_InteractiveObject)& object);

            /// Сеттеры
            void SetTCP (gp_Trsf& newTCP);
            void SetTCP (gp_Trsf newTCP);
            void SetTransform(gp_Trsf& newPosition);

            /// Гетеры
            Mode GetMode();
            gp_Trsf GetTCP();
            gp_Trsf GetTransform();
            gp_Ax1 GetAxis();
            Handle (AIS_InteractiveObject) GetAISShape();

        private:
            Handle (AIS_InteractiveObject) _object; // Сегмент может состояить из нескольких объектов
            gp_Trsf _currentPosition;
            gp_Trsf _currentTCP;
            gp_Ax1 _axis;
            Mode _mode;
        };

}}

#endif // TOOL_H
