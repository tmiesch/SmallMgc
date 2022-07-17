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

#ifndef SMALLMGCGCCINPROGRESSFSM_H_
#define SMALLMGCGCCINPROGRESSFSM_H_

#include "smallmgcgccfsm.h"

class SmallMGC_GCC_InProgressFsm : public SmallMGC_GCC_Fsm
{
public:
  static SmallMGC_GCC_InProgressFsm* instance(class H248STACK_Database* database=0, class H248STACK_CallAdapter* adapter=0);
private :
   static SmallMGC_GCC_InProgressFsm* s_instance;
   SmallMGC_GCC_InProgressFsm(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter):
     SmallMGC_GCC_Fsm(database, adapter)
     {

     };

  SmallMGC_GCC_InProgressFsm();
  virtual ~SmallMGC_GCC_InProgressFsm();
public:
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

};

#endif /* SMALLMGCGCCINPROGRESSFSM_H_ */
