#ifndef ANIMATION_H
#define ANIMATION_H

#include <AIS_AnimationObject.hxx>
#include <src/algorithms/Ikinematic.h>
#include "src/data/Manipulator.h"
#include "src/common/DocumentCommon.h"


namespace Lib
{
    namespace Common
    {
        ///
        /// \brief The Animation class
        ///
        class Animation
        {
        public:
            //void operator=(const Animation&) = delete;
            Animation(const Animation&) = delete;

            Animation() = default;
            ~Animation() = default;

            ///
            /// \brief Animation
            /// \param manipulator
            ///
            Animation(DocumentCommon* document, std::shared_ptr<Lib::Data::Manipulator> manipulator);

            ///
            /// \brief SetManipulator
            /// \param manipulator
            ///
            void SetManipulator(std::shared_ptr<Lib::Data::Manipulator> manipulator);

            ///
            /// \brief SetPoints
            /// \param points
            ///
            void SetPoints(const std::vector<gp_Trsf>& points);

            ///
            /// \brief Compute
            /// \return bool
            ///
            bool Compute();

            ///
            /// \brief Animation
            /// \return Handle (AIS_AnimationObject)
            ///
            Handle (AIS_Animation) GetAnimation() const;

        private:
            Handle (AIS_Animation) CreateAnimation(std::vector<std::shared_ptr<Data::Segment>> segments,
                                                   std::shared_ptr<Data::Tool> tool,
                                                   KDL::JntArray start,
                                                   KDL::JntArray end,
                                                   int count);


            Handle (AIS_Animation) _animation;
            std::vector<gp_Trsf> _points;
            std::shared_ptr<Lib::Data::Manipulator> _manipulator;
            std::shared_ptr<Lib::Kinematic::IKinematic> _modelKinematic;
            DocumentCommon* _document;
        };
}}

#endif // ANIMATION_H
