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
#include "smallmgchccidlefsm.h"
#include "smallmgchccstartedfsm.h"
#include "smallmgchccwaitingnumberfsm.h"
#include "smallmgchccreleasingfsm.h"

#include "smallmgc_h_messagebuilder.h"
#include "h248stack_calladapter.h"
#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"


using Poco::Util::Application;
using Poco::NumberFormatter;


SmallMGC_HCC_IdleFsm::SmallMGC_HCC_IdleFsm()
{
  // TODO Auto-generated constructor stub

}

SmallMGC_HCC_IdleFsm::~SmallMGC_HCC_IdleFsm()
{
  // TODO Auto-generated destructor stub
}

SmallMGC_HCC_IdleFsm*  SmallMGC_HCC_IdleFsm::s_instance = 0;

SmallMGC_HCC_IdleFsm*  SmallMGC_HCC_IdleFsm::instance(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter)
{
  if(s_instance == 0)
  {
    Poco::UInt32 xcode;
    s_instance = new SmallMGC_HCC_IdleFsm(database, adapter);
  }
  return s_instance;
}


Poco::UInt32 SmallMGC_HCC_IdleFsm::startCall(class H248STACK_Call* call, Poco::UInt32 transactionId, std::string& term )
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: HCC IDLE;  Event:  startCall\n");

  call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
  call->m_outgoing->m_outstandingMessage = SmallMGC_HMessageBuilder::buildModifyOffHookDialTone(call->m_outgoing->m_outstandingReplyTransactionId, term,m_database->getEventRequestId());
  m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
  call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", term );

  m_adapter->startCall(call, SmallMGC_OUT);
  call->m_outgoing->m_dialledDigits.clear();

  return xcode;

}
Poco::UInt32 SmallMGC_HCC_IdleFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  modifyReply1\n");
  if (direction == SmallMGC_OUT)
  {
    if (call->m_outgoing->m_timer != 0)
    {
      m_adapter->stopTimer(call->m_outgoing->m_timer);
      call->m_outgoing->m_timer = 0;
    }
    call->m_outgoing->m_repeations = 0;
    // clear outstanding message
    call->m_outgoing->m_outstandingMessage.clear();

    if (call->m_releaser == H248STACK_Outgoing)
    {
      m_adapter->idleInd(call, direction);
    }
  }

  return xcode;

}
 //virtual Poco::UInt32 modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId);                                                   //G, I,K, Q,O, R,S
Poco::UInt32 SmallMGC_HCC_IdleFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  modifyReply2\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_IdleFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  addReply1\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_IdleFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  addReply2\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_HCC_IdleFsm::subtractReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  subtractReply1\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_HCC_IdleFsm::notifyDigit(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& digits)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  notify Digit\n");

  call->m_outgoing->m_dialledDigits.clear();
  call->m_outgoing->m_dialledDigits.append(digits);

  call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
  call->m_outgoing->m_outstandingMessage =
  SmallMGC_HMessageBuilder::buildModifyMoreDigits(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_physical, m_database->getEventRequestId());
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
          SmallMGC_HMessageBuilder::buildModifyBusyNoEphemeral(call->m_outgoing->m_outstandingReplyTransactionId,
                       "-", call->m_outgoing->m_physical, m_database );
      m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
      call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
  }
  else if (busy == 2)
  {
    call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
    call->m_outgoing->m_outstandingMessage =
       SmallMGC_HMessageBuilder::buildModifyBusyNoEphemeral(call->m_outgoing->m_outstandingReplyTransactionId,
                               "-", call->m_outgoing->m_physical, m_database );
    m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
    call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
  }
  else
  {
    // use incoming timer to look after dial time out for outgoing termination
    call->m_incoming->m_timer = m_adapter->startTimer(20000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
  }
  call->m_outleg = SmallMGC_HCC_WaitingNumberFsm::instance();
  return xcode;
}

Poco::UInt32 SmallMGC_HCC_IdleFsm::notifyOffHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  notify off hook\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_IdleFsm::notifyOnHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  notify on hook\n");
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
     SmallMGC_HMessageBuilder::buildModifyAfterStartup(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_physical,
                                        m_database->getEventRequestId());
    m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
   call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);

  }
  return xcode;

}

Poco::UInt32 SmallMGC_HCC_IdleFsm::notifyTone(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& tones)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  notify tone\n");
  return xcode;
}

Poco::UInt32 SmallMGC_HCC_IdleFsm::notifyFlashHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  notify flash hook\n");
  return xcode;
}
Poco::UInt32 SmallMGC_HCC_IdleFsm::timerExpiry(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId,std::string& term)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  timer Expiry\n");
  xcode = SmallMGC_CC_Fsm::timerExpiry(call, direction);

  return xcode;

}

Poco::UInt32 SmallMGC_HCC_IdleFsm::startCallReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  start call req\n");
  call->m_inleg = SmallMGC_HCC_StartedFsm::instance();
  call->m_incoming->m_dialledDigits.clear();
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_IdleFsm::bearerInformationReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& sdp)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  bearer information req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_IdleFsm::establishedReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  established req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_IdleFsm::releaseReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  release req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_IdleFsm::ringingReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  ringing req\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_HCC_IdleFsm::switchToT38Req(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  switchToT38 req\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_HCC_IdleFsm::switchToModemReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  switchToModem req\n");
  // message not expected in this state
  return xcode;

}


Poco::UInt32 SmallMGC_HCC_IdleFsm::setup(class H248STACK_Call* call, enum SmallMGCLegDirection direction, enum SmallMGCIsdnBABearer bearer)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  isdn setup\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_IdleFsm::information(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& number)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  isdn information\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_IdleFsm::alerting(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  isdn alerting\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_IdleFsm::connect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  isdn connect\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_IdleFsm::connectAck(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  isdn connectAck\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_IdleFsm::disconnect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  isdn disconnect\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_IdleFsm::release(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  isdn release\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_IdleFsm::releaseComplete(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC IDLE;  Event:  isdn release complete\n");
  // message not expected in this state
  return xcode;

}


 Poco::UInt32 SmallMGC_HCC_IdleFsm::sendToMg(std::string const& msg)
 {
   // for reply messages
 }
