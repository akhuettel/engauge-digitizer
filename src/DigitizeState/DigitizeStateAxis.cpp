/******************************************************************************************************
 * (C) 2014 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#include "CmdAddPointAxis.h"
#include "CmdMediator.h"
#include "CursorFactory.h"
#include "DigitizeStateAxis.h"
#include "DigitizeStateContext.h"
#include "DlgEditPointAxis.h"
#include "Document.h"
#include "GraphicsScene.h"
#include "GraphicsView.h"
#include "Logger.h"
#include "MainWindow.h"
#include "PointStyle.h"
#include <QCursor>
#include <QImage>
#include <QMessageBox>
#include <QTimer>

DigitizeStateAxis::DigitizeStateAxis (DigitizeStateContext &context) :
  DigitizeStateAbstractBase (context)
{
}

DigitizeStateAxis::~DigitizeStateAxis ()
{
}

QString DigitizeStateAxis::activeCurve () const
{
  return AXIS_CURVE_NAME;
}

void DigitizeStateAxis::begin (CmdMediator *cmdMediator,
                               DigitizeState /* previousState */)
{

  setCursor(cmdMediator);
  context().setDragMode(QGraphicsView::NoDrag);
  context().mainWindow().handleGuidelinesActiveChange (false);
  context().mainWindow().updateViewsOfSettings(activeCurve ());
}

bool DigitizeStateAxis::canPaste (const Transformation & /* transformation */,
                                  const QSize & /* size */) const
{
  return false;
}

void DigitizeStateAxis::createTemporaryPoint (CmdMediator *cmdMediator,
                                              const QPointF &posScreen)
{

  GeometryWindow *NULL_GEOMETRY_WINDOW = nullptr;

  // Temporary point that user can see while DlgEditPointAxis is active
  const Curve &curveAxes = cmdMediator->curveAxes();
  PointStyle pointStyleAxes = curveAxes.curveStyle().pointStyle();
  GraphicsPoint *point = context().mainWindow().scene().createPoint(Point::temporaryPointIdentifier (),
                                                                    pointStyleAxes,
                                                                    posScreen,
                                                                    NULL_GEOMETRY_WINDOW);

  context().mainWindow().scene().addTemporaryPoint (Point::temporaryPointIdentifier(),
                                                    point);
}

QCursor DigitizeStateAxis::cursor(CmdMediator *cmdMediator) const
{

  CursorFactory cursorFactory;
  QCursor cursor = cursorFactory.generate (cmdMediator->document().modelDigitizeCurve());

  return cursor;
}

void DigitizeStateAxis::end ()
{
}

bool DigitizeStateAxis::guidelinesAreSelectable () const
{
  return false;
}

void DigitizeStateAxis::handleContextMenuEventAxis (CmdMediator * /* cmdMediator */,
                                                    const QString &pointIdentifier)
{
}

void DigitizeStateAxis::handleContextMenuEventGraph (CmdMediator * /* cmdMediator */,
                                                     const QStringList &pointIdentifiers)
{
}

void DigitizeStateAxis::handleCurveChange(CmdMediator * /* cmdMediator */)
{
}

void DigitizeStateAxis::handleKeyPress (CmdMediator *cmdMediator,
                                        Qt::Key key,
                                        bool atLeastOneSelectedItem)
{

  handleKeyPressArrow (cmdMediator,
                       key,
                       atLeastOneSelectedItem);
}

void DigitizeStateAxis::handleMouseMove (CmdMediator * /* cmdMediator */,
                                         QPointF /* posScreen */)
{
}

void DigitizeStateAxis::handleMousePress (CmdMediator * /* cmdMediator */,
                                          QPointF /* posScreen */)
{
}

void DigitizeStateAxis::handleMouseRelease (CmdMediator *cmdMediator,
                                            QPointF posScreen)
{

  if (context().mainWindow().transformIsDefined()) {

    QMessageBox::warning (nullptr,
                          QObject::tr ("Engauge Digitizer"),
                          QObject::tr ("Three axis points have been defined, and no more are needed or allowed."));

  } else {

    createTemporaryPoint (cmdMediator,
                          posScreen);

    // Ask user for coordinates
    DlgEditPointAxis *dlg = new DlgEditPointAxis (context ().mainWindow (),
                                                  cmdMediator->document().modelCoords(),
                                                  cmdMediator->document().modelGeneral(),
                                                  context().mainWindow().modelMainWindow(),
                                                  context().mainWindow().transformation(),
                                                  cmdMediator->document().documentAxesPointsRequired());
    int rtn = dlg->exec ();

    bool isXOnly;
    QPointF posGraph = dlg->posGraph (isXOnly);
    delete dlg;

    // Remove temporary point
    context().mainWindow().scene().removePoint(Point::temporaryPointIdentifier ());

    if (rtn == QDialog::Accepted) {

      // User wants to add this axis point, but let's perform sanity checks first

      bool isError;
      QString errorMessage;
      int nextOrdinal = cmdMediator->document().nextOrdinalForCurve(AXIS_CURVE_NAME);

      cmdMediator->document().checkAddPointAxis(posScreen,
                                                posGraph,
                                                isError,
                                                errorMessage,
                                                isXOnly);

      if (isError) {

        QMessageBox::warning (nullptr,
                              QObject::tr ("Engauge Digitizer"),
                              errorMessage);

      } else {

        // Create command to add point
        Document &document = cmdMediator->document ();
        QUndoCommand *cmd = new CmdAddPointAxis (context ().mainWindow(),
                                                 document,
                                                 posScreen,
                                                 posGraph,
                                                 nextOrdinal,
                                                 isXOnly);
        context().appendNewCmd(cmdMediator,
                               cmd);
      }
    }
  }
}

QString DigitizeStateAxis::state() const
{
  return "DigitizeStateAxis";
}

void DigitizeStateAxis::updateAfterPointAddition ()
{
}

void DigitizeStateAxis::updateModelDigitizeCurve (CmdMediator *cmdMediator,
                                                  const DocumentModelDigitizeCurve & /*modelDigitizeCurve */)
{

  setCursor(cmdMediator);
}

void DigitizeStateAxis::updateModelSegments(const DocumentModelSegments & /* modelSegments */)
{
}
