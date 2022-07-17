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

#include "smallmgcnccreleasingfsm.h"
#include "smallmgcnccidlefsm.h"

#include "h248stack_calladapter.h"
#include "smallmgc_n_messagebuilder.h"



#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"


using Poco::Util::Application;
using Poco::NumberFormatter;

SmallMGC_NCC_ReleasingFsm::SmallMGC_NCC_ReleasingFsm()
{
  // TODO Auto-generated constructor stub

}

SmallMGC_NCC_ReleasingFsm::~SmallMGC_NCC_ReleasingFsm()
{
  // TODO Auto-generated destructor stub
}

SmallMGC_NCC_ReleasingFsm*  SmallMGC_NCC_ReleasingFsm::s_instance = 0;

SmallMGC_NCC_ReleasingFsm*  SmallMGC_NCC_ReleasingFsm::instance(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter)
{
  if(s_instance == 0)
  {
    Poco::UInt32 xcode;
    s_instance = new SmallMGC_NCC_ReleasingFsm(database, adapter);
  }
  return s_instance;
}


Poco::UInt32 SmallMGC_NCC_ReleasingFsm::startCall(class H248STACK_Call* call, Poco::UInt32 transactionId, std::string& term )
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: NCC Releasing;  Event:  startCall\n");
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ReleasingFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  modifyReply1\n");

  if (direction == SmallMGC_OUT)
  {
    if (  call->m_outgoing->m_timer != 0)
    {
      m_adapter->stopTimer(call->m_outgoing->m_timer);
      call->m_outgoing->m_timer = 0;
    }
    call->m_outleg = SmallMGC_NCC_IdleFsm::instance();
  }
  else
  {
    if (  call->m_incoming->m_timer != 0)
    {
      m_adapter->stopTimer(call->m_incoming->m_timer);
      call->m_incoming->m_timer = 0;
    }
    call->m_inleg = SmallMGC_NCC_IdleFsm::instance();
  }
  m_adapter->idleInd(call, direction);

  return xcode;

}
 //virtual Poco::UInt32 modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId);                                                   //G, I,K, Q,O, R,S
Poco::UInt32 SmallMGC_NCC_ReleasingFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  modifyReply2\n");
  /*if (direction == SmallMGC_OUT)
  {
    if (  call->m_outgoing->m_timer != 0)
    {
      m_adapter->stopTimer(call->m_outgoing->m_timer);
      call->m_outgoing->m_timer = 0;
    }
    call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
    call->m_outgoing->m_outstandingMessage =
               SmallMGC_NMessageBuilder::buildSubtract(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId);
    m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
    call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
  }
  else
  {
    if (  call->m_incoming->m_timer != 0)
    {
      m_adapter->stopTimer(call->m_incoming->m_timer);
      call->m_incoming->m_timer = 0;
    }
    call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
    call->m_incoming->m_outstandingMessage =
        SmallMGC_NMessageBuilder::buildSubtract(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId);
    m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
    call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);

  }*/
  return xcode;
}

Poco::UInt32 SmallMGC_NCC_ReleasingFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  addReply1\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ReleasingFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  addReply2\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_NCC_ReleasingFsm::subtractReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  subtractReply1\n");
  if (direction == SmallMGC_OUT)
    {
      if (call->m_outgoing->m_timer != 0)
      {
        call->m_outgoing->m_outstandingMessage.clear();
        m_adapter->stopTimer(call->m_outgoing->m_timer);
        call->m_outgoing->m_timer = 0;
      }

      if (call->m_outgoing->m_ephemeralSubtractReceived == true)
      {
        if (call->m_releaser == H248STACK_Outgoing)
        {
          call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
          call->m_outgoing->m_outstandingMessage =
            SmallMGC_NMessageBuilder::buildSubtract(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId, call->m_outgoing->m_physical);
          m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
          call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
        }
      }
      else
      {
        call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
        call->m_outgoing->m_outstandingMessage =
             SmallMGC_NMessageBuilder::buildModifyOffHook(call->m_outgoing->m_outstandingReplyTransactionId,  call->m_outgoing->m_physical, m_database->getEventRequestId());
        m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
        call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
      }
    }
    else
    {
      if (call->m_incoming->m_timer != 0)
      {
        call->m_incoming->m_outstandingMessage.clear();
        m_adapter->stopTimer(call->m_incoming->m_timer);
        call->m_incoming->m_timer = 0;
      }

      if (call->m_incoming->m_ephemeralSubtractReceived == true)
      {
        if (call->m_releaser == H248STACK_Incoming)
        {
          call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
          call->m_incoming->m_outstandingMessage =
               SmallMGC_NMessageBuilder::buildSubtract(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId, call->m_incoming->m_physical);
          m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
          call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);

        }
      }
      else
      {
        call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
        call->m_incoming->m_outstandingMessage =
          SmallMGC_NMessageBuilder::buildModifyOffHook(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_physical, m_database->getEventRequestId());
        m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
        call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
      }
    }

    return xcode;
}

