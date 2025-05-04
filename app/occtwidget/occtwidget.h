#ifndef OCCTWIDGET_H
#define OCCTWIDGET_H

#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

class OcctWidget : public QOpenGLWidget, QOpenGLFunctions {
    Q_OBJECT

public:
    OcctWidget(QWidget *parent = nullptr);
    ~OcctWidget();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

private:
    QOpenGLDebugLogger m_logger;

    void printDebugMessages() const;
};

#endif // OCCTWIDGET_H
