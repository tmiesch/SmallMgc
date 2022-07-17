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

#include "smallmgcnccstartedfsm.h"
#include "smallmgcnccinprogressfsm.h"
#include "smallmgcnccreleasingfsm.h"

#include "h248stack_calladapter.h"
#include "smallmgc_n_messagebuilder.h"

#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"


using Poco::Util::Application;
using Poco::NumberFormatter;


SmallMGC_NCC_StartedFsm::SmallMGC_NCC_StartedFsm()
{
  // TODO Auto-generated constructor stub

}

SmallMGC_NCC_StartedFsm::~SmallMGC_NCC_StartedFsm()
{
  // TODO Auto-generated destructor stub
}

SmallMGC_NCC_StartedFsm*  SmallMGC_NCC_StartedFsm::s_instance = 0;

SmallMGC_NCC_StartedFsm*  SmallMGC_NCC_StartedFsm::instance(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter)
{
  if(s_instance == 0)
  {
    Poco::UInt32 xcode;
    s_instance = new SmallMGC_NCC_StartedFsm(database, adapter);
  }
  return s_instance;
}


Poco::UInt32 SmallMGC_NCC_StartedFsm::startCall(class H248STACK_Call* call, Poco::UInt32 transactionId, std::string& term )
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: NCC STARTED;  Event:  startCall\n");
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_StartedFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  modifyReply1\n");

  return xcode;

}
 //virtual Poco::UInt32 modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId);                                                   //G, I,K, Q,O, R,S
Poco::UInt32 SmallMGC_NCC_StartedFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  modifyReply2\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_StartedFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  addReply1\n");
  if (direction == SmallMGC_IN)
    {
      if (call->m_incoming->m_timer != 0)
      {
        // stop timer in case waiting for more digits
        m_adapter->stopTimer(call->m_incoming->m_timer);
        call->m_incoming->m_timer = 0;
      }
      if  (call->m_incoming->m_modifyEphmSend == true)
      {
        call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
        call->m_incoming->m_outstandingMessage =
          SmallMGC_NMessageBuilder::buildAddEphemeral(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId, m_database->getEventRequestId());

        m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
        call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
      }
      //call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
      //SmallMGC_NMessageBuilder::buildAddEphemeralIncoming(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId, m_database->getEventRequestId(), call->m_outgoing->m_localDescriptor);

      //call->m_incoming->m_outstandingMessage =
      //  SmallMGC_NMessageBuilder::buildAddChooseIncoming(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_physical, m_database->getEventRequestId(), sdp);
      //m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
      //call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
    }
return xcode;

}
Poco::UInt32 SmallMGC_NCC_StartedFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  addReply2\n");
  // message not expected in this state
  if (direction == SmallMGC_OUT)
    {
      if (call->m_outgoing->m_timer != 0)
      {
        m_adapter->stopTimer(call->m_outgoing->m_timer);
        call->m_outgoing->m_timer = 0;

        call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
        call->m_outgoing->m_outstandingMessage =
             SmallMGC_NMessageBuilder::buildModifyOnHookSc(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_physical, call->m_outgoing->m_contextId, m_database->getEventRequestId());
        m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
        call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);

        m_adapter->bearerInformationInd(call, direction,localSDP);
        call->m_outleg = SmallMGC_NCC_InProgressFsm::instance();
      }
    }
    else
    {
      if (call->m_incoming->m_timer != 0)
      {
        m_adapter->stopTimer(call->m_incoming->m_timer);
        call->m_incoming->m_timer = 0;
        call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
        call->m_incoming->m_outstandingMessage =
           SmallMGC_NMessageBuilder::buildModifyRinging(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_physical, call->m_incoming->m_contextId, m_database->getEventRequestId());
        m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
        call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
        m_adapter->bearerInformationInd(call, direction,localSDP);

        call->m_inleg = SmallMGC_NCC_InProgressFsm::instance();
      }
    }

  return xcode;

}

