#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), fileDialog(this),
      statusLabel(this) {
    ui->setupUi(this);

    QObject::connect(
        ui->occtWidget, &OcctWidget::statusChanged, this,
        &MainWindow::updateStatus
    );
    QObject::connect(
        ui->occtWidget, &OcctWidget::messageRaised, this,
        &MainWindow::showMessage
    );
    QObject::connect(
        ui->occtWidget, &OcctWidget::modelLoaded, this, &MainWindow::addModel
    );
    QObject::connect(
        ui->occtWidget, &OcctWidget::modelUnloaded, this,
        &MainWindow::removeModel
    );

    ui->treeWidget->sortByColumn(0, Qt::AscendingOrder);

    QObject::connect(
        ui->actionOpen, &QAction::triggered, &fileDialog, &QDialog::open
    );

    statusLabel.setText("No model loaded");
    statusBar()->addWidget(&statusLabel);

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

void MainWindow::updateStatus(const QString &status) {
    statusLabel.setText(status);
}

void MainWindow::showMessage(const QString &message) {
    statusBar()->showMessage(message, 5000);
}

void MainWindow::addModel(const ModelInfo &model) {
    QObject::connect(
        &model, &ModelInfo::visibleShapeChanged, ui->occtWidget,
        &OcctWidget::displayModel
    );

    QObject::connect(
        ui->treeWidget, &QTreeWidget::itemChanged, &model,
        &ModelInfo::updateVisibility
    );

    ui->treeWidget->addTopLevelItem(model.treeWidgetItem());
}

void MainWindow::removeModel(const ModelInfo &model) {
    QObject::disconnect(
        &model, &ModelInfo::visibleShapeChanged, ui->occtWidget,
        &OcctWidget::displayModel
    );

    QObject::disconnect(
        ui->treeWidget, &QTreeWidget::itemChanged, &model,
        &ModelInfo::updateVisibility
    );
}
