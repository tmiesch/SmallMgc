/******************************************************************************
 * SmallMGC
 *
 * Copyright (C) 2009 Thomas Miesch
 *
 * Authors: Thomas Miesch <thomas.miesch@smallmgc.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>
 *
 * created:
 * filename:
 * purpose:
 *
 ******************************************************************************/

#include <stdio.h>
#include <string.h>


#include "h248stack_gateway.h"
#include "h248stack_database.h"

H248STACK_Gateway::H248STACK_Gateway()
{
    m_state = H248STACK_ASSOC_OUTSERVICE;
    m_heartbeatCounter = 0;
    //m_heartbeatRepeationTimer = 0;
    m_heartbeatTimer = 0;
    m_startupTimer = 0;
    m_startupCounter = 0;
    m_answerReceived = 0;
    //m_heartbeatRepeatCounter = 0;
    m_wasModifyItSent = false;
    m_associationLost = false;
    hasIsdnSubscriberConfigured = false;
    m_heartbeat = new SmallMGC_TransactionData();
    m_root = new SmallMGC_TransactionData();
}

H248STACK_Gateway::~H248STACK_Gateway()
{

}
/*
H248STACK_TerminationState H248STACK_Gateway::getTerminationState(std::string& termination)
{

  H248STACK_TerminationPhysical* term = m_physicalTerminations[termination];
  if (term != 0)
    return term->state;
  else
    return H248STACK_TERM_OUTSERIVCE_REMOTE;
}

void H248STACK_Gateway::setTerminationState(std::string& termination, H248STACK_TerminationState state)
{
  H248STACK_TerminationPhysical* term = m_physicalTerminations[termination];
  if (term != 0)
     term->state = state;
}

std::string& H248STACK_Gateway::getOwnNumber(std::string& termination)
{
  return m_physicalTerminations[termination]->m_number;
}
*/
