/******************************************************************************************************
 * (C) 2014 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#include "CmdAddPointsGraph.h"
#include "DigitizeStateContext.h"
#include "DigitizeStateSegment.h"
#include "EngaugeAssert.h"
#include "GraphicsScene.h"
#include "Logger.h"
#include "MainWindow.h"
#include "OrdinalGenerator.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QImage>
#include <QSize>
#include "Segment.h"
#include "SegmentFactory.h"
#include "Transformation.h"

DigitizeStateSegment::DigitizeStateSegment (DigitizeStateContext &context) :
  DigitizeStateAbstractBase (context)
{
}

DigitizeStateSegment::~DigitizeStateSegment ()
{
}

QString DigitizeStateSegment::activeCurve () const
{
  return context().mainWindow().selectedGraphCurve();
}

void DigitizeStateSegment::begin (CmdMediator *cmdMediator,
                                  DigitizeState /* previousState */)
{

  m_cmdMediator = cmdMediator; // Save for slotMouseClickOnSegment

  setCursor(cmdMediator);
  context().setDragMode(QGraphicsView::NoDrag);
  context().mainWindow().handleGuidelinesActiveChange (false);
  context().mainWindow().updateViewsOfSettings(activeCurve ());

  handleCurveChange(cmdMediator);
}

bool DigitizeStateSegment::canPaste (const Transformation &transformation,
                                     const QSize &viewSize) const
{
  return canPasteProtected (transformation,
                            viewSize);
}

QCursor DigitizeStateSegment::cursor(CmdMediator * /* cmdMediator */) const
{

  return QCursor (Qt::ArrowCursor);
}

void DigitizeStateSegment::end ()
{

  GraphicsScene &scene = context().mainWindow().scene();
  SegmentFactory segmentFactory (dynamic_cast<QGraphicsScene &> (scene),
                                 context().isGnuplot());

  segmentFactory.clearSegments(m_segments);
}

bool DigitizeStateSegment::guidelinesAreSelectable () const
{
  return false;
}

void DigitizeStateSegment::handleContextMenuEventAxis (CmdMediator * /* cmdMediator */,
                                                       const QString &pointIdentifier)
{
}

void DigitizeStateSegment::handleContextMenuEventGraph (CmdMediator * /* cmdMediator */,
                                                        const QStringList &pointIdentifiers)
{
}

void DigitizeStateSegment::handleCurveChange(CmdMediator *cmdMediator)
{

  QImage img = context().mainWindow().imageFiltered();

  GraphicsScene &scene = context().mainWindow().scene();
  SegmentFactory segmentFactory (dynamic_cast<QGraphicsScene &> (scene),
                                 context().isGnuplot());

  segmentFactory.clearSegments (m_segments);

  // Create new segments
  segmentFactory.makeSegments (img,
                               cmdMediator->document().modelSegments(),
                               m_segments);

  // Connect signals of the new segments
  QList<Segment*>::iterator itr;
  for (itr = m_segments.begin(); itr != m_segments.end(); itr++) {
    Segment *segment = *itr;


    connect (segment, SIGNAL (signalMouseClickOnSegment (QPointF)), this, SLOT (slotMouseClickOnSegment (QPointF)));
  }
}

void DigitizeStateSegment::handleKeyPress (CmdMediator *cmdMediator,
                                           Qt::Key key,
                                           bool atLeastOneSelectedItem)
{

  handleKeyPressArrow (cmdMediator,
                       key,
                       atLeastOneSelectedItem);
}

void DigitizeStateSegment::handleMouseMove (CmdMediator * /* cmdMediator */,
                                            QPointF /* posScreen */)
{
}

void DigitizeStateSegment::handleMousePress (CmdMediator * /* cmdMediator */,
                                             QPointF /* posScreen */)
{
}

void DigitizeStateSegment::handleMouseRelease (CmdMediator * /* cmdMediator */,
                                               QPointF /* posScreen */)
{
}

Segment *DigitizeStateSegment::segmentFromSegmentStart (const QPointF &posSegmentStart) const
{

  QList<Segment*>::const_iterator itr;
  for (itr = m_segments.begin(); itr != m_segments.end(); itr++) {
    Segment *segment = *itr;

    if (segment->firstPoint() == posSegmentStart) {

      return segment;
    }
  }

  ENGAUGE_ASSERT (false);
  return nullptr;
}

void DigitizeStateSegment::slotMouseClickOnSegment(QPointF posSegmentStart)
{

  Segment *segment = segmentFromSegmentStart (posSegmentStart);

  // Create single-entry list that is expected by SegmentFactory
  QList<Segment*> segments;
  segments.push_back (segment);

  // Generate point coordinates. Nothing is created in the GraphicsScene at this point
  GraphicsScene &scene = context().mainWindow().scene();
  SegmentFactory segmentFactory (dynamic_cast<QGraphicsScene &> (scene),
                                 context().isGnuplot());

  QList<QPoint> points = segmentFactory.fillPoints (m_cmdMediator->document().modelSegments(),
                                                    segments);

  // Create one ordinal for each point
  OrdinalGenerator ordinalGenerator;
  Document &document = m_cmdMediator->document ();
  const Transformation &transformation = context ().mainWindow ().transformation();
  QList<double> ordinals;
  QList<QPoint>::iterator itr;
  for (itr = points.begin(); itr != points.end(); itr++) {

    QPoint point = *itr;
    ordinals << ordinalGenerator.generateCurvePointOrdinal(document,
                                                           transformation,
                                                           point,
                                                           activeCurve ());
  }

  // Create command to add points
  QUndoCommand *cmd = new CmdAddPointsGraph (context ().mainWindow(),
                                             document,
                                             context ().mainWindow().selectedGraphCurve(),
                                             points,
                                             ordinals);
  context().appendNewCmd(m_cmdMediator,
                         cmd);
}

QString DigitizeStateSegment::state() const
{
  return "DigitizeStateSegment";
}

void DigitizeStateSegment::updateAfterPointAddition ()
{
}

void DigitizeStateSegment::updateModelDigitizeCurve (CmdMediator * /* cmdMediator */,
                                                     const DocumentModelDigitizeCurve & /*modelDigitizeCurve */)
{
}

void DigitizeStateSegment::updateModelSegments(const DocumentModelSegments &modelSegments)
{

  QList<Segment*>::const_iterator itr;
  for (itr = m_segments.begin(); itr != m_segments.end(); itr++) {
    Segment *segment = *itr;

    segment->updateModelSegment (modelSegments);
  }
}
