#ifndef SEGMENT_H
#define SEGMENT_H

#include <AIS_InteractiveObject.hxx>
#include <gp_Trsf.hxx>
#include <kdl/frames.hpp>
#include "src/data/CommonType.h"


namespace Lib
{
    namespace Data
    {
    ///
    /// \brief Класс полноностью описывающий звено
    ///
    class Segment
    {
    public:

        struct Limit
        {
            Limit(double init = 0, double max = 0, double min = 0) :
                init(init), max(max), min(min){}

            double init;
            double max;
            double min;
        };

        ///
        /// \brief Констурктор
        /// \param shapes Фигуры для инициализации сегмента
        /// \param crsys система координат звена
        /// \param mode режим звена
        ///
        Segment(std::vector<Handle (AIS_InteractiveObject)> shapes = {},
                gp_Pnt crsys = {},
                Mode mode = Mode::none,
                Limit limit = Limit());

        ///
        /// \brief IsPart
        ///
        bool IsPart (Handle(AIS_InteractiveObject)& object);

        // Сетеры
        void SetGeom(double x, double y, double z);
        void SetShapes (std::vector<Handle (AIS_InteractiveObject)> shapes);
        void SetTransform (gp_Trsf& newPos);
        void SetMode (Mode newMode);
        void SetAxis (gp_Ax1 sys);
        void SetLimitations(Limit limit);

        // Гетеры
        KDL::Vector GetGeom();
        Mode GetMode();
        gp_Ax1 GetAxis();
        Limit GetLimitations();
        gp_Trsf GetTransform();
        std::vector<Handle (AIS_InteractiveObject)> GetAISShapes();

        void AddAISShape (Handle (AIS_InteractiveObject) shape);

    private:
        KDL::Vector _geom;
        std::vector<Handle (AIS_InteractiveObject)> _objects; // Сегмент может состояить из нескольких объектов
        gp_Ax1 _coorsys;
        gp_Trsf _currentTransform;
        Mode _mode;
        Limit _limit;
    };

}}

#endif // SEGMENT_H
