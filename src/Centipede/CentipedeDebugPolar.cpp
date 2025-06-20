/******************************************************************************************************
 * (C) 2020 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#include "CentipedeDebugPolar.h"
#include "DocumentModelCoords.h"
#include "Logger.h"
#include "mmsubs.h"
#include <QColor>
#include <qdebug.h>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <qmath.h>
#include "QtToString.h"
#include "Transformation.h"

CentipedeDebugPolar::CentipedeDebugPolar() :
  m_posScreenParallelogramTL (0, 0),
  m_posScreenParallelogramTR (0, 0),
  m_posScreenParallelogramBL (0, 0),
  m_posScreenParallelogramBR (0, 0),
  m_angleGraphAxisFromScreenAxis (0),
  m_angleEllipseFromMajorAxis (0),
  m_aAligned (0),
  m_bAligned (0),
  m_radius (0),
  m_legendYPos (0)
{
}

CentipedeDebugPolar::CentipedeDebugPolar(const QPointF &posScreenParallelogramTL,
                                         const QPointF &posScreenParallelogramTR,
                                         const QPointF &posScreenParallelogramBL,
                                         const QPointF &posScreenParallelogramBR,
                                         double angleGraphAxisFromScreenAxis,
                                         double angleEllipseFromMajorAxis,
                                         double aAligned,
                                         double bAligned,
                                         double radius) :
  m_posScreenParallelogramTL (posScreenParallelogramTL),
  m_posScreenParallelogramTR (posScreenParallelogramTR),
  m_posScreenParallelogramBL (posScreenParallelogramBL),
  m_posScreenParallelogramBR (posScreenParallelogramBR),
  m_angleGraphAxisFromScreenAxis (angleGraphAxisFromScreenAxis),
  m_angleEllipseFromMajorAxis (angleEllipseFromMajorAxis),
  m_aAligned (aAligned),
  m_bAligned (bAligned),
  m_radius (radius),
  m_legendYPos (0)
{
}

CentipedeDebugPolar::CentipedeDebugPolar (const CentipedeDebugPolar &other) :
  m_posScreenParallelogramTL (other.posScreenParallelogramTL ()),
  m_posScreenParallelogramTR (other.posScreenParallelogramTR ()),
  m_posScreenParallelogramBL (other.posScreenParallelogramBL ()),
  m_posScreenParallelogramBR (other.posScreenParallelogramBR ()),
  m_angleGraphAxisFromScreenAxis (other.angleGraphAxisFromScreenAxis ()),
  m_angleEllipseFromMajorAxis (other.angleEllipseFromMajorAxis ()),
  m_aAligned (other.aAligned ()),
  m_bAligned (other.bAligned ()),
  m_radius (other.radius ()),
  m_legendYPos (0)
{
}

CentipedeDebugPolar &CentipedeDebugPolar::operator= (const CentipedeDebugPolar &other)
{
  m_posScreenParallelogramTL = other.posScreenParallelogramTL ();
  m_posScreenParallelogramTR = other.posScreenParallelogramTR ();
  m_posScreenParallelogramBL = other.posScreenParallelogramBL ();
  m_posScreenParallelogramBR = other.posScreenParallelogramBR ();
  m_angleGraphAxisFromScreenAxis = other.angleGraphAxisFromScreenAxis ();
  m_angleEllipseFromMajorAxis = other.angleEllipseFromMajorAxis ();
  m_aAligned = other.aAligned ();
  m_bAligned = other.bAligned ();
  m_radius = other.radius ();
  m_legendYPos = 0;

  return *this;
}

CentipedeDebugPolar::~CentipedeDebugPolar()
{
}

double CentipedeDebugPolar::aAligned () const
{
  return m_aAligned;
}

void CentipedeDebugPolar::addToLegend (QGraphicsScene &scene,
                                       const QString &entry,
                                       const QColor &color)
{
  const int LEGEND_X_POS = 5, LEGEND_Y_STEP = 20;

  QGraphicsTextItem *itemGraphCoords = new QGraphicsTextItem (entry);
  itemGraphCoords->setPos (QPointF (LEGEND_X_POS, LEGEND_Y_STEP * (m_legendYPos++)));
  itemGraphCoords->setDefaultTextColor (color);
  scene.addItem (itemGraphCoords);
}

double CentipedeDebugPolar::angleEllipseFromMajorAxis () const
{
  return m_angleEllipseFromMajorAxis;
}

double CentipedeDebugPolar::angleGraphAxisFromScreenAxis () const
{
  return m_angleGraphAxisFromScreenAxis;
}

double CentipedeDebugPolar::bAligned () const
{
  return m_bAligned;
}

void CentipedeDebugPolar::display (QGraphicsScene &scene,
                                   const DocumentModelCoords &modelCoords,
                                   const Transformation &transformation)
{
}

void CentipedeDebugPolar::displayTics (QGraphicsScene &scene,
                                       const Transformation &transformation,
                                       const QPointF &posOriginScreen,
                                       const QPointF &posAAxisScreen,
                                       const QColor &colorGraphCoordinates,
                                       const QColor &colorScreenCoordinates)
{
  const int DEGREES_BETWEEN_HIGHLIGHTS = 10;

  // Legend
  addToLegend (scene,
               "Graphics Coords",
               colorGraphCoordinates);
  addToLegend (scene,
               "Screen Coords",
               colorScreenCoordinates);

  // Orthogonal basis vectors aligned to orthognal axes of ellipse, in screen coordinates
  QPointF basisX = (posAAxisScreen - posOriginScreen) / magnitude (posAAxisScreen - posOriginScreen);
  QPointF basisY (basisX.y(),
                  -1.0 * basisX.x());

  // Show one inner set of radial tic lines and one outer set of radial tic lines
  double cosOffset = qCos (m_angleGraphAxisFromScreenAxis);
  double sinOffset = qSin (m_angleGraphAxisFromScreenAxis);
  for (int degrees = 0; degrees < 360; degrees++) {

    QString degreesMinus180ToPlus180 = QString::number (degrees > 180 ?
                                                          degrees - 360 :
                                                          degrees);

    // Inner set of radial tic lines is regularly spaced in screen coordinates. We solve x=r*cosT, y=r*sinT,
    // x^2/a^2 + y^2/b^2 = 1. Ellipse points are then rotated
    double radians = qDegreesToRadians ((double) degrees) - m_angleEllipseFromMajorAxis;
    double cosLoop = qCos(-1.0 * radians - m_angleEllipseFromMajorAxis);
    double sinLoop = qSin(-1.0 * radians - m_angleEllipseFromMajorAxis);
    double denominator = cosLoop * cosLoop / m_aAligned / m_aAligned  + sinLoop * sinLoop / m_bAligned / m_bAligned;
    double radius = qSqrt (1.0 / denominator);
    double x = radius * cosLoop;
    double y = radius * sinLoop;
    double xRotated = cosOffset * x - sinOffset * y;
    double yRotated = sinOffset * x + cosOffset * y;
    QPointF posRadial (xRotated,
                       yRotated);

    QLineF linePortion = portionOfLineLast (QLineF (posOriginScreen,
                                                    posOriginScreen + posRadial),
                                            degrees,
                                            DEGREES_BETWEEN_HIGHLIGHTS);
    QGraphicsLineItem *radialFirst = new QGraphicsLineItem (linePortion);
    radialFirst->setPen (QPen (colorScreenCoordinates));
    scene.addItem (radialFirst);

    if (degrees % DEGREES_BETWEEN_HIGHLIGHTS == 0) {
      QGraphicsTextItem *labelFirst = new QGraphicsTextItem (degreesMinus180ToPlus180);
      labelFirst->setPos (linePortion.p1());
      labelFirst->setDefaultTextColor (colorScreenCoordinates);
      scene.addItem (labelFirst);
    }

    // Second set of radial tic lines is regularly spaced in graph coordinates
    transformation.transformRawGraphToScreen (QPointF (degrees,
                                                       m_radius),
                                              posRadial);

    linePortion = portionOfLineNext (QLineF (posOriginScreen,
                                             posRadial),
                                     degrees,
                                     DEGREES_BETWEEN_HIGHLIGHTS);
    QGraphicsLineItem *radialSecond = new QGraphicsLineItem (linePortion);
    radialSecond->setPen (QPen (colorGraphCoordinates));
    scene.addItem (radialSecond);

    if (degrees % DEGREES_BETWEEN_HIGHLIGHTS == 0) {
      QGraphicsTextItem *labelSecond = new QGraphicsTextItem (degreesMinus180ToPlus180);
      labelSecond->setPos (linePortion.p1());
      labelSecond->setDefaultTextColor (colorGraphCoordinates);
      scene.addItem (labelSecond);
    }
  }
}

void CentipedeDebugPolar::dumpEllipseGraphicsItem (const QString &callerMethod,
                                                   const QGraphicsEllipseItem *ellipse) const
{
}

QLineF CentipedeDebugPolar::portionOfLineLast (const QLineF &line,
                                               int degrees,
                                               int degreesBetweenHighlights) const
{
  double s = (degrees % degreesBetweenHighlights == 0 ? 0.9 : 0.975);
  QPointF posNewStart = (1.0 - s) * line.p1() + s * line.p2 ();
  return QLineF(posNewStart,
                line.p2());
}

QLineF CentipedeDebugPolar::portionOfLineNext (const QLineF &line,
                                               int degrees,
                                               int degreesBetweenHighlights) const
{
  double s = (degrees % degreesBetweenHighlights == 0 ? 1.1 : 1.025);
  QPointF posNewStart = (1.0 - s) * line.p1() + s * line.p2 ();
  return QLineF(posNewStart,
                line.p2());
}

QPointF CentipedeDebugPolar::posScreenParallelogramBL () const
{
  return m_posScreenParallelogramBL;
}

QPointF CentipedeDebugPolar::posScreenParallelogramBR () const
{
  return m_posScreenParallelogramBR;
}

QPointF CentipedeDebugPolar::posScreenParallelogramTL () const
{
  return m_posScreenParallelogramTL;
}

QPointF CentipedeDebugPolar::posScreenParallelogramTR () const
{
  return m_posScreenParallelogramTR;
}

double CentipedeDebugPolar::radius () const
{
  return m_radius;
}
