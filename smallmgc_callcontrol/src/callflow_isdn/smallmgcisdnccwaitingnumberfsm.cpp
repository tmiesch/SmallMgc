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

#include "smallmgcisdnccwaitingnumberfsm.h"
#include "smallmgcisdnccinprogressfsm.h"
#include "smallmgcisdnccreleasingfsm.h"
#include "smallmgcisdnccstartedfsm.h"

#include "h248stack_calladapter.h"
#include "h248stack_isdn_messagebuilder.h"

#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"


using Poco::Util::Application;
using Poco::NumberFormatter;

SmallMGC_IsdnCC_WaitingNumberFsm::SmallMGC_IsdnCC_WaitingNumberFsm()
{
  // TODO Auto-generated constructor stub

}

SmallMGC_IsdnCC_WaitingNumberFsm::~SmallMGC_IsdnCC_WaitingNumberFsm()
{
  // TODO Auto-generated destructor stub
}

SmallMGC_IsdnCC_WaitingNumberFsm*  SmallMGC_IsdnCC_WaitingNumberFsm::s_instance = 0;

SmallMGC_IsdnCC_WaitingNumberFsm*  SmallMGC_IsdnCC_WaitingNumberFsm::instance(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter)
{
  if(s_instance == 0)
  {
    Poco::UInt32 xcode;
    s_instance = new SmallMGC_IsdnCC_WaitingNumberFsm(database, adapter);
  }
  return s_instance;
}


Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::startCall(class H248STACK_Call* call, Poco::UInt32 transactionId, std::string& term )
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: IsdnCC WaitingNumber;  Event:  startCall\n");
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC WaitingNumber;  Event:  modifyReply1\n");

  if (direction == SmallMGC_OUT)
  {
    if (call->m_outgoing->m_timer != 0)
    {
      call->m_outgoing->m_outstandingMessage.clear();
      m_adapter->stopTimer(call->m_outgoing->m_timer);
      call->m_outgoing->m_timer = 0;
    }
    if (call->m_outgoing->m_stopTonesNecessary == true)
    {
      call->m_outgoing->m_stopTonesNecessary = false;
      call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
      call->m_outgoing->m_outstandingMessage =
         H248STACK_IsdnMessageBuilder::buildModifyNoSignal(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId, call->m_outgoing->m_physical);
      m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
      call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
      call->m_outleg = SmallMGC_IsdnCC_StartedFsm::instance();
    }
  }

  return xcode;

}
 //virtual Poco::UInt32 modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId);                                                   //G, I,K, Q,O, R,S
Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC WaitingNumber;  Event:  modifyReply2\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC WaitingNumber;  Event:  addReply1\n");
  if (direction == SmallMGC_OUT)
  {
    if (call->m_outgoing->m_timer != 0)
    {
      m_adapter->stopTimer(call->m_outgoing->m_timer);
    }
    call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
    call->m_outgoing->m_outstandingMessage =
       H248STACK_IsdnMessageBuilder::buildModifyDialIsdn(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId, call->m_outgoing->m_physical);
    m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
    call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);

  }
  else
  {
  }
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC WaitingNumber;  Event:  addReply2\n");
  if (direction == SmallMGC_OUT)
    {
      if (call->m_outgoing->m_timer != 0)
      {
        m_adapter->stopTimer(call->m_outgoing->m_timer);
      }
      call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
      call->m_outgoing->m_outstandingMessage =
         H248STACK_IsdnMessageBuilder::buildModifyDialIsdn(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId, call->m_outgoing->m_physical);
      m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
      call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);

    }
    else
    {
    }
  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::subtractReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC WaitingNumber;  Event:  subtractReply1\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::notifyDigit(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& digits)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC WaitingNumber;  Event:  notify Digit\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::notifyOffHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC WaitingNumber;  Event:  notify off hook\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::notifyOnHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC WaitingNumber;  Event:  notify on hook\n");
  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::timerExpiry(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId,std::string& term)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC WaitingNumber;  Event:  timer Expiry\n");
  xcode = SmallMGC_CC_Fsm::timerExpiry(call, direction);

  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::startCallReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC WaitingNumber;  Event:  start call req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::bearerInformationReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& sdp)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC WaitingNumber;  Event:  bearer information req\n");
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::establishedReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC WaitingNumber;  Event:  established req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::releaseReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC WaitingNumber;  Event:  release req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::ringingReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC WaitingNumber;  Event:  ringing req\n");
  // message not expected in this state
  return xcode;

}


Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::setup(class H248STACK_Call* call, enum SmallMGCLegDirection direction, enum SmallMGCIsdnBABearer bearer)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC WaitingNumber;  Event:  isdn setup\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::information(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& number)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC WaitingNumber;  Event:  isdn information\n");
  // message not expected in this state
  if (direction == SmallMGC_OUT)
  {
      if (call->m_outgoing->m_timer != 0)
      {
        call->m_outgoing->m_stopTonesNecessary = true;
      }
      else
      {
        if (call->m_outgoing->m_dialledDigits.empty())
        {
          call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
          call->m_outgoing->m_outstandingMessage =
            H248STACK_IsdnMessageBuilder::buildModifyNoSignal(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId, call->m_outgoing->m_physical);
          m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
          call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
          // call->m_outleg = SmallMGC_IsdnCC_StartedFsm::instance();
        }
      }

      call->m_outgoing->m_dialledDigits.append(number);
      Poco::UInt32 busy = m_adapter->checkNumber(call, direction, call->m_outgoing->m_dialledDigits);
      if (busy == 0)
      {
        // number found
        call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
        // add context and ephemeral
        m_database->setCallObjectTransaction(call->m_incoming->m_outstandingReplyTransactionId, call);

        call->m_outgoing->m_outstandingMessage =
          H248STACK_IsdnMessageBuilder::buildAddChooseOutgoingEphemeral(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId);
        m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
        call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);

        m_adapter->callProceedingReq(call, call->m_outgoing->m_isdnIntf, call->m_outgoing->m_isdnCall);

        call->m_outleg = SmallMGC_IsdnCC_StartedFsm::instance();
      }
  }
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::alerting(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC WaitingNumber;  Event:  isdn alerting\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::connect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC WaitingNumber;  Event:  isdn connect\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::connectAck(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC WaitingNumber;  Event:  isdn connectAck\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::disconnect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC WaitingNumber;  Event:  isdn disconnect\n");
  if (direction == SmallMGC_OUT)
  {
    if(call->m_releaser == H248STACK_NotReleased)
    {
      call->m_releaser = H248STACK_Outgoing;
      //call->m_outgoing->m_isdnReleased = true;
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
Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::release(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC WaitingNumber;  Event:  isdn release\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::releaseComplete(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC WaitingNumber;  Event:  isdn release complete\n");
  // message not expected in this state
  return xcode;

}


 Poco::UInt32 SmallMGC_IsdnCC_WaitingNumberFsm::sendToMg(std::string const& msg)
 {
   // for reply messages
 }

