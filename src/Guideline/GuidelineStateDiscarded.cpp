/******************************************************************************************************
 * (C) 2019 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#include "EngaugeAssert.h"
#include "GuidelineAbstract.h"
#include "GuidelineState.h"
#include "GuidelineStateContext.h"
#include "GuidelineStateDiscarded.h"
#include "Logger.h"
#include <QPen>
#include "ZValues.h"

GuidelineStateDiscarded::GuidelineStateDiscarded (GuidelineStateContext &context) :
  GuidelineStateAbstractBase (context)
{
}

GuidelineStateDiscarded::~GuidelineStateDiscarded ()
{
}

void GuidelineStateDiscarded::begin ()
{
  context().guideline().setGraphicsItemZValue (Z_VALUE_GUIDELINE_DISCARDED);
  context().guideline().setGraphicsItemVisible (false); //
  context().guideline().setGraphicsItemAcceptHoverEvents (false);
  context().guideline().setGraphicsItemPen (QColor (Qt::transparent),
                                            context ().modelGuideline ().lineWidthInactive ());
}

QPointF GuidelineStateDiscarded::convertGraphCoordinateToScreenPoint (double /* valueGraph */) const
{
  ENGAUGE_ASSERT (false);

  return QPointF (0, 0); // No-harm value
}

double GuidelineStateDiscarded::convertScreenPointToGraphCoordinate (const QPointF & /* posScreen */) const
{
  ENGAUGE_ASSERT (false);

  return 0.1; // No-harm value even if using log scale
}

bool GuidelineStateDiscarded::doPaint () const
{
  return false;
}

void GuidelineStateDiscarded::end ()
{
}

void GuidelineStateDiscarded::handleActiveChange (bool /* active */)
{
}

void GuidelineStateDiscarded::handleGuidelineMode (bool /* visible */,
                                                   bool /* locked */)
{
}

void GuidelineStateDiscarded::handleHoverEnterEvent ()
{
  // Noop. Never called since hover only works when visible
}

void GuidelineStateDiscarded::handleHoverLeaveEvent ()
{
  // Noop. Never called since hover only works when visible
}

void GuidelineStateDiscarded::handleMousePress (const QPointF & /* posScene */)
{
  // Noop
}

void GuidelineStateDiscarded::handleMouseRelease (const QPointF & /* posScene */)
{
  // Noop
}

void GuidelineStateDiscarded::handleState ()
{
  // Noop
}

void GuidelineStateDiscarded::handleTimeout ()
{
  // Noop
}

EllipseParameters GuidelineStateDiscarded::pointToEllipse (const QPointF & /* poscreen */) const
{
  return EllipseParameters();
}

QLineF GuidelineStateDiscarded::pointToLine (const QPointF & /* posGraph */) const
{
  return QLineF (0, 0, 0, 0);
}

QString GuidelineStateDiscarded::stateName () const
{
  return guidelineStateAsString (GUIDELINE_STATE_DISCARDED);
}

void GuidelineStateDiscarded::updateWithLatestTransformation ()
{
  // Noop
}
