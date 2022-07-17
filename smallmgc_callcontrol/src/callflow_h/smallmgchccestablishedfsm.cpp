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
#include <algorithm>
#include <cctype>
#include <sstream>

#include "smallmgcgccfsm.h"

#include "smallmgchccestablishedfsm.h"
#include "smallmgchccreleasingfsm.h"
#include "smallmgchccchangefaxfsm.h"
#include "smallmgchccchangemodemfsm.h"

#include "h248stack_calladapter.h"
#include "smallmgc_h_messagebuilder.h"

#include "Poco/Util/Application.h"
#include "Poco/NumberFormatter.h"


using Poco::Util::Application;
using Poco::NumberFormatter;


SmallMGC_HCC_EstablishedFsm::SmallMGC_HCC_EstablishedFsm()
{
  // TODO Auto-generated constructor stub

}

SmallMGC_HCC_EstablishedFsm::~SmallMGC_HCC_EstablishedFsm()
{
  // TODO Auto-generated destructor stub
}

SmallMGC_HCC_EstablishedFsm*  SmallMGC_HCC_EstablishedFsm::s_instance = 0;

SmallMGC_HCC_EstablishedFsm*  SmallMGC_HCC_EstablishedFsm::instance(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter)
{
  if(s_instance == 0)
  {
    Poco::UInt32 xcode;
    s_instance = new SmallMGC_HCC_EstablishedFsm(database, adapter);
  }
  return s_instance;
}


Poco::UInt32 SmallMGC_HCC_EstablishedFsm::startCall(class H248STACK_Call* call, Poco::UInt32 transactionId, std::string& term )
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().information("State: HCC Established;  Event:  startCall\n");
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_EstablishedFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
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
    call->m_outleg = SmallMGC_HCC_ReleasingFsm::instance();

  }
  else
  {
    if (call->m_incoming->m_timer != 0)
    {
      call->m_incoming->m_outstandingMessage.clear();
      m_adapter->stopTimer(call->m_incoming->m_timer);
      call->m_incoming->m_timer = 0;
    }
    call->m_inleg = SmallMGC_HCC_ReleasingFsm::instance();
  }
  return xcode;

}
 //virtual Poco::UInt32 modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId);                                                   //G, I,K, Q,O, R,S
Poco::UInt32 SmallMGC_HCC_EstablishedFsm::modifyReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
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
     call->m_outleg = SmallMGC_HCC_ReleasingFsm::instance();

   }
   else
   {
     if (call->m_incoming->m_timer != 0)
     {
       call->m_incoming->m_outstandingMessage.clear();
       m_adapter->stopTimer(call->m_incoming->m_timer);
       call->m_incoming->m_timer = 0;
     }
     call->m_inleg = SmallMGC_HCC_ReleasingFsm::instance();
   }
   return xcode;


}
Poco::UInt32 SmallMGC_HCC_EstablishedFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  addReply1\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_EstablishedFsm::addReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& localSDP)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  addReply2\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_HCC_EstablishedFsm::subtractReply(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  subtractReply1\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_HCC_EstablishedFsm::notifyDigit(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& digits)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  notify Digit\n");
  // message not expected in this state
  return xcode;

}

Poco::UInt32 SmallMGC_HCC_EstablishedFsm::notifyOffHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  notify off hook\n");
}
Poco::UInt32 SmallMGC_HCC_EstablishedFsm::notifyOnHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
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
               SmallMGC_HMessageBuilder::buildModifyReleasing(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId, call->m_outgoing->m_physical, call->m_outgoing->m_ephemeral, m_database->getEventRequestId());
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
                SmallMGC_HMessageBuilder::buildSubtract(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId);
      m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);

    }
    call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
    call->m_outleg = SmallMGC_HCC_ReleasingFsm::instance();
  }
  else
  {
    if (call->m_releaser == H248STACK_NotReleased)
    {
      call->m_releaser = H248STACK_Incoming;
      call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();

      call->m_incoming->m_outstandingMessage =
             SmallMGC_HMessageBuilder::buildModifyReleasing(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId, call->m_incoming->m_physical, call->m_incoming->m_ephemeral, m_database->getEventRequestId());
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
           SmallMGC_HMessageBuilder::buildSubtract(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId);
      m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);

    }
    call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
    call->m_inleg = SmallMGC_HCC_ReleasingFsm::instance();
  }
  return xcode;

}

