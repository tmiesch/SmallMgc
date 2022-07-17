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

#include "smallmgcetsiwaitingnumberfsm.h"
#include "smallmgcetsistartedfsm.h"
#include "smallmgcetsiinprogressfsm.h"
#include "smallmgcetsireleasingfsm.h"

#include "h248stack_calladapter.h"
#include "smallmgc_etsi_messagebuilder.h"

#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"


using Poco::Util::Application;
using Poco::NumberFormatter;


SmallMGC_ETSI_WaitingNumberFsm::SmallMGC_ETSI_WaitingNumberFsm()
{
  // TODO Auto-generated constructor stub

}

SmallMGC_ETSI_WaitingNumberFsm::~SmallMGC_ETSI_WaitingNumberFsm()
{
  // TODO Auto-generated destructor stub
}

SmallMGC_ETSI_WaitingNumberFsm*  SmallMGC_ETSI_WaitingNumberFsm::s_instance = 0;

SmallMGC_ETSI_WaitingNumberFsm*  SmallMGC_ETSI_WaitingNumberFsm::instance(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter)
{
  if(s_instance == 0)
  {
    Poco::UInt32 xcode;
    s_instance = new SmallMGC_ETSI_WaitingNumberFsm(database, adapter);
  }
  return s_instance;
}


Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::startCall(class H248STACK_Call* call, Poco::UInt32 transactionId, std::string& term )
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: HCC WaitingNumber;  Event:  startCall\n");
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  modifyReply1\n");

  if (direction == SmallMGC_OUT)
  {
    if (call->m_outgoing->m_timer != 0)
    {
      call->m_outgoing->m_outstandingMessage.clear();
      m_adapter->stopTimer(call->m_outgoing->m_timer);
      call->m_outgoing->m_timer = 0;
      if(!call->m_incoming->m_physical.empty())
      {
        // if the incoming physical is not empty then a number dial was found
        call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
        call->m_outgoing->m_outstandingMessage =
        	SmallMGC_EtsiMessageBuilder::buildAddChooseOutgoing(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_physical, m_database->getEventRequestId());
        m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
        call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
        call->m_outleg = SmallMGC_ETSI_StartedFsm::instance();
      }
      else
      {
        // do nothing
      }
    }
  }

  return xcode;

}
 //virtual Poco::UInt32 modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId);                                                   //G, I,K, Q,O, R,S
Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  modifyReply2\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  addReply1\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  addReply2\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::subtractReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  subtractReply1\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::notifyDigit(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& digits)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  notify Digit\n");
  call->m_outgoing->m_dialledDigits.append(digits);

   call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
   call->m_outgoing->m_outstandingMessage =
	   SmallMGC_EtsiMessageBuilder::buildModifyMoreDigits(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_physical, m_database->getEventRequestId());
   m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
   call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);

   Poco::UInt32 busy = m_adapter->checkNumber(call, direction, call->m_outgoing->m_dialledDigits);
   if (busy == 0)
   {

   }
   else if (busy == 1)
   {
       call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
       // called party busy play busy tone
       call->m_outgoing->m_outstandingMessage =
    	   SmallMGC_EtsiMessageBuilder::buildModifyBusyNoEphemeral(call->m_outgoing->m_outstandingReplyTransactionId,
                        "-", call->m_outgoing->m_physical, m_database );
       m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
       call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
   }
   else if (busy == 2)
   {
     call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
     call->m_outgoing->m_outstandingMessage =
    	 SmallMGC_EtsiMessageBuilder::buildModifyBusyNoEphemeral(call->m_outgoing->m_outstandingReplyTransactionId,
                                "-", call->m_outgoing->m_physical, m_database );
     m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
     call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
   }
   else
   {
     // use incoming timer to look after dial time out for outgoing termination
     call->m_incoming->m_timer = m_adapter->startTimer(20000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
   }
  return xcode;

}

Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::notifyOffHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  notify off hook\n");
  if (direction == SmallMGC_OUT)
  {
      if (call->m_outgoing->m_timer != 0)
      {
        m_adapter->stopTimer(call->m_outgoing->m_timer);
        call->m_outgoing->m_timer = 0;
      }
      call->m_releaser = H248STACK_Outgoing;
      // TODO: if transaction is in progress cancel action, stop timer ans clear message

     call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();

     call->m_outgoing->m_outstandingMessage =
    	 SmallMGC_EtsiMessageBuilder::buildModifyAfterStartup(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_physical,
                                          m_database->getEventRequestId());
     m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
     call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
     m_adapter->releaseInd(call, direction);
     call->m_outleg = SmallMGC_ETSI_ReleasingFsm::instance();
  }
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::notifyOnHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  notify on hook\n");
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
   /*
    call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
        call->m_outgoing->m_outstandingMessage =
             H248STACK_SMessageBuilder::buildSubtract(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId, call->m_outgoing->m_physical, m_database->getEventRequestId(), m_database->getEventRequestId());
    m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
    call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);

     m_adapter->releaseInd(call, direction);
     call->m_outleg = SmallMGC_HCC_ReleasingFsm::instance();
     */
     call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();

     call->m_outgoing->m_outstandingMessage =
    	 SmallMGC_EtsiMessageBuilder::buildModifyAfterStartup(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_physical,
                               m_database->getEventRequestId());
     m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
     call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
     m_adapter->idleInd(call, direction);
  }
  return xcode;

}

Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::notifyTone(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& tones)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  notify tone\n");
  return xcode;
}
Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::notifyFlashHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  notify flash hook\n");
  return xcode;
}

Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::timerExpiry(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  timer Expiry\n");
  if (call->m_outgoing->m_outstandingMessage.empty())
  {
    if (direction == SmallMGC_OUT)
    {
      // timeout for waiting on more digits
      call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
      // play congestion tone
      call->m_outgoing->m_congestionToneSent = true;
      call->m_outgoing->m_outstandingMessage =
    	  SmallMGC_EtsiMessageBuilder::buildModifyBusyNoEphemeral(call->m_outgoing->m_outstandingReplyTransactionId,
                           "-", call->m_outgoing->m_physical, m_database );
      m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
      call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
    }
  }
  else
  {
    xcode = SmallMGC_CC_Fsm::timerExpiry(call, direction);
  }

  return xcode;

}

Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::startCallReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  start call req\n");
  // message not expected in this state
  if (call->m_outgoing->m_physical.size() == 0)
  {

  }
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::bearerInformationReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& sdp)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  bearer information req\n");
  if (direction == SmallMGC_IN)
  {
    if (call->m_incoming->m_timer != 0)
    {
      // stop timer in case waiting for more digits
      m_adapter->stopTimer(call->m_incoming->m_timer);
      call->m_incoming->m_timer = 0;
    }
    call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
    call->m_incoming->m_outstandingMessage =
    	SmallMGC_EtsiMessageBuilder::buildAddChooseIncoming(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_physical, m_database->getEventRequestId(), sdp);
    m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
    call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
  }
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::establishedReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  established req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::releaseReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  release req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::ringingReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  ringing req\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::switchToT38Req(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  switchToT38 req\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::switchToModemReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  switchToModem req\n");
  // message not expected in this state
  return xcode;

}


Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::setup(class H248STACK_Call* call, enum SmallMGCLegDirection direction, enum SmallMGCIsdnBABearer bearer)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  isdn setup\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::information(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& number)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  isdn information\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::alerting(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  isdn alerting\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::connect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  isdn connect\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::connectAck(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  isdn connectAck\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::disconnect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  isdn disconnect\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::release(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  isdn release\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::releaseComplete(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC WaitingNumber;  Event:  isdn release complete\n");
  // message not expected in this state
  return xcode;

}


 Poco::UInt32 SmallMGC_ETSI_WaitingNumberFsm::sendToMg(std::string const& msg)
 {
   // for reply messages
 }



