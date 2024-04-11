#ifndef SEGMENT_H
#define SEGMENT_H

#include <AIS_InteractiveObject.hxx>
#include <gp_Trsf.hxx>
#include "src/data/CommonType.h"
///
/// Нумерация сегментов начинается от -1 до N.
/// -1 номер базы.
/// 0-N номера сегментов.
///

namespace App
{
    namespace Data
    {

    ///
    /// \brief The Mode enum
    ///

    ///
    /// \brief Класс полноностью описывающий звено
    ///
    class Segment
    {
    public:

        struct Limit
        {
            Limit(double init = 0, double max = 0, double min = 0) :
                init(init),max(max),min(min){}

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
                Limit limit = Limit(),
                int numberSegment = -1);

        ///
        /// \brief IsPart
        ///
        bool IsPart (Handle(AIS_InteractiveObject)& object);

        // Сетеры
        void SetShapes (std::vector<Handle (AIS_InteractiveObject)> shapes);
        void SetTransform (gp_Trsf& newPos);
        void SetMode (Mode newMode);
        void SetAxis (gp_Ax1 sys);
        void SetLimitations(Limit limit);

        // Гетеры
        Mode GetMode();
        gp_Ax1 GetAxis();
        Limit GetLimitations();
        std::vector<Handle (AIS_InteractiveObject)>& GetAISShapes();

        void AddAISShape (Handle (AIS_InteractiveObject) shape);

    private:
        std::vector<Handle (AIS_InteractiveObject)> _objects; // Сегмент может состояить из нескольких объектов
        gp_Ax1 _coorsys;
        Mode _mode;
        Limit _limit;
    };

}}

#endif // SEGMENT_H