Poco::UInt32 SmallMGC_HCC_EstablishedFsm::notifyTone(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& tones)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  notify tone\n");
  std::transform(tones.begin(), tones.end(), tones.begin(),(int(*)(int)) std::toupper);
  if ((tones.find("ANSBAR") != std::string::npos) ||
	  (tones.find("ANSAMBAR") != std::string::npos))
  {
    m_adapter->switchToModemInd(call, direction);
    if (direction == SmallMGC_OUT)
      call->m_outleg = SmallMGC_HCC_ChangeModemFsm::instance();
    else
      call->m_inleg = SmallMGC_HCC_ChangeModemFsm::instance();
  }
  else if (tones.find("V21FLAG") != std::string::npos)
  {
    m_adapter->switchToT38Ind(call, direction);
    if (direction == SmallMGC_OUT)
      call->m_inleg = SmallMGC_HCC_ChangeFaxFsm::instance();
    else
      call->m_outleg = SmallMGC_HCC_ChangeFaxFsm::instance();
  }
  else
    {}

  return xcode;
}

Poco::UInt32 SmallMGC_HCC_EstablishedFsm::notifyFlashHook(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  notify flash hook\n");
  return xcode;
}

Poco::UInt32 SmallMGC_HCC_EstablishedFsm::timerExpiry(class H248STACK_Call* call, enum SmallMGCLegDirection direction, Poco::UInt32 transactionId,std::string& term)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  timer Expiry\n");
  xcode = SmallMGC_CC_Fsm::timerExpiry(call, direction);

  return xcode;

}

Poco::UInt32 SmallMGC_HCC_EstablishedFsm::startCallReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  start call req\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_EstablishedFsm::bearerInformationReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& sdp)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  bearer information req\n");
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_EstablishedFsm::establishedReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  established req\n");
  return xcode;
}
Poco::UInt32 SmallMGC_HCC_EstablishedFsm::releaseReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
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
         SmallMGC_HMessageBuilder::buildModifyBusy(call->m_outgoing->m_outstandingReplyTransactionId,
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
         SmallMGC_HMessageBuilder::buildModifyBusy(call->m_incoming->m_outstandingReplyTransactionId,
                           call->m_incoming->m_contextId, call->m_incoming->m_physical, call->m_incoming->m_ephemeral, m_database->getEventRequestId() );
     m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);
     call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
  }

  return xcode;

}

