#ifndef DIAGRAMPARAMSCALCULATION_H
#define DIAGRAMPARAMSCALCULATION_H

class DrawingWidget; // Вместо #include "drawingwidget.h"
#include "diagramParamsCalculation.h"
#include <QList>

class DiagramParamsCalculation {
public:
  DiagramParamsCalculation(DrawingWidget *drawingWidget);

  QList<double> calculateLongitudinalForces();
  QList<double> calculateStresses();
  QList<double> calculateExtensions();

  QList<double> longitudinalForces;
  QList<double> stresses;
  QList<double> extensions;

private:
  DrawingWidget *m_drawingWidget;

};

#endif
