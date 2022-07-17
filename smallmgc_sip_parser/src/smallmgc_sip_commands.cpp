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
#include <stdio.h>

#include "Poco/Util/Application.h"

#include "smallmgc_sip_commands.h"
//#include "h248stack_messagebuilder.h"
#include "smallmgc_stdcallcontrol.h"
#include "h248stack_gatewaycontrol.h"
#include "sub_api.h"

using Poco::Util::Application;
//----------------------------------------------------------------------------
// Implementation of the H248STACK_TeamMember.
//----------------------------------------------------------------------------


SmallMGC_SIP_Commands::SmallMGC_SIP_Commands(SmallMGC_SIP_Types     transactionType) :
    Poco::Notification(), m_transactionType(transactionType), m_useInTestMode(true)
{

}


//----------------------------------------------------------------------------


SmallMGC_SIP_Commands::~SmallMGC_SIP_Commands() {
  //removeSubCommand();
}


//----------------------------------------------------------------------------
SmallMGC_SIP_Types SmallMGC_SIP_Commands::getTransactionTypes()
{
    return m_transactionType;
}


Poco::UInt32 SmallMGC_SIP_Commands::process(class H248STACK_GatewayControl* gatewayHandler)
{
     Poco::UInt32 xcode = 0;
     Application::instance().logger().information("Wrong base process function called");

     return xcode;
}

Poco::UInt32 SmallMGC_SIP_Commands::process(class SmallMGC_StdCallControl* callHandler)
 {
     Poco::UInt32 xcode = 0;

     Application::instance().logger().information("Wrong base process function called");

     return xcode;
}

Poco::UInt32 SmallMGC_SIP_Commands::process(class SUB_API* api)
{
     Poco::UInt32 xcode = 0;
     Application::instance().logger().information("Wrong base process function called");

     return xcode;
}


bool SmallMGC_SIP_Commands::isForCallControl()
{
  return true;
}

std::string SmallMGC_SIP_Commands::getAllow() const
{
  return m_allow;
}
std::string SmallMGC_SIP_Commands::getCallid() const
{
  return m_callid;
}
std::string SmallMGC_SIP_Commands::getContact() const
{
  return m_contact;
}
std::string SmallMGC_SIP_Commands::getContent() const
{
  return m_content;
}
std::string SmallMGC_SIP_Commands::getContentLength() const
{
  return m_contentLength;
}
std::string SmallMGC_SIP_Commands::getCseq() const
{
  return m_cseq;
}
std::string SmallMGC_SIP_Commands::getExpires() const
{
  return m_expires;
}

std::string SmallMGC_SIP_Commands::getFrom() const
{
  return m_from;
}
std::string SmallMGC_SIP_Commands::getMaxForwards() const
{
  return m_maxForwards;
}
std::string SmallMGC_SIP_Commands::getRequest() const
{
  return m_request;
}
std::string SmallMGC_SIP_Commands::getTo() const
{
  return m_to;
}
std::string SmallMGC_SIP_Commands::getUserAgent() const
{
  return m_userAgent;
}
std::list<std::string> SmallMGC_SIP_Commands::getVia() const
{
  return m_via;
}
void SmallMGC_SIP_Commands::setAllow(std::string allow)
{
  m_allow = allow;
}
void SmallMGC_SIP_Commands::setCallid(std::string callid)
{
  m_callid = callid;
}
void SmallMGC_SIP_Commands::setContact(std::string contact)
{
  m_contact = contact;
}
void SmallMGC_SIP_Commands::setContent(std::string content)
{
  m_content = content;
}
void SmallMGC_SIP_Commands::setContentLength(std::string contentLength)
{
  m_contentLength = contentLength;
}
void SmallMGC_SIP_Commands::setCseq(std::string cseq)
{
  m_cseq = cseq;
}
void SmallMGC_SIP_Commands::setExpires(std::string expires)
{
  m_expires = expires;
}
void SmallMGC_SIP_Commands::setFrom(std::string from)
{
  m_from = from;
}
void SmallMGC_SIP_Commands::setMaxForwards(std::string maxForwards)
{
  m_maxForwards = maxForwards;
}
void SmallMGC_SIP_Commands::setRequest(std::string request)
{
  m_request = request;
}
void SmallMGC_SIP_Commands::setTo(std::string to)
{
  m_to = to;
}
void SmallMGC_SIP_Commands::setUserAgent(std::string userAgent)
{
  m_userAgent = userAgent;
}
void SmallMGC_SIP_Commands::setVia(std::string via)
{
  m_via.push_back(via);
}


