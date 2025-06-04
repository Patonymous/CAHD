#ifndef MODELINFO_H
#define MODELINFO_H

#include <BRep_Builder.hxx>
#include <TopoDS.hxx>

#include <QList>
#include <QTreeWidgetItem>

class ModelInfo : public QObject {
    Q_OBJECT

    struct Part {
        bool isVisible = true;
    };
    struct Face : public Part {
        Face(const TopoDS_Face &occt) : topo(occt) {}

        TopoDS_Face topo;
    };
    struct Shell : public Part {
        Shell(const TopoDS_Shell &occt, QList<Face> faces)
            : topo(occt), faces(faces) {}

        TopoDS_Shell topo;
        QList<Face>  faces;
    };
    struct Solid : public Part {
        Solid(const TopoDS_Solid &occt, QList<Shell> shells)
            : topo(occt), shells(shells) {}

        TopoDS_Solid topo;
        QList<Shell> shells;
    };
    struct Shape : public Part {
        Shape(const TopoDS_Shape &occt, QList<Solid> solids)
            : topo(occt), solids(solids) {}

        TopoDS_Shape topo;
        QList<Solid> solids;
    };

public:
    ModelInfo(
        QObject *parent, const QString &name, const TopoDS_Shape &occtShape
    );
    ~ModelInfo();

    const TopoDS_Shape &originalShape() const;
    const TopoDS_Shape &visibleShape() const;
    QTreeWidgetItem    *treeWidgetItem() const;

public slots:
    void updateVisibility(QTreeWidgetItem *item, int column);

signals:
    void visibleShapeChanged(const TopoDS_Shape &occt);

private:
    static Shape processShape(const TopoDS_Shape &occtShape);
    static Solid processSolid(const TopoDS_Solid &occtSolid);
    static Shell processShell(const TopoDS_Shell &occtShell);
    static Face  processFace(const TopoDS_Face &occtFace);

    static QTreeWidgetItem *treeForShape(Shape &shape, const QString &name);
    static QTreeWidgetItem *treeForSolid(Solid &solid, int index);
    static QTreeWidgetItem *treeForShell(Shell &shell, int index);
    static QTreeWidgetItem *treeForFace(Face &face, int index);

    static TopoDS_Shape buildShape(const Shape &shape);
    static TopoDS_Solid buildSolid(const Solid &solid);
    static TopoDS_Shell buildShell(const Shell &shell);
    static TopoDS_Face  buildFace(const Face &face);

    const static BRep_Builder s_builder;

    TopoDS_Shape m_original;

    Shape            m_shape;
    TopoDS_Shape     m_occt;
    QTreeWidgetItem *m_tree;
};

#endif // MODELINFO_H
