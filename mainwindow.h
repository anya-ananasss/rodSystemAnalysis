#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "diagramParamsCalculation.h"
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
signals:
  void forceUpdated(double newForce);
    void endForceValueChanged(double value);
  void resilinceValueChanged(double value);
    void doCalculations();
private slots:
  void activateRodSettings();
  void deactivateRodSettings();
  void updateRodSettings(double forceValue, double q);
  void updateRectParams(double length, double width);
  void endForceUpdated();
  void resilinceValueUpdated();
  void updateResultsTable(const QList<double>& nValues,
                     const QList<double>& stressValues,
                          const QList<double>& extensionsValues);

  void on_forceEdit_textChanged(const double &arg1);
  void on_forceCheckBox_checkStateChanged(const Qt::CheckState &arg1);

  void on_calculateButton_clicked();

  private:
  Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