Poco::UInt32 SmallMGC_HCC_EstablishedFsm::ringingReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  ringing req\n");
  return xcode;
}
Poco::UInt32 SmallMGC_HCC_EstablishedFsm::switchToT38Req(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
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

    // add for fax switch over manipulate the remote descriptor
    std::string audioPort("");
    int nPosA = call->m_outgoing->m_remoteDescriptor.find("audio")+6;
    int nPosR = call->m_outgoing->m_remoteDescriptor.find("RTP",nPosA)-1;
    audioPort =  call->m_outgoing->m_remoteDescriptor.substr(nPosA,nPosR-nPosA);

    int nPosB = call->m_outgoing->m_remoteDescriptor.find("c=")+8;
    //nPosB = call->m_outgoing->m_remoteDescriptor.find("=",nPosB)-2;
    int nPosRB = call->m_outgoing->m_remoteDescriptor.find("=",nPosB)-2;
    std::string mediaIp(call->m_outgoing->m_remoteDescriptor.substr(nPosB,nPosRB-nPosB));

    std::stringstream remote;
    remote << "R{v=0\r\nc=IN IP4"<< mediaIp <<"\nm=image " << audioPort << " udptl t38\r\na=ptime:20\na=T38FaxRateManagement:transferredTCF\r\na=T38FaxUdpEC:t38UDPRedundancy\r\na=T38MaxBitRate:14400\r\n}";
    call->m_outgoing->m_outstandingMessage =
                SmallMGC_HMessageBuilder::buildModifyLocalFax(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId,
                      call->m_outgoing->m_physical, call->m_outgoing->m_ephemeral, remote.str() /*call->m_outgoing->m_remoteDescriptor*/, m_database->getEventRequestId());

    m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
    call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
    call->m_outgoing->m_secondModifyExpected = true;
    call->m_outleg = SmallMGC_HCC_ChangeFaxFsm::instance();
  }
  else
  {
    if (  call->m_incoming->m_timer != 0)
    {
      m_adapter->stopTimer(call->m_incoming->m_timer);
      call->m_incoming->m_timer = 0;
    }
    call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
    // add for fax switch over manipulate the remote descriptor
    std::string audioPort("");
    int nPosA = call->m_incoming->m_remoteDescriptor.find("audio")+6;
    int nPosR = call->m_incoming->m_remoteDescriptor.find("RTP",nPosA)-1;
    audioPort =  call->m_incoming->m_remoteDescriptor.substr(nPosA,nPosR-nPosA);

    int nPosB = call->m_incoming->m_remoteDescriptor.find("c=")+8;
    int nPosRB = call->m_incoming->m_remoteDescriptor.find("=",nPosB)-2;
    std::string mediaIp(call->m_incoming->m_remoteDescriptor.substr(nPosB,nPosRB-nPosB));
    std::stringstream remote;
    remote << "R{v=0\r\nc=IN IP4"<< mediaIp <<"\nm=image " << audioPort << " udptl t38\r\na=ptime:20\r\na=T38FaxRateManagement:transferredTCF\r\na=T38FaxUdpEC:t38UDPRedundancy\r\na=T38MaxBitRate:14400\r\n}";

    call->m_incoming->m_outstandingMessage =
        SmallMGC_HMessageBuilder::buildModifyLocalFax(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId,
                            call->m_incoming->m_physical, call->m_incoming->m_ephemeral, remote.str() /*call->m_incoming->m_remoteDescriptor*/, m_database->getEventRequestId());
    m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);

    call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
    call->m_incoming->m_secondModifyExpected = true;
    call->m_inleg = SmallMGC_HCC_ChangeFaxFsm::instance();
  }
  return xcode;

}

