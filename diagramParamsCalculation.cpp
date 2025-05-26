#include "DiagramParamsCalculation.h"
#include "drawingwidget.h"

DiagramParamsCalculation::DiagramParamsCalculation(DrawingWidget *drawingWidget)
    : m_drawingWidget(drawingWidget) {}

QList<double> DiagramParamsCalculation::calculateLongitudinalForces() {
  const auto &rods = m_drawingWidget->getRods();
  QList<double> longitudinalForces;

  double prevForce = -m_drawingWidget->endForce;
  qDebug() << "Longitudinal";
  qDebug() << "Стержень " << rods.length() << ":" << prevForce;
  longitudinalForces.append(prevForce);

  for (int i = rods.length() - 1; i > 0; i--) {
    double force = rods[i].getForceValue();
    prevForce += -force;
    qDebug() << "Стержень " << i << ":" << prevForce;
    longitudinalForces.append(prevForce);
  }

  std::reverse(std::begin(longitudinalForces), std::end(longitudinalForces));
  this->longitudinalForces = longitudinalForces;

  return longitudinalForces;
}
QList<double> DiagramParamsCalculation::calculateStresses() {
  const auto &rods = m_drawingWidget->getRods();
  QList<double> stresses;

  for (int i = 0; i < rods.length(); i++) {
    int reverseIndex = rods.length() - 1 - i;

    double radius_cm = rods[reverseIndex].getTrueWidth();
    double radius_m = radius_cm / 100.0;

    double area_m2 = M_PI * qPow(radius_m, 2);

    double force_kN = longitudinalForces[reverseIndex];
    double force_N = force_kN * 1000.0;

    double stress_Pa = force_N / area_m2;
    double stress_MPa = stress_Pa / 1e6;

    double area_cm2 = M_PI * qPow(radius_cm, 2);

    qDebug() << "Rod" << reverseIndex << "| Radius:" << radius_cm << "cm"
             << "| Area:" << area_cm2 << "cm^2"
             << "| Force:" << force_kN << "kN"
             << "| Stress:" << stress_MPa << "MPa";

    stresses.append(stress_MPa);
  }
  std::reverse(std::begin(stresses), std::end(stresses));
  this->stresses = stresses;
  return stresses;
}
QList<double> DiagramParamsCalculation::calculateExtensions() {
  const auto &rods = m_drawingWidget->getRods();
  double E_MPa = m_drawingWidget->E;
  QList<double> extensions;

  for (int i = 0; i < rods.length(); i++) {
      int reverseIndex = rods.length() - 1 - i;

      double radius_cm = rods[reverseIndex].getTrueWidth();
      double radius_mm = radius_cm * 10.0;

      double area_mm2 = M_PI * qPow(radius_mm, 2);

      double force_kN = longitudinalForces[reverseIndex];
      double force_N = force_kN * 1000.0;

      double height_m = rods[reverseIndex].getTrueHeight();

      double extension_mm = (force_N * height_m) / (area_mm2 * E_MPa) * 1000;

      qDebug() << "Rod" << reverseIndex << "| Radius:" << radius_cm << "cm"
               << "| Area:" << area_mm2 << "mm²"
               <<"| Height:"<< height_m
               << "| Force:" << force_kN << "kN"
               << "| E:" << E_MPa
               << "| Extension:" << extension_mm << "MPa";

      extensions.append(extension_mm);
    }
  std::reverse(std::begin(extensions), std::end(extensions));
  this->extensions = extensions;
  return extensions;
}
