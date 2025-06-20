/******************************************************************************************************
 * (C) 2014 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#include "DigitizeStateEmpty.h"
#include "DigitizeStateContext.h"
#include "Logger.h"
#include "MainWindow.h"
#include <QCursor>
#include <QSize>
#include "Transformation.h"

DigitizeStateEmpty::DigitizeStateEmpty (DigitizeStateContext &context) :
  DigitizeStateAbstractBase (context)
{
}

DigitizeStateEmpty::~DigitizeStateEmpty ()
{
}

QString DigitizeStateEmpty::activeCurve () const
{
  return "";
}

void DigitizeStateEmpty::begin (CmdMediator *cmdMediator,
                                DigitizeState /* previousState */)
{

  setCursor(cmdMediator);
  context().mainWindow().handleGuidelinesActiveChange (false);
  context().mainWindow().updateViewsOfSettings(activeCurve ());
}

bool DigitizeStateEmpty::canPaste (const Transformation & /* transformation */,
                                   const QSize & /* size */) const
{
  return false;
}

QCursor DigitizeStateEmpty::cursor(CmdMediator * /* cmdMediator */) const
{

  return QCursor (Qt::ArrowCursor);
}

void DigitizeStateEmpty::end ()
{
}

bool DigitizeStateEmpty::guidelinesAreSelectable () const
{
  return false;
}

void DigitizeStateEmpty::handleContextMenuEventAxis (CmdMediator * /* cmdMediator */,
                                                     const QString &pointIdentifier)
{
}

void DigitizeStateEmpty::handleContextMenuEventGraph (CmdMediator * /* cmdMediator */,
                                                      const QStringList &pointIdentifiers)
{
}

void DigitizeStateEmpty::handleCurveChange(CmdMediator * /* cmdMediator */)
{
}

void DigitizeStateEmpty::handleKeyPress (CmdMediator * /* cmdMediator */,
                                         Qt::Key key,
                                         bool /* atLeastOneSelectedItem */)
{
}

void DigitizeStateEmpty::handleMouseMove (CmdMediator * /* cmdMediator */,
                                          QPointF /* posScreen */)
{
}

void DigitizeStateEmpty::handleMousePress (CmdMediator * /* cmdMediator */,
                                           QPointF /* posScreen */)
{
}

void DigitizeStateEmpty::handleMouseRelease (CmdMediator * /* cmdMediator */,
                                             QPointF /* posScreen */)
{
}

QString DigitizeStateEmpty::state() const
{
  return "DigitizeStateEmpty";
}

void DigitizeStateEmpty::updateAfterPointAddition ()
{
}

void DigitizeStateEmpty::updateModelDigitizeCurve (CmdMediator * /* cmdMediator */,
                                                   const DocumentModelDigitizeCurve & /*modelDigitizeCurve */)
{
}

void DigitizeStateEmpty::updateModelSegments(const DocumentModelSegments & /* modelSegments */)
{
}
