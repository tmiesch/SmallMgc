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

#include "smallmgcgccstartedfsm.h"
#include "smallmgcgccinprogressfsm.h"
#include "h248stack_calladapter.h"
#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"




using Poco::Util::Application;
using Poco::NumberFormatter;


SmallMGC_GCC_StartedFsm::SmallMGC_GCC_StartedFsm()
{
  // TODO Auto-generated constructor stub

}

SmallMGC_GCC_StartedFsm::~SmallMGC_GCC_StartedFsm()
{
  // TODO Auto-generated destructor stub
}

SmallMGC_GCC_StartedFsm*  SmallMGC_GCC_StartedFsm::s_instance = 0;

SmallMGC_GCC_StartedFsm*  SmallMGC_GCC_StartedFsm::instance(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter)
{
  if(s_instance == 0)
  {
    Poco::UInt32 xcode;
    s_instance = new SmallMGC_GCC_StartedFsm(database, adapter);
  }
  return s_instance;
}



Poco::UInt32 SmallMGC_GCC_StartedFsm::startCallInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC STARTED;  Event:  startCallInd\n");
  return xcode;

}
Poco::UInt32 SmallMGC_GCC_StartedFsm::checkNumber(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string digits)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC STARTED;  Event:  check Number\n");
  xcode = m_database->searchIncoming(call->m_outgoing->m_dialledDigits, call);

  if (!call->m_incoming->m_physical.empty())
  {
    call->m_incoming->m_ownNumber = call->m_outgoing->m_dialledDigits;
    m_adapter->startCallReq(call, SmallMGC_IN);
  }
  return xcode;
}

Poco::UInt32 SmallMGC_GCC_StartedFsm::bearerInfoInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string sdp)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC STARTED;  Event:  bearerInfoInd\n");
  if(!sdp.empty())
  {
      if (sdp.compare(1,1,"{") == 0)
      {
        sdp.replace(0,1,"R");
      }
      else
      {
        sdp.erase(0,5);  //remove Local
        sdp.insert(0,"Remote");
      }
  }
  if (direction == SmallMGC_OUT)
    m_adapter->bearerInformationReq(call, SmallMGC_IN, sdp);
  else
    m_adapter->bearerInformationReq(call, SmallMGC_OUT, sdp);
  call->m_extendedCallState = SmallMGC_GCC_InProgressFsm::instance();

  return xcode;

}

Poco::UInt32 SmallMGC_GCC_StartedFsm::ringingInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC STARTED;  Event:  ringingInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_StartedFsm::establishInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC STARTED;  Event:  establishInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_StartedFsm::releaseInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC STARTED;  Event:  releaseInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_StartedFsm::idleInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC STARTED;  Event:  idleInd\n");
  if (direction == SmallMGC_OUT)
   {
     H248STACK_Gateway* gateway = call->m_outgoing->m_gateway;
     if (gateway != 0)
     {
       gateway->removeUsedTermination( call->m_outgoing->m_ephemeral);
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
       gateway->removeUsedTermination( call->m_incoming->m_physical);
       gateway->removeUsedContext(call->m_incoming->m_contextId);
     }

     call->m_incoming->m_contextId.clear();

   }

   if ((call->m_outgoing->m_contextId.empty()) &&(call->m_incoming->m_contextId.empty()))
   {
     delete call;
   }
   return xcode;


}

Poco::UInt32 SmallMGC_GCC_StartedFsm::switchToT38Ind(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC STARTED;  Event:  switchToT38Ind\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_StartedFsm::switchToModemInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC STARTED;  Event:  switchToModemInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_StartedFsm::faxEstablishInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC STARTED;  Event:  faxEstablishedInd\n");
  return xcode;

}

