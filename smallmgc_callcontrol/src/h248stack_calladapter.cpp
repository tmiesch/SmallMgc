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
#include <stdio.h>
#include "Poco/Timer.h"
#include "Poco/Thread.h"
#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"

#include "h248stack_calladapter.h"
#include "h248stack_commands.h"
#include "h248stack_database.h"

#include "smallmgcgccfsm.h"
#include "smallmgcccfsm.h"
#include "sub_h248_udp.h"
#include "timer.h"

//----------------------------------------------------------------------------
// Implementation of the H248STACK_CallAdapter.
//----------------------------------------------------------------------------
using Poco::Util::Application;
using Poco::NumberFormatter;

H248STACK_CallAdapter::H248STACK_CallAdapter (Poco::NotificationQueue& callQueue, class SUB_H248_UDP* udp, Poco::NotificationQueue& isdnQueue) :
  m_callQueue(callQueue), m_udp(udp), m_isdnQueue(isdnQueue)
{

}

H248STACK_CallAdapter::~H248STACK_CallAdapter() {}


Poco::UInt32 H248STACK_CallAdapter::sendToMg(std::string& ipAddress, std::string& msg)
{
  // for reply messages
  Poco::UInt32 xcode = 0;
  m_udp->sendTo(ipAddress, msg);
  return xcode;
}

Poco::Timer* H248STACK_CallAdapter::startTimer1(Poco::UInt32 duration, std::string ipAddress,
               Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId)
{
  Poco::Timer* timer = new Poco::Timer(duration, 0);
  H248STACK_Timeout* notification = new H248STACK_Timeout(Timeout, transactionId, contextId, terminationId, m_callQueue);
  notification->setIpAddress(ipAddress);
  //notification->setIpPort(0);
  timer->start(Poco::TimerCallback<H248STACK_Timeout>((*notification), &H248STACK_Timeout::onTimer));
  return timer;
}

Timer* H248STACK_CallAdapter::startTimer(Poco::UInt32 duration, std::string ipAddress,
               Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId)
{
  Timer* timer = new Timer(Timeout, transactionId, contextId, terminationId, m_callQueue);
  timer->setIpAddress(ipAddress);
  //notification->setIpPort(0);
  timer->start(duration);
  return timer;
}

void H248STACK_CallAdapter::stopTimer(class Timer* timer)
{
  if (timer != 0)
  {
    timer->stop();
    //delete timer;
    //timer->release();
  }
  timer = 0;
}

void H248STACK_CallAdapter::stopTimer1(class Poco::Timer* timer)
{
  timer->stop();
  timer= 0;
}

Poco::UInt32 H248STACK_CallAdapter::releaseInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  call->m_extendedCallState->releaseInd(call, direction);
}

Poco::UInt32 H248STACK_CallAdapter::establishedInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  call->m_extendedCallState->establishInd(call, direction);
}

Poco::UInt32 H248STACK_CallAdapter::startCall(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  call->m_extendedCallState->startCallInd(call, direction);
}

Poco::UInt32 H248STACK_CallAdapter::checkNumber(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string digits)
{
   return call->m_extendedCallState->checkNumber(call, direction, digits);
}

Poco::UInt32 H248STACK_CallAdapter::bearerInformationInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string localSDP)
{
  return call->m_extendedCallState->bearerInfoInd(call, direction, localSDP);
}

Poco::UInt32 H248STACK_CallAdapter::ringingInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  return call->m_extendedCallState->ringingInd(call, direction);
}

Poco::UInt32 H248STACK_CallAdapter::idleInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  return call->m_extendedCallState->idleInd(call, direction);
}

Poco::UInt32 H248STACK_CallAdapter::faxEstablishedInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  return call->m_extendedCallState->faxEstablishInd(call, direction);
}

Poco::UInt32 H248STACK_CallAdapter::switchToT38Ind(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  return call->m_extendedCallState->switchToT38Ind(call, direction);
}

Poco::UInt32 H248STACK_CallAdapter::switchToModemInd(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  return call->m_extendedCallState->switchToModemInd(call, direction);
}


Poco::UInt32 H248STACK_CallAdapter::releaseReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  if (direction == SmallMGC_OUT)
    call->m_outleg->releaseReq(call, direction);
  else
    call->m_inleg->releaseReq(call, direction);
}

Poco::UInt32 H248STACK_CallAdapter::establishedReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  if (direction == SmallMGC_OUT)
    call->m_outleg->establishedReq(call, direction);
  else
    call->m_inleg->establishedReq(call, direction);
}

Poco::UInt32 H248STACK_CallAdapter::startCallReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  if (direction == SmallMGC_OUT)
    call->m_outleg->startCallReq(call, direction);
  else
    call->m_inleg->startCallReq(call, direction);

}

Poco::UInt32 H248STACK_CallAdapter::bearerInformationReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string localSDP)
{
  if (direction == SmallMGC_OUT)
    call->m_outleg->bearerInformationReq(call, direction, localSDP);
  else
    call->m_inleg->bearerInformationReq(call, direction, localSDP);
}

Poco::UInt32 H248STACK_CallAdapter::ringingReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  if (direction == SmallMGC_OUT)
    call->m_outleg->ringingReq(call, direction);
  else
    call->m_inleg->ringingReq(call, direction);

}

Poco::UInt32 H248STACK_CallAdapter::switchToT38Req(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  if (direction == SmallMGC_OUT)
    call->m_outleg->switchToT38Req(call, direction);
  else
    call->m_inleg->switchToT38Req(call, direction);

}

Poco::UInt32 H248STACK_CallAdapter::switchToModemReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  if (direction == SmallMGC_OUT)
    call->m_outleg->switchToModemReq(call, direction);
  else
    call->m_inleg->switchToModemReq(call, direction);

}

