/******************************************************************************************************
 * (C) 2014 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#include "BackgroundStateContext.h"
#include "BackgroundStateOriginal.h"
#include "DocumentModelColorFilter.h"
#include "DocumentModelGridRemoval.h"
#include "GraphicsScene.h"
#include "GraphicsView.h"
#include "Logger.h"
#include <QPixmap>
#include "Transformation.h"

BackgroundStateOriginal::BackgroundStateOriginal(BackgroundStateContext &context,
                                                 GraphicsScene &scene) :
  BackgroundStateAbstractBase(context,
                              scene)
{
}

void BackgroundStateOriginal::begin()
{

  setImageVisible (true);
}

void BackgroundStateOriginal::end()
{

  setImageVisible (false);
}

void BackgroundStateOriginal::fitInView (GraphicsView &view)
{

  view.fitInView (imageItem ().boundingRect());
}

void BackgroundStateOriginal::setCurveSelected (bool /* isGnuplot */,
                                                const Transformation & /* transformation */,
                                                const DocumentModelGridRemoval & /* modelGridRemoval */,
                                                const DocumentModelColorFilter & /* modelColorFilter */,
                                                const QString & /* curveSelected */)
{
}

void BackgroundStateOriginal::setPixmap (bool /* isGnuplot */,
                                         const Transformation & /* transformation */,
                                         const DocumentModelGridRemoval & /* modelGridRemoval */,
                                         const DocumentModelColorFilter & /* modelColorFilter */,
                                         const QPixmap &pixmapOriginal,
                                         const QString & /* curveSelected */)
{

  // Unfiltered original image
  setProcessedPixmap (pixmapOriginal);
}

QString BackgroundStateOriginal::state () const
{
  return "BackgroundStateOriginal";
}

void BackgroundStateOriginal::updateColorFilter (bool /* isGnuplot */,
                                                 const Transformation & /* transformation */,
                                                 const DocumentModelGridRemoval & /* modelGridRemoval */,
                                                 const DocumentModelColorFilter & /* modelColorFilter */,
                                                 const QString & /* curveSelected */)
{
}
