#include "modelinfo.h"

#include <BRep_Builder.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Solid.hxx>

const BRep_Builder ModelInfo::s_builder = BRep_Builder();

ModelInfo::ModelInfo(
    QObject *parent, const QString &name, const TopoDS_Shape &occtShape
)
    : QObject(parent), m_original(occtShape), m_shape(processShape(occtShape)),
      m_occt(buildShape(m_shape)), m_tree(treeForShape(m_shape, name)) {}
ModelInfo::~ModelInfo() {
    // This will also delete children, I believe
    delete m_tree;
}

const TopoDS_Shape &ModelInfo::originalShape() const { return m_original; }
const TopoDS_Shape &ModelInfo::visibleShape() const { return m_occt; }
QTreeWidgetItem    *ModelInfo::treeWidgetItem() const { return m_tree; }

void ModelInfo::updateVisibility(QTreeWidgetItem *item, int column) {
    // We should check if `item` is a descendant of our own `m_tree`,
    // but it's not trivial and for now only one model will be bound to the
    // QTreeWidget anyway.

    // Converting back from uint64 to Part*
    auto shape =
        (Part *)item->data(0, Qt::ItemDataRole::UserRole).toULongLong();
    shape->isVisible = item->checkState(column) != Qt::CheckState::Unchecked;

    m_occt = buildShape(m_shape);
    emit visibleShapeChanged(m_occt);
}

ModelInfo::Shape ModelInfo::processShape(const TopoDS_Shape &occtShape) {
    QList<Solid> solids;
    for (TopExp_Explorer explorer(occtShape, TopAbs_SOLID); explorer.More();
         explorer.Next()) {
        solids << processSolid(TopoDS::Solid(explorer.Current()));
    }
    return Shape(occtShape, solids);
}

ModelInfo::Solid ModelInfo::processSolid(const TopoDS_Solid &occtSolid) {
    QList<Shell> shells;
    for (TopExp_Explorer explorer(occtSolid, TopAbs_SHELL); explorer.More();
         explorer.Next()) {
        shells << processShell(TopoDS::Shell(explorer.Current()));
    }
    return Solid(occtSolid, shells);
}

ModelInfo::Shell ModelInfo::processShell(const TopoDS_Shell &occtShell) {
    QList<Face> faces;
    for (TopExp_Explorer explorer(occtShell, TopAbs_FACE); explorer.More();
         explorer.Next()) {
        faces << processFace(TopoDS::Face(explorer.Current()));
    }
    return Shell(occtShell, faces);
}

ModelInfo::Face ModelInfo::processFace(const TopoDS_Face &occtFace) {
    return Face(occtFace);
}

QTreeWidgetItem *ModelInfo::treeForShape(Shape &shape, const QString &name) {
    auto *tree = new QTreeWidgetItem;
    tree->setFlags(
        Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemIsSelectable
    );
    tree->setData(0, Qt::ItemDataRole::DisplayRole, name);
    // Storing a Part* as uint64... sure, why not?
    tree->setData(0, Qt::ItemDataRole::UserRole, (quint64)&shape);
    for (int i = 0; i < shape.solids.size(); i++)
        tree->addChild(treeForSolid(shape.solids[i], i));
    return tree;
}

QTreeWidgetItem *ModelInfo::treeForSolid(Solid &solid, int index) {
    auto *tree = new QTreeWidgetItem;
    tree->setFlags(
        Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemIsUserCheckable
        | Qt::ItemFlag::ItemIsSelectable
    );
    tree->setCheckState(0, Qt::CheckState::Checked);
    tree->setData(
        0, Qt::ItemDataRole::DisplayRole, QString("Solid %1").arg(index, 3)
    );
    // Storing a Part* as uint64... sure, why not?
    tree->setData(0, Qt::ItemDataRole::UserRole, (quint64)&solid);
    for (int i = 0; i < solid.shells.size(); i++)
        tree->addChild(treeForShell(solid.shells[i], i));
    return tree;
}

QTreeWidgetItem *ModelInfo::treeForShell(Shell &shell, int index) {
    auto *tree = new QTreeWidgetItem;
    tree->setFlags(
        Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemIsUserCheckable
        | Qt::ItemFlag::ItemIsSelectable
    );
    tree->setCheckState(0, Qt::CheckState::Checked);
    tree->setData(
        0, Qt::ItemDataRole::DisplayRole, QString("Shell %1").arg(index, 3)
    );
    // Storing a Part* as uint64... sure, why not?
    tree->setData(0, Qt::ItemDataRole::UserRole, (quint64)&shell);
    for (int i = 0; i < shell.faces.size(); i++)
        tree->addChild(treeForFace(shell.faces[i], i));
    return tree;
}

QTreeWidgetItem *ModelInfo::treeForFace(Face &face, int index) {
    auto *tree = new QTreeWidgetItem;
    tree->setFlags(
        Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemIsUserCheckable
        | Qt::ItemFlag::ItemIsSelectable | Qt::ItemFlag::ItemNeverHasChildren
    );
    tree->setCheckState(0, Qt::CheckState::Checked);
    tree->setData(
        0, Qt::ItemDataRole::DisplayRole, QString("Face %1").arg(index, 3)
    );
    // Storing a Part* as uint64... sure, why not?
    tree->setData(0, Qt::ItemDataRole::UserRole, (quint64)&face);
    return tree;
}

TopoDS_Shape ModelInfo::buildShape(const Shape &shape) {
    TopoDS_Compound occt;
    s_builder.MakeCompound(occt);
    for (const auto &solid : shape.solids)
        if (solid.isVisible)
            s_builder.Add(occt, buildSolid(solid));
    return occt;
}

TopoDS_Solid ModelInfo::buildSolid(const Solid &solid) {
    TopoDS_Solid occt;
    s_builder.MakeSolid(occt);
    for (const auto &shell : solid.shells)
        if (shell.isVisible)
            s_builder.Add(occt, buildShell(shell));
    return occt;
}

TopoDS_Shell ModelInfo::buildShell(const Shell &shell) {
    TopoDS_Shell occt;
    s_builder.MakeShell(occt);
    for (const auto &face : shell.faces)
        if (face.isVisible)
            s_builder.Add(occt, buildFace(face));
    return occt;
}

TopoDS_Face ModelInfo::buildFace(const Face &face) { return face.topo; }
