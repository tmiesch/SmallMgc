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

#include "smallmgcgccidlefsm.h"
#include "smallmgcgccstartedfsm.h"
//#include "h248stack_s_messagebuilder.h"
#include "h248stack_calladapter.h"
#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"


using Poco::Util::Application;
using Poco::NumberFormatter;


SmallMGC_GCC_IdleFsm::SmallMGC_GCC_IdleFsm()
{
  // TODO Auto-generated constructor stub

}

SmallMGC_GCC_IdleFsm::~SmallMGC_GCC_IdleFsm()
{
  // TODO Auto-generated destructor stub
}

SmallMGC_GCC_IdleFsm*  SmallMGC_GCC_IdleFsm::s_instance = 0;

SmallMGC_GCC_IdleFsm*  SmallMGC_GCC_IdleFsm::instance(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter)
{
  if(s_instance == 0)
  {
    Poco::UInt32 xcode;
    s_instance = new SmallMGC_GCC_IdleFsm(database, adapter);
  }
  return s_instance;
}



Poco::UInt32 SmallMGC_GCC_IdleFsm::startCallInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC IDLE;  Event:  startCallInd\n");
  call->m_extendedCallState = SmallMGC_GCC_StartedFsm::instance();
  call->m_cdrIndex = m_database->callStart(call->m_outgoing->m_ownNumber , call->m_outgoing->m_physical,direction);
  return xcode;

}
Poco::UInt32 SmallMGC_GCC_IdleFsm::checkNumber(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string digits)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC IDLE;  Event:  check Number\n");
  xcode = m_database->searchIncoming(call->m_outgoing->m_dialledDigits, call);
  return xcode;
}

Poco::UInt32 SmallMGC_GCC_IdleFsm::bearerInfoInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string sdp)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC IDLE;  Event:  bearerInfoInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_IdleFsm::ringingInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC IDLE;  Event:  ringingInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_IdleFsm::establishInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC IDLE;  Event:  establishInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_IdleFsm::releaseInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC IDLE;  Event:  releaseInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_IdleFsm::idleInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC IDLE;  Event:  idleInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_IdleFsm::switchToT38Ind(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC IDLE;  Event:  switchToT38Ind\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_IdleFsm::switchToModemInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC IDLE;  Event:  switchToModemInd\n");
  return xcode;

}

Poco::UInt32 SmallMGC_GCC_IdleFsm::faxEstablishInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: GCC IDLE;  Event:  faxEstablishedInd\n");
  return xcode;

}

