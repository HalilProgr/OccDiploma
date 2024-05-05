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

        ///
        /// \brief The Tool class
        ///
        class Tool
        {
        public:
            ///
            /// \brief Tool
            /// \param startPnt
            /// \param axis
            /// \param toollenght
            /// \param coneheight
            /// \param conebottomdiameter
            /// \param conetopdiameter
            ///
            Tool(gp_Pnt startPnt, gp_Ax2 axis,
                 double toollenght, double coneheight,
                 double conebottomdiameter,double conetopdiameter);

            ///
            /// \brief IsPart
            /// \param conetopdiameter
            ///
            bool IsPart (Handle(AIS_InteractiveObject)& object);

            ///
            /// \brief SetTCP
            /// \param newTCP
            ///
            void SetTCP (const gp_Trsf& newTCP);

            ///
            /// \brief SetTransform
            /// \param newPosition
            ///
            void SetTransform(const gp_Trsf& newPosition);

            ///
            /// \brief GetMode
            /// \return
            ///
            Mode GetMode() const;

            ///
            /// \brief GetTCP
            /// \return
            ///
            gp_Trsf GetTCP() const;

            ///
            /// \brief GetTransform
            /// \return
            ///
            gp_Trsf GetTransform() const;

            ///
            /// \brief GetAxis
            /// \return
            ///
            gp_Ax1 GetAxis() const;

            ///
            /// \brief GetAxis
            /// \return
            ///
            Handle (AIS_InteractiveObject) GetAISShape() const;

        private:
            Handle (AIS_InteractiveObject) _object; // Сегмент может состояить из нескольких объектов
            gp_Trsf _currentPosition;
            gp_Trsf _currentTCP;
            gp_Ax1 _axis;
            Mode _mode;
        };

}}

#endif // TOOL_H
