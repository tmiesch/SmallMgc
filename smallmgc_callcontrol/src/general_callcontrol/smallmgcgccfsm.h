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

#ifndef SMALLMGCGCCFSM_H_
#define SMALLMGCGCCFSM_H_

#include "Poco/Types.h"
#include "h248stack_database.h"

class SmallMGC_GCC_Fsm
{
public:
  SmallMGC_GCC_Fsm(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter);
  SmallMGC_GCC_Fsm() {};
  virtual ~SmallMGC_GCC_Fsm();
  virtual Poco::UInt32 startCallInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  virtual Poco::UInt32 checkNumber(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string digits);
  virtual Poco::UInt32 bearerInfoInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string sdp);
  virtual Poco::UInt32 ringingInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  virtual Poco::UInt32 establishInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  virtual Poco::UInt32 releaseInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  virtual Poco::UInt32 idleInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  virtual Poco::UInt32 switchToT38Ind(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  virtual Poco::UInt32 switchToModemInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  virtual Poco::UInt32 faxEstablishInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction);


protected:
  class H248STACK_Database* m_database;
  class H248STACK_CallAdapter* m_adapter;

};

#endif /* SMALLMGCGCCFSM_H_ */
