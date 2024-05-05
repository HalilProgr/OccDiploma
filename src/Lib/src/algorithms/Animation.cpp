#include "animation.h"
#include <src/common/Tools.h>


namespace Lib
{
    namespace Common
    {

        Animation::Animation(DocumentCommon* document, std::shared_ptr<Lib::Data::Manipulator> manipulator)
        {
            _document = document;
            _manipulator = manipulator;
            _modelKinematic = manipulator->GetKinematicModel();
        }

        void Animation::SetManipulator(std::shared_ptr<Data::Manipulator> manipulator)
        {
            _manipulator = manipulator;
            _modelKinematic = manipulator->GetKinematicModel();
        }

        void Animation::SetPoints(const std::vector<gp_Trsf> &points)
        {
            _points = points;
        }

        bool Animation::Compute()
        {
            /// _animation
            if (_manipulator.get() == nullptr) return false;

            _animation = new AIS_Animation("res");
            auto segments = _manipulator->GetSegmnets();
            auto tool = _manipulator->GetTool();

            KDL::JntArray initJoins = _modelKinematic->GetPosition();
            KDL::JntArray startJoins = initJoins;
            int countFrame = 0;
            for (auto& pnt : _points)
            {
                if (!_modelKinematic->MoveTCP(Lib::Tools::TrsfToFrame(pnt)))
                {
                    _modelKinematic->SetPosition(initJoins);
                    return false;
                }

                KDL::JntArray endJoins = _modelKinematic->GetPosition();
                _animation->Add(CreateAnimation(segments, tool, startJoins, endJoins, countFrame));
                startJoins = endJoins;
                countFrame++;
            }

            return true;
        }

        Handle (AIS_Animation) Animation::GetAnimation() const
        {
            return _animation;
        }

        Handle (AIS_Animation) Animation::CreateAnimation(std::vector<std::shared_ptr<Data::Segment>> segments,
                                                          std::shared_ptr<Data::Tool> tool,
                                                          KDL::JntArray start,
                                                          KDL::JntArray end,
                                                          int count)
        {
            Handle (AIS_Animation) resAnim = new AIS_Animation(count);

            int countSegmentsKinematic = 0;
            gp_Trsf startPosSegments;
            gp_Trsf endPosSegments;

            /// каждый сегмент.
            for (auto& segment : segments)
            {
                /// пропустить построение анимации для базы
                if (segment->GetMode() == Lib::Data::Mode::base )
                {
                    startPosSegments = segment->GetTransform();
                    endPosSegments = startPosSegments;
                    continue;
                }

                /// Если это не база найдём начльное положение сегмента и конечное.
                gp_Trsf tempStart = segment->GetTransform();
                gp_Trsf tempEnd = tempStart;

                tempStart.SetRotation(segment->GetAxis(), start(countSegmentsKinematic));
                tempEnd.SetRotation(segment->GetAxis(), end(countSegmentsKinematic));
                startPosSegments.Multiply(tempStart);
                endPosSegments.Multiply(tempEnd);

                /// для каждой AIS фигуры создаём AIS_AnimationObject
                for(auto& AISShape: segment->GetAISShapes())
                {
                    Handle(AIS_AnimationObject) frame = new AIS_AnimationObject(
                                                            count,
                                                            _document->GetContext(),
                                                            AISShape,
                                                            startPosSegments,
                                                            endPosSegments);
                    resAnim->Add(frame);
                }

                countSegmentsKinematic++;
            }

            /// сощдаём AIS_AIS_AnimationObject для Tool
            gp_Trsf tempTool = tool->GetTransform();
            startPosSegments.Multiply(tempTool);
            endPosSegments.Multiply(tempTool);

            Handle(AIS_AnimationObject) frame = new AIS_AnimationObject(
                count,
                _document->GetContext(),
                tool->GetAISShape(),
                startPosSegments,
                endPosSegments);

            resAnim->Add(frame);

            /// Настраиваем весь фрэйм со всмеи кадрами перемещения каждого AIS_Shape.
            resAnim->SetOwnDuration(0.05);
            resAnim->SetStartPts(count);

            return resAnim;
        }
}}
