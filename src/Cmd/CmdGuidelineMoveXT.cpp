/******************************************************************************************************
 * (C) 2014 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#include "CmdGuidelineMoveXT.h"
#include "Document.h"
#include "DocumentSerialize.h"
#include "Logger.h"
#include "MainWindow.h"
#include "Xml.h"

const QString CMD_DESCRIPTION ("GuidelineMoveXT");

CmdGuidelineMoveXT::CmdGuidelineMoveXT(MainWindow &mainWindow,
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

CmdGuidelineMoveXT::CmdGuidelineMoveXT (MainWindow &mainWindow,
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

CmdGuidelineMoveXT::~CmdGuidelineMoveXT ()
{
}

void CmdGuidelineMoveXT::cmdRedo ()
{

  restoreState ();
  mainWindow().guidelineMoveXT (m_identifier,
                                m_valueAfter);
}

void CmdGuidelineMoveXT::cmdUndo ()
{

  restoreState ();
  mainWindow().guidelineMoveXT (m_identifier,
                                m_valueBefore);
}

void CmdGuidelineMoveXT::saveXml (QXmlStreamWriter &writer) const
{
  writer.writeStartElement(DOCUMENT_SERIALIZE_CMD);
  writer.writeAttribute(DOCUMENT_SERIALIZE_CMD_TYPE, DOCUMENT_SERIALIZE_CMD_GUIDELINE_MOVE_X_T);
  writer.writeAttribute(DOCUMENT_SERIALIZE_CMD_DESCRIPTION, QUndoCommand::text ());
  writer.writeAttribute(DOCUMENT_SERIALIZE_IDENTIFIER, m_identifier);
  writer.writeAttribute(DOCUMENT_SERIALIZE_GRAPH_BEFORE, QString::number (m_valueBefore));
  writer.writeAttribute(DOCUMENT_SERIALIZE_GRAPH_AFTER, QString::number (m_valueAfter));
  baseAttributes (writer);
  writer.writeEndElement();
}
