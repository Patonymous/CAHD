// Adapted from
// https://github.com/gkv311/occt-samples-qopenglwidget/blob/master/occt-qopenglwidget/OcctQtViewer.h

#ifndef OCCTWIDGET_H
#define OCCTWIDGET_H

#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <AIS_ViewController.hxx>
#include <AIS_ViewCube.hxx>
#include <V3d_View.hxx>

class OcctWidget : public QOpenGLWidget,
                   QOpenGLFunctions,
                   public AIS_ViewController {
    Q_OBJECT

public:
    OcctWidget(QWidget *parent = nullptr);
    ~OcctWidget();

    // Return Viewer.
    const Handle(V3d_Viewer) & Viewer() const { return m_viewer; }

    // Return View.
    const Handle(V3d_View) & View() const { return m_view; }

    // Return AIS context.
    const Handle(AIS_InteractiveContext) & Context() const { return m_context; }

    // Return OpenGL info.
    const QString &getGlInfo() const { return m_glInfo; }

    // Minial widget size.
    virtual QSize minimumSizeHint() const override { return QSize(200, 200); }

    // Default widget size.
    virtual QSize sizeHint() const override { return QSize(720, 480); }

    // Handle subview focus change.
    virtual void OnSubviewChanged(
        const Handle(AIS_InteractiveContext) &, const Handle(V3d_View) &,
        const Handle(V3d_View) & theNewView
    ) override;

public slots:
    void loadModelFromFile(const std::string &path);

signals:
    void loadedModelInfo(const QList<int> shellsFaceCounts);
    void modelLoadingMessage(const QString &message);

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void closeEvent(QCloseEvent *theEvent) override;
    void keyPressEvent(QKeyEvent *theEvent) override;
    void mousePressEvent(QMouseEvent *theEvent) override;
    void mouseReleaseEvent(QMouseEvent *theEvent) override;
    void mouseMoveEvent(QMouseEvent *theEvent) override;
    void wheelEvent(QWheelEvent *theEvent) override;

private:
    // Print debug messages.
    void printDebugMessages() const;

    // Dump OpenGL info.
    void dumpGlInfo(bool theIsBasic, bool theToPrint);

    // Handle view redraw.
    virtual void handleViewRedraw(
        const Handle(AIS_InteractiveContext) & theCtx,
        const Handle(V3d_View) & theView
    ) override;

    // clang-format off
    Handle(V3d_Viewer)             m_viewer;
    Handle(V3d_View)               m_view;
    Handle(V3d_View)               m_focusView;
    Handle(AIS_InteractiveContext) m_context;
    Handle(AIS_ViewCube)           m_viewCube;
    Handle(AIS_Shape)              m_shape;
    // clang-format on

    QString            m_glInfo;
    bool               m_isCoreProfile;
    QOpenGLDebugLogger m_logger;
};

#endif // OCCTWIDGET_H
