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

#include "smallmgcgccfaxestablishedfsm.h"
#include "smallmgcgccreleasingfsm.h"
#include "h248stack_calladapter.h"
#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"


using Poco::Util::Application;
using Poco::NumberFormatter;


SmallMGC_GCC_FaxEstablishedFsm*  SmallMGC_GCC_FaxEstablishedFsm::s_instance = 0;

SmallMGC_GCC_FaxEstablishedFsm*  SmallMGC_GCC_FaxEstablishedFsm::instance(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter)
{
  if(s_instance == 0)
  {
    Poco::UInt32 xcode;
    s_instance = new SmallMGC_GCC_FaxEstablishedFsm(database, adapter);
  }
  return s_instance;
}

Poco::UInt32 SmallMGC_GCC_FaxEstablishedFsm::startCallInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC FaxEstablished;  Event:  startCallInd\n");
  return xcode;

}
Poco::UInt32 SmallMGC_GCC_FaxEstablishedFsm::checkNumber(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string digits)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC FaxEstablished;  Event:  check Number\n");
  return xcode;
}

Poco::UInt32 SmallMGC_GCC_FaxEstablishedFsm::bearerInfoInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string sdp)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC FaxEstablished;  Event:  bearerInfoInd\n");
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

Poco::UInt32 SmallMGC_GCC_FaxEstablishedFsm::ringingInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC FaxEstablished;  Event:  ringingInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_FaxEstablishedFsm::establishInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC FaxEstablished;  Event:  establishInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_FaxEstablishedFsm::releaseInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC FaxEstablished;  Event:  releaseInd\n");
  if (direction == SmallMGC_OUT)
    m_adapter->releaseReq(call, SmallMGC_IN);
  else
    m_adapter->releaseReq(call, SmallMGC_OUT);
  call->m_extendedCallState = SmallMGC_GCC_ReleasingFsm::instance();

  return xcode;

}

Poco::UInt32 SmallMGC_GCC_FaxEstablishedFsm::idleInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC FaxEstablished;  Event:  idleInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_FaxEstablishedFsm::switchToT38Ind(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC FaxEstablished;  Event:  switchToT38Ind\n");

  return xcode;

}

Poco::UInt32 SmallMGC_GCC_FaxEstablishedFsm::switchToModemInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC FaxEstablished;  Event:  switchToModemInd\n");

  return xcode;

}

Poco::UInt32 SmallMGC_GCC_FaxEstablishedFsm::faxEstablishInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC FaxEstablished;  Event:  faxEstablishInd\n");
  return xcode;

}



SmallMGC_GCC_FaxEstablishedFsm::SmallMGC_GCC_FaxEstablishedFsm()
{
  // TODO Auto-generated constructor stub

}

SmallMGC_GCC_FaxEstablishedFsm::~SmallMGC_GCC_FaxEstablishedFsm()
{
  // TODO Auto-generated destructor stub
}
