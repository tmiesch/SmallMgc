#ifndef H248STACK_CALLADAPTER_H
#define H248STACK_CALLADAPTER_H
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


#include <string>
#include <map>
#include "Poco/Types.h"
#include "Poco/NotificationQueue.h"
#include "Poco/Timer.h"

#include "h248stack_database.h"

class H248STACK_CallAdapter
{
public :
  H248STACK_CallAdapter(class Poco::NotificationQueue& callQueue, class SUB_H248_UDP* udp, Poco::NotificationQueue& isdnQueue);
  virtual ~H248STACK_CallAdapter();
  virtual  class Poco::Timer* startTimer1(Poco::UInt32 duration, std::string ipAddress,
               Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId);
  virtual void stopTimer1(class Poco::Timer* timer);
  virtual class Timer* startTimer(Poco::UInt32 duration, std::string ipAddress,
               Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId);

   virtual void stopTimer(class Timer* timer);


public :

  Poco::UInt32 sendToMg(std::string& ipAddress, std::string& msg); // for reply messages
  Poco::UInt32 releaseInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  Poco::UInt32 establishedInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  Poco::UInt32 startCall(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  Poco::UInt32 checkNumber(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string digits);
  Poco::UInt32 bearerInformationInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string localSDP);
  Poco::UInt32 ringingInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  Poco::UInt32 idleInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  Poco::UInt32 faxEstablishedInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  Poco::UInt32 switchToT38Ind(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  Poco::UInt32 switchToModemInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction);


  Poco::UInt32 releaseReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  Poco::UInt32 establishedReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  Poco::UInt32 startCallReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  Poco::UInt32 bearerInformationReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string localSDP);
  Poco::UInt32 ringingReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  Poco::UInt32 switchToT38Req(class H248STACK_Call* call, enum SmallMGCLegDirection direction);
  Poco::UInt32 switchToModemReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction);

  Poco::UInt32 setupAckReq(class H248STACK_Call* call, class Q931_Interface* interface, class Q931_Call* isdnCall);
  Poco::UInt32 setupReq(class H248STACK_Call* call, class Q931_Interface* interface, class Q931_Call* isdnCall);
  Poco::UInt32 callProceedingReq(class H248STACK_Call* call, class Q931_Interface* interface, class Q931_Call* isdnCall);
  Poco::UInt32 alertingReq(class H248STACK_Call* call, class Q931_Interface* interface, class Q931_Call* isdnCall);
  Poco::UInt32 connectReq(class H248STACK_Call* call, class Q931_Interface* interface, class Q931_Call* isdnCall);
  Poco::UInt32 connectAckReq(class H248STACK_Call* call, class Q931_Interface* interface, class Q931_Call* isdnCall);
  Poco::UInt32 releaseIsdnReq(class H248STACK_Call* call, class Q931_Interface* interface, class Q931_Call* isdnCall);
  Poco::UInt32 disconnectReq(class H248STACK_Call* call, class Q931_Interface* interface, class Q931_Call* isdnCall);
  Poco::UInt32 releaseCompleteReq(class H248STACK_Call* call, class Q931_Interface* interface, class Q931_Call* isdnCall);
  Poco::UInt32 freeIsdnResources(class H248STACK_Call* call, class Q931_Interface* interface, class Q931_Call* isdnCall);


private:
  class SUB_H248_UDP* m_udp;
  class Poco::NotificationQueue& m_callQueue;
  Poco::NotificationQueue& m_isdnQueue;
  //typedef std::map<std::string, Poco::UInt32> ReplyWaitGrp;
  //ReplyWaitGrp reply_wait_s_;

private: // inhibit access to not implemented default C++ operators
  H248STACK_CallAdapter            (const H248STACK_CallAdapter&);  // copy constructor
  H248STACK_CallAdapter& operator= (const H248STACK_CallAdapter&);  // copy assignment
};

#endif



