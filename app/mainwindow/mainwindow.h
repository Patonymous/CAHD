#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QLabel>
#include <QMainWindow>

#include "modelinfo.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void loadModel();
    void updateStatus(const QString &status);
    void showMessage(const QString &message);

    void addModel(const ModelInfo &modelinfo);
    void removeModel(const ModelInfo &model);

private:
    Ui::MainWindow *ui;
    QFileDialog     fileDialog;
    QLabel          statusLabel;
};
#endif // MAINWINDOW_H
