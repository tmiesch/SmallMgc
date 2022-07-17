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
#include "smallmgcisdnccidlefsm.h"
#include "smallmgcisdnccstartedfsm.h"
#include "smallmgcisdnccreleasingfsm.h"
#include "smallmgcisdnccwaitingnumberfsm.h"

#include "h248stack_isdn_messagebuilder.h"
#include "h248stack_calladapter.h"
#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"


using Poco::Util::Application;
using Poco::NumberFormatter;


SmallMGC_IsdnCC_IdleFsm::SmallMGC_IsdnCC_IdleFsm()
{
  // TODO Auto-generated constructor stub

}

SmallMGC_IsdnCC_IdleFsm::~SmallMGC_IsdnCC_IdleFsm()
{
  // TODO Auto-generated destructor stub
}

SmallMGC_IsdnCC_IdleFsm*  SmallMGC_IsdnCC_IdleFsm::s_instance = 0;

SmallMGC_IsdnCC_IdleFsm*  SmallMGC_IsdnCC_IdleFsm::instance(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter)
{
  if(s_instance == 0)
  {
    Poco::UInt32 xcode;
    s_instance = new SmallMGC_IsdnCC_IdleFsm(database, adapter);
  }
  return s_instance;
}


Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::startCall(class H248STACK_Call* call, Poco::UInt32 transactionId, std::string& term )
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: IsdnCC IDLE;  Event:  startCall\n");
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC IDLE;  Event:  modifyReply1\n");
  // message not expected in this state
  return xcode;

}
 //virtual Poco::UInt32 modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId);                                                   //G, I,K, Q,O, R,S
Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC IDLE;  Event:  modifyReply2\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC IDLE;  Event:  addReply1\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC IDLE;  Event:  addReply2\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::subtractReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC IDLE;  Event:  subtractReply1\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::notifyDigit(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& digits)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC IDLE;  Event:  notify Digit\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::notifyOffHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC IDLE;  Event:  notify off hook\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::notifyOnHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC IDLE;  Event:  notify on hook\n");
  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::timerExpiry(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId,std::string& term)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC IDLE;  Event:  timer Expiry\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::startCallReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC IDLE;  Event:  start call req\n");
  // set that interface is occupied
  m_adapter->setupReq(call, call->m_incoming->m_isdnIntf, 0);
  call->m_inleg = SmallMGC_IsdnCC_StartedFsm::instance();
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::bearerInformationReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& sdp)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC IDLE;  Event:  bearer information req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::establishedReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC IDLE;  Event:  established req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::releaseReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC IDLE;  Event:  release req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::ringingReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC IDLE;  Event:  ringing req\n");
  // message not expected in this state
  return xcode;

}


Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::setup(class H248STACK_Call* call, enum SmallMGCLegDirection direction, enum SmallMGCIsdnBABearer bearer)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC IDLE;  Event:  isdn setup\n");
  call->m_outgoing->m_ownNumber = call->m_outgoing->m_isdnIntf->getConfiguredNumber();
  m_adapter->startCall(call, SmallMGC_OUT);

  if (!call->m_outgoing->m_dialledDigits.empty())
  {
    if (!m_adapter->checkNumber(call, direction, call->m_outgoing->m_dialledDigits))
    {
      // number found
      call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
      // add context and ephemeral
      call->m_outgoing->m_outstandingMessage =
        H248STACK_IsdnMessageBuilder::buildAddChooseOutgoing(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_physical, m_database->getEventRequestId());
        //H248STACK_IsdnMessageBuilder::buildAddChooseOutgoingTDM(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_physical);
      m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
      call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);

      m_adapter->callProceedingReq(call, call->m_outgoing->m_isdnIntf, call->m_outgoing->m_isdnCall);

      call->m_outleg = SmallMGC_IsdnCC_StartedFsm::instance();
    }
    else
    {
      call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
      // only the context
      call->m_outgoing->m_outstandingMessage =
        H248STACK_IsdnMessageBuilder::buildAddChooseOutgoingTDM(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_physical);
      //H248STACK_IsdnMessageBuilder::buildAddChooseOutgoing(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_physical, m_database->getEventRequestId());
      m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
      call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);

      m_adapter->setupAckReq(call, call->m_outgoing->m_isdnIntf, call->m_outgoing->m_isdnCall);

      call->m_outleg = SmallMGC_IsdnCC_WaitingNumberFsm::instance();

    }
  }
  else
  {
    call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
    // only the context
    call->m_outgoing->m_outstandingMessage =
      H248STACK_IsdnMessageBuilder::buildAddChooseOutgoingTDM(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_physical);
    //H248STACK_IsdnMessageBuilder::buildAddChooseOutgoing(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_physical, m_database->getEventRequestId());
    m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
    call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);

    m_adapter->setupAckReq(call,call->m_outgoing->m_isdnIntf, call->m_outgoing->m_isdnCall);
    call->m_outleg = SmallMGC_IsdnCC_WaitingNumberFsm::instance();
  }
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::information(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& number)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC IDLE;  Event:  isdn information\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::alerting(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC IDLE;  Event:  isdn alerting\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::connect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC IDLE;  Event:  isdn connect\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::connectAck(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC IDLE;  Event:  isdn connectAck\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::disconnect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC IDLE;  Event:  isdn disconnect\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::release(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC IDLE;  Event:  isdn release\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::releaseComplete(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC IDLE;  Event:  isdn release complete\n");
  // message not expected in this state
  return xcode;

}


 Poco::UInt32 SmallMGC_IsdnCC_IdleFsm::sendToMg(std::string const& msg)
 {
   // for reply messages
 }

