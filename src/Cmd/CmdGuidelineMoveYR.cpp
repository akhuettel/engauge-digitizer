/******************************************************************************************************
 * (C) 2014 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#include "CmdGuidelineMoveYR.h"
#include "Document.h"
#include "DocumentSerialize.h"
#include "Logger.h"
#include "MainWindow.h"
#include "Xml.h"

const QString CMD_DESCRIPTION ("GuidelineMoveYR");

CmdGuidelineMoveYR::CmdGuidelineMoveYR(MainWindow &mainWindow,
                                       Document &document,
                                       const QString &identifier,
                                       double valueBefore,
                                       double valueAfter) :
  CmdAbstract(mainWindow,
              document,
              CMD_DESCRIPTION),
  m_identifier (identifier),
  m_valueBefore (valueBefore),
  m_valueAfter (valueAfter)
{
}

CmdGuidelineMoveYR::CmdGuidelineMoveYR (MainWindow &mainWindow,
                                        Document &document,
                                        const QString &cmdDescription,
                                        QXmlStreamReader &reader) :
  CmdAbstract (mainWindow,
               document,
               cmdDescription)
{

  QXmlStreamAttributes attributes = reader.attributes();

  QStringList requiredAttributesLeaf;
  requiredAttributesLeaf << DOCUMENT_SERIALIZE_IDENTIFIER
                         << DOCUMENT_SERIALIZE_GRAPH_BEFORE
                         << DOCUMENT_SERIALIZE_GRAPH_AFTER;
  leafAndBaseAttributes (attributes,
                         requiredAttributesLeaf,
                         reader);

  m_identifier = attributes.value(DOCUMENT_SERIALIZE_IDENTIFIER).toString();
  m_valueBefore = attributes.value(DOCUMENT_SERIALIZE_GRAPH_BEFORE).toDouble();
  m_valueAfter = attributes.value(DOCUMENT_SERIALIZE_GRAPH_AFTER).toDouble();
}

CmdGuidelineMoveYR::~CmdGuidelineMoveYR ()
{
}

void CmdGuidelineMoveYR::cmdRedo ()
{

  restoreState ();
  mainWindow().guidelineMoveYR (m_identifier,
                                m_valueAfter);
}

void CmdGuidelineMoveYR::cmdUndo ()
{

  restoreState ();
  mainWindow().guidelineMoveYR (m_identifier,
                                m_valueBefore);
}

void CmdGuidelineMoveYR::saveXml (QXmlStreamWriter &writer) const
{
  writer.writeStartElement(DOCUMENT_SERIALIZE_CMD);
  writer.writeAttribute(DOCUMENT_SERIALIZE_CMD_TYPE, DOCUMENT_SERIALIZE_CMD_GUIDELINE_MOVE_Y_R);
  writer.writeAttribute(DOCUMENT_SERIALIZE_CMD_DESCRIPTION, QUndoCommand::text ());
  writer.writeAttribute(DOCUMENT_SERIALIZE_IDENTIFIER, m_identifier);
  writer.writeAttribute(DOCUMENT_SERIALIZE_GRAPH_BEFORE, QString::number (m_valueBefore));
  writer.writeAttribute(DOCUMENT_SERIALIZE_GRAPH_AFTER, QString::number (m_valueAfter));
  baseAttributes (writer);
  writer.writeEndElement();
}
