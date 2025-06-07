/******************************************************************************************************
 * (C) 2014 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#include "Compatibility.h"
#include "DocumentModelSegments.h"
#include "EngaugeAssert.h"
#include "gnuplot.h"
#include <iostream>
#include "Logger.h"
#include "mmsubs.h"
#include <QFile>
#include <QGraphicsScene>
#include <qmath.h>
#include <QTextStream>
#include "QtToString.h"
#include "Segment.h"
#include "SegmentLine.h"

Segment::Segment(QGraphicsScene &scene,
                 int y,
                 bool isGnuplot) :
  m_scene (scene),
  m_yLast (y),
  m_length (0),
  m_isGnuplot (isGnuplot)
{
}

Segment::~Segment()
{
  QList<SegmentLine*>::iterator itr;
  for (itr = m_lines.begin(); itr != m_lines.end(); itr++) {

    SegmentLine *segmentLine = *itr;
    m_scene.removeItem (segmentLine);
  }
}

void Segment::appendColumn(int x,
                           int y,
                           const DocumentModelSegments &modelSegments)
{
  int xOld = x - 1;
  int yOld = m_yLast;
  int xNew = x;
  int yNew = y;

  SegmentLine* line = new SegmentLine(m_scene,
                                      modelSegments,
                                      this);
  ENGAUGE_CHECK_PTR(line);
  line->setLine(QLineF (xOld,
                        yOld,
                        xNew,
                        yNew));

  // Do not show this line or its segment. this is handled later

  m_lines.append(line);

  // Update total length using distance formula
  m_length += qSqrt((1.0) * (1.0) + (y - m_yLast) * (y - m_yLast));

  m_yLast = y;
}

void Segment::createAcceptablePoint(bool *pFirst,
                                    QList<QPoint> *pList,
                                    double *xPrev,
                                    double *yPrev,
                                    double x,
                                    double y)
{
  int iOld = qFloor (*xPrev + 0.5);
  int jOld = qFloor (*yPrev + 0.5);
  int i = qFloor (x + 0.5);
  int j = qFloor (y + 0.5);

  if (*pFirst || (iOld != i) || (jOld != j)) {
    *xPrev = x;
    *yPrev = y;

    ENGAUGE_CHECK_PTR(pList);
    pList->append(QPoint(i, j));
  }

  *pFirst = false;
}

void Segment::dumpToGnuplot (QTextStream &strDump,
                             int xInt,
                             int yInt,
                             const SegmentLine *lineOld,
                             const SegmentLine *lineNew) const
{
  // Only show this dump spew when logging is opened up completely
}

QList<QPoint> Segment::fillPoints(const DocumentModelSegments &modelSegments)
{

  if (modelSegments.fillCorners()) {
    return fillPointsFillingCorners(modelSegments);
  } else {
    return fillPointsWithoutFillingCorners(modelSegments);
  }
}

QList<QPoint> Segment::fillPointsFillingCorners(const DocumentModelSegments &modelSegments)
{
  QList<QPoint> list;

  if (m_lines.count() > 0) {

    double xLast = m_lines.first()->line().x1();
    double yLast = m_lines.first()->line().y1();
    double x, xNext;
    double y, yNext;
    double distanceCompleted = 0.0;

    // Variables for createAcceptablePoint
    bool firstPoint = true;
    double xPrev = m_lines.first()->line().x1();
    double yPrev = m_lines.first()->line().y1();

    QList<SegmentLine*>::iterator itr;
    for (itr = m_lines.begin(); itr != m_lines.end(); itr++) {

      SegmentLine *line = *itr;

      ENGAUGE_CHECK_PTR(line);
      xNext = double (line->line().x2());
      yNext = double (line->line().y2());

      double xStart = double (line->line().x1());
      double yStart = double (line->line().y1());
      if (isCorner (yPrev, yStart, yNext)) {

        // Insert a corner point
        createAcceptablePoint(&firstPoint, &list, &xPrev, &yPrev, xStart, yStart);
        distanceCompleted = 0.0;
      }

      // Distance formula
      double segmentLength = sqrt((xNext - xLast) * (xNext - xLast) + (yNext - yLast) * (yNext - yLast));
      if (segmentLength > 0.0) {

        // Loop since we might need to insert multiple points within a single line. This
        // is the case when removeUnneededLines has consolidated many segment lines
        while (distanceCompleted <= segmentLength) {

          double s = distanceCompleted / segmentLength;

          // Coordinates of new point
          x = (1.0 - s) * xLast + s * xNext;
          y = (1.0 - s) * yLast + s * yNext;

          createAcceptablePoint(&firstPoint, &list, &xPrev, &yPrev, x, y);

          distanceCompleted += modelSegments.pointSeparation();
        }

        distanceCompleted -= segmentLength;
      }

      xLast = xNext;
      yLast = yNext;
    }
  }

  return list;
}

QPointF Segment::firstPoint () const
{
  // There has to be at least one SegmentLine since this only gets called when a SegmentLine is clicked on
  ENGAUGE_ASSERT (m_lines.count () > 0);

  SegmentLine *line = m_lines.first();
  QPointF pos = line->line().p1();

  return pos;
}

void Segment::forwardMousePress()
{
  emit signalMouseClickOnSegment (firstPoint ());
}

bool Segment::isCorner (double yLast,
                        double yPrev,
                        double yNext) const
{
  // Rather than deal with slopes, and a risk of dividing by zero, we just use the y deltas
  double deltaYBefore = yPrev - yLast;
  double deltaYAfter = yNext - yPrev;
  bool upThenAcrossOrDown = (deltaYBefore > 0) && (deltaYAfter <= 0);
  bool downThenAcrossOrUp = (deltaYBefore < 0) && (deltaYAfter >= 0);

  return upThenAcrossOrDown || downThenAcrossOrUp;
}

QList<QPoint> Segment::fillPointsWithoutFillingCorners(const DocumentModelSegments &modelSegments)
{
  QList<QPoint> list;

  if (m_lines.count() > 0) {

    double xLast = m_lines.first()->line().x1();
    double yLast = m_lines.first()->line().y1();
    double x, xNext;
    double y, yNext;
    double distanceCompleted = 0.0;

    // Variables for createAcceptablePoint
    bool firstPoint = true;
    double xPrev = m_lines.first()->line().x1();
    double yPrev = m_lines.first()->line().y1();

    QList<SegmentLine*>::iterator itr;
    for (itr = m_lines.begin(); itr != m_lines.end(); itr++) {

      SegmentLine *line = *itr;

      ENGAUGE_CHECK_PTR(line);
      xNext = double (line->line().x2());
      yNext = double (line->line().y2());

      // Distance formula
      double segmentLength = sqrt((xNext - xLast) * (xNext - xLast) + (yNext - yLast) * (yNext - yLast));
      if (segmentLength > 0.0) {

        // Loop since we might need to insert multiple points within a single line. This
        // is the case when removeUnneededLines has consolidated many segment lines
        while (distanceCompleted <= segmentLength) {

          double s = distanceCompleted / segmentLength;

          // Coordinates of new point
          x = (1.0 - s) * xLast + s * xNext;
          y = (1.0 - s) * yLast + s * yNext;

          createAcceptablePoint(&firstPoint, &list, &xPrev, &yPrev, x, y);

          distanceCompleted += modelSegments.pointSeparation();
        }

        distanceCompleted -= segmentLength;
      }

      xLast = xNext;
      yLast = yNext;
    }
  }

  return list;
}

double Segment::length() const
{
  return m_length;
}

int Segment::lineCount() const
{
  return m_lines.count();
}

void Segment::lockHoverState()
{
  QList<SegmentLine*>::iterator itr;
  for (itr = m_lines.begin(); itr != m_lines.end(); itr++) {

    SegmentLine *line = *itr;
    line->setAcceptHoverEvents (false);
  }
}

bool Segment::pointIsCloseToLine(double xLeft,
                                 double yLeft,
                                 double xInt,
                                 double yInt,
                                 double xRight,
                                 double yRight)
{
  double xProj, yProj, projectedDistanceOutsideLine, distanceToLine;
  projectPointOntoLine(xInt, yInt, xLeft, yLeft, xRight, yRight, &xProj, &yProj, &projectedDistanceOutsideLine, &distanceToLine);

  return (
    (xInt - xProj) * (xInt - xProj) +
    (yInt - yProj) * (yInt - yProj) < 0.5 * 0.5);
}

bool Segment::pointsAreCloseToLine(double xLeft,
                                   double yLeft,
                                   QList<QPoint> removedPoints,
                                   double xRight,
                                   double yRight)
{
  QList<QPoint>::iterator itr;
  for (itr = removedPoints.begin(); itr != removedPoints.end(); ++itr) {
    if (!pointIsCloseToLine(xLeft,
                            yLeft,
                            double ((*itr).x()),
                            double ((*itr).y()),
                            xRight,
                            yRight)) {
      return false;
    }
  }

  return true;
}

void Segment::removeUnneededLines (int *foldedLines)
{

  QFile *fileDump = nullptr;
  QTextStream *strDump = nullptr;
  if (m_isGnuplot) {

    QString filename ("segment.gnuplot");

    std::cout << GNUPLOT_FILE_MESSAGE.toLatin1().data() << filename.toLatin1().data() << "\n";

    fileDump = new QFile (filename);
    fileDump->open (QIODevice::WriteOnly | QIODevice::Text);
    strDump = new QTextStream (fileDump);

  }

  // Pathological case is y=0.001*x*x, since the small slope can fool a naive algorithm
  // into optimizing away all but one point at the origin and another point at the far right.
  // From this we see that we cannot simply throw away points that were optimized away since they
  // are needed later to see if we have diverged from the curve
  SegmentLine *linePrevious = nullptr; // Previous line which corresponds to itrPrevious
  QList<SegmentLine*>::iterator itr, itrPrevious;
  QList<QPoint> removedPoints;
  for (itr = m_lines.begin(); itr != m_lines.end(); itr++) {

    SegmentLine *line = *itr;
    ENGAUGE_CHECK_PTR(line);

    if (linePrevious != nullptr) {

      double xLeft = linePrevious->line().x1();
      double yLeft = linePrevious->line().y1();
      double xInt = linePrevious->line().x2();
      double yInt = linePrevious->line().y2();

      // If linePrevious is the last line of one Segment and line is the first line of another Segment then
      // it makes no sense to remove any point so we continue the loop
      if (linePrevious->line().p2() == line->line().p1()) {

        double xRight = line->line().x2();
        double yRight = line->line().y2();

        if (pointIsCloseToLine(xLeft, yLeft, xInt, yInt, xRight, yRight) &&
          pointsAreCloseToLine(xLeft, yLeft, removedPoints, xRight, yRight)) {

          if (m_isGnuplot) {

            // Dump
            dumpToGnuplot (*strDump,
                           qFloor (xInt),
                           qFloor (yInt),
                           linePrevious,
                           line);
          }

          // Remove intermediate point, by removing older line and stretching new line to first point
          ++(*foldedLines);

          removedPoints.append(QPoint(qFloor (xInt),
                                      qFloor (yInt)));
          m_lines.erase (itrPrevious);
          delete linePrevious;

          // New line
          line->setLine (xLeft, yLeft, xRight, yRight);

        } else {

          // Keeping this intermediate point and clear out the removed points list
          removedPoints.clear();
        }
      }
    }

    linePrevious = line;
    itrPrevious = itr;

    // This theoretically should not be needed, but for some reason modifying the last point triggers a segfault
    if (itr == m_lines.end()) {
      break;
    }
  }

  if (strDump != nullptr) {

    // Final gnuplot processing
    *strDump << "set terminal x11 persist\n";
    fileDump->close ();
    delete strDump;
    delete fileDump;

  }
}

void Segment::slotHover (bool hover)
{

  QList<SegmentLine*>::iterator itr, itrPrevious;
  for (itr = m_lines.begin(); itr != m_lines.end(); itr++) {

    SegmentLine *line = *itr;
    line->setHover(hover);
  }
}

void Segment::updateModelSegment(const DocumentModelSegments &modelSegments)
{

  QList<SegmentLine*>::iterator itr;
  for (itr = m_lines.begin(); itr != m_lines.end(); itr++) {

    SegmentLine *line = *itr;
    line->updateModelSegment (modelSegments);
  }
}
