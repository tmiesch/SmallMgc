#ifndef SMALLMGC_SIP_COMMANDS_H
#define SMALLMGC_SIP_COMMANDS_H
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


#include <string>
#include <list>

#include "Poco/Types.h"
#include "Poco/Timer.h"
#include "Poco/Notification.h"
#include "Poco/NotificationQueue.h"

/**
 * */
enum SmallMGC_SIP_Types
{
    Request,
    Response,
    Timeout,
    Internal
};


class SmallMGC_SIP_Commands : public Poco::Notification
{
public :
  typedef Poco::AutoPtr<SmallMGC_SIP_Commands> Ptr;
  SmallMGC_SIP_Commands(SmallMGC_SIP_Types    transactionType);
  virtual ~SmallMGC_SIP_Commands();
  virtual bool isForCallControl();
  virtual bool useInTestMode() {return m_useInTestMode; };
public :
  /**
   * Get locally stored value.
   */
  virtual SmallMGC_SIP_Types getTransactionTypes();

  virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
  virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
  virtual Poco::UInt32 process(class SUB_API* api);
  virtual std::string getAllow() const;
  virtual std::string getCallid() const;
  virtual std::string getContact() const;
  virtual std::string getContent() const;
  virtual std::string getContentLength() const;
  virtual std::string getCseq() const;
  virtual std::string getExpires() const;
  virtual std::string getFrom() const;
  virtual std::string getMaxForwards() const;
  virtual std::string getRequest() const;
  virtual std::string getTo() const;
  virtual std::string getUserAgent() const;
  virtual std::list<std::string> getVia() const;
  virtual void setAllow(std::string m_allow);
  virtual void setCallid(std::string m_callid);
  virtual void setContact(std::string m_contact);
  virtual void setContent(std::string m_content);
  virtual void setContentLength(std::string m_contentLength);
  virtual void setCseq(std::string m_cseq);
  virtual void setExpires(std::string m_expires);
  virtual void setFrom(std::string m_from);
  virtual void setMaxForwards(std::string m_maxForwards);
  virtual void setRequest(std::string m_request);
  virtual void setTo(std::string m_to);
  virtual void setUserAgent(std::string m_userAgent);
  virtual void setVia(std::string m_via);

public:
 /*
   * @param timeout_in_ms time to wait in milliseconds [ms]
   * @param messageIdent  ident to use to cancel the sending of the message
   * @return              execution status
   */
protected:

  SmallMGC_SIP_Types         m_transactionType;
  bool                       m_useInTestMode;

  std::string                m_request;
  std::list <std::string>    m_via;
  std::string                m_from;
  std::string                m_to;
  std::string                m_contact;
  std::string                m_callid;
  std::string                m_cseq;
  std::string                m_expires;
  std::string                m_userAgent;
  std::string                m_maxForwards;
  std::string                m_allow;
  std::string                m_contentLength;
  std::string                m_content;



private: // inhibit access to not implemented default C++ operators
  SmallMGC_SIP_Commands            (const SmallMGC_SIP_Commands&);  // copy constructor
  SmallMGC_SIP_Commands& operator= (const SmallMGC_SIP_Commands&);  // copy assignment
};

class SmallMGC_SIP_Timeout : public SmallMGC_SIP_Commands
{
public:
  SmallMGC_SIP_Timeout(SmallMGC_SIP_Types    transactionType, Poco::NotificationQueue& queue);
     virtual ~SmallMGC_SIP_Timeout();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
     virtual Poco::UInt32 process(class SUB_API* api);
     virtual void sendToMessageHandler();
     virtual void onTimer(class Poco::Timer& timer);
     virtual void timerExpired();
     virtual Poco::UInt32 getTimerId() {
       return m_id;
     }
     virtual Poco::UInt32 setTimerId(Poco::UInt32 id) {
       m_id = id;
     }
protected:
      Poco::NotificationQueue& m_queue;
      Poco::UInt32             m_id;
};

class SmallMGC_SIP_Register : public SmallMGC_SIP_Commands
{
public:
  SmallMGC_SIP_Register(SmallMGC_SIP_Types    transactionType);
     virtual ~SmallMGC_SIP_Register();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class SUB_API* api);

     virtual void sendToMessageHandler();
};


class SmallMGC_SIP_Invite : public SmallMGC_SIP_Commands
{
public:
  SmallMGC_SIP_Invite(SmallMGC_SIP_Types    transactionType);
     virtual ~SmallMGC_SIP_Invite();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class SUB_API* api);

     virtual void sendToMessageHandler();
};
class SmallMGC_SIP_Ack : public SmallMGC_SIP_Commands
{
public:
  SmallMGC_SIP_Ack(SmallMGC_SIP_Types    transactionType);
     virtual ~SmallMGC_SIP_Ack();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class SUB_API* api);

     virtual void sendToMessageHandler();
};
class SmallMGC_SIP_Cancel : public SmallMGC_SIP_Commands
{
public:
  SmallMGC_SIP_Cancel(SmallMGC_SIP_Types    transactionType);
     virtual ~SmallMGC_SIP_Cancel();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class SUB_API* api);

     virtual void sendToMessageHandler();
};
class SmallMGC_SIP_Bye : public SmallMGC_SIP_Commands
{
public:
  SmallMGC_SIP_Bye(SmallMGC_SIP_Types    transactionType);
     virtual ~SmallMGC_SIP_Bye();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class SUB_API* api);

     virtual void sendToMessageHandler();
};
class SmallMGC_SIP_Notify : public SmallMGC_SIP_Commands
{
public:
  SmallMGC_SIP_Notify(SmallMGC_SIP_Types    transactionType);
     virtual ~SmallMGC_SIP_Notify();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class SUB_API* api);

     virtual void sendToMessageHandler();
};
class SmallMGC_SIP_Refer : public SmallMGC_SIP_Commands
{
public:
  SmallMGC_SIP_Refer(SmallMGC_SIP_Types    transactionType);
     virtual ~SmallMGC_SIP_Refer();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class SUB_API* api);

     virtual void sendToMessageHandler();
};
class SmallMGC_SIP_Options : public SmallMGC_SIP_Commands
{
public:
  SmallMGC_SIP_Options(SmallMGC_SIP_Types    transactionType);
     virtual ~SmallMGC_SIP_Options();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class SUB_API* api);

     virtual void sendToMessageHandler();
};
class SmallMGC_SIP_Info : public SmallMGC_SIP_Commands
{
public:
  SmallMGC_SIP_Info(SmallMGC_SIP_Types    transactionType);
     virtual ~SmallMGC_SIP_Info();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class SUB_API* api);

     virtual void sendToMessageHandler();
};
class SmallMGC_SIP_Subscribe : public SmallMGC_SIP_Commands
{
public:
  SmallMGC_SIP_Subscribe(SmallMGC_SIP_Types    transactionType);
     virtual ~SmallMGC_SIP_Subscribe();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class SUB_API* api);

     virtual void sendToMessageHandler();
};


#endif

