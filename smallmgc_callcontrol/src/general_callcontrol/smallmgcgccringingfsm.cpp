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

#include "smallmgcgccringingfsm.h"
#include "smallmgcgccestablishedfsm.h"
#include "smallmgcgccreleasingfsm.h"
#include "h248stack_calladapter.h"
#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"


using Poco::Util::Application;
using Poco::NumberFormatter;


SmallMGC_GCC_RingingFsm*  SmallMGC_GCC_RingingFsm::s_instance = 0;

SmallMGC_GCC_RingingFsm*  SmallMGC_GCC_RingingFsm::instance(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter)
{
  if(s_instance == 0)
  {
    Poco::UInt32 xcode;
    s_instance = new SmallMGC_GCC_RingingFsm(database, adapter);
  }
  return s_instance;
}



Poco::UInt32 SmallMGC_GCC_RingingFsm::startCallInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC Ringing;  Event:  startCallInd\n");
  return xcode;

}
Poco::UInt32 SmallMGC_GCC_RingingFsm::checkNumber(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string digits)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC Ringing;  Event:  check Number\n");
  return xcode;
}

Poco::UInt32 SmallMGC_GCC_RingingFsm::bearerInfoInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string sdp)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC InProgress;  Event:  bearerInfoInd\n");
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
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_RingingFsm::ringingInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC Ringing;  Event:  ringingInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_RingingFsm::establishInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC Ringing;  Event:  establishInd\n");
  if (direction == SmallMGC_OUT)
  {
    // don't expect to happen
    m_adapter->establishedReq(call, SmallMGC_IN);
  }
  else
  {
    m_adapter->establishedReq(call, SmallMGC_OUT);
    m_database->callConnected(call->m_cdrIndex, call->m_incoming->m_contextId, call->m_incoming->m_ephemeral);
    call->m_callEstablished = new Poco::Timestamp();
  }
  call->m_extendedCallState = SmallMGC_GCC_EstablishedFsm::instance();
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_RingingFsm::releaseInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC Ringing;  Event:  releaseInd\n");

  if (direction == SmallMGC_OUT)
     m_adapter->releaseReq(call, SmallMGC_IN);
   else
     m_adapter->releaseReq(call, SmallMGC_OUT);
  call->m_extendedCallState = SmallMGC_GCC_ReleasingFsm::instance();
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_RingingFsm::idleInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC Ringing;  Event:  idleInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_RingingFsm::switchToT38Ind(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC Ringing;  Event:  switchToT38Ind\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_RingingFsm::switchToModemInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC Ringing;  Event:  switchToModemInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_RingingFsm::faxEstablishInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC Ringing;  Event:  faxEstablishInd\n");
  return xcode;

}


SmallMGC_GCC_RingingFsm::SmallMGC_GCC_RingingFsm()
{
  // TODO Auto-generated constructor stub

}

SmallMGC_GCC_RingingFsm::~SmallMGC_GCC_RingingFsm()
{
  // TODO Auto-generated destructor stub
}
