/******************************************************************************************************
 * (C) 2014 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#include "Logger.h"
#include "TutorialButton.h"
#include "TutorialStateChecklistWizardPoints.h"
#include "TutorialStateContext.h"

TutorialStateChecklistWizardPoints::TutorialStateChecklistWizardPoints (TutorialStateContext &context) : 
  TutorialStateChecklistWizardAbstract (context)
{
}

void TutorialStateChecklistWizardPoints::begin ()
{

  TutorialStateChecklistWizardAbstract::begin();
  connect (previous(), SIGNAL (signalTriggered ()), this, SLOT (slotPrevious ()));
}

void TutorialStateChecklistWizardPoints::end ()
{

  TutorialStateChecklistWizardAbstract::end();
}

void TutorialStateChecklistWizardPoints::slotPrevious ()
{

  context().requestDelayedStateTransition (TUTORIAL_STATE_POINT_MATCH);
}
