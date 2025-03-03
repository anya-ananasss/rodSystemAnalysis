#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
signals:
    void forceUpdated(double newForce);
private slots:
  void activateRodSettings();
  void deactivateRodSettings();
  void updateRodSettings(double forceValue, double q);
  void updateRectParams(double length, double width);


  void on_forceEdit_textChanged(const QString &arg1);

  private:
  Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
