#include "occtwidget.h"

OcctWidget::OcctWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_logger(this) {

    QSurfaceFormat format;
    // format.setVersion(4, 5);
    // format.setProfile(QSurfaceFormat::CoreProfile);
    format.setOption(QSurfaceFormat::DebugContext);
    setFormat(format);
}

OcctWidget::~OcctWidget() {}

void OcctWidget::initializeGL() {
    initializeOpenGLFunctions();

    m_logger.initialize();

    printDebugMessages();
}

void OcctWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);

    printDebugMessages();
}

void OcctWidget::paintGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    printDebugMessages();
}

void OcctWidget::printDebugMessages() const {
    for (auto &m : m_logger.loggedMessages())
        qDebug() << m.message();
}
