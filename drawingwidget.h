#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H
#include "basicrodview.h"
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <QWidget>
class DrawingWidget : public QWidget {
  Q_OBJECT

  const int DEFAULT_WIDTH = 90;
  const int DEFAULT_HEIGHT = 100;
  const int DEFAULT_FORCE_HEIGHT = 30;
  const QColor DEFAULT_COLOR = QColor(105, 105, 105);
  const QColor DEFAULT_SELECTED_COLOR = QColor(34, 139, 34, 80);
  const QColor DEFAULT_FILLING_COLOR = QColor(220, 220, 220);
  const QColor DEFAULT_FORCE_COLOR = QColor(220, 220, 220);
  const double PX_TO_METERS_COEFF = 0.01;
  const double METERS_TO_PX_COEFF = 100;
  const double PX_TO_MMS_COEFF = 0.0055;
  const double MMS_TO_PX_COEFF = 90;

public:
  DrawingWidget(QWidget *parent = nullptr);
  ~DrawingWidget();
  // ParamsBlock paramsBlock;

signals:
  void rodSelected();
  void rodUnselected();
  void updateRodSettings(double forceValue, double q);
  void updateRectParams(double length, double width);

public slots:
  void on_addRodButton_clicked();
  void on_lengthEdit_textEdited(const QString &arg1);
  void on_radEdit_textEdited(const QString &arg1);
  void on_removeRodButton_clicked();
  void on_force_updated(double newForce);

private:
  void paintEvent(QPaintEvent *event);
  QList<BasicRodView> rods;
  int selectedRectIndex;
  void initDefault();
  QPoint lastMousePosition;
  bool isSelected;
  bool isResizable;
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void updateOtherRods();
};

#endif // DRAWINGWIDGET_H
