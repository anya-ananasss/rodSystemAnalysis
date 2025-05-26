#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H
#include "basicrodview.h"
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <QWidget>
class DiagramParamsCalculation;  // Предварительное объявление вместо включения
class DrawingWidget : public QWidget {
  Q_OBJECT
    /**
   * @brief DEFAULT_WIDTH стандартная ширина элемента стержня
   */
  const int DEFAULT_WIDTH = 90;
  /**
   * @brief DEFAULT_HEIGHT стандартная высота элемента стержня
   */
  const int DEFAULT_HEIGHT = 100;
  /**
   * @brief DEFAULT_FORCE_HEIGHT стандартная длина стрелки силы
   */
  const int DEFAULT_FORCE_HEIGHT = 30;
  /**
   * @brief DEFAULT_LEFTCORNER_X точка центра по X для отрисовки элементов стержня в 1/4 части доступного экрана
   */
  const int DEFAULT_LEFTCORNER_X = ((width()-40)/4)/2;

  const QColor DEFAULT_COLOR = QColor(105, 105, 105);
  const QColor DEFAULT_SELECTED_COLOR = QColor(34, 139, 34, 80);
  const QColor DEFAULT_FILLING_COLOR = QColor(220, 220, 220);
  const QColor DEFAULT_FORCE_COLOR = QColor(220, 220, 220);
  const double PX_TO_METERS_COEFF = 0.01;
  const double METERS_TO_PX_COEFF = 100;
  const double PX_TO_MMS_COEFF = 0.0055;
  const double MMS_TO_PX_COEFF = 90;


  // New constants for diagram scaling
  const int DIAGRAM_WIDTH = 100;  // Width of each diagram in pixels
  const double FORCE_SCALE = 0.4; // Pixels per kN (adjust based on max force)
  const double STRESS_SCALE = 2000.0; // Pixels per MPa (adjust based on max stress)
  const int DIAGRAM_OFFSET_X = 150; // Starting X position for the first diagram (right of the rod)

public:
  DrawingWidget(QWidget *parent = nullptr);
  ~DrawingWidget();
  const QList<BasicRodView>& getRods() const { return rods; }
  double endForce = 0.0;
  double E = 210000.0; //MPa

  bool m_programmaticUpdate = false;
  // ParamsBlock paramsBlock;

signals:
  void rodSelected();
  void rodUnselected();
  void updateRodSettings(double forceValue, double q);
  void updateRectParams(double length, double width);
  void on_calculations_complete(QList<double> longitudinalForces,
                                QList<double> stresses, QList<double> extensions);

public slots:
  void on_addRodButton_clicked();
  void on_lengthEdit_textEdited(const double &arg1);
  void on_radEdit_textEdited(const double &arg1);
  void on_removeRodButton_clicked();
  void on_force_updated(double newForce);
void on_q_updated(double newQ);
  void on_calculateButton_clicked();
  void on_endForce_updated(double value);
  void on_resilince_updated(double value);

private:
  void paintEvent(QPaintEvent *event);
  QList<BasicRodView> rods;
  QList<double> nValues;
  QList<double> stressValues;
  QList<double> extensionsValues;
  DiagramParamsCalculation* diagramParamsCalculation;

  int selectedRectIndex;
  void initDefault();
  QPoint lastMousePosition;
  bool isSelected;
  bool isResizable;
  bool drawDiagrams;
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void updateOtherRods();
  void drawForceArrow(QPainter &painter, const QRect &rect, double forceValue, bool isEndForce);
};

#endif // DRAWINGWIDGET_H