SmallMGC_SIP_Timeout::SmallMGC_SIP_Timeout(SmallMGC_SIP_Types    transactionType, Poco::NotificationQueue& queue) :
    SmallMGC_SIP_Commands(transactionType), m_queue(queue)
{
  m_useInTestMode = false;
}
SmallMGC_SIP_Timeout:: ~SmallMGC_SIP_Timeout()
  {}

void SmallMGC_SIP_Timeout::sendToMessageHandler()
{
}

void SmallMGC_SIP_Timeout::timerExpired()
{
  Application::instance().logger().debug("Timer queued");
  m_queue.enqueueNotification(this);
}
void SmallMGC_SIP_Timeout::onTimer(class Poco::Timer& timer)
{
  Application::instance().logger().debug("Timer queued 1");
  m_queue.enqueueNotification(this);
}

Poco::UInt32 SmallMGC_SIP_Timeout::process(SmallMGC_StdCallControl* callHandler)
 {
     Poco::UInt32 xcode = 0;
     Application::instance().logger().debug("Timeout called");

     //callHandler->timerExpiry(m_ipAddress,  m_contextId, m_transactionId,m_terminationId);
     return xcode;
}

Poco::UInt32 SmallMGC_SIP_Timeout::process(class H248STACK_GatewayControl* gatewayHandler)
{
  Poco::UInt32 xcode = 0;
  Application::instance().logger().debug("Timeout for gateway control message");
  switch (m_id)
  {
  }
  return xcode;
}
Poco::UInt32 SmallMGC_SIP_Timeout::process(class SUB_API* api)
{
     Poco::UInt32 xcode = 0;
     Application::instance().logger().debug("Timeout called");

     return xcode;
}

SmallMGC_SIP_Register::SmallMGC_SIP_Register(SmallMGC_SIP_Types    transactionType) :
    SmallMGC_SIP_Commands(transactionType)
                                  {}
SmallMGC_SIP_Register:: ~SmallMGC_SIP_Register()
  {}

void SmallMGC_SIP_Register::sendToMessageHandler()
{
}


Poco::UInt32 SmallMGC_SIP_Register::process(SmallMGC_StdCallControl* callHandler)
{
     Poco::UInt32 xcode = 0;

          return xcode;
 }

Poco::UInt32 SmallMGC_SIP_Register::process(class SUB_API* api)
{
     Poco::UInt32 xcode = 0;
     Application::instance().logger().debug("Register called");
     return xcode;
 }



SmallMGC_SIP_Invite::SmallMGC_SIP_Invite(SmallMGC_SIP_Types    transactionType) :
    SmallMGC_SIP_Commands(transactionType)
                                  {}
SmallMGC_SIP_Invite:: ~SmallMGC_SIP_Invite()
  {}

void SmallMGC_SIP_Invite::sendToMessageHandler()
{
}


Poco::UInt32 SmallMGC_SIP_Invite::process(SmallMGC_StdCallControl* callHandler)
{
     Poco::UInt32 xcode = 0;

          return xcode;
 }

Poco::UInt32 SmallMGC_SIP_Invite::process(class SUB_API* api)
{
     Poco::UInt32 xcode = 0;
     Application::instance().logger().debug("Register called");
     return xcode;
 }
SmallMGC_SIP_Ack::SmallMGC_SIP_Ack(SmallMGC_SIP_Types    transactionType) :
    SmallMGC_SIP_Commands(transactionType)
                                  {}
SmallMGC_SIP_Ack:: ~SmallMGC_SIP_Ack()
  {}

void SmallMGC_SIP_Ack::sendToMessageHandler()
{
}


Poco::UInt32 SmallMGC_SIP_Ack::process(SmallMGC_StdCallControl* callHandler)
{
     Poco::UInt32 xcode = 0;

          return xcode;
 }

Poco::UInt32 SmallMGC_SIP_Ack::process(class SUB_API* api)
{
     Poco::UInt32 xcode = 0;
     Application::instance().logger().debug("Register called");
     return xcode;
 }
SmallMGC_SIP_Cancel::SmallMGC_SIP_Cancel(SmallMGC_SIP_Types    transactionType) :
    SmallMGC_SIP_Commands(transactionType)
                                  {}
SmallMGC_SIP_Cancel:: ~SmallMGC_SIP_Cancel()
  {}

void SmallMGC_SIP_Cancel::sendToMessageHandler()
{
}


Poco::UInt32 SmallMGC_SIP_Cancel::process(SmallMGC_StdCallControl* callHandler)
{
     Poco::UInt32 xcode = 0;

          return xcode;
 }

Poco::UInt32 SmallMGC_SIP_Cancel::process(class SUB_API* api)
{
     Poco::UInt32 xcode = 0;
     Application::instance().logger().debug("Register called");
     return xcode;
 }