Poco::UInt32 SmallMGC_NCC_ReleasingFsm::notifyDigit(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& digits)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  notify Digit\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_NCC_ReleasingFsm::notifyOffHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  notify off hook\n");
}
Poco::UInt32 SmallMGC_NCC_ReleasingFsm::notifyOnHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  notify on hook\n");
  if (direction == SmallMGC_OUT)
  {
    if (  call->m_outgoing->m_timer != 0)
    {
      m_adapter->stopTimer(call->m_outgoing->m_timer);
      call->m_outgoing->m_timer = 0;
    }
    call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
    call->m_outgoing->m_outstandingMessage =
                SmallMGC_NMessageBuilder::buildSubtract(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId, call->m_outgoing->m_physical);
    m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
    call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
  }
  else
  {
    if (  call->m_incoming->m_timer != 0)
    {
      m_adapter->stopTimer(call->m_incoming->m_timer);
      call->m_incoming->m_timer = 0;
    }
    call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
    call->m_incoming->m_outstandingMessage =
           SmallMGC_NMessageBuilder::buildSubtract(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId, call->m_incoming->m_physical);
    m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
    call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
  }
  return xcode;

}

Poco::UInt32 SmallMGC_NCC_ReleasingFsm::notifyTone(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& tones)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  notify tone\n");
  return xcode;
}

Poco::UInt32 SmallMGC_NCC_ReleasingFsm::notifyFlashHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  notify flash hook\n");
  return xcode;
}

Poco::UInt32 SmallMGC_NCC_ReleasingFsm::timerExpiry(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId,std::string& term)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  timer Expiry\n");
  xcode = SmallMGC_CC_Fsm::timerExpiry(call, direction);

  return xcode;

}

Poco::UInt32 SmallMGC_NCC_ReleasingFsm::startCallReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  start call req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ReleasingFsm::bearerInformationReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& sdp)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  bearer information req\n");
  return xcode;

}

Poco::UInt32 SmallMGC_NCC_ReleasingFsm::releaseReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  release req\n");
  return xcode;

}

Poco::UInt32 SmallMGC_NCC_ReleasingFsm::ringingReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  ringing req\n");
  return xcode;
}

Poco::UInt32 SmallMGC_NCC_ReleasingFsm::establishedReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  established req\n");
  return xcode;
}

Poco::UInt32 SmallMGC_NCC_ReleasingFsm::switchToT38Req(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  switchToT38 req\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_NCC_ReleasingFsm::switchToModemReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  switchToModem req\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_NCC_ReleasingFsm::setup(class H248STACK_Call* call, enum SmallMGCLegDirection direction, enum SmallMGCIsdnBABearer bearer)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  isdn setup\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ReleasingFsm::information(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& number)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  isdn information\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ReleasingFsm::alerting(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  isdn alerting\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ReleasingFsm::connect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  isdn connect\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ReleasingFsm::connectAck(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  isdn connectAck\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ReleasingFsm::disconnect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  isdn disconnect\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ReleasingFsm::release(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  isdn release\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ReleasingFsm::releaseComplete(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Releasing;  Event:  isdn release complete\n");
  // message not expected in this state
  return xcode;

}


 Poco::UInt32 SmallMGC_NCC_ReleasingFsm::sendToMg(std::string const& msg)
 {
   // for reply messages
 }
