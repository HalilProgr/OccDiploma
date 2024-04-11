#include "Reader.h"
#include <QJsonArray>
#include <QJsonObject>
#include <AIS_Shape.hxx>
#include <Prs3d_LineAspect.hxx>
#include "src/common/Tools.h"


namespace App
{
namespace Common
{

Reader::Reader() : _loaded(false), _path("")
{}

Reader::Reader(std::string path) : _loaded(false), _path(QString::fromStdString(path))
{}

Reader::Reader(QDir path) : _loaded(false), _path(path)
{}

void Reader::SetPath(const std::string& path)
{
    _path.setPath(QString::fromStdString(path));
}
void Reader::SetPath(QDir path)
{
    _path.setPath(path.filesystemAbsolutePath());
}
void Reader::SetPath(char* path)
{
    _path.setPath(path);
}


    bool Reader::ParseData()
    {
        // step 1
        QFile file_obj(_path.filesystemAbsolutePath());
        if (!file_obj.open(QIODevice::ReadOnly))
        {
            std::cout << "Failed to open " << _path.filesystemAbsolutePath() << std::endl;
            return false;
        }

        // step 2
        QTextStream file_text(&file_obj);
        QString json_string;
        json_string = file_text.readAll();
        file_obj.close();
        QByteArray json_bytes = json_string.toLocal8Bit();

        // step 3
        auto json_doc = QJsonDocument::fromJson(json_bytes);

        if (json_doc.isNull()) {
            std::cout << "Failed to create JSON doc." << std::endl;
            return false;
        }
        if (!json_doc.isArray()) {
            std::cout << "JSON is not an object." << std::endl;
            return false;
        }

        QJsonArray json_array = json_doc.array();

        if (json_array.isEmpty()) {
            std::cout << "JSON object is empty." << std::endl;
            return false;
        }

        // step 4

        _actualSegments.clear();
        for (auto val : json_array)
        {
            if(val.isObject())
                _actualSegments.push_back(ParseSegment(val.toObject()));
            else
            {
                return false;
            }
        }

        _loaded = true;
        return true;
    }

    std::shared_ptr<Data::Segment> Reader::ParseSegment(const QJsonObject& value)
    {
        std::shared_ptr<Data::Segment> segment = std::make_shared<Data::Segment>();

        segment->SetShapes(ReadFile(value["src"].toString()));
        segment->SetMode(App::Tools::Mode(value["mode"].toString()));
        segment->SetLimitations(Data::Segment::Limit(
            value["jointInit"].toDouble(),
            value["jointMax"].toDouble(),
            value["jointMin"].toDouble()));
        segment->SetAxis(App::Tools::Axis(
            gp_Pnt(value["localx"].toDouble(), value["localy"].toDouble(), value["localz"].toDouble()),
            App::Tools::Mode(value["mode"].toString())));

        return segment;
    }

    std::vector<Handle (AIS_InteractiveObject)> Reader::ReadFile(const QString theStepName)
    {
        std::vector<Handle (AIS_InteractiveObject)> res;

        STEPCAFControl_Controller::Init();

        Handle(TDocStd_Document) aDoc;
        Handle(XCAFApp_Application) anApp = XCAFApp_Application::GetApplication();
        anApp->NewDocument("MDTV-XCAF", aDoc);

        STEPCAFControl_Reader aStepReader;
        aStepReader.SetColorMode(true);
        aStepReader.SetNameMode(true);
        aStepReader.SetLayerMode(true);
        aStepReader.SetPropsMode(true);

        aStepReader.ReadFile (theStepName.toStdString().c_str());
        aStepReader.Transfer (aDoc);

        TDF_Label aRootLabel = aDoc->Main();

        Handle( XCAFDoc_ShapeTool) aShapeTool = XCAFDoc_DocumentTool::ShapeTool(aRootLabel);
        Handle( XCAFDoc_ColorTool) aColorTool = XCAFDoc_DocumentTool::ColorTool(aRootLabel);

        Visit(aShapeTool, aColorTool, aRootLabel, res);

        return res;
    }

    void Reader::Visit(Handle( XCAFDoc_ShapeTool) aShapeTool, Handle( XCAFDoc_ColorTool) aColorTool,
                       const TDF_Label& theLabel, std::vector<Handle (AIS_InteractiveObject)>& outVec)
    {
        Quantity_Color aColor;
        TopoDS_Shape aShape;

        bool skip=0;
        if (aShapeTool->IsShape(theLabel))
        {
            //TopoDS_Shape aShape;
            //if a shape can be made from the current label, look for the color of that label:
            if(aShapeTool->GetShape(theLabel, aShape)){
                //std::cout<<"made a shape"<<std::endl;

                if(aColorTool->GetColor(aShape,XCAFDoc_ColorSurf,aColor)){
                    skip=0;
                } else { skip=1;}

            }

            if(skip==0)
            {
                outVec.push_back(new AIS_Shape(aShape));
                outVec.back()->SetColor(aColor);
                outVec.back()->SetDisplayMode(AIS_Shaded);
                outVec.back()->Attributes()->SetFaceBoundaryDraw(true);
                outVec.back()->Attributes()->SetFaceBoundaryAspect(
                    new Prs3d_LineAspect(Quantity_NOC_BLACK, Aspect_TOL_SOLID, 1.));
                outVec.back()->Attributes()->SetIsoOnTriangulation(true);
            }
        }

        for (TDF_ChildIterator c(theLabel); c.More(); c.Next())
            Visit(aShapeTool, aColorTool, c.Value(), outVec);
    }

}}