SmallMGC_SIP_Bye::SmallMGC_SIP_Bye(SmallMGC_SIP_Types    transactionType) :
    SmallMGC_SIP_Commands(transactionType)
                                  {}
SmallMGC_SIP_Bye:: ~SmallMGC_SIP_Bye()
  {}

void SmallMGC_SIP_Bye::sendToMessageHandler()
{
}


Poco::UInt32 SmallMGC_SIP_Bye::process(SmallMGC_StdCallControl* callHandler)
{
     Poco::UInt32 xcode = 0;

          return xcode;
 }

Poco::UInt32 SmallMGC_SIP_Bye::process(class SUB_API* api)
{
     Poco::UInt32 xcode = 0;
     Application::instance().logger().debug("Register called");
     return xcode;
 }
SmallMGC_SIP_Notify::SmallMGC_SIP_Notify(SmallMGC_SIP_Types    transactionType) :
    SmallMGC_SIP_Commands(transactionType)
                                  {}
SmallMGC_SIP_Notify:: ~SmallMGC_SIP_Notify()
  {}

void SmallMGC_SIP_Notify::sendToMessageHandler()
{
}


Poco::UInt32 SmallMGC_SIP_Notify::process(SmallMGC_StdCallControl* callHandler)
{
     Poco::UInt32 xcode = 0;

          return xcode;
 }

Poco::UInt32 SmallMGC_SIP_Notify::process(class SUB_API* api)
{
     Poco::UInt32 xcode = 0;
     Application::instance().logger().debug("Register called");
     return xcode;
 }
SmallMGC_SIP_Refer::SmallMGC_SIP_Refer(SmallMGC_SIP_Types    transactionType) :
    SmallMGC_SIP_Commands(transactionType)
                                  {}
SmallMGC_SIP_Refer:: ~SmallMGC_SIP_Refer()
  {}

void SmallMGC_SIP_Refer::sendToMessageHandler()
{
}


Poco::UInt32 SmallMGC_SIP_Refer::process(SmallMGC_StdCallControl* callHandler)
{
     Poco::UInt32 xcode = 0;

          return xcode;
 }

Poco::UInt32 SmallMGC_SIP_Refer::process(class SUB_API* api)
{
     Poco::UInt32 xcode = 0;
     Application::instance().logger().debug("Register called");
     return xcode;
 }
SmallMGC_SIP_Options::SmallMGC_SIP_Options(SmallMGC_SIP_Types    transactionType) :
    SmallMGC_SIP_Commands(transactionType)
                                  {}
SmallMGC_SIP_Options:: ~SmallMGC_SIP_Options()
  {}

void SmallMGC_SIP_Options::sendToMessageHandler()
{
}


Poco::UInt32 SmallMGC_SIP_Options::process(SmallMGC_StdCallControl* callHandler)
{
     Poco::UInt32 xcode = 0;

          return xcode;
 }

Poco::UInt32 SmallMGC_SIP_Options::process(class SUB_API* api)
{
     Poco::UInt32 xcode = 0;
     Application::instance().logger().debug("Register called");
     return xcode;
 }
SmallMGC_SIP_Info::SmallMGC_SIP_Info(SmallMGC_SIP_Types    transactionType) :
    SmallMGC_SIP_Commands(transactionType)
                                  {}
SmallMGC_SIP_Info:: ~SmallMGC_SIP_Info()
  {}

void SmallMGC_SIP_Info::sendToMessageHandler()
{
}


Poco::UInt32 SmallMGC_SIP_Info::process(SmallMGC_StdCallControl* callHandler)
{
     Poco::UInt32 xcode = 0;

          return xcode;
 }

Poco::UInt32 SmallMGC_SIP_Info::process(class SUB_API* api)
{
     Poco::UInt32 xcode = 0;
     Application::instance().logger().debug("Register called");
     return xcode;
 }
SmallMGC_SIP_Subscribe::SmallMGC_SIP_Subscribe(SmallMGC_SIP_Types    transactionType) :
    SmallMGC_SIP_Commands(transactionType)
                                  {}
SmallMGC_SIP_Subscribe:: ~SmallMGC_SIP_Subscribe()
  {}

void SmallMGC_SIP_Subscribe::sendToMessageHandler()
{
}


Poco::UInt32 SmallMGC_SIP_Subscribe::process(SmallMGC_StdCallControl* callHandler)
{
     Poco::UInt32 xcode = 0;

          return xcode;
 }

Poco::UInt32 SmallMGC_SIP_Subscribe::process(class SUB_API* api)
{
     Poco::UInt32 xcode = 0;
     Application::instance().logger().debug("Register called");
     return xcode;
 }
