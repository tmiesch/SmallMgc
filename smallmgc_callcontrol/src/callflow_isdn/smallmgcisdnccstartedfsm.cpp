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

#include "smallmgcisdnccstartedfsm.h"
#include "smallmgcisdnccinprogressfsm.h"
#include "smallmgcisdnccreleasingfsm.h"

#include "h248stack_calladapter.h"
#include "h248stack_isdn_messagebuilder.h"

#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"


using Poco::Util::Application;
using Poco::NumberFormatter;


SmallMGC_IsdnCC_StartedFsm::SmallMGC_IsdnCC_StartedFsm()
{
  // TODO Auto-generated constructor stub

}

SmallMGC_IsdnCC_StartedFsm::~SmallMGC_IsdnCC_StartedFsm()
{
  // TODO Auto-generated destructor stub
}

SmallMGC_IsdnCC_StartedFsm*  SmallMGC_IsdnCC_StartedFsm::s_instance = 0;

SmallMGC_IsdnCC_StartedFsm*  SmallMGC_IsdnCC_StartedFsm::instance(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter)
{
  if(s_instance == 0)
  {
    Poco::UInt32 xcode;
    s_instance = new SmallMGC_IsdnCC_StartedFsm(database, adapter);
  }
  return s_instance;
}


Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::startCall(class H248STACK_Call* call, Poco::UInt32 transactionId, std::string& term )
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: IsdnCC STARTED;  Event:  startCall\n");
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC STARTED;  Event:  modifyReply1\n");

  if (direction == SmallMGC_OUT)
  {
    if (call->m_outgoing->m_timer != 0)
    {
      call->m_outgoing->m_outstandingMessage.clear();
      m_adapter->stopTimer(call->m_outgoing->m_timer);
      call->m_outgoing->m_timer = 0;

    }
    call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
    call->m_outgoing->m_outstandingMessage =
      H248STACK_IsdnMessageBuilder::buildAddChooseOutgoingEphemeral(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId);
    m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
    call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
  }
  else
  {
    /*if (call->m_incoming->m_timer != 0)
    {
      call->m_incoming->m_outstandingMessage.clear();
      m_adapter->stopTimer(call->m_incoming->m_timer);
      call->m_incoming->m_timer = 0;
    }
    call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
    call->m_incoming->m_outstandingMessage =
    H248STACK_IsdnMessageBuilder::buildAddChooseIncoming(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_physical, m_database->getEventRequestId());
    m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
    call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
    */
  }
  return xcode;

}
 //virtual Poco::UInt32 modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId);                                                   //G, I,K, Q,O, R,S
Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC STARTED;  Event:  modifyReply2\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC STARTED;  Event:  addReply1\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC STARTED;  Event:  addReply2\n");
  // message not expected in this state
  if (direction == SmallMGC_OUT)
    {
      if (call->m_outgoing->m_timer != 0)
      {
        m_adapter->stopTimer(call->m_outgoing->m_timer);
        call->m_outgoing->m_timer = 0;
        m_adapter->bearerInformationInd(call, direction,localSDP);
        call->m_outleg = SmallMGC_IsdnCC_InProgressFsm::instance();
      }
    }
    else
    {
      if (call->m_incoming->m_timer != 0)
      {
        m_adapter->stopTimer(call->m_incoming->m_timer);
        call->m_incoming->m_timer = 0;
        m_adapter->bearerInformationInd(call, direction,localSDP);
        call->m_inleg = SmallMGC_IsdnCC_InProgressFsm::instance();

      }
    }

  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::subtractReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC STARTED;  Event:  subtractReply1\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::notifyDigit(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& digits)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC STARTED;  Event:  notify Digit\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::notifyOffHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC STARTED;  Event:  notify off hook\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::notifyOnHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC STARTED;  Event:  notify on hook\n");
  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::timerExpiry(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId,std::string& term)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC STARTED;  Event:  timer Expiry\n");
  xcode = SmallMGC_CC_Fsm::timerExpiry(call, direction);

  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::startCallReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC STARTED;  Event:  start call req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::bearerInformationReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& sdp)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC STARTED;  Event:  bearer information req\n");
  if (direction == SmallMGC_IN)
  {
    if (call->m_incoming->m_timer != 0)
    {
      // stop timer in case waiting for more digits
      m_adapter->stopTimer(call->m_incoming->m_timer);
      call->m_incoming->m_timer = 0;
    }

    call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
    m_database->setCallObjectTransaction(call->m_incoming->m_outstandingReplyTransactionId, call);
    call->m_incoming->m_outstandingMessage =
      H248STACK_IsdnMessageBuilder::buildAddChooseIncomingEphemeral(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_physical, m_database->getEventRequestId(), sdp);
    m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
    call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
  }
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::establishedReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC STARTED;  Event:  established req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::releaseReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC STARTED;  Event:  release req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::ringingReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC STARTED;  Event:  ringing req\n");
  if (direction == SmallMGC_OUT)
  {
     if (call->m_outgoing->m_timer != 0)
     {
       m_adapter->stopTimer(call->m_outgoing->m_timer);
       call->m_outgoing->m_timer =0;
     }
     m_adapter->disconnectReq(call, call->m_outgoing->m_isdnIntf, call->m_outgoing->m_isdnCall);

     call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
     call->m_outgoing->m_outstandingMessage =
         H248STACK_IsdnMessageBuilder::buildModifyBusy(call->m_outgoing->m_outstandingReplyTransactionId,
                            call->m_outgoing->m_contextId, call->m_outgoing->m_physical, call->m_outgoing->m_ephemeral, m_database );
     m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
     call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
     call->m_outleg = SmallMGC_IsdnCC_ReleasingFsm::instance();
  }
  else
  {
    if (call->m_incoming->m_timer != 0)
     {
       m_adapter->stopTimer(call->m_incoming->m_timer);
       call->m_incoming->m_timer = 0;
     }
    m_adapter->disconnectReq(call, call->m_incoming->m_isdnIntf, call->m_incoming->m_isdnCall);

     call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
     call->m_incoming->m_outstandingMessage =
         H248STACK_IsdnMessageBuilder::buildModifyBusy(call->m_incoming->m_outstandingReplyTransactionId,
                           call->m_incoming->m_contextId, call->m_incoming->m_physical, call->m_incoming->m_ephemeral, m_database );
     m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
     call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
     call->m_inleg = SmallMGC_IsdnCC_ReleasingFsm::instance();
  }

  return xcode;

}


Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::setup(class H248STACK_Call* call, enum SmallMGCLegDirection direction, enum SmallMGCIsdnBABearer bearer)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC STARTED;  Event:  isdn setup\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::information(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& number)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC STARTED;  Event:  isdn information\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::alerting(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC STARTED;  Event:  isdn alerting\n");
  // message not expected in this state
  if (direction == SmallMGC_IN)
  {
    call->m_incoming->m_alerting = true;
  }

  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::connect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC STARTED;  Event:  isdn connect\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::connectAck(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC STARTED;  Event:  isdn connectAck\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::disconnect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC STARTED;  Event:  isdn disconnect\n");
  if (direction == SmallMGC_OUT)
  {
    if(call->m_releaser == H248STACK_NotReleased)
    {
      call->m_releaser = H248STACK_Outgoing;
      call->m_outgoing->m_isdnReleased = true;
      m_adapter->releaseIsdnReq(call, call->m_outgoing->m_isdnIntf, call->m_outgoing->m_isdnCall);
      call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();

      call->m_outgoing->m_outstandingMessage =
        H248STACK_IsdnMessageBuilder::buildModifyInactiveIsdn(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId, call->m_outgoing->m_physical);
      m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
      m_adapter->releaseInd(call, direction);
      call->m_outleg = SmallMGC_IsdnCC_ReleasingFsm::instance();

    }
  }

  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::release(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC STARTED;  Event:  isdn release\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::releaseComplete(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC STARTED;  Event:  isdn release complete\n");
  // message not expected in this state
  return xcode;

}


 Poco::UInt32 SmallMGC_IsdnCC_StartedFsm::sendToMg(std::string const& msg)
 {
   // for reply messages
 }
