#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "drawingwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

  ui->setupUi(this);

  ui->lengthEdit->setEnabled(false);
  ui->radEdit->setEnabled(false);
  ui->forceEdit->setEnabled(false);
  ui->qEdit->setEnabled(false);
  ui->removeRodButton->setEnabled(false);
  ui->removeRodButton->setEnabled(false);
  DrawingWidget *drawingWidget = new DrawingWidget(this);

  QVBoxLayout *layout = new QVBoxLayout(ui->drawingWidget);
  layout->addWidget(drawingWidget);
  connect(ui->addRodButton, &QPushButton::clicked, drawingWidget,
          &DrawingWidget::on_addRodButton_clicked);
  connect(ui->removeRodButton, &QPushButton::clicked, drawingWidget,
          &DrawingWidget::on_removeRodButton_clicked);
  connect(drawingWidget, &DrawingWidget::rodSelected, this,
          &MainWindow::activateRodSettings);
  connect(drawingWidget, &DrawingWidget::rodUnselected, this,
          &MainWindow::deactivateRodSettings);
  connect(drawingWidget, &DrawingWidget::updateRodSettings, this,
          &MainWindow::updateRodSettings);
  connect(drawingWidget, &DrawingWidget::updateRectParams, this,
          &MainWindow::updateRectParams);
  connect(this, &MainWindow::forceUpdated, drawingWidget,
          &DrawingWidget::on_force_updated);

  // drawingWidget->paramsBlock.setLength(ui->lengthEdit);
  // drawingWidget->paramsBlock.setRadius(ui->radEdit);
  // drawingWidget->paramsBlock.setForce(ui->forceEdit);
  // drawingWidget->paramsBlock.setQ(ui->qEdit);

  connect(ui->lengthEdit, &QLineEdit::textEdited, drawingWidget,
          &DrawingWidget::on_lengthEdit_textEdited);
  connect(ui->radEdit, &QLineEdit::textEdited, drawingWidget,
          &DrawingWidget::on_radEdit_textEdited);
}

MainWindow::~MainWindow() { delete ui; }
void MainWindow::activateRodSettings() {
  ui->lengthEdit->setEnabled(true);
  ui->radEdit->setEnabled(true);
  ui->forceEdit->setEnabled(true);
  ui->qEdit->setEnabled(true);
  ui->removeRodButton->setEnabled(true);
}

void MainWindow::deactivateRodSettings() {
  ui->lengthEdit->setEnabled(false);
  ui->radEdit->setEnabled(false);
  ui->forceEdit->setEnabled(false);
  ui->qEdit->setEnabled(false);
  ui->removeRodButton->setEnabled(false);

  ui->lengthEdit->setText("");
  ui->radEdit->setText("");
  ui->forceEdit->setText("");
  ui->qEdit->setText("");
}

void MainWindow::updateRectParams(double length, double width) {
  ui->lengthEdit->setText(QString::number(length));
  ui->radEdit->setText(QString::number(width));
}

void MainWindow::updateRodSettings(double forceValue, double q) {
  ui->forceEdit->setText(QString::number(forceValue));
  ui->qEdit->setText(QString::number(q));
}

void MainWindow::on_forceEdit_textChanged(const QString &arg1) {
  bool isNum;
  double forceValue = arg1.toDouble(&isNum);
  if (isNum) {
    emit forceUpdated(forceValue);
  } else if (arg1 == "") {
    emit forceUpdated(0);
  }
}
