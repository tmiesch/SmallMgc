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

#include "smallmgcgccfsm.h"

SmallMGC_GCC_Fsm::SmallMGC_GCC_Fsm(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter):
      m_database(database), m_adapter(adapter)
{
  // TODO Auto-generated constructor stub

}

SmallMGC_GCC_Fsm::~SmallMGC_GCC_Fsm()
{
  // TODO Auto-generated destructor stub
}


Poco::UInt32 SmallMGC_GCC_Fsm::startCallInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}
Poco::UInt32 SmallMGC_GCC_Fsm::checkNumber(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string digits)
{

}

Poco::UInt32 SmallMGC_GCC_Fsm::bearerInfoInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string sdp)
{

}

Poco::UInt32 SmallMGC_GCC_Fsm::ringingInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}

Poco::UInt32 SmallMGC_GCC_Fsm::establishInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}

Poco::UInt32 SmallMGC_GCC_Fsm::releaseInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}

Poco::UInt32 SmallMGC_GCC_Fsm::idleInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}

Poco::UInt32 SmallMGC_GCC_Fsm::switchToT38Ind(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}

Poco::UInt32 SmallMGC_GCC_Fsm::switchToModemInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}

Poco::UInt32 SmallMGC_GCC_Fsm::faxEstablishInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{

}
