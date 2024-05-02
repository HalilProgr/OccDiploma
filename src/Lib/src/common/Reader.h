#ifndef READER_H
#define READER_H

#include <vector>
#include <string>
#include <memory>
#include <QDir>
#include <QJsonDocument>
#include <gp_Pnt.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include "src/data/Segment.h"
#include "src/data/Tool.h"


namespace Lib
{
    namespace Common
    {

    ///
    /// \brief Класс считывает Json файл с описанием сегментов звеньев робота.
    ///
    class Reader
    {
    public:
        Reader();
        Reader(std::string path);
        Reader(QDir path);

        ~Reader() = default;

        ///
        /// \brief SetPath
        /// \param path
        ///
        void SetPath(const std::string& path);
        void SetPath(QDir path);
        void SetPath(char* path);

        ///
        /// \brief Empty
        /// \return
        ///
        bool Empty() const;

        ///
        /// \brief ParseData
        /// \return
        ///
        bool ParseData();

        ///
        /// \brief GetSegments
        /// \return
        ///
        std::vector<std::shared_ptr<Data::Segment>> GetSegments();
        std::shared_ptr<Lib::Data::Tool> GetTool();

    private:
        // Методы для чтения Step файлов.
        std::shared_ptr<Data::Segment> ParseSegment(const QJsonObject& value, gp_Pnt& latsPos);
        std::shared_ptr<Data::Tool> ParseTool(const QJsonObject& value, gp_Pnt& lastPoint);
        std::vector<Handle (AIS_InteractiveObject)> ReadFile(const QString theStepName);
        void Visit( Handle( XCAFDoc_ShapeTool) aShapeTool, Handle( XCAFDoc_ColorTool) aColorTool,
                   const TDF_Label& theLabel, std::vector<Handle (AIS_InteractiveObject)>& outVec);

    private:
        bool _loaded;
        QDir _path;
        std::vector<std::shared_ptr<Data::Segment>> _actualSegments;
        std::shared_ptr<Lib::Data::Tool>_actualTool;
    };
}}

#endif // READER_H
