#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), fileDialog(this),
      modelInfoLabel(this) {
    ui->setupUi(this);

    QObject::connect(
        ui->occtWidget, &OcctWidget::modelLoadingMessage, this,
        &MainWindow::showMessage
    );
    QObject::connect(
        ui->occtWidget, &OcctWidget::loadedModelInfo, this,
        &MainWindow::updateModelInfo
    );

    QObject::connect(
        ui->actionOpen, &QAction::triggered, &fileDialog, &QDialog::open
    );

    modelInfoLabel.setText("No model loaded");
    statusBar()->addWidget(&modelInfoLabel);

    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setNameFilter("BREP and STEP files (*.brep *.step)");
    fileDialog.setWindowTitle("Open model file");
    fileDialog.setDirectory(QDir::homePath());
    fileDialog.setViewMode(QFileDialog::Detail);
    fileDialog.setOption(QFileDialog::DontUseNativeDialog, true);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    QObject::connect(
        &fileDialog, &QFileDialog::accepted, this, &MainWindow::loadModel
    );
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::loadModel() {
    QStringList fileNames = fileDialog.selectedFiles();
    if (fileNames.isEmpty())
        return;

    ui->occtWidget->loadModelFromFile(fileNames.first().toStdString());
}

void MainWindow::updateModelInfo(int shellCount, int faceCount) {
    modelInfoLabel.setText(
        QString("Shells: %1, Faces: %2").arg(shellCount).arg(faceCount)
    );
}

void MainWindow::showMessage(const QString &message) {
    statusBar()->showMessage(message, 5000);
}
