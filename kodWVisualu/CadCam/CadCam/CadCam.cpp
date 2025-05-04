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

using namespace std;

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
        int faceCount = 0;
        for (TopExp_Explorer exp2(explorer.Current(), TopAbs_FACE); exp2.More(); exp2.Next()) {
            faceCount++;
            //cout << exp2.Current()<<"/n";
            auto face = TopoDS::Face(exp2.Current());
        }
        cout << "Liczba face w shellu "<<shellCount<<": " << faceCount << endl;
        ++shellCount;
    }

    return 0;
}