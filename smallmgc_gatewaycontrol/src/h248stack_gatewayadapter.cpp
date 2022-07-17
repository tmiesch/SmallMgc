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

#include "h248stack_commands.h"

#include "h248stack_gatewayadapter.h"
#include "sub_h248_udp.h"
#include "timer.h"

//----------------------------------------------------------------------------
// Implementation of the H248STACK_GatewayAdapter.
//----------------------------------------------------------------------------


H248STACK_GatewayAdapter::H248STACK_GatewayAdapter (Poco::NotificationQueue& gateQueue, Poco::NotificationQueue& callQueue,class SUB_H248_UDP* udp) :
  m_gateQueue(gateQueue), m_callQueue(callQueue), m_udp(udp)
{

}

H248STACK_GatewayAdapter::~H248STACK_GatewayAdapter() {}


Poco::UInt32 H248STACK_GatewayAdapter::sendToMg(std::string& ipAddress, std::string& msg, std::string portNr)
{
  // for reply messages
  Poco::UInt32 xcode = 0;
  m_udp->sendTo(ipAddress, msg, portNr);
  return xcode;
}

Poco::Timer* H248STACK_GatewayAdapter::startTimer(Poco::UInt32 timerId, Poco::UInt32 duration, std::string ipAddress,
               Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId)
{
  Poco::Timer* timer = new Poco::Timer(duration, 0);
  H248STACK_Timeout* notification = new H248STACK_Timeout(Timeout, transactionId, contextId, terminationId, m_gateQueue);
  notification->setIpAddress(ipAddress);
  notification->setTimerId(timerId);
  //notification->setIpPort(0);
  timer->start(Poco::TimerCallback<H248STACK_Timeout>((*notification), &H248STACK_Timeout::onTimer));
  return timer;
}

Timer* H248STACK_GatewayAdapter::startTimer1(Poco::UInt32 timerId, Poco::UInt32 duration, std::string ipAddress,
               Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId)
{
  //Poco::AutoPtr<Timer> timer = new Timer(Timeout, transactionId, contextId, terminationId, m_gateQueue);
  Timer* timer = new Timer(Timeout, transactionId, contextId, terminationId, m_gateQueue);
  timer->setIpAddress(ipAddress);
  timer->setTimerId(timerId);
  //notification->setIpPort(0);
  timer->start(duration);
  return timer;
}

Poco::Timer* H248STACK_GatewayAdapter::startTimer(Poco::UInt32 duration, std::string ipAddress,
               Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId)
{
  Poco::Timer* timer = new Poco::Timer(duration, 0);
  H248STACK_Timeout* notification = new H248STACK_Timeout(Timeout, transactionId, contextId, terminationId, m_callQueue);
  notification->setIpAddress(ipAddress);
  //notification->setIpPort(0);
  timer->start(Poco::TimerCallback<H248STACK_Timeout>((*notification), &H248STACK_Timeout::onTimer));
  return timer;
}

Timer* H248STACK_GatewayAdapter::startTimer1(Poco::UInt32 duration, std::string ipAddress,
               Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId)
{
  //Poco::AutoPtr<Timer> timer = new Timer(Timeout, transactionId, contextId, terminationId, m_callQueue);
  Timer* timer = new Timer(Timeout, transactionId, contextId, terminationId, m_callQueue);
  timer->setIpAddress(ipAddress);
  //notification->setIpPort(0);
  timer->start(duration);
  return timer;
}
void H248STACK_GatewayAdapter::stopTimer(class Poco::Timer* timer)
{
  if (timer != 0)
  {
    timer->stop();
    delete timer;
    timer = 0;
  }
}

void H248STACK_GatewayAdapter::stopTimer1(class Timer* timer)
{
  if (timer != 0)
  {
    timer->stop();
    //delete timer;
    //timer->release();
    timer = 0;
  }
}

void H248STACK_GatewayAdapter::takenOutOfService(std::string ipAddress, std::string  terminationId)
{
  //Poco::AutoPtr<SmallMGC_Internal_OutOfService> notification = new SmallMGC_Internal_OutOfService(terminationId);
  SmallMGC_Internal_OutOfService* notification = new SmallMGC_Internal_OutOfService(terminationId);
  notification->setIpAddress(ipAddress);

  m_callQueue.enqueueNotification(notification);
}

void H248STACK_GatewayAdapter::associationLost(std::string ipAddress)
{
  //Poco::AutoPtr<SmallMGC_Internal_AssociationLost> notification = new SmallMGC_Internal_AssociationLost();
  SmallMGC_Internal_AssociationLost* notification = new SmallMGC_Internal_AssociationLost();
  notification->setIpAddress(ipAddress);

  m_callQueue.enqueueNotification(notification);
}
