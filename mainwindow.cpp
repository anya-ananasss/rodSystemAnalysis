#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "drawingwidget.h"
#include "diagramParamsCalculation.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

  ui->setupUi(this);

  ui->lengthEdit->setEnabled(false);
  ui->radEdit->setEnabled(false);
  ui->forceEdit->setEnabled(false);
  ui->qEdit->setEnabled(false);
  ui->removeRodButton->setEnabled(false);
  ui->removeRodButton->setEnabled(false);
  ui->forceCheckBox->setChecked(true);
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
  connect(ui->calculateButton, &QPushButton::clicked, drawingWidget,
          &DrawingWidget::on_calculateButton_clicked);
  connect(ui->endForceSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          this, &MainWindow::endForceUpdated);
  connect(this, &MainWindow::endForceValueChanged,
          drawingWidget, &DrawingWidget::on_endForce_updated);


  connect(ui->resilenceSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          this, &MainWindow::resilinceValueUpdated);
  connect(this, &MainWindow::resilinceValueChanged,
          drawingWidget, &DrawingWidget::on_resilince_updated);

  // drawingWidget->paramsBlock.setLength(ui->lengthEdit);
  // drawingWidget->paramsBlock.setRadius(ui->radEdit);
  // drawingWidget->paramsBlock.setForce(ui->forceEdit);
  // drawingWidget->paramsBlock.setQ(ui->qEdit);

  connect(ui->lengthEdit, QOverload<double>::of(&QDoubleSpinBox::valueChanged), drawingWidget,
          &DrawingWidget::on_lengthEdit_textEdited);
  connect(ui->radEdit, QOverload<double>::of(&QDoubleSpinBox::valueChanged), drawingWidget,
          &DrawingWidget::on_radEdit_textEdited);

  connect(ui->forceEdit, QOverload<double>::of(&QDoubleSpinBox::valueChanged), drawingWidget,
          &DrawingWidget::on_force_updated);

  connect(ui->qEdit, QOverload<double>::of(&QDoubleSpinBox::valueChanged), drawingWidget,
          &DrawingWidget::on_q_updated);

  ui->resultsTable->setColumnCount(4);
  ui->resultsTable->verticalHeader()->setVisible(false);


  QStringList headers;
  headers << "Стержень" << "Продольная сила, кН" << "Напряжение, МПа" << "Растяжение, мм";
  ui->resultsTable->setHorizontalHeaderLabels(headers);


  ui->resultsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
  ui->resultsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
  ui->resultsTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
  ui->resultsTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);


  ui->resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);


  connect(this, &MainWindow::doCalculations, drawingWidget,DrawingWidget::on_calculateButton_clicked);
  connect(drawingWidget, DrawingWidget::on_calculations_complete, this, MainWindow::updateResultsTable);

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

  ui->lengthEdit->blockSignals(true);
  ui->radEdit->blockSignals(true);
  ui->forceEdit->blockSignals(true);
  ui->qEdit->blockSignals(true);

  ui->lengthEdit->setValue(0.0);
  ui->radEdit->setValue(0.0);
  ui->forceEdit->setValue(0.0);
  ui->qEdit->setValue(0.0);

  ui->lengthEdit->blockSignals(false);
  ui->radEdit->blockSignals(false);
  ui->forceEdit->blockSignals(false);
  ui->qEdit->blockSignals(false);
}

void MainWindow::updateRectParams(double length, double width) {
ui->lengthEdit->blockSignals(true);
ui->radEdit->blockSignals(true);

  ui->lengthEdit->setValue(length);
  ui->radEdit->setValue(width);

  ui->lengthEdit->blockSignals(false);
  ui->radEdit->blockSignals(false);
}

void MainWindow::updateRodSettings(double forceValue, double q) {
  ui->forceEdit->setValue(forceValue);
  ui->qEdit->setValue(q);
}

void MainWindow::on_forceEdit_textChanged(const double &arg1) {
    emit forceUpdated(arg1);
}

void MainWindow::on_forceCheckBox_checkStateChanged(const Qt::CheckState &arg1)
{
    ui->endForceSpinBox->setEnabled(arg1 == Qt::Checked);
    if (arg1==false){
        emit endForceValueChanged(0.0);
    } else {
        emit endForceValueChanged(ui->endForceSpinBox->value());
    }
}

void MainWindow::endForceUpdated()
{
    emit endForceValueChanged(ui->endForceSpinBox->value());
}

void MainWindow::resilinceValueUpdated()
{
    emit resilinceValueChanged(ui->resilenceSpinBox->value());
}

void MainWindow::updateResultsTable(const QList<double>& nValues,
                                    const QList<double>& stressValues,
                                    const QList<double>& extensionsValues)
{

    int numRods = nValues.length();
    ui->resultsTable->setRowCount(numRods);

    for (int i = 0; i < numRods; ++i) {
        QTableWidgetItem *itemName = new QTableWidgetItem(QString("Стержень %1").arg(i + 1));
        ui->resultsTable->setItem(i, 0, itemName);


        QTableWidgetItem *forceItem = new QTableWidgetItem(QString::number(nValues[i], 'f', 2));
        ui->resultsTable->setItem(i, 1, forceItem);


        QTableWidgetItem *stressItem = new QTableWidgetItem(QString::number(stressValues[i], 'f', 2));
        ui->resultsTable->setItem(i, 2, stressItem);

        QTableWidgetItem *extensionItem = new QTableWidgetItem(QString::number(extensionsValues[i], 'f', 2));
        ui->resultsTable->setItem(i, 3, extensionItem);

        if (itemName) itemName->setTextAlignment(Qt::AlignCenter);
        if (forceItem) forceItem->setTextAlignment(Qt::AlignCenter);
        if (stressItem) stressItem->setTextAlignment(Qt::AlignCenter);
        if (extensionItem) extensionItem->setTextAlignment(Qt::AlignCenter);
    }
}


void MainWindow::on_calculateButton_clicked()
{
    emit doCalculations();
}

