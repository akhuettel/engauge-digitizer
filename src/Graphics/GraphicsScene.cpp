/******************************************************************************************************
 * (C) 2014 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#include "CallbackSceneUpdateAfterCommand.h"
#include "Curve.h"
#include "CurvesGraphs.h"
#include "CurveStyles.h"
#include "DataKey.h"
#include "EngaugeAssert.h"
#include "EnumsToQt.h"
#include "GeometryWindow.h"
#include "GraphicsItemType.h"
#include "GraphicsPoint.h"
#include "GraphicsPointFactory.h"
#include "GraphicsScene.h"
#include "Logger.h"
#include "MainWindow.h"
#include "Point.h"
#include "PointStyle.h"
#include <QApplication>
#include <QGraphicsItem>
#include "QtToString.h"
#include "SplineDrawer.h"
#include "Transformation.h"

GraphicsScene::GraphicsScene(MainWindow *mainWindow) :
  QGraphicsScene(mainWindow),
  m_pathItemMultiValued (nullptr)
{
}

GraphicsScene::~GraphicsScene()
{
}

void GraphicsScene::addTemporaryPoint (const QString &identifier,
                                       GraphicsPoint *point)
{

  m_graphicsLinesForCurves.addPoint (AXIS_CURVE_NAME,
                                     identifier,
                                     Point::UNDEFINED_ORDINAL (),
                                     *point);
}

void GraphicsScene::addTemporaryScaleBar (GraphicsPoint *point0,
                                          GraphicsPoint *point1,
                                          const QString &pointIdentifier0,
                                          const QString &pointIdentifier1)
{

  const double ORDINAL_0 = 0, ORDINAL_1 = 1;

  m_graphicsLinesForCurves.addPoint (AXIS_CURVE_NAME,
                                     pointIdentifier0,
                                     ORDINAL_0,
                                     *point0);
  m_graphicsLinesForCurves.addPoint (AXIS_CURVE_NAME,
                                     pointIdentifier1,
                                     ORDINAL_1,
                                     *point1);
}

GraphicsPoint *GraphicsScene::createPoint (const QString &identifier,
                                           const PointStyle &pointStyle,
                                           const QPointF &posScreen,
                                           GeometryWindow *geometryWindow)
{

  // Ordinal value is initially computed as one plus the max ordinal seen so far. This initial ordinal value will be overridden if the
  // cordinates determine the ordinal values.
  //
  // This is an N-squared algorithm and may be worth replacing later
  GraphicsPointFactory pointFactory;
  GraphicsPoint *point = pointFactory.createPoint (*this,
                                                   identifier,
                                                   posScreen,
                                                   pointStyle,
                                                   geometryWindow);

  point->setData (DATA_KEY_GRAPHICS_ITEM_TYPE, GRAPHICS_ITEM_TYPE_POINT);

  return point;
}

QString GraphicsScene::dumpCursors () const
{
  QString cursorOverride = (QApplication::overrideCursor () != nullptr) ?
                             QtCursorToString (QApplication::overrideCursor ()->shape ()) :
                             "<null>";
  QString cursorImage = QtCursorToString (image()->cursor().shape ());

  QString dump = QString ("overrideCursor=%1 imageCursor=%2")
      .arg (cursorOverride)
      .arg (cursorImage);

  return dump;
}

void GraphicsScene::hideAllItemsExceptImage()
{

  for (int index = 0; index < QGraphicsScene::items().count(); index++) {
    QGraphicsItem *item = QGraphicsScene::items().at(index);

    if (item->data (DATA_KEY_GRAPHICS_ITEM_TYPE).toInt() == GRAPHICS_ITEM_TYPE_IMAGE) {

      item->show();

    } else {

      item->hide();

    }
  }
}

const QGraphicsPixmapItem *GraphicsScene::image () const
{
  // Loop through items in scene to find the image
  QList<QGraphicsItem*> items = QGraphicsScene::items();
  QList<QGraphicsItem*>::iterator itr;
  for (itr = items.begin(); itr != items.end(); itr++) {

    QGraphicsItem* item = *itr;
    if (item->data (DATA_KEY_GRAPHICS_ITEM_TYPE).toInt () == GRAPHICS_ITEM_TYPE_IMAGE) {

      return dynamic_cast<QGraphicsPixmapItem *> (item);
    }
  }

  return nullptr;
}

QStringList GraphicsScene::positionHasChangedPointIdentifiers () const
{

  QStringList movedIds;

  const QList<QGraphicsItem*> &items = QGraphicsScene::items();
  QList<QGraphicsItem*>::const_iterator itr;
  for (itr = items.begin(); itr != items.end(); itr++) {

    const QGraphicsItem *item = *itr;

    // Skip the image and only keep the Points
    bool isPoint = (item->data (DATA_KEY_GRAPHICS_ITEM_TYPE).toInt () == GRAPHICS_ITEM_TYPE_POINT);
    if (isPoint) {

      QString identifier = item->data (DATA_KEY_IDENTIFIER).toString ();
      bool positionHasChanged = item->data (DATA_KEY_POSITION_HAS_CHANGED).toBool ();

      if (isPoint && positionHasChanged) {

        // Add Point to the list
        movedIds << item->data(DATA_KEY_IDENTIFIER).toString ();

      }
    }
  }

  return  movedIds;
}

void GraphicsScene::printStream (QString indentation,
                                 QTextStream &str)
{
  m_graphicsLinesForCurves.printStream (indentation,
                                        str);
}

void GraphicsScene::removePoint (const QString &identifier)
{

  m_graphicsLinesForCurves.removePoint (identifier);
}

void GraphicsScene::removeTemporaryPointIfExists ()
{

  m_graphicsLinesForCurves.removeTemporaryPointIfExists ();
}

void GraphicsScene::removeTemporaryScaleBarIfExists ()
{
}

void GraphicsScene::resetOnLoad()
{

  int itemsBefore = items().count();

  m_graphicsLinesForCurves.resetOnLoad();

  int itemsAfter = items().count();

}

void GraphicsScene::resetPositionHasChangedFlags()
{

  QList<QGraphicsItem*> itms = items ();
  QList<QGraphicsItem*>::const_iterator itr;
  for (itr = itms.begin (); itr != itms.end (); itr++) {

    QGraphicsItem *item = *itr;
    item->setData (DATA_KEY_POSITION_HAS_CHANGED, false);
  }
}

void GraphicsScene::showCurves (bool show,
                                bool showAll,
                                const QString &curveNameWanted)
{
  const QList<QGraphicsItem*> &items = QGraphicsScene::items();
  QList<QGraphicsItem*>::const_iterator itr;
  for (itr = items.begin(); itr != items.end(); itr++) {

    QGraphicsItem* item = *itr;

    // Skip the image and only process the Points
    bool isPoint = (item->data (DATA_KEY_GRAPHICS_ITEM_TYPE).toInt () == GRAPHICS_ITEM_TYPE_POINT);
    bool isCurve = (item->data (DATA_KEY_GRAPHICS_ITEM_TYPE).toInt () == GRAPHICS_ITEM_TYPE_LINE);

    if (isPoint || isCurve) {

      bool showThis = show;
      if (show && !showAll) {
        QString identifier = item->data (DATA_KEY_IDENTIFIER).toString ();

        if (isPoint) {

          QString curveNameGot = Point::curveNameFromPointIdentifier (identifier);
          showThis = (curveNameWanted == curveNameGot);

        } else {

          showThis = (curveNameWanted == identifier);

        }
      }

      item->setVisible (showThis);

    }
  }
}

void GraphicsScene::updateAfterCommand (CmdMediator &cmdMediator,
                                        double highlightOpacity,
                                        GeometryWindow *geometryWindow,
                                        const Transformation &transformation)
{

  m_graphicsLinesForCurves.updateHighlightOpacity (highlightOpacity);

  updateCurves (cmdMediator);

  // Update the points
  updatePointMembership (cmdMediator,
                         geometryWindow,
                         transformation);
}

void GraphicsScene::updateCurves (CmdMediator &cmdMediator)
{

  // Desired curve names include both axes and graph curve names
  QStringList curveNames;
  curveNames << AXIS_CURVE_NAME;
  curveNames << cmdMediator.document().curvesGraphsNames();

  m_graphicsLinesForCurves.addRemoveCurves (*this,
                                            curveNames);
}

void GraphicsScene::updateCurveStyles (const CurveStyles &modelCurveStyles)
{

  m_graphicsLinesForCurves.updateCurveStyles (modelCurveStyles);
}

void GraphicsScene::updateGraphicsLinesToMatchGraphicsPoints (const CurveStyles &curveStyles,
                                                              const Transformation &transformation)
{

  if (transformation.transformIsDefined()) {

    // Ordinals must be updated to reflect reordering that may have resulted from dragging points
    m_graphicsLinesForCurves.updatePointOrdinalsAfterDrag (curveStyles,
                                                           transformation);

    // Recompute the lines one time for efficiency
    SplineDrawer splineDrawer (transformation);
    QPainterPath pathMultiValued;
    LineStyle lineMultiValued;
    m_graphicsLinesForCurves.updateGraphicsLinesToMatchGraphicsPoints (curveStyles,
                                                                       splineDrawer,
                                                                       pathMultiValued,
                                                                       lineMultiValued);

    updatePathItemMultiValued (pathMultiValued,
                               lineMultiValued);
  }
}

void GraphicsScene::updatePathItemMultiValued (const QPainterPath &pathMultiValued,
                                               const LineStyle &lineMultiValued)
{
  // It looks much better to use a consistent line width
  int lineWidth = signed (lineMultiValued.width());

  // Draw m_curveMultiValued. If empty then nothing will appear
  delete m_pathItemMultiValued;
  m_pathItemMultiValued = this->addPath (pathMultiValued);
  m_pathItemMultiValued->setPen (QPen (QBrush (QColor (Qt::red)),
                                       lineWidth,
                                       Qt::DotLine));
  m_pathItemMultiValued->setAcceptHoverEvents (true);
  m_pathItemMultiValued->setToolTip (tr ("Function currently has multiple Y values for one X value. Please adjust nearby points, "
                                         "or change the curve type in Curve Properties"));
}

void GraphicsScene::updatePointMembership (CmdMediator &cmdMediator,
                                           GeometryWindow *geometryWindow,
                                           const Transformation &transformation)
{

  CallbackSceneUpdateAfterCommand ftor (m_graphicsLinesForCurves,
                                        *this,
                                        cmdMediator.document (),
                                        geometryWindow);
  Functor2wRet<const QString &, const Point &, CallbackSearchReturn> ftorWithCallback = functor_ret (ftor,
                                                                                                     &CallbackSceneUpdateAfterCommand::callback);

  // First pass:
  // 1) Mark all points as Not Wanted (this is done while creating the map)
  m_graphicsLinesForCurves.lineMembershipReset ();

  // Next pass:
  // 1) Existing points that are found in the map are marked as Wanted
  // 2) Add new points that were just created in the Document. The new points are marked as Wanted
  cmdMediator.iterateThroughCurvePointsAxes (ftorWithCallback);
  cmdMediator.iterateThroughCurvesPointsGraphs (ftorWithCallback);

  // Next pass:
  // 1) Remove points that were just removed from the Document
  SplineDrawer splineDrawer (transformation);
  QPainterPath pathMultiValued;
  LineStyle lineMultiValued;
  m_graphicsLinesForCurves.lineMembershipPurge (cmdMediator.document().modelCurveStyles(),
                                                splineDrawer,
                                                pathMultiValued,
                                                lineMultiValued);
  updatePathItemMultiValued (pathMultiValued,
                             lineMultiValued);
}
