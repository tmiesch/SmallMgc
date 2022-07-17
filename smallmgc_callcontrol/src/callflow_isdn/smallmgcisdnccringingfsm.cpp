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

#include "smallmgcisdnccringingfsm.h"
#include "smallmgcisdnccestablishedfsm.h"
#include "smallmgcisdnccreleasingfsm.h"

#include "h248stack_calladapter.h"
#include "h248stack_isdn_messagebuilder.h"


#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"


using Poco::Util::Application;
using Poco::NumberFormatter;


SmallMGC_IsdnCC_RingingFsm::SmallMGC_IsdnCC_RingingFsm()
{
  // TODO Auto-generated constructor stub

}

SmallMGC_IsdnCC_RingingFsm::~SmallMGC_IsdnCC_RingingFsm()
{
  // TODO Auto-generated destructor stub
}

SmallMGC_IsdnCC_RingingFsm*  SmallMGC_IsdnCC_RingingFsm::s_instance = 0;

SmallMGC_IsdnCC_RingingFsm*  SmallMGC_IsdnCC_RingingFsm::instance(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter)
{
  if(s_instance == 0)
  {
    Poco::UInt32 xcode;
    s_instance = new SmallMGC_IsdnCC_RingingFsm(database, adapter);
  }
  return s_instance;
}


Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::startCall(class H248STACK_Call* call, Poco::UInt32 transactionId, std::string& term )
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: IsdnCC Ringing;  Event:  startCall\n");
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Ringing;  Event:  modifyReply1\n");

  if (direction == SmallMGC_OUT)
  {
    if (call->m_outgoing->m_timer != 0)
    {
      call->m_outgoing->m_outstandingMessage.clear();
      m_adapter->stopTimer(call->m_outgoing->m_timer);
      call->m_outgoing->m_timer = 0;
    }
    call->m_outleg = SmallMGC_IsdnCC_EstablishedFsm::instance();

  }
  else
  {
    if (call->m_incoming->m_timer != 0)
    {
      m_adapter->stopTimer(call->m_incoming->m_timer);
      call->m_incoming->m_timer = 0;
    }
    call->m_inleg = SmallMGC_IsdnCC_EstablishedFsm::instance();


  }
  return xcode;

}
 //virtual Poco::UInt32 modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId);                                                   //G, I,K, Q,O, R,S
Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Ringing;  Event:  modifyReply2\n");
  if (direction == SmallMGC_OUT)
    {
      if (call->m_outgoing->m_timer != 0)
      {
        call->m_outgoing->m_outstandingMessage.clear();
        m_adapter->stopTimer(call->m_outgoing->m_timer);
        call->m_outgoing->m_timer = 0;
      }
      call->m_outleg = SmallMGC_IsdnCC_EstablishedFsm::instance();

    }
    else
    {
      if (call->m_incoming->m_timer != 0)
      {
        m_adapter->stopTimer(call->m_incoming->m_timer);
        call->m_incoming->m_timer = 0;
      }
      call->m_inleg = SmallMGC_IsdnCC_EstablishedFsm::instance();


    }
    return xcode;
}
Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Ringing;  Event:  addReply1\n");
  // message not expected in this state
  if (direction == SmallMGC_IN)
  {
      call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();

      call->m_incoming->m_outstandingMessage =
              H248STACK_IsdnMessageBuilder::buildModifySR(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId, call->m_incoming->m_physical);
      m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
      call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_ephemeral);
  }
  else
  {
    //call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();

     //call->m_outgoing->m_outstandingMessage =
     //        H248STACK_IsdnMessageBuilder::buildModifySR(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId, call->m_outgoing->m_ephemeral);
     //m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
     //call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_ephemeral);
  }
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Ringing;  Event:  addReply2\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::subtractReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Ringing;  Event:  subtractReply1\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::notifyDigit(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& digits)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Ringing;  Event:  notify Digit\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::notifyOffHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Ringing;  Event:  notify off hook\n");
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::notifyOnHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Ringing;  Event:  notify on hook\n");
  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::timerExpiry(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId,std::string& term)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Ringing;  Event:  timer Expiry\n");
  if (direction == SmallMGC_OUT)
  {
    xcode = SmallMGC_CC_Fsm::timerExpiry(call, direction);
      }
  else
  {
    xcode = SmallMGC_CC_Fsm::timerExpiry(call, direction);
  }

  return xcode;

}

Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::startCallReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Ringing;  Event:  start call req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::bearerInformationReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& sdp)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Ringing;  Event:  bearer information req\n");
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::establishedReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Ringing;  Event:  established req\n");
  if (direction == SmallMGC_OUT)
  {
    m_adapter->connectReq(call, call->m_outgoing->m_isdnIntf, call->m_outgoing->m_isdnCall);
    call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
    call->m_outgoing->m_outstandingMessage =
               H248STACK_IsdnMessageBuilder::buildModifySRIsdn(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId, call->m_outgoing->m_physical);
    m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
    call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);

  }
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::releaseReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Ringing;  Event:  release req\n");
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
         H248STACK_IsdnMessageBuilder::buildModifyInactiveIsdn(call->m_incoming->m_outstandingReplyTransactionId,
                           call->m_incoming->m_contextId, call->m_incoming->m_ephemeral);
     //H248STACK_IsdnMessageBuilder::buildModifyBusy(call->m_incoming->m_outstandingReplyTransactionId,
     //                  call->m_incoming->m_contextId, call->m_incoming->m_physical, call->m_incoming->m_ephemeral, m_database );

     m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
     call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
     call->m_inleg = SmallMGC_IsdnCC_ReleasingFsm::instance();
  }

  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::ringingReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Ringing;  Event:  ringing req\n");
  return xcode;

}


Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::setup(class H248STACK_Call* call, enum SmallMGCLegDirection direction, enum SmallMGCIsdnBABearer bearer)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Ringing;  Event:  isdn setup\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::information(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& number)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Ringing;  Event:  isdn information\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::alerting(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Ringing;  Event:  isdn alerting\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::connect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Ringing;  Event:  isdn connect\n");
  if (direction == SmallMGC_IN)
  {
     m_adapter->connectAckReq(call, call->m_incoming->m_isdnIntf, call->m_incoming->m_isdnCall);
     call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();

     call->m_incoming->m_outstandingMessage =
           H248STACK_IsdnMessageBuilder::buildAddPhysical(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId, call->m_incoming->m_physical);
     m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
     call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_ephemeral);

     m_adapter->establishedInd(call, direction);
  }

  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::connectAck(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Ringing;  Event:  isdn connectAck\n");
  if (direction == SmallMGC_OUT)
  {
    call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
    call->m_outgoing->m_outstandingMessage =
               H248STACK_IsdnMessageBuilder::buildModifySR(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId, call->m_outgoing->m_ephemeral);
    m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
    call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_ephemeral);
  }
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::disconnect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Ringing;  Event:  isdn disconnect\n");
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
Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::release(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Ringing;  Event:  isdn release\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::releaseComplete(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: IsdnCC Ringing;  Event:  isdn release complete\n");
  // message not expected in this state
  return xcode;

}


 Poco::UInt32 SmallMGC_IsdnCC_RingingFsm::sendToMg(std::string const& msg)
 {
   // for reply messages
 }


