/******************************************************************************************************
 * (C) 2014 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#include "CoordSystemContext.h"
#include "EngaugeAssert.h"
#include "Logger.h"

const CoordSystemIndex DEFAULT_COORD_SYSTEM_INDEX = 0;

CoordSystemContext::CoordSystemContext() :
  m_coordSystemIndex (DEFAULT_COORD_SYSTEM_INDEX)
{
}

CoordSystemContext::~CoordSystemContext()
{

  qDeleteAll (m_coordSystems);
}

void CoordSystemContext::addCoordSystems(unsigned int numberCoordSystemToAdd)
{

  // The CoordSystem vector is populated with defaults here
  for (unsigned int i = 0; i < numberCoordSystemToAdd; i++) {
    m_coordSystems.push_back (new CoordSystem ());
  }
}

void CoordSystemContext::addGraphCurveAtEnd (const QString &curveName)
{

  m_coordSystems [signed (m_coordSystemIndex)]->addGraphCurveAtEnd(curveName);
}

void CoordSystemContext::addPointAxisWithGeneratedIdentifier (const QPointF &posScreen,
                                                              const QPointF &posGraph,
                                                              QString &identifier,
                                                              double ordinal,
                                                              bool isXOnly)
{

  m_coordSystems [signed (m_coordSystemIndex)]->addPointAxisWithGeneratedIdentifier(posScreen,
                                                                                    posGraph,
                                                                                    identifier,
                                                                                    ordinal,
                                                                                    isXOnly);
}

void CoordSystemContext::addPointAxisWithSpecifiedIdentifier (const QPointF &posScreen,
                                                              const QPointF &posGraph,
                                                              const QString &identifier,
                                                              double ordinal,
                                                              bool isXOnly)
{

  m_coordSystems [signed (m_coordSystemIndex)]->addPointAxisWithSpecifiedIdentifier(posScreen,
                                                                                    posGraph,
                                                                                    identifier,
                                                                                    ordinal,
                                                                                    isXOnly);
}

void CoordSystemContext::addPointGraphWithGeneratedIdentifier (const QString &curveName,
                                                               const QPointF &posScreen,
                                                               QString &generatedIdentifier,
                                                               double ordinal)
{

  m_coordSystems [signed (m_coordSystemIndex)]->addPointGraphWithGeneratedIdentifier(curveName,
                                                                                     posScreen,
                                                                                     generatedIdentifier,
                                                                                     ordinal);
}

void CoordSystemContext::addPointGraphWithSpecifiedIdentifier (const QString &curveName,
                                                               const QPointF &posScreen,
                                                               const QString &identifier,
                                                               double ordinal)
{

  m_coordSystems [signed (m_coordSystemIndex)]->addPointGraphWithSpecifiedIdentifier(curveName,
                                                                                     posScreen,
                                                                                     identifier,
                                                                                     ordinal);
}

void CoordSystemContext::addPointsInCurvesGraphs (CurvesGraphs &curvesGraphs)
{

  m_coordSystems [signed (m_coordSystemIndex)]->addPointsInCurvesGraphs(curvesGraphs);
}

void CoordSystemContext::checkAddPointAxis (const QPointF &posScreen,
                                            const QPointF &posGraph,
                                            bool &isError,
                                            QString &errorMessage,
                                            bool isXOnly,
                                            DocumentAxesPointsRequired documentAxesPointsRequired)
{

  m_coordSystems [signed (m_coordSystemIndex)]->checkAddPointAxis(posScreen,
                                                                  posGraph,
                                                                  isError,
                                                                  errorMessage,
                                                                  isXOnly,
                                                                  documentAxesPointsRequired);
}

void CoordSystemContext::checkEditPointAxis (const QString &pointIdentifier,
                                             const QPointF &posScreen,
                                             const QPointF &posGraph,
                                             bool &isError,
                                             QString &errorMessage,
                                             DocumentAxesPointsRequired documentAxesPointsRequired)
{

  m_coordSystems [signed (m_coordSystemIndex)]->checkEditPointAxis(pointIdentifier,
                                                                   posScreen,
                                                                   posGraph,
                                                                   isError,
                                                                   errorMessage,
                                                                   documentAxesPointsRequired);
}

const CoordSystem &CoordSystemContext::coordSystem () const
{

  return *(m_coordSystems [signed (m_coordSystemIndex)]);
}

unsigned int CoordSystemContext::coordSystemCount() const
{
  return unsigned (m_coordSystems.count());
}

CoordSystemIndex CoordSystemContext::coordSystemIndex () const
{
  return m_coordSystemIndex;
}

const Curve &CoordSystemContext::curveAxes () const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->curveAxes();
}

Curve *CoordSystemContext::curveForCurveName (const QString &curveName)
{

  return m_coordSystems [signed (m_coordSystemIndex)]->curveForCurveName(curveName);
}

const Curve *CoordSystemContext::curveForCurveName (const QString &curveName) const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->curveForCurveName(curveName);
}

const CurvesGraphs &CoordSystemContext::curvesGraphs () const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->curvesGraphs();
}

QStringList CoordSystemContext::curvesGraphsNames () const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->curvesGraphsNames();
}

int CoordSystemContext::curvesGraphsNumPoints (const QString &curveName) const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->curvesGraphsNumPoints(curveName);
}

void CoordSystemContext::editPointAxis (const QPointF &posGraph,
                                        const QString &identifier)
{

  m_coordSystems [signed (m_coordSystemIndex)]->editPointAxis(posGraph,
                                                              identifier);
}

void CoordSystemContext::editPointGraph (bool isX,
                                         bool isY,
                                         double x,
                                         double y,
                                         const QStringList &identifiers,
                                         const Transformation &transformation)
{

  m_coordSystems [signed (m_coordSystemIndex)]->editPointGraph (isX,
                                                                isY,
                                                                x,
                                                                y,
                                                                identifiers,
                                                                transformation);
}

bool CoordSystemContext::isXOnly (const QString &pointIdentifier) const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->isXOnly (pointIdentifier);
}

void CoordSystemContext::iterateThroughCurvePointsAxes (const Functor2wRet<const QString &, const Point &, CallbackSearchReturn> &ftorWithCallback)
{

  m_coordSystems [signed (m_coordSystemIndex)]->iterateThroughCurvePointsAxes(ftorWithCallback);
}

void CoordSystemContext::iterateThroughCurvePointsAxes (const Functor2wRet<const QString  &, const Point &, CallbackSearchReturn> &ftorWithCallback) const
{

  m_coordSystems [signed (m_coordSystemIndex)]->iterateThroughCurvePointsAxes(ftorWithCallback);
}

void CoordSystemContext::iterateThroughCurveSegments (const QString &curveName,
                                                      const Functor2wRet<const Point &, const Point &, CallbackSearchReturn> &ftorWithCallback) const
{

  m_coordSystems [signed (m_coordSystemIndex)]->iterateThroughCurveSegments(curveName,
                                                                            ftorWithCallback);
}

void CoordSystemContext::iterateThroughCurvesPointsGraphs (const Functor2wRet<const QString &, const Point &, CallbackSearchReturn> &ftorWithCallback)
{

  m_coordSystems [signed (m_coordSystemIndex)]->iterateThroughCurvesPointsGraphs(ftorWithCallback);
}

void CoordSystemContext::iterateThroughCurvesPointsGraphs (const Functor2wRet<const QString &, const Point &, CallbackSearchReturn> &ftorWithCallback) const
{

  m_coordSystems [signed (m_coordSystemIndex)]->iterateThroughCurvesPointsGraphs(ftorWithCallback);
}

bool CoordSystemContext::loadCurvesFile (const QString &curvesFile)
{

  return m_coordSystems [signed (m_coordSystemIndex)]->loadCurvesFile (curvesFile);
}

void CoordSystemContext::loadPreVersion6 (QDataStream &str,
                                          double version,
                                          DocumentAxesPointsRequired &documentAxesPointsRequired)
{

  m_coordSystems [signed (m_coordSystemIndex)]->loadPreVersion6 (str,
                                                                 version,
                                                                 documentAxesPointsRequired);
}

void CoordSystemContext::loadVersion6 (QXmlStreamReader &reader,
                                       DocumentAxesPointsRequired &documentAxesPointsRequired)
{

  m_coordSystems [signed (m_coordSystemIndex)]->loadVersion6 (reader,
                                                              documentAxesPointsRequired);
}

void CoordSystemContext::loadVersions7AndUp (QXmlStreamReader &reader)
{

  int indexLast = m_coordSystems.count() - 1;
  m_coordSystems [indexLast]->loadVersions7AndUp (reader);
}

DocumentModelAxesChecker CoordSystemContext::modelAxesChecker() const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->modelAxesChecker();
}

DocumentModelColorFilter CoordSystemContext::modelColorFilter() const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->modelColorFilter();
}

DocumentModelCoords CoordSystemContext::modelCoords () const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->modelCoords();
}

CurveStyles CoordSystemContext::modelCurveStyles() const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->modelCurveStyles();
}

DocumentModelDigitizeCurve CoordSystemContext::modelDigitizeCurve() const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->modelDigitizeCurve();
}

DocumentModelExportFormat CoordSystemContext::modelExport() const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->modelExport();
}

DocumentModelGeneral CoordSystemContext::modelGeneral() const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->modelGeneral();
}

DocumentModelGridDisplay CoordSystemContext::modelGridDisplay() const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->modelGridDisplay();
}

DocumentModelGridRemoval CoordSystemContext::modelGridRemoval() const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->modelGridRemoval();
}

DocumentModelGuideline CoordSystemContext::modelGuideline() const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->modelGuideline();
}

DocumentModelPointMatch CoordSystemContext::modelPointMatch() const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->modelPointMatch();
}

DocumentModelSegments CoordSystemContext::modelSegments() const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->modelSegments();
}

void CoordSystemContext::movePoint (const QString &pointIdentifier,
                                    const QPointF &deltaScreen)
{

  return m_coordSystems [signed (m_coordSystemIndex)]->movePoint(pointIdentifier,
                                                                 deltaScreen);
}

int CoordSystemContext::nextOrdinalForCurve (const QString &curveName) const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->nextOrdinalForCurve(curveName);
}

QPointF CoordSystemContext::positionGraph (const QString &pointIdentifier) const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->positionGraph(pointIdentifier);
}

QPointF CoordSystemContext::positionScreen (const QString &pointIdentifier) const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->positionScreen(pointIdentifier);
}

void CoordSystemContext::print () const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->print();
}

void CoordSystemContext::printStream (QString indentation,
                                      QTextStream &str) const
{

  m_coordSystems [signed (m_coordSystemIndex)]->printStream(indentation,
                                                            str);
}

QString CoordSystemContext::reasonForUnsuccessfulRead () const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->reasonForUnsuccessfulRead();
}

void CoordSystemContext::removePointAxis (const QString &identifier)
{

  m_coordSystems [signed (m_coordSystemIndex)]->removePointAxis(identifier);
}

void CoordSystemContext::removePointGraph (const QString &identifier)
{

  m_coordSystems [signed (m_coordSystemIndex)]->removePointGraph(identifier);
}

void CoordSystemContext::removePointsInCurvesGraphs (CurvesGraphs &curvesGraphs)
{

  m_coordSystems [signed (m_coordSystemIndex)]->removePointsInCurvesGraphs(curvesGraphs);
}

void CoordSystemContext::saveXml (QXmlStreamWriter &writer) const
{

  for (int index = 0; index < m_coordSystems.count(); index++) {
    m_coordSystems [index]->saveXml (writer);
  }
}

QString CoordSystemContext::selectedCurveName () const
{
  return m_coordSystems [signed (m_coordSystemIndex)]->selectedCurveName();
}

void CoordSystemContext::setCoordSystemIndex(CoordSystemIndex coordSystemIndex)
{

  ENGAUGE_ASSERT(coordSystemIndex < unsigned (m_coordSystems.count()));

  m_coordSystemIndex = coordSystemIndex;
}

void CoordSystemContext::setCurveAxes (const Curve &curveAxes)
{

  m_coordSystems [signed (m_coordSystemIndex)]->setCurveAxes(curveAxes);
}

void CoordSystemContext::setCurvesGraphs (const CurvesGraphs &curvesGraphs)
{

  m_coordSystems [signed (m_coordSystemIndex)]->setCurvesGraphs(curvesGraphs);
}

void CoordSystemContext::setModelAxesChecker(const DocumentModelAxesChecker &modelAxesChecker)
{

  m_coordSystems [signed (m_coordSystemIndex)]->setModelAxesChecker(modelAxesChecker);
}

void CoordSystemContext::setModelColorFilter(const DocumentModelColorFilter &modelColorFilter)
{

  m_coordSystems [signed (m_coordSystemIndex)]->setModelColorFilter(modelColorFilter);
}

void CoordSystemContext::setModelCoords (const DocumentModelCoords &modelCoords)
{

  m_coordSystems [signed (m_coordSystemIndex)]->setModelCoords(modelCoords);
}

void CoordSystemContext::setModelCurveStyles(const CurveStyles &modelCurveStyles)
{

  m_coordSystems [signed (m_coordSystemIndex)]->setModelCurveStyles(modelCurveStyles);
}

void CoordSystemContext::setModelDigitizeCurve (const DocumentModelDigitizeCurve &modelDigitizeCurve)
{

  m_coordSystems [signed (m_coordSystemIndex)]->setModelDigitizeCurve(modelDigitizeCurve);
}

void CoordSystemContext::setModelExport(const DocumentModelExportFormat &modelExport)
{

  m_coordSystems [signed (m_coordSystemIndex)]->setModelExport (modelExport);
}

void CoordSystemContext::setModelGeneral (const DocumentModelGeneral &modelGeneral)
{

  m_coordSystems [signed (m_coordSystemIndex)]->setModelGeneral(modelGeneral);
}

void CoordSystemContext::setModelGridDisplay(const DocumentModelGridDisplay &modelGridDisplay)
{

  m_coordSystems [signed (m_coordSystemIndex)]->setModelGridDisplay(modelGridDisplay);
}

void CoordSystemContext::setModelGridRemoval(const DocumentModelGridRemoval &modelGridRemoval)
{

  m_coordSystems [signed (m_coordSystemIndex)]->setModelGridRemoval(modelGridRemoval);
}

void CoordSystemContext::setModelGuideline(const DocumentModelGuideline &modelGuideline)
{

  m_coordSystems [signed (m_coordSystemIndex)]->setModelGuideline(modelGuideline);
}

void CoordSystemContext::setModelPointMatch(const DocumentModelPointMatch &modelPointMatch)
{

  m_coordSystems [signed (m_coordSystemIndex)]->setModelPointMatch(modelPointMatch);
}

void CoordSystemContext::setModelSegments(const DocumentModelSegments &modelSegments)
{

  m_coordSystems [signed (m_coordSystemIndex)]->setModelSegments(modelSegments);
}

void CoordSystemContext::setSelectedCurveName(const QString &selectedCurveName)
{
  m_coordSystems [signed (m_coordSystemIndex)]->setSelectedCurveName(selectedCurveName);
}

bool CoordSystemContext::successfulRead () const
{

  return m_coordSystems [signed (m_coordSystemIndex)]->successfulRead();
}

void CoordSystemContext::updatePointOrdinals (const Transformation &transformation)
{

  m_coordSystems [signed (m_coordSystemIndex)]->updatePointOrdinals(transformation);
}
