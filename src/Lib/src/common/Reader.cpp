#include "Reader.h"
#include <QJsonArray>
#include <QJsonObject>
#include <AIS_Shape.hxx>
#include <Prs3d_LineAspect.hxx>
#include "src/common/Tools.h"


#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepAlgoAPI_Fuse.hxx>

#include <BRepTools_ReShape.hxx>
#include <BRepTools.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <gp_Trsf.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopExp.hxx>

#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Compound.hxx>
#include <TPrsStd_AISPresentation.hxx>
#include <STEPControl_Writer.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <STEPCAFControl_Controller.hxx>
#include <Geom_Line.hxx>

#include <Geom_Circle.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_CartesianPoint.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeArcOfEllipse.hxx>
#include <GC_MakeCircle.hxx>
#include <GC_MakeEllipse.hxx>
#include <GC_MakeSegment.hxx>

#include <gce_MakeRotation.hxx>
#include <TopExp.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Edge.hxx>
#include <TDocStd_Document.hxx>

#include <TDocStd_Application.hxx>
#include <TDF_Label.hxx>
#include <TDF_AttributeIterator.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_LabelSequence.hxx>
#include <TDataStd_Name.hxx>

#include <TDataStd_TreeNode.hxx>
#include <TDataStd_UAttribute.hxx>
#include <TNaming_NamedShape.hxx>
#include <TopTools.hxx>
#include <Geom_Plane.hxx>
#include <Geom_CylindricalSurface.hxx>

#include <GCE2d_MakeSegment.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <XCAFDoc_Area.hxx>
#include <XCAFDoc_Centroid.hxx>
#include <XCAFDoc_Datum.hxx>
#include <XCAFDoc_Dimension.hxx>
#include <XCAFDoc_Location.hxx>
#include <XCAFDoc_Volume.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <Quantity_Color.hxx>
#include <Quantity_ColorRGBA.hxx>

#include "XCAFPrs_DocumentExplorer.hxx"
#include <TDataStd_Name.hxx>
#include <XCAFDoc_AssemblyItemId.hxx>
#include <XCAFDoc_AssemblyItemRef.hxx>

#include <BRepTools.hxx>


namespace Lib::Common
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

    bool Reader::Empty() const
    {
        return _loaded;
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

        QJsonArray json_array = json_doc.array();

        if (json_array.isEmpty()) {
            std::cout << "JSON object is empty." << std::endl;
            return false;
        }

        // step 4
        _actualSegments.clear();
        gp_Pnt lastPos{0,0,0};
        for (auto val : json_array)
        {
            if(val.isObject())
            {
                QJsonObject obj = val.toObject();
                if (obj["mode"].toString() == "tool")
                {
                    _actualTool = ParseTool(obj,lastPos);
                    break;
                }
                _actualSegments.push_back(ParseSegment(obj, lastPos));
            }
            else
            {
                return false;
            }
        }

        _loaded = true;
        return true;
    }

    std::vector<std::shared_ptr<Data::Segment> > Reader::GetSegments()
    {
        return _actualSegments;
    }

    std::shared_ptr<Data::Tool> Reader::GetTool()
    {
        return _actualTool;
    }

    std::shared_ptr<Data::Segment> Reader::ParseSegment(const QJsonObject& value, gp_Pnt& lastPoint)
    {
        std::shared_ptr<Data::Segment> segment = std::make_shared<Data::Segment>();

        gp_Pnt localPosition;
        if (_actualSegments.empty())
        {
            localPosition.SetX(value["localx"].toDouble());
            localPosition.SetY(value["localy"].toDouble());
            localPosition.SetZ(value["localz"].toDouble());
        }
        else
        {
            localPosition.SetX(value["localx"].toDouble() + lastPoint.X());
            localPosition.SetY(value["localy"].toDouble() + lastPoint.Y());
            localPosition.SetZ(value["localz"].toDouble() + lastPoint.Z());
        }

        segment->SetGeom(value["localx"].toDouble(), value["localy"].toDouble(), value["localz"].toDouble());

        segment->SetShapes(ReadFile(value["src"].toString()));
        segment->SetMode(Lib::Tools::Mode(value["mode"].toString()));
        segment->SetLimitations(Lib::Data::Segment::Limit(
                                    value["jointInit"].toDouble(),
                                    value["jointMax"].toDouble(),
                                    value["jointMin"].toDouble())
                                );
        segment->SetAxis(
            Lib::Tools::Axis(
                lastPoint,
                Lib::Tools::Mode(value["mode"].toString()))
            );

        lastPoint = localPosition;

        return segment;
    }

    std::shared_ptr<Data::Tool> Reader::ParseTool(const QJsonObject &value, gp_Pnt &lastPoint)
    {
        double toollenght = value["toollenght"].toDouble();
        double coneheight = value["coneheight"].toDouble();
        double conebottomdiameter = value["conebottomdiameter"].toDouble();
        double conetopdiameter = value["conetopdiameter"].toDouble();

        qDebug() << lastPoint.X();
        qDebug() << lastPoint.Y();
        qDebug() << lastPoint.Z();

        gp_Pnt localPosition = {lastPoint.X() - toollenght, lastPoint.Y(), lastPoint.Z()};

        qDebug() << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
        qDebug() << localPosition.X();
        qDebug() << localPosition.Y();
        qDebug() << localPosition.Z();

        gp_Ax2 axisCon;
        if (value["dir"].toString() == "x")
        {
            gp_Dir xDirection(1,0,0); // Direction is auto normalized by occ.
            gp_Dir normalDirection(0,1,0);
            axisCon = gp_Ax2(localPosition, xDirection, normalDirection);
        }
        else if (value["dir"].toString() == "y")
        {
            gp_Dir xDirection(0,1,0); // Direction is auto normalized by occ.
            gp_Dir normalDirection(1,0,0);
            axisCon = gp_Ax2(localPosition, xDirection, normalDirection);
        }
        else if (value["dir"].toString() == "z")
        {
            gp_Dir xDirection(0,0,1); // Direction is auto normalized by occ.
            gp_Dir normalDirection(0,1,0);
            axisCon = gp_Ax2(localPosition, xDirection, normalDirection);
        }

        return std::make_shared<Data::Tool>(localPosition, axisCon,
                                            toollenght, coneheight,
                                            conebottomdiameter, conetopdiameter);
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

        bool skip = false;
        if (aShapeTool->IsShape(theLabel))
        {
            if(aShapeTool->GetShape(theLabel, aShape) && !aColorTool->GetColor(aShape,XCAFDoc_ColorSurf,aColor))
                skip = true;

            if(!skip)
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

}
