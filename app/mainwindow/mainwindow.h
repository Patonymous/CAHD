#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QLabel>
#include <QMainWindow>

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
    void updateModelInfo(int shellCount, int faceCount);
    void showMessage(const QString &message);

private:
    Ui::MainWindow *ui;
    QFileDialog     fileDialog;
    QLabel          modelInfoLabel;
};
#endif // MAINWINDOW_H
