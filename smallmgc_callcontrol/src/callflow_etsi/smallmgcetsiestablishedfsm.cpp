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

#include "smallmgcetsiestablishedfsm.h"
#include "smallmgcetsireleasingfsm.h"
#include "smallmgcetsichangefaxfsm.h"
#include "smallmgcetsichangemodemfsm.h"

#include "h248stack_calladapter.h"
#include "smallmgc_etsi_messagebuilder.h"

#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"


using Poco::Util::Application;
using Poco::NumberFormatter;


SmallMGC_ETSI_EstablishedFsm::SmallMGC_ETSI_EstablishedFsm()
{
  // TODO Auto-generated constructor stub

}

SmallMGC_ETSI_EstablishedFsm::~SmallMGC_ETSI_EstablishedFsm()
{
  // TODO Auto-generated destructor stub
}

SmallMGC_ETSI_EstablishedFsm*  SmallMGC_ETSI_EstablishedFsm::s_instance = 0;

SmallMGC_ETSI_EstablishedFsm*  SmallMGC_ETSI_EstablishedFsm::instance(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter)
{
  if(s_instance == 0)
  {
    Poco::UInt32 xcode;
    s_instance = new SmallMGC_ETSI_EstablishedFsm(database, adapter);
  }
  return s_instance;
}


Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::startCall(class H248STACK_Call* call, Poco::UInt32 transactionId, std::string& term )
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: HCC Established;  Event:  startCall\n");
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  modifyReply1\n");

  if (direction == SmallMGC_OUT)
  {
    if (call->m_outgoing->m_timer != 0)
    {
      call->m_outgoing->m_outstandingMessage.clear();
      m_adapter->stopTimer(call->m_outgoing->m_timer);
      call->m_outgoing->m_timer = 0;
    }
    call->m_outleg = SmallMGC_ETSI_ReleasingFsm::instance();

  }
  else
  {
    if (call->m_incoming->m_timer != 0)
    {
      call->m_incoming->m_outstandingMessage.clear();
      m_adapter->stopTimer(call->m_incoming->m_timer);
      call->m_incoming->m_timer = 0;
    }
    call->m_inleg = SmallMGC_ETSI_ReleasingFsm::instance();
  }
  return xcode;

}
 //virtual Poco::UInt32 modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId);                                                   //G, I,K, Q,O, R,S
Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  modifyReply2\n");

  if (direction == SmallMGC_OUT)
   {
     if (call->m_outgoing->m_timer != 0)
     {
       call->m_outgoing->m_outstandingMessage.clear();
       m_adapter->stopTimer(call->m_outgoing->m_timer);
       call->m_outgoing->m_timer = 0;
     }
     call->m_outleg = SmallMGC_ETSI_ReleasingFsm::instance();

   }
   else
   {
     if (call->m_incoming->m_timer != 0)
     {
       call->m_incoming->m_outstandingMessage.clear();
       m_adapter->stopTimer(call->m_incoming->m_timer);
       call->m_incoming->m_timer = 0;
     }
     call->m_inleg = SmallMGC_ETSI_ReleasingFsm::instance();
   }
   return xcode;


}
Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  addReply1\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  addReply2\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::subtractReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  subtractReply1\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::notifyDigit(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& digits)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  notify Digit\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::notifyOffHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  notify off hook\n");
}
Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::notifyOnHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  notify on hook\n");
  if (direction == SmallMGC_OUT)
  {
    if (call->m_releaser == H248STACK_NotReleased)
    {
      call->m_releaser = H248STACK_Outgoing;
      call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();

      call->m_outgoing->m_outstandingMessage =
    	  SmallMGC_EtsiMessageBuilder::buildModifyReleasing(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId, call->m_outgoing->m_physical, call->m_outgoing->m_ephemeral, m_database->getEventRequestId());
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

      call->m_outgoing->m_outstandingMessage =
    	  SmallMGC_EtsiMessageBuilder::buildSubtract(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId);
      m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);

    }
    call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
    call->m_outleg = SmallMGC_ETSI_ReleasingFsm::instance();
  }
  else
  {
    if (call->m_releaser == H248STACK_NotReleased)
    {
      call->m_releaser = H248STACK_Incoming;
      call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();

      call->m_incoming->m_outstandingMessage =
    	  SmallMGC_EtsiMessageBuilder::buildModifyReleasing(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId, call->m_incoming->m_physical, call->m_incoming->m_ephemeral, m_database->getEventRequestId());
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
      call->m_incoming->m_outstandingMessage =
    	  SmallMGC_EtsiMessageBuilder::buildSubtract(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId);
      m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);

    }
    call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
    call->m_inleg = SmallMGC_ETSI_ReleasingFsm::instance();
  }
  return xcode;

}

Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::notifyTone(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& tones)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  notify tone\n");
  std::transform(tones.begin(), tones.end(), tones.begin(),(int(*)(int)) std::toupper);
  if (tones.find("ANSBAR") != std::string::npos)
  {
    m_adapter->switchToModemReq(call, direction);
  }
  else if (tones.find("V21FLAG") != std::string::npos)
  {
    m_adapter->switchToT38Req(call, direction);
  }
  else
    {}

  return xcode;
}

Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::notifyFlashHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  notify flash hook\n");
  return xcode;
}

Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::timerExpiry(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId,std::string& term)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  timer Expiry\n");
  xcode = SmallMGC_CC_Fsm::timerExpiry(call, direction);

  return xcode;

}

Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::startCallReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  start call req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::bearerInformationReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& sdp)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  bearer information req\n");
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::establishedReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  established req\n");
  return xcode;
}
Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::releaseReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  release req\n");
  if (direction == SmallMGC_OUT)
  {
     if (call->m_outgoing->m_timer != 0)
     {
       m_adapter->stopTimer(call->m_outgoing->m_timer);
       call->m_outgoing->m_timer = 0;
     }
     call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
     call->m_outgoing->m_outstandingMessage =
    	 SmallMGC_EtsiMessageBuilder::buildModifyBusy(call->m_outgoing->m_outstandingReplyTransactionId,
                            call->m_outgoing->m_contextId, call->m_outgoing->m_physical, call->m_outgoing->m_ephemeral, m_database->getEventRequestId() );
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
     call->m_incoming->m_outstandingMessage =
    	 SmallMGC_EtsiMessageBuilder::buildModifyBusy(call->m_incoming->m_outstandingReplyTransactionId,
                           call->m_incoming->m_contextId, call->m_incoming->m_physical, call->m_incoming->m_ephemeral, m_database->getEventRequestId() );
     m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
     call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
  }

  return xcode;

}

Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::ringingReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  ringing req\n");
  return xcode;
}
Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::switchToT38Req(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  switchToT38 req\n");
  if (direction == SmallMGC_OUT)
  {
    if (  call->m_outgoing->m_timer != 0)
    {
      m_adapter->stopTimer(call->m_outgoing->m_timer);
      call->m_outgoing->m_timer = 0;
    }
    call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();

    call->m_outgoing->m_outstandingMessage =
    	SmallMGC_EtsiMessageBuilder::buildModifyLocalFax(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId,
                      call->m_outgoing->m_physical, call->m_outgoing->m_ephemeral, call->m_outgoing->m_remoteDescriptor, m_database->getEventRequestId());
    m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
    call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
    call->m_outgoing->m_secondModifyExpected = true;
    call->m_outleg = SmallMGC_ETSI_ChangeFaxFsm::instance();
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
    	SmallMGC_EtsiMessageBuilder::buildModifyLocalFax(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId,
                            call->m_incoming->m_physical, call->m_incoming->m_ephemeral, call->m_incoming->m_remoteDescriptor, m_database->getEventRequestId());
    m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);

    call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
    call->m_incoming->m_secondModifyExpected = true;
    call->m_inleg = SmallMGC_ETSI_ChangeFaxFsm::instance();
  }
  return xcode;

}

Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::switchToModemReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  switchToModem req\n");
  if (direction == SmallMGC_OUT)
  {
    if (  call->m_outgoing->m_timer != 0)
    {
      m_adapter->stopTimer(call->m_outgoing->m_timer);
      call->m_outgoing->m_timer = 0;
    }
    call->m_outgoing->m_outstandingReplyTransactionId = m_database->getNextTransactionId();

    call->m_outgoing->m_outstandingMessage =
    	SmallMGC_EtsiMessageBuilder::buildModifyLocalModem(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId,
                      call->m_outgoing->m_physical, call->m_outgoing->m_ephemeral, call->m_outgoing->m_remoteDescriptor, m_database->getEventRequestId());
    m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
    call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
    call->m_outleg = SmallMGC_ETSI_ChangeModemFsm::instance();
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
    	SmallMGC_EtsiMessageBuilder::buildModifyLocalModem(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId,
                            call->m_incoming->m_physical, call->m_incoming->m_ephemeral, call->m_incoming->m_remoteDescriptor, m_database->getEventRequestId());
    m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);

    call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
    call->m_inleg = SmallMGC_ETSI_ChangeModemFsm::instance();
  }
  return xcode;

}

Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::setup(class H248STACK_Call* call, enum SmallMGCLegDirection direction, enum SmallMGCIsdnBABearer bearer)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  isdn setup\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::information(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& number)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  isdn information\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::alerting(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  isdn alerting\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::connect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  isdn connect\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::connectAck(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  isdn connectAck\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::disconnect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  isdn disconnect\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::release(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  isdn release\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::releaseComplete(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  isdn release complete\n");
  // message not expected in this state
  return xcode;

}


 Poco::UInt32 SmallMGC_ETSI_EstablishedFsm::sendToMg(std::string const& msg)
 {
   // for reply messages
 }







