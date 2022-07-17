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

#include "smallmgcgccreleasingfsm.h"
#include "smallmgcgccidlefsm.h"
#include "h248stack_calladapter.h"
#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"


using Poco::Util::Application;
using Poco::NumberFormatter;


SmallMGC_GCC_ReleasingFsm*  SmallMGC_GCC_ReleasingFsm::s_instance = 0;

SmallMGC_GCC_ReleasingFsm*  SmallMGC_GCC_ReleasingFsm::instance(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter)
{
  if(s_instance == 0)
  {
    Poco::UInt32 xcode;
    s_instance = new SmallMGC_GCC_ReleasingFsm(database, adapter);
  }
  return s_instance;
}



Poco::UInt32 SmallMGC_GCC_ReleasingFsm::startCallInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC Releasing;  Event:  startCallInd\n");
  return xcode;

}
Poco::UInt32 SmallMGC_GCC_ReleasingFsm::checkNumber(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string digits)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC Releasing;  Event:  check Number\n");
  return xcode;
}

Poco::UInt32 SmallMGC_GCC_ReleasingFsm::bearerInfoInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string sdp)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC Releasing;  Event:  bearerInfoInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_ReleasingFsm::ringingInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC Releasing;  Event:  ringingInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_ReleasingFsm::establishInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC Releasing;  Event:  establishInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_ReleasingFsm::releaseInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC Releasing;  Event:  releaseInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_ReleasingFsm::idleInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC Releasing;  Event:  idleInd\n");
  if (direction == SmallMGC_OUT)
  {
    H248STACK_Gateway* gateway = call->m_outgoing->m_gateway;
    if (gateway != 0)
    {
      gateway->removeUsedTermination( call->m_outgoing->m_ephemeral);
      if (m_database->getTerminationState(call->m_outgoing->m_physical, gateway->mgId) == H248STACK_TERM_INCALL_Graceful_LOCAL)
      {
        m_database->setTerminationState(call->m_outgoing->m_physical, gateway->mgId, H248STACK_TERM_OUTSERVICE_LOCAL);
      }
      if (m_database->getTerminationState(call->m_outgoing->m_physical, gateway->mgId) == H248STACK_TERM_INCALL_Graceful_REMOTE)
      {
        // put termination in out of service state
        m_database->setTerminationState(call->m_outgoing->m_physical, gateway->mgId, H248STACK_TERM_OUTSERIVCE_REMOTE);
      }
      gateway->removeUsedTermination( call->m_outgoing->m_physical);
      gateway->removeUsedContext(call->m_outgoing->m_contextId);
    }
    call->m_outgoing->m_contextId.clear();

  }
  else
  {
    H248STACK_Gateway* gateway = call->m_incoming->m_gateway;
    if (gateway != 0)
    {
      gateway->removeUsedTermination( call->m_incoming->m_ephemeral);
      if (m_database->getTerminationState(call->m_incoming->m_physical, gateway->mgId) == H248STACK_TERM_INCALL_Graceful_LOCAL)
      {
        m_database->setTerminationState(call->m_incoming->m_physical, gateway->mgId, H248STACK_TERM_OUTSERVICE_LOCAL);
      }
      if (m_database->getTerminationState(call->m_incoming->m_physical, gateway->mgId) == H248STACK_TERM_INCALL_Graceful_REMOTE)
      {
        // put termination in out of service state
        m_database->setTerminationState(call->m_incoming->m_physical, gateway->mgId, H248STACK_TERM_OUTSERIVCE_REMOTE);
      }

      gateway->removeUsedTermination( call->m_incoming->m_physical);
      gateway->removeUsedContext(call->m_incoming->m_contextId);
    }

    call->m_incoming->m_contextId.clear();

  }

  if ((call->m_outgoing->m_contextId.empty()) &&(call->m_incoming->m_contextId.empty()))
  {
    delete call;
    call = 0;
  }
  return xcode;

}
Poco::UInt32 SmallMGC_GCC_ReleasingFsm::switchToT38Ind(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC Releasing;  Event:  switchToT38Ind\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_ReleasingFsm::switchToModemInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC Releasing;  Event:  switchToModemInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_ReleasingFsm::faxEstablishInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC Releasing;  Event:  faxEstablished\n");
  return xcode;

}
SmallMGC_GCC_ReleasingFsm::SmallMGC_GCC_ReleasingFsm()
{
  // TODO Auto-generated constructor stub

}

SmallMGC_GCC_ReleasingFsm::~SmallMGC_GCC_ReleasingFsm()
{
  // TODO Auto-generated destructor stub
}