Poco::UInt32 SmallMGC_HCC_EstablishedFsm::switchToModemReq(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
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

    // add for modem switch over manipulate the remote descriptor
    std::string audioPort("");
    int nPosA = call->m_outgoing->m_remoteDescriptor.find("audio")+6;
    int nPosR = call->m_outgoing->m_remoteDescriptor.find("RTP",nPosA)-1;
    audioPort =  call->m_outgoing->m_remoteDescriptor.substr(nPosA,nPosR-nPosA);

    int nPosB = call->m_outgoing->m_remoteDescriptor.find("c=")+8;
    int nPosRB = call->m_outgoing->m_remoteDescriptor.find("=",nPosB)-2;
    std::string mediaIp(call->m_outgoing->m_remoteDescriptor.substr(nPosB,nPosRB-nPosB));

    std::stringstream remote;
    remote << "R{v=0\r\nc=IN IP4"<< mediaIp <<"\nm=audio " << audioPort << " RTP/AVP 8\r\na=ptime:20\r\n}";

    call->m_outgoing->m_outstandingMessage =
                SmallMGC_HMessageBuilder::buildModifyModem(call->m_outgoing->m_outstandingReplyTransactionId, call->m_outgoing->m_contextId,
                      call->m_outgoing->m_physical, call->m_outgoing->m_ephemeral, remote.str()/*call->m_outgoing->m_remoteDescriptor*/, m_database->getEventRequestId());
    m_adapter->sendToMg(call->m_outgoing->m_gateway->mgIPAddress, call->m_outgoing->m_outstandingMessage);
    call->m_outgoing->m_timer = m_adapter->startTimer(1000,call->m_outgoing->m_gateway->mgIPAddress,call->m_outgoing->m_outstandingReplyTransactionId, "", call->m_outgoing->m_physical);
    call->m_outleg = SmallMGC_HCC_ChangeModemFsm::instance();
  }
  else
  {
    if (  call->m_incoming->m_timer != 0)
    {
      m_adapter->stopTimer(call->m_incoming->m_timer);
      call->m_incoming->m_timer = 0;
    }
    call->m_incoming->m_outstandingReplyTransactionId = m_database->getNextTransactionId();
    std::string audioPort("");
    int nPosA = call->m_incoming->m_remoteDescriptor.find("audio")+6;
    int nPosR = call->m_incoming->m_remoteDescriptor.find("RTP",nPosA)-1;
    audioPort =  call->m_incoming->m_remoteDescriptor.substr(nPosA,nPosR-nPosA);

    int nPosB = call->m_incoming->m_remoteDescriptor.find("c=")+8;
    int nPosRB = call->m_incoming->m_remoteDescriptor.find("=",nPosB)-2;
    std::string mediaIp(call->m_incoming->m_remoteDescriptor.substr(nPosB,nPosRB-nPosB));

    std::stringstream remote;
    remote << "R{v=0\r\nc=IN IP4"<< mediaIp <<"\nm=audio " << audioPort << " RTP/AVP 8\r\na=ptime:20\r\n}";


    call->m_incoming->m_outstandingMessage =
        SmallMGC_HMessageBuilder::buildModifyModem(call->m_incoming->m_outstandingReplyTransactionId, call->m_incoming->m_contextId,
                            call->m_incoming->m_physical, call->m_incoming->m_ephemeral, remote.str() /*call->m_incoming->m_remoteDescriptor*/, m_database->getEventRequestId());
    m_adapter->sendToMg(call->m_incoming->m_gateway->mgIPAddress, call->m_incoming->m_outstandingMessage);

    call->m_incoming->m_timer = m_adapter->startTimer(1000,call->m_incoming->m_gateway->mgIPAddress,call->m_incoming->m_outstandingReplyTransactionId, "", call->m_incoming->m_physical);
    call->m_inleg = SmallMGC_HCC_ChangeModemFsm::instance();
  }
  return xcode;

}

Poco::UInt32 SmallMGC_HCC_EstablishedFsm::setup(class H248STACK_Call* call, enum SmallMGCLegDirection direction, enum SmallMGCIsdnBABearer bearer)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  isdn setup\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_EstablishedFsm::information(class H248STACK_Call* call, enum SmallMGCLegDirection direction, std::string& number)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  isdn information\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_EstablishedFsm::alerting(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  isdn alerting\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_EstablishedFsm::connect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  isdn connect\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_EstablishedFsm::connectAck(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  isdn connectAck\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_EstablishedFsm::disconnect(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  isdn disconnect\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_EstablishedFsm::release(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  isdn release\n");
  // message not expected in this state
  return xcode;

}
Poco::UInt32 SmallMGC_HCC_EstablishedFsm::releaseComplete(class H248STACK_Call* call, enum SmallMGCLegDirection direction)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().trace(" State: HCC Established;  Event:  isdn release complete\n");
  // message not expected in this state
  return xcode;

}


 Poco::UInt32 SmallMGC_HCC_EstablishedFsm::sendToMg(std::string const& msg)
 {
   // for reply messages
 }







