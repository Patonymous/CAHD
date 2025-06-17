#include <iostream>
#include <string>
#include <filesystem>

#include <TopoDS_Shape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopAbs_ShapeEnum.hxx>

#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <STEPControl_Reader.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Solid.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <BRepClass3d_SolidClassifier.hxx>
#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>
#include <gp_Pnt.hxx>
#include <Standard_Boolean.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
using namespace std;


bool IsShellInside(const TopoDS_Shell& shellToTest, const TopoDS_Solid& solid)
{
    TopExp_Explorer exp;
    exp.Init(shellToTest, TopAbs_VERTEX);
    if (!exp.More()) return false;
    gp_Pnt testPoint = BRep_Tool::Pnt(TopoDS::Vertex(exp.Current()));
    BRepClass3d_SolidClassifier classifier(solid);
    classifier.Perform(testPoint, 1e-6);
    auto state = classifier.State();
    return classifier.State() == TopAbs_IN;
}

std::vector<TopoDS_Shell> ExtractOuterShells(const TopoDS_Shape& shape)
{
    TopTools_IndexedMapOfShape shellMap;
    TopExp_Explorer exp;
    for (exp.Init(shape, TopAbs_SHELL); exp.More(); exp.Next()) {
        shellMap.Add(exp.Current());
    }

    std::vector<TopoDS_Solid> solids;
    BRep_Builder builder;

    for (int i = 1; i <= shellMap.Extent(); ++i) {
        TopoDS_Shell shell = TopoDS::Shell(shellMap(i));
        solids.push_back(BRepBuilderAPI_MakeSolid(shell));
    }

    std::vector<TopoDS_Shell> outerShells;

    for (int i = 1; i <= shellMap.Extent(); ++i) {
        TopoDS_Shell shell_i = TopoDS::Shell(shellMap(i));
        bool isInsideAnother = false;

        for (int j = 1; j <= shellMap.Extent(); ++j) {
            if (i == j) continue;

            if (IsShellInside(shell_i, solids[j - 1])) {
                isInsideAnother = true;
                break;
            }
        }

        if (!isInsideAnother) {
            outerShells.push_back(shell_i);
        }
    }

    return outerShells;
}
int main() {
    string filePath;
    cout << "Podaj sciezke do pliku STEP lub BREP: ";
    getline(cin, filePath);
    if (!filePath.starts_with("C:")) {
        filePath = "C:/OpenCASCADE-7.7.0-vc14-64/opencascade-7.7.0/data/occ/" + filePath;
    }

    if (!filesystem::exists(filePath)) {
        cerr << "Plik nie istnieje: " << filePath << endl;
        return 1;
    }

    TopoDS_Shape shape;
    string ext = filesystem::path(filePath).extension().string();

    if (ext == ".step" || ext == ".stp" || ext == ".STEP" || ext == ".STP") {
        STEPControl_Reader reader;
        IFSelect_ReturnStatus status = reader.ReadFile(filePath.c_str());

        if (status != IFSelect_RetDone) {
            cerr << "Nie udalo sie wczytac pliku STEP." << endl;
            return 1;
        }

        reader.TransferRoots();
        shape = reader.OneShape();
    }
    else if (ext == ".brep" || ext == ".BREP") {
        BRep_Builder builder;
        if (!BRepTools::Read(shape, filePath.c_str(), builder)) {
            cerr << "Nie udalo sie wczytac pliku BREP." << endl;
            return 1;
        }
    }
    else {
        cerr << "Nieobslugiwany format pliku: " << ext << endl;
        return 1;
    }
    int shellCount = 0;
    for (TopExp_Explorer explorer(shape, TopAbs_SHELL); explorer.More(); explorer.Next()) {
        ++shellCount;
    }
    cout <<"Shelli zewnetrznych: " << ExtractOuterShells(shape).size()<<endl;
    cout << "Shelli lacznie: " << shellCount <<endl;
    

    return 0;
}