Poco::UInt32 SmallMGC_NCC_StartedFsm::subtractReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  subtractReply1\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_NCC_StartedFsm::notifyDigit(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& digits)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  notify Digit\n");
  // message not expected in this state

  return xcode;

}

Poco::UInt32 SmallMGC_NCC_StartedFsm::notifyOffHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  notify off hook\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_StartedFsm::notifyOnHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  notify on hook\n");
  if (direction == SmallMGC_OUT)
  {
    if (call->m_outgoing->m_timer != 0)
    {
      // stop timer in case waiting for more digits
      m_adapter->stopTimer(call->m_outgoing->m_timer);
      call->m_outgoing->m_timer = 0;
      // perhaps I should do a clean up and subtrac
    }
    call->m_releaser = H248STACK_Outgoing;
      // TODO: if transaction is in progress cancel action, stop timer ans clear message

    call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
    call->m_outgoing->m_outstandingMessage =
             SmallMGC_NMessageBuilder::buildSubtract(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId, call->m_outgoing->m_ephemeral);
    m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
    call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_ephemeral);

     m_adapter->releaseInd(call, direction);
     call->m_outleg = SmallMGC_NCC_ReleasingFsm::instance();

     call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
  }
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_StartedFsm::notifyTone(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& tones)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  notify tone\n");
  return xcode;
}

Poco::UInt32 SmallMGC_NCC_StartedFsm::notifyFlashHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  notify flash hook\n");
  return xcode;
}

Poco::UInt32 SmallMGC_NCC_StartedFsm::timerExpiry(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  timer Expiry\n");
  xcode = SmallMGC_CC_Fsm::timerExpiry(call, direction);

  return xcode;

}

Poco::UInt32 SmallMGC_NCC_StartedFsm::startCallReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  start call req\n");
  // message not expected in this state
  if (call->m_outgoing->m_physical.size() == 0)
    {

    }
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_StartedFsm::bearerInformationReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& sdp)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  bearer information req\n");
  if (direction == SmallMGC_IN)
  {
    if (call->m_incoming->m_timer != 0)
    {
      // stop timer in case waiting for more digits
     // m_adapter->stopTimer(call->m_incoming->m_timer);
     // call->m_incoming->m_timer = 0;
      call->m_incoming->m_modifyEphmSend = true;
    }
    else
    {
      call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
      call->m_incoming->m_outstandingMessage =
        SmallMGC_NMessageBuilder::buildAddEphemeral(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId, m_database->getEventRequestId());

      //call->m_incoming->m_outstandingMessage =
      //  SmallMGC_NMessageBuilder::buildAddChooseIncoming(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_physical, m_database->getEventRequestId(), sdp);
      m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
      call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
    }
  }
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_StartedFsm::establishedReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  established req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_StartedFsm::releaseReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  release req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_StartedFsm::ringingReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  ringing req\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_NCC_StartedFsm::switchToT38Req(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Started;  Event:  switchToT38 req\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_NCC_StartedFsm::switchToModemReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC Started;  Event:  switchToModem req\n");
  // message not expected in this state
  return xcode;

}



Poco::UInt32 SmallMGC_NCC_StartedFsm::setup(class H248STACK_Call* call, enum SmallMGCLegDirection direction, enum SmallMGCIsdnBABearer bearer)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  isdn setup\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_StartedFsm::information(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& number)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  isdn information\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_StartedFsm::alerting(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  isdn alerting\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_StartedFsm::connect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  isdn connect\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_StartedFsm::connectAck(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  isdn connectAck\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_StartedFsm::disconnect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  isdn disconnect\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_StartedFsm::release(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  isdn release\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_StartedFsm::releaseComplete(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC STARTED;  Event:  isdn release complete\n");
  // message not expected in this state
  return xcode;

}


 Poco::UInt32 SmallMGC_NCC_StartedFsm::sendToMg(std::string const& msg)
 {
   // for reply messages
 }



