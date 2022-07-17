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
#include "smallmgcgccchangemodemfsm.h"
#include "smallmgcgccmodemestablishedfsm.h"
#include "smallmgcgccreleasingfsm.h"
#include "h248stack_calladapter.h"
#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"


using Poco::Util::Application;
using Poco::NumberFormatter;


SmallMGC_GCC_ChangeModemFsm*  SmallMGC_GCC_ChangeModemFsm::s_instance = 0;

SmallMGC_GCC_ChangeModemFsm*  SmallMGC_GCC_ChangeModemFsm::instance(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter)
{
  if(s_instance == 0)
  {
    Poco::UInt32 xcode;
    s_instance = new SmallMGC_GCC_ChangeModemFsm(database, adapter);
  }
  return s_instance;
}

Poco::UInt32 SmallMGC_GCC_ChangeModemFsm::startCallInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC ChangeModem;  Event:  startCallInd\n");
  return xcode;

}
Poco::UInt32 SmallMGC_GCC_ChangeModemFsm::checkNumber(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string digits)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC ChangeModem;  Event:  check Number\n");
  return xcode;
}

Poco::UInt32 SmallMGC_GCC_ChangeModemFsm::bearerInfoInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string sdp)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC ChangeModem;  Event:  bearerInfoInd\n");
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

  call->m_extendedCallState = SmallMGC_GCC_ModemEstablishedFsm::instance();

  return xcode;

}

Poco::UInt32 SmallMGC_GCC_ChangeModemFsm::ringingInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC ChangeModem;  Event:  ringingInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_ChangeModemFsm::establishInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC ChangeModem;  Event:  establishInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_ChangeModemFsm::releaseInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC ChangeModem;  Event:  releaseInd\n");
  if (direction == SmallMGC_OUT)
    m_adapter->releaseReq(call, SmallMGC_IN);
  else
    m_adapter->releaseReq(call, SmallMGC_OUT);
  call->m_extendedCallState = SmallMGC_GCC_ReleasingFsm::instance();

  return xcode;

}

Poco::UInt32 SmallMGC_GCC_ChangeModemFsm::idleInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC ChangeModem;  Event:  idleInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_ChangeModemFsm::switchToT38Ind(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC ChangeModem;  Event:  switchToT38Ind\n");

  return xcode;

}

Poco::UInt32 SmallMGC_GCC_ChangeModemFsm::switchToModemInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC ChangeModem;  Event:  switchToModemInd\n");

  return xcode;

}

Poco::UInt32 SmallMGC_GCC_ChangeModemFsm::faxEstablishInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC ChangeModem;  Event:  faxEstablishInd\n");
  return xcode;

}


SmallMGC_GCC_ChangeModemFsm::SmallMGC_GCC_ChangeModemFsm()
{
  // TODO Auto-generated constructor stub

}

SmallMGC_GCC_ChangeModemFsm::~SmallMGC_GCC_ChangeModemFsm()
{
  // TODO Auto-generated destructor stub
}
