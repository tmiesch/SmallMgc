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
#include "smallmgcnccchangefaxfsm.h"
#include "smallmgcnccchangemodemfsm.h"
#include "smallmgcnccmodemestablishedfsm.h"

#include "h248stack_calladapter.h"
#include "smallmgc_h_messagebuilder.h"

#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"


using Poco::Util::Application;
using Poco::NumberFormatter;


SmallMGC_NCC_ChangeModemFsm::SmallMGC_NCC_ChangeModemFsm()
{
  // TODO Auto-generated constructor stub

}

SmallMGC_NCC_ChangeModemFsm::~SmallMGC_NCC_ChangeModemFsm()
{
  // TODO Auto-generated destructor stub
}

SmallMGC_NCC_ChangeModemFsm*  SmallMGC_NCC_ChangeModemFsm::s_instance = 0;

SmallMGC_NCC_ChangeModemFsm*  SmallMGC_NCC_ChangeModemFsm::instance(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter)
{
  if(s_instance == 0)
  {
    Poco::UInt32 xcode;
    s_instance = new SmallMGC_NCC_ChangeModemFsm(database, adapter);
  }
  return s_instance;
}


Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::startCall(class H248STACK_Call* call, Poco::UInt32 transactionId, std::string& term )
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: NCC ChangeModem;  Event:  startCall\n");
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  modifyReply1\n");

  if (direction == SmallMGC_OUT)
  {
    if (call->m_outgoing->m_timer != 0)
    {
      call->m_outgoing->m_outstandingMessage.clear();
      m_adapter->stopTimer(call->m_outgoing->m_timer);
      call->m_outgoing->m_timer = 0;
    }
    call->m_outleg = SmallMGC_NCC_ReleasingFsm::instance();

  }
  else
  {
    if (call->m_incoming->m_timer != 0)
    {
      call->m_incoming->m_outstandingMessage.clear();
      m_adapter->stopTimer(call->m_incoming->m_timer);
      call->m_incoming->m_timer = 0;
    }
    call->m_inleg = SmallMGC_NCC_ReleasingFsm::instance();
  }
  return xcode;

}
 //virtual Poco::UInt32 modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId);                                                   //G, I,K, Q,O, R,S
Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  modifyReply2\n");

  if (direction == SmallMGC_OUT)
  {
    if (  call->m_outgoing->m_timer != 0)
    {
      m_adapter->stopTimer(call->m_outgoing->m_timer);
      call->m_outgoing->m_timer = 0;
    }
    if (call->m_releaser == H248STACK_NotReleased)
    {
      m_adapter->bearerInformationInd(call, direction,localSDP);
      call->m_outleg = SmallMGC_NCC_ModemEstablishedFsm::instance();
    }
    else
    {
    }
  }
  else
  {
    if (  call->m_incoming->m_timer != 0)
    {
       m_adapter->stopTimer(call->m_incoming->m_timer);
       call->m_incoming->m_timer = 0;
    }

    if (call->m_releaser == H248STACK_NotReleased)
    {
      m_adapter->bearerInformationInd(call, direction,localSDP);
      call->m_inleg = SmallMGC_NCC_ModemEstablishedFsm::instance();
    }
  }
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  addReply1\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  addReply2\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::subtractReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  subtractReply1\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::notifyDigit(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& digits)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  notify Digit\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::notifyOffHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  notify off hook\n");
}
Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::notifyOnHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  notify on hook\n");
  if (direction == SmallMGC_OUT)
  {
    if (call->m_releaser == H248STACK_NotReleased)
    {
      call->m_releaser = H248STACK_Outgoing;
      call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();

      //call->m_outgoing->m_outstandingMessage =
        //       SmallMGC_NMessageBuilder::buildModifyReleasing(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId, call->m_outgoing->m_physical, call->m_outgoing->m_ephemeral, m_database->getEventRequestId());
       m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
       m_adapter->releaseInd(call, direction);
    }
    else
    {
      if (  call->m_outgoing->m_timer != 0)
      {
        m_adapter->stopTimer(call->m_outgoing->m_timer);
        call->m_outgoing->m_timer = 0;
      }
      call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();

      //call->m_outgoing->m_outstandingMessage =
        //        SmallMGC_NMessageBuilder::buildSubtract(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId);
      m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);

    }
    call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
    call->m_outleg = SmallMGC_NCC_ReleasingFsm::instance();
  }
  else
  {
    if (call->m_releaser == H248STACK_NotReleased)
    {
      call->m_releaser = H248STACK_Incoming;
      call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();

      //call->m_incoming->m_outstandingMessage =
        //     SmallMGC_NMessageBuilder::buildModifyReleasing(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId, call->m_incoming->m_physical, call->m_incoming->m_ephemeral, m_database->getEventRequestId());
      m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
      m_adapter->releaseInd(call, direction);
    }
    else
    {
      if (  call->m_incoming->m_timer != 0)
      {
         m_adapter->stopTimer(call->m_incoming->m_timer);
         call->m_incoming->m_timer = 0;
      }
      call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
      //call->m_incoming->m_outstandingMessage =
        //   SmallMGC_NMessageBuilder::buildSubtract(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId);
      m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);

    }
    call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
    call->m_inleg = SmallMGC_NCC_ReleasingFsm::instance();
  }
  return xcode;

}

Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::notifyTone(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& tones)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  notify tone\n");
  return xcode;
}

Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::notifyFlashHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  notify flash hook\n");
  return xcode;
}

Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::timerExpiry(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId,std::string& term)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  timer Expiry\n");
  xcode = SmallMGC_CC_Fsm::timerExpiry(call, direction);

  return xcode;

}

Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::startCallReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  start call req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::bearerInformationReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& sdp)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  bearer information req\n");
  if (direction == SmallMGC_OUT)
  {
    if (  call->m_outgoing->m_timer != 0)
    {
      m_adapter->stopTimer(call->m_outgoing->m_timer);
      call->m_outgoing->m_timer = 0;
    }
    call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();

    //call->m_outgoing->m_outstandingMessage =
      //          SmallMGC_NMessageBuilder::buildModifyModem(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId,
              //        call->m_outgoing->m_physical, call->m_outgoing->m_ephemeral, call->m_outgoing->m_remoteDescriptor, m_database->getEventRequestId());
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
    //call->m_incoming->m_outstandingMessage =
      //  SmallMGC_NMessageBuilder::buildModifyModem(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId,
               //             call->m_incoming->m_physical, call->m_incoming->m_ephemeral, call->m_incoming->m_remoteDescriptor, m_database->getEventRequestId());
    m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);

    call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
  }
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::establishedReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  ChangeModem req\n");
  return xcode;
}
Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::releaseReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  release req\n");
  if (direction == SmallMGC_OUT)
  {
     if (call->m_outgoing->m_timer != 0)
     {
       m_adapter->stopTimer(call->m_outgoing->m_timer);
       call->m_outgoing->m_timer = 0;
     }
     call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
     //call->m_outgoing->m_outstandingMessage =
       //  SmallMGC_NMessageBuilder::buildModifyBusy(call->m_outgoing->m_outstandingReplyTransactionId,
                     //       call->m_outgoing->m_contextId, call->m_outgoing->m_physical, call->m_outgoing->m_ephemeral, m_database->getEventRequestId() );
     m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
     call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
  }
  else
  {
    if (call->m_incoming->m_timer != 0)
     {
       m_adapter->stopTimer(call->m_incoming->m_timer);
     }
     call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
     //call->m_incoming->m_outstandingMessage =
       //  SmallMGC_NMessageBuilder::buildModifyBusy(call->m_incoming->m_outstandingReplyTransactionId,
       //                    call->m_incoming->m_contextId, call->m_incoming->m_physical, call->m_incoming->m_ephemeral, m_database->getEventRequestId() );
     m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
     call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
  }

  return xcode;

}

Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::ringingReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  ringing req\n");
  return xcode;
}
Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::switchToT38Req(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  switchToT38 req\n");
  return xcode;

}

Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::switchToModemReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  switchToModem req\n");
  return xcode;
}

Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::setup(class H248STACK_Call* call, enum SmallMGCLegDirection direction, enum SmallMGCIsdnBABearer bearer)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  isdn setup\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::information(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& number)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  isdn information\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::alerting(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  isdn alerting\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::connect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  isdn connect\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::connectAck(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  isdn connectAck\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::disconnect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  isdn disconnect\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::release(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  isdn release\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::releaseComplete(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeModem;  Event:  isdn release complete\n");
  // message not expected in this state
  return xcode;

}


 Poco::UInt32 SmallMGC_NCC_ChangeModemFsm::sendToMg(std::string const& msg)
 {
   // for reply messages
 }

