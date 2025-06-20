/******************************************************************************************************
 * (C) 2014 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#include "CmdGong.h"
#include "Document.h"
#include "DocumentSerialize.h"
#include "Logger.h"
#include "MainWindow.h"

const QString CMD_DESCRIPTION ("Gong");

CmdGong::CmdGong(MainWindow &mainWindow,
                 Document &document) :
  CmdAbstract(mainWindow,
              document,
              CMD_DESCRIPTION)
{
}

CmdGong::CmdGong (MainWindow &mainWindow,
                  Document &document,
                  const QString &cmdDescription,
                  QXmlStreamReader & /* reader */) :
  CmdAbstract (mainWindow,
               document,
               cmdDescription)
{
}

CmdGong::~CmdGong ()
{
}

void CmdGong::cmdRedo ()
{

  // No setup, teardown or other updates are required since this command only sends a signal
  restoreState ();
  mainWindow().sendGong ();
}

void CmdGong::cmdUndo ()
{

  restoreState ();
}

void CmdGong::saveXml (QXmlStreamWriter & /* writer */) const
{
  // Noop. This command is only for unit testing, and is completely independent of the Document that is being saved
}
