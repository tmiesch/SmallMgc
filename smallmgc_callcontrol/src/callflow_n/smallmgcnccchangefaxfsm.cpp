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
# include <algorithm>
# include <cctype>

#include "smallmgcgccfsm.h"

#include "smallmgcnccreleasingfsm.h"
#include "smallmgcnccchangefaxfsm.h"
#include "smallmgcnccchangemodemfsm.h"
#include "smallmgcnccfaxestablishedfsm.h"

#include "h248stack_calladapter.h"
#include "smallmgc_h_messagebuilder.h"

#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"


using Poco::Util::Application;
using Poco::NumberFormatter;


SmallMGC_NCC_ChangeFaxFsm::SmallMGC_NCC_ChangeFaxFsm()
{
  // TODO Auto-generated constructor stub

}

SmallMGC_NCC_ChangeFaxFsm::~SmallMGC_NCC_ChangeFaxFsm()
{
  // TODO Auto-generated destructor stub
}

SmallMGC_NCC_ChangeFaxFsm*  SmallMGC_NCC_ChangeFaxFsm::s_instance = 0;

SmallMGC_NCC_ChangeFaxFsm*  SmallMGC_NCC_ChangeFaxFsm::instance(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter)
{
  if(s_instance == 0)
  {
    Poco::UInt32 xcode;
    s_instance = new SmallMGC_NCC_ChangeFaxFsm(database, adapter);
  }
  return s_instance;
}


Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::startCall(class H248STACK_Call* call, Poco::UInt32 transactionId, std::string& term )
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: NCC ChangeFax;  Event:  startCall\n");
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  modifyReply1\n");

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
Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  modifyReply2\n");

  if (direction == SmallMGC_OUT)
  {
    if (  call->m_outgoing->m_timer != 0)
    {
      m_adapter->stopTimer(call->m_outgoing->m_timer);
      call->m_outgoing->m_timer = 0;
    }
    if (call->m_releaser == H248STACK_NotReleased)
    {
      if (call->m_outgoing->m_modifyReceived > 0)
      {
        m_adapter->faxEstablishedInd(call, direction);
        call->m_outleg = SmallMGC_NCC_FaxEstablishedFsm::instance();
      }
      else
      {
        m_adapter->bearerInformationInd(call, direction,localSDP);
        if (!call->m_outgoing->m_secondModifyExpected == true)
          call->m_outleg = SmallMGC_NCC_FaxEstablishedFsm::instance();
      }
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
      if (call->m_incoming->m_modifyReceived > 0)
      {
        m_adapter->faxEstablishedInd(call, direction);
        call->m_inleg = SmallMGC_NCC_FaxEstablishedFsm::instance();
      }
      else
      {
        m_adapter->bearerInformationInd(call, direction,localSDP);
        if (!call->m_incoming->m_secondModifyExpected == true)
          call->m_inleg = SmallMGC_NCC_FaxEstablishedFsm::instance();
      }
    }
  }
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  addReply1\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  addReply2\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::subtractReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  subtractReply1\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::notifyDigit(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& digits)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  notify Digit\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::notifyOffHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  notify off hook\n");
}
Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::notifyOnHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  notify on hook\n");
  if (direction == SmallMGC_OUT)
  {
    if (call->m_releaser == H248STACK_NotReleased)
    {
      call->m_releaser = H248STACK_Outgoing;
      call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();

     // call->m_outgoing->m_outstandingMessage =
       //        SmallMGC_NMessageBuilder::buildModifyReleasing(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId, call->m_outgoing->m_physical, call->m_outgoing->m_ephemeral, m_database->getEventRequestId());
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

     // call->m_outgoing->m_outstandingMessage =
       //         SmallMGC_NMessageBuilder::buildSubtract(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId);
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
       //      SmallMGC_NMessageBuilder::buildModifyReleasing(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId, call->m_incoming->m_physical, call->m_incoming->m_ephemeral, m_database->getEventRequestId());
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
      //     SmallMGC_NMessageBuilder::buildSubtract(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId);
      m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);

    }
    call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
    call->m_inleg = SmallMGC_NCC_ReleasingFsm::instance();
  }
  return xcode;

}

Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::notifyTone(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& tones)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  notify tone\n");
  return xcode;
}

Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::notifyFlashHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  notify flash hook\n");
  return xcode;
}

Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::timerExpiry(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId,std::string& term)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  timer Expiry\n");
  xcode = SmallMGC_CC_Fsm::timerExpiry(call, direction);

  return xcode;

}

Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::startCallReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  start call req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::bearerInformationReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& sdp)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  bearer information req\n");
  if (direction == SmallMGC_OUT)
  {
    if (  call->m_outgoing->m_timer != 0)
    {
      m_adapter->stopTimer(call->m_outgoing->m_timer);
      call->m_outgoing->m_timer = 0;
    }
    call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();

   // call->m_outgoing->m_outstandingMessage =
    //            SmallMGC_NMessageBuilder::buildModifyLocalChooseFax(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId,
               //       call->m_outgoing->m_physical, call->m_outgoing->m_ephemeral, call->m_outgoing->m_remoteDescriptor, m_database->getEventRequestId());
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
      //  SmallMGC_NMessageBuilder::buildModifyLocalChooseFax(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId,
        //                    call->m_incoming->m_physical, call->m_incoming->m_ephemeral, call->m_incoming->m_remoteDescriptor, m_database->getEventRequestId());
    m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);

    call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
  }
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::establishedReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  ChangeFax req\n");
  return xcode;
}
Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::releaseReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  release req\n");
  if (direction == SmallMGC_OUT)
  {
     if (call->m_outgoing->m_timer != 0)
     {
       m_adapter->stopTimer(call->m_outgoing->m_timer);
       call->m_outgoing->m_timer = 0;
     }
     call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
    // call->m_outgoing->m_outstandingMessage =
     //    SmallMGC_NMessageBuilder::buildModifyBusy(call->m_outgoing->m_outstandingReplyTransactionId,
     //                 call->m_outgoing->m_contextId, call->m_outgoing->m_physical, call->m_outgoing->m_ephemeral, m_database->getEventRequestId() );
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
      //   SmallMGC_NMessageBuilder::buildModifyBusy(call->m_incoming->m_outstandingReplyTransactionId,
        //                   call->m_incoming->m_contextId, call->m_incoming->m_physical, call->m_incoming->m_ephemeral, m_database->getEventRequestId() );
     m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
     call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
  }

  return xcode;

}

Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::ringingReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  ringing req\n");
  return xcode;
}
Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::switchToT38Req(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  switchToT38 req\n");
  return xcode;

}

Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::switchToModemReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  switchToModem req\n");
  return xcode;
}

Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::setup(class H248STACK_Call* call, enum SmallMGCLegDirection direction, enum SmallMGCIsdnBABearer bearer)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  isdn setup\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::information(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& number)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  isdn information\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::alerting(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  isdn alerting\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::connect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  isdn connect\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::connectAck(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  isdn connectAck\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::disconnect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  isdn disconnect\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::release(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  isdn release\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::releaseComplete(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: NCC ChangeFax;  Event:  isdn release complete\n");
  // message not expected in this state
  return xcode;

}


 Poco::UInt32 SmallMGC_NCC_ChangeFaxFsm::sendToMg(std::string const& msg)
 {
   // for reply messages
 }
