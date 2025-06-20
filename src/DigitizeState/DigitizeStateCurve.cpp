/******************************************************************************************************
 * (C) 2014 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#include "CmdAddPointGraph.h"
#include "CmdMediator.h"
#include "CursorFactory.h"
#include "DigitizeStateContext.h"
#include "DigitizeStateCurve.h"
#include "Logger.h"
#include "MainWindow.h"
#include "OrdinalGenerator.h"
#include <QCursor>
#include <QGraphicsScene>
#include <QImage>
#include <QSize>
#include "Transformation.h"

DigitizeStateCurve::DigitizeStateCurve (DigitizeStateContext &context) :
  DigitizeStateAbstractBase (context)
{
}

DigitizeStateCurve::~DigitizeStateCurve ()
{
}

QString DigitizeStateCurve::activeCurve () const
{
  return context().mainWindow().selectedGraphCurve();
}

void DigitizeStateCurve::begin (CmdMediator *cmdMediator,
                                DigitizeState /* previousState */)
{

  setCursor(cmdMediator);
  context().setDragMode(QGraphicsView::NoDrag);
  context().mainWindow().handleGuidelinesActiveChange (false);
  context().mainWindow().updateViewsOfSettings(activeCurve ());
}

bool DigitizeStateCurve::canPaste (const Transformation &transformation,
                                   const QSize &size) const
{
  return canPasteProtected (transformation,
                            size);
}

QCursor DigitizeStateCurve::cursor(CmdMediator *cmdMediator) const
{

  CursorFactory cursorFactory;
  QCursor cursor = cursorFactory.generate (cmdMediator->document().modelDigitizeCurve());

  return cursor;
}

void DigitizeStateCurve::end ()
{
}

bool DigitizeStateCurve::guidelinesAreSelectable () const
{
  return false;
}

void DigitizeStateCurve::handleContextMenuEventAxis (CmdMediator * /* cmdMediator */,
                                                     const QString &pointIdentifier)
{
}

void DigitizeStateCurve::handleContextMenuEventGraph (CmdMediator * /* cmdMediator */,
                                                      const QStringList &pointIdentifiers)
{
}

void DigitizeStateCurve::handleCurveChange(CmdMediator * /* cmdMediator */)
{
}

void DigitizeStateCurve::handleKeyPress (CmdMediator *cmdMediator,
                                         Qt::Key key,
                                         bool atLeastOneSelectedItem)
{

  handleKeyPressArrow (cmdMediator,
                       key,
                       atLeastOneSelectedItem);
}

void DigitizeStateCurve::handleMouseMove (CmdMediator * /* cmdMediator */,
                                          QPointF /* posScreen */)
{
}

void DigitizeStateCurve::handleMousePress (CmdMediator * /* cmdMediator */,
                                           QPointF /* posScreen */)
{
}

void DigitizeStateCurve::handleMouseRelease (CmdMediator *cmdMediator,
                                             QPointF posScreen)
{

  // Create command to add point
  OrdinalGenerator ordinalGenerator;
  Document &document = cmdMediator->document ();
  const Transformation &transformation = context ().mainWindow ().transformation();
  QUndoCommand *cmd = new CmdAddPointGraph (context ().mainWindow(),
                                            document,
                                            context ().mainWindow().selectedGraphCurve(),
                                            posScreen,
                                            ordinalGenerator.generateCurvePointOrdinal(document,
                                                                                       transformation,
                                                                                       posScreen,
                                                                                       activeCurve ()));
  context().appendNewCmd(cmdMediator,
                         cmd);
}

QString DigitizeStateCurve::state() const
{
  return "DigitizeStateCurve";
}

void DigitizeStateCurve::updateAfterPointAddition ()
{
}

void DigitizeStateCurve::updateModelDigitizeCurve (CmdMediator *cmdMediator,
                                                   const DocumentModelDigitizeCurve & /*modelDigitizeCurve */)
{

  setCursor(cmdMediator);
}

void DigitizeStateCurve::updateModelSegments(const DocumentModelSegments & /* modelSegments */)
{
}