#include "sub_iua_notify.h"
Poco::UInt32 H248STACK_CallAdapter::setupAckReq(class H248STACK_Call* call, class Q931_Interface* interface, class Q931_Call* isdnCall)
{
  std::vector<Poco::UInt8> message;
  SUB_IUA_Q931_Base* iuaMessage = new SUB_IUA_Q931_SetupAck(message);
  iuaMessage->setIsdnCall(isdnCall);
  iuaMessage->setInterface(interface);
  m_isdnQueue.enqueueNotification(iuaMessage);

}
Poco::UInt32 H248STACK_CallAdapter::setupReq(class H248STACK_Call* call, class Q931_Interface* interface, class Q931_Call* isdnCall)
{
  std::vector<Poco::UInt8> message;
  SUB_IUA_Q931_Base* iuaMessage = new SUB_IUA_Q931_SetupReq(message);
  iuaMessage->setIsdnCall(isdnCall);
  iuaMessage->setInterface(interface);
  ((SUB_IUA_Q931_SetupReq*)iuaMessage)->setSelectedBChannel(call->m_incoming->m_physical);
  ((SUB_IUA_Q931_SetupReq*)iuaMessage)->setCalledPartyNumber(call->m_incoming->m_ownNumber);
  ((SUB_IUA_Q931_SetupReq*)iuaMessage)->setCallingPartyNumber(call->m_outgoing->m_ownNumber);
  m_isdnQueue.enqueueNotification(iuaMessage);
}
Poco::UInt32 H248STACK_CallAdapter::callProceedingReq(class H248STACK_Call* call, class Q931_Interface* interface, class Q931_Call* isdnCall)
{
  std::vector<Poco::UInt8> message;
  SUB_IUA_Q931_Base* iuaMessage = new SUB_IUA_Q931_CallProceeding(message);
  iuaMessage->setIsdnCall(isdnCall);
  iuaMessage->setInterface(interface);
  m_isdnQueue.enqueueNotification(iuaMessage);
}
Poco::UInt32 H248STACK_CallAdapter::alertingReq(class H248STACK_Call* call, class Q931_Interface* interface, class Q931_Call* isdnCall)
{
  std::vector<Poco::UInt8> message;
  SUB_IUA_Q931_Base* iuaMessage = new SUB_IUA_Q931_AlertingReq(message);
  iuaMessage->setIsdnCall(isdnCall);
  iuaMessage->setInterface(interface);
  m_isdnQueue.enqueueNotification(iuaMessage);
}
Poco::UInt32 H248STACK_CallAdapter::connectReq(class H248STACK_Call* call, class Q931_Interface* interface, class Q931_Call* isdnCall)
{
  std::vector<Poco::UInt8> message;
  SUB_IUA_Q931_Base* iuaMessage = new SUB_IUA_Q931_ConnectReq(message);
  iuaMessage->setIsdnCall(isdnCall);
  iuaMessage->setInterface(interface);
  m_isdnQueue.enqueueNotification(iuaMessage);
}
Poco::UInt32 H248STACK_CallAdapter::connectAckReq(class H248STACK_Call* call, class Q931_Interface* interface, class Q931_Call* isdnCall)
{
  std::vector<Poco::UInt8> message;
  SUB_IUA_Q931_Base* iuaMessage = new SUB_IUA_Q931_ConnectAckReq(message);
  iuaMessage->setIsdnCall(isdnCall);
  iuaMessage->setInterface(interface);
  m_isdnQueue.enqueueNotification(iuaMessage);
}
Poco::UInt32 H248STACK_CallAdapter::releaseIsdnReq(class H248STACK_Call* call, class Q931_Interface* interface, class Q931_Call* isdnCall)
{
  std::vector<Poco::UInt8> message;
  SUB_IUA_Q931_Base* iuaMessage = new SUB_IUA_Q931_ReleaseReq(message);
  iuaMessage->setIsdnCall(isdnCall);
  iuaMessage->setInterface(interface);
  m_isdnQueue.enqueueNotification(iuaMessage);
}
Poco::UInt32 H248STACK_CallAdapter::disconnectReq(class H248STACK_Call* call, class Q931_Interface* interface, class Q931_Call* isdnCall)
{
  std::vector<Poco::UInt8> message;
  SUB_IUA_Q931_Base* iuaMessage = new SUB_IUA_Q931_DisconnectReq(message);
  iuaMessage->setIsdnCall(isdnCall);
  iuaMessage->setInterface(interface);
  m_isdnQueue.enqueueNotification(iuaMessage);

}
Poco::UInt32 H248STACK_CallAdapter::releaseCompleteReq(class H248STACK_Call* call, class Q931_Interface* interface, class Q931_Call* isdnCall)
{
  std::vector<Poco::UInt8> message;
  SUB_IUA_Q931_Base* iuaMessage = new SUB_IUA_Q931_ReleaseCompleteReq(message);
  iuaMessage->setIsdnCall(isdnCall);
  iuaMessage->setInterface(interface);
  m_isdnQueue.enqueueNotification(iuaMessage);

}


Poco::UInt32 H248STACK_CallAdapter::freeIsdnResources(class H248STACK_Call* call, class Q931_Interface* interface, class Q931_Call* isdnCall)
{
  std::vector<Poco::UInt8> message;
  SUB_IUA_Q931_Base* iuaMessage = new SUB_IUA_Q931_FreeCallResources(message);
  iuaMessage->setIsdnCall(isdnCall);
  iuaMessage->setInterface(interface);
  m_isdnQueue.enqueueNotification(iuaMessage);

}

