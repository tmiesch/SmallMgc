#ifndef H248STACK_COMMANDS_H
#define H248STACK_COMMANDS_H
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
enum H248STACK_TransactionTypes
{
    Request,
    Reply,
    Pending,
    Timeout,
    Internal
};

enum SmallMGC_Internal_Tones
{
    Dial,
    Ringback,
    Busy,
    Congestion,
    Howler
};

class H248STACK_Commands : public Poco::Notification
{
public :
  typedef Poco::AutoPtr<H248STACK_Commands> Ptr;
  H248STACK_Commands(H248STACK_TransactionTypes    transactionType,
  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId  );
  virtual ~H248STACK_Commands();
  virtual bool isForCallControl();
  virtual bool useInTestMode() {return m_useInTestMode; };
public :
  /**
   * Get locally stored value.
   */
  virtual H248STACK_TransactionTypes getTransactionTypes();
  virtual Poco::UInt32 getTransactionId();
  virtual std::string& getContextId();
  virtual std::string& getTerminationId();
  virtual std::string& getIpAddress();
  virtual std::string& getIpPort();
  virtual std::string& getMgName();
  virtual std::string& getMid();
  virtual H248STACK_Commands* getSubCommand();

  /**
   * Check the range and store it if good.
   *
   * @param value new value to be stored in the member class
   * @return      execution status
   */
  virtual void setImmediateAckResponseRequired (bool responseRequired) {
    m_immediateAckResponseRequired = responseRequired;
  }
  virtual void setTransactionTypes(H248STACK_TransactionTypes  type);
  virtual void setTransactionId(Poco::UInt32 );
  virtual void setContextId(std::string context);
  virtual void setTerminationId(std::string termination);
  virtual void setIpAddress(std::string ipAddr);
  virtual void setIpPort(std::string ipPort);
  virtual void setMgName(std::string mgName);
  virtual void setMid(std::string mid);
  virtual void setEventId(std::string eventId);
  virtual void setUseInTestMode(bool useInTestMode) {m_useInTestMode = useInTestMode;};
  virtual void addSubCommand(H248STACK_Commands* command);
  virtual void removeSubCommand();

  virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
  virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
  virtual Poco::UInt32 process(class SUB_API* api);

public:
 /*
   * @param timeout_in_ms time to wait in milliseconds [ms]
   * @param messageIdent  ident to use to cancel the sending of the message
   * @return              execution status
   */
protected:

  H248STACK_TransactionTypes     m_transactionType;
  bool                       m_immediateAckResponseRequired;
  Poco::UInt32                 m_transactionId;
  std::string                m_contextId;
  std::string                m_terminationId;
  std::string                m_ipAddress;
  std::string                m_ipPort;
  std::string                m_mgName;
  std::string                m_mid;
  std::string                m_eventId;
  H248STACK_Commands*        m_subCommand;
  bool                       m_useInTestMode;

private: // inhibit access to not implemented default C++ operators
  H248STACK_Commands            (const H248STACK_Commands&);  // copy constructor
  H248STACK_Commands& operator= (const H248STACK_Commands&);  // copy assignment
};

class H248STACK_CommandsComplex : public H248STACK_Commands
{
public:
    H248STACK_CommandsComplex(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId);
     virtual ~H248STACK_CommandsComplex();
     virtual void setLocalDescriptor(std::string descriptor);
     virtual void setRemoteDescriptor(std::string descriptor);
     virtual void sendToMessageHandler();
protected:
    std::string m_localDescriptor;
    std::string m_remoteDescriptor;
};

class H248STACK_Timeout : public H248STACK_Commands
{
public:
    H248STACK_Timeout(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId, Poco::NotificationQueue& queue);
     virtual ~H248STACK_Timeout();
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

class H248STACK_Add : public H248STACK_CommandsComplex
{
public:
    H248STACK_Add(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId);
     virtual ~H248STACK_Add();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class SUB_API* api);

     virtual void sendToMessageHandler();
};

class H248STACK_Modify : public H248STACK_CommandsComplex
{
public:
    H248STACK_Modify(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId);
     virtual ~H248STACK_Modify();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
     virtual Poco::UInt32 process(class SUB_API* api);
     virtual void sendToMessageHandler();
     virtual bool isForCallControl() {
       return m_isForCallControl;
     }
protected:
     bool m_isForCallControl;
};

class H248STACK_Move : public H248STACK_CommandsComplex
{
public:
    H248STACK_Move(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId);
     virtual ~H248STACK_Move();
     virtual void sendToMessageHandler();
};

class H248STACK_Subtract : public H248STACK_Commands
{
public:
    H248STACK_Subtract(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId);
     virtual ~H248STACK_Subtract();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class SUB_API* api);
     virtual void sendToMessageHandler();
};


class H248STACK_AuditValue : public H248STACK_Commands
{
public:
    H248STACK_AuditValue(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId);
     virtual ~H248STACK_AuditValue();
     virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
     virtual Poco::UInt32 process(class SUB_API* api);
     virtual bool isForCallControl();

};

class H248STACK_AuditCapabilities : public H248STACK_Commands
{
public:
    H248STACK_AuditCapabilities(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId);
     virtual ~H248STACK_AuditCapabilities();
     virtual void sendToMessageHandler();
};

class H248STACK_Notify : public H248STACK_Commands
{
public:
    H248STACK_Notify(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId);
     virtual ~H248STACK_Notify();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class SUB_API* api);
     void setObservedEvent(std::string event);
     void setObservedParameter(std::string param);
     //void setEventId(std::string eventId);
     virtual void sendToMessageHandler();
private:
    std::string m_observedEvent;
    std::string m_observedParameter;
    //std::string m_eventId;

};

class H248STACK_Error : public H248STACK_Commands
{
public:
    H248STACK_Error(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId);
     virtual ~H248STACK_Error();
     virtual Poco::UInt32 process(class H248STACK_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
     virtual Poco::UInt32 process(class SUB_API* api);
     virtual bool isForCallControl();
     void setErrorCode(std::string errorCode);
     virtual void sendToMessageHandler();
private:
    std::string m_errorCode;
};

class H248STACK_Pending : public H248STACK_Commands
{
public:
    H248STACK_Pending(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId);
     virtual ~H248STACK_Pending();
     virtual Poco::UInt32 process(class H248STACK_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
     virtual Poco::UInt32 process(class SUB_API* api);
     virtual bool isForCallControl();
     virtual void sendToMessageHandler();
private:

};

class H248STACK_ServiceChange : public H248STACK_Commands
{
public:
    H248STACK_ServiceChange(H248STACK_TransactionTypes    transactionType,
                                  Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId);
     virtual ~H248STACK_ServiceChange();
     virtual Poco::UInt32 process(class H248STACK_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
     virtual Poco::UInt32 process(class SUB_API* api);
     virtual bool isForCallControl();
     void setReason(std::string reason);
     void setMethod(std::string method);
     virtual void sendToMessageHandler();
private:
    std::string m_reason;
    std::string m_method;
    Poco::UInt32  m_serviceChangeDelay;
};

/*
class SmallMGC_Int_StartTone : public H248STACK_Commands
{
public:
  SmallMGC_Int_StartTone(std::string  terminationId, SmallMGC_Internal_Tones tone);
     virtual ~SmallMGC_Int_StartTone();
     virtual Poco::UInt32 process(class H248STACK_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
     virtual bool isForCallControl();
     virtual void sendToMessageHandler();
private:
     SmallMGC_Internal_Tones m_tone;
};

class SmallMGC_Int_StopTone : public H248STACK_Commands
{
public:
  SmallMGC_Int_StopTone(std::string  terminationId);
     virtual ~SmallMGC_Int_StopTone();
     virtual Poco::UInt32 process(class H248STACK_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
     virtual bool isForCallControl();
     virtual void sendToMessageHandler();
private:

};

class SmallMGC_Int_StopIsdnCall : public H248STACK_Commands
{
public:
  SmallMGC_Int_StopIsdnCall(std::string  terminationId);
     virtual ~SmallMGC_Int_StopIsdnCall();
     virtual Poco::UInt32 process(class H248STACK_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
     virtual bool isForCallControl();
     virtual void sendToMessageHandler();
private:

};

class SmallMGC_Int_Connect : public H248STACK_Commands
{
public:
  SmallMGC_Int_Connect(std::string  terminationId);
     virtual ~SmallMGC_Int_Connect();
     virtual Poco::UInt32 process(class H248STACK_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
     virtual bool isForCallControl();
     virtual void sendToMessageHandler();
private:

};*/

class SmallMGC_Int_Base : public H248STACK_Commands
{
public:
  SmallMGC_Int_Base(std::string  terminationId);
  SmallMGC_Int_Base(std::string  terminationId, std::string digit);
  virtual ~SmallMGC_Int_Base();
  virtual Poco::UInt32 process(class H248STACK_StdCallControl* callHandler);
  virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
  virtual bool isForCallControl();
  virtual void sendToMessageHandler();
    std::string getDigit() const
    {
        return m_digit;
    }

    void setDigit(std::string m_digit)
    {
        this->m_digit = m_digit;
    }

    class Q931_Call *getCall() const
    {
        return m_call;
    }

    void setCall(class Q931_Call *m_call)
    {
        this->m_call = m_call;
    }

    class Q931_Interface *getInterface() const
    {
        return m_interface;
    }

    void setInterface(class Q931_Interface *m_interface)
    {
        this->m_interface = m_interface;
    }

protected:
  class Q931_Call* m_call;
  class Q931_Interface* m_interface;
  std::string      m_digit;
private:

};

class SmallMGC_Int_DisconnectInd : public SmallMGC_Int_Base
{
public:
  SmallMGC_Int_DisconnectInd(std::string  terminationId);
     virtual ~SmallMGC_Int_DisconnectInd();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
     virtual bool isForCallControl();
     virtual void sendToMessageHandler();
private:

};

class SmallMGC_Int_ReleaseInd : public SmallMGC_Int_Base
{
public:
  SmallMGC_Int_ReleaseInd(std::string  terminationId);
  virtual ~SmallMGC_Int_ReleaseInd();
  virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
  virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
  virtual bool isForCallControl();
  virtual void sendToMessageHandler();
private:

};

class SmallMGC_Int_ReleaseCompleteInd : public SmallMGC_Int_Base
{
public:
  SmallMGC_Int_ReleaseCompleteInd(std::string  terminationId);
  virtual ~SmallMGC_Int_ReleaseCompleteInd();
  virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
  virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
  virtual bool isForCallControl();
  virtual void sendToMessageHandler();
private:

};


class SmallMGC_Int_SetupInd : public SmallMGC_Int_Base
{
public:
  SmallMGC_Int_SetupInd(std::string  terminationId, std::string digit);
     virtual ~SmallMGC_Int_SetupInd();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
     virtual bool isForCallControl();
     virtual void sendToMessageHandler();
private:

};

class SmallMGC_Int_InformationInd : public SmallMGC_Int_Base
{
public:
  SmallMGC_Int_InformationInd(std::string  terminationId, std::string digit);
     virtual ~SmallMGC_Int_InformationInd();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
     virtual bool isForCallControl();
     virtual void sendToMessageHandler();
private:
};

class SmallMGC_Int_AlertingInd : public SmallMGC_Int_Base
{
public:
  SmallMGC_Int_AlertingInd(std::string  terminationId);
     virtual ~SmallMGC_Int_AlertingInd();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
     virtual bool isForCallControl();
     virtual void sendToMessageHandler();
private:

};

class SmallMGC_Int_ConnectInd : public SmallMGC_Int_Base
{
public:
  SmallMGC_Int_ConnectInd(std::string  terminationId);
     virtual ~SmallMGC_Int_ConnectInd();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
     virtual bool isForCallControl();
     virtual void sendToMessageHandler();
private:

};

class SmallMGC_Int_ConnectAckInd : public SmallMGC_Int_Base
{
public:
  SmallMGC_Int_ConnectAckInd(std::string  terminationId);
     virtual ~SmallMGC_Int_ConnectAckInd();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
     virtual bool isForCallControl();
     virtual void sendToMessageHandler();
private:

};


class SmallMGC_Internal_Base : public H248STACK_Commands
{
public:
  SmallMGC_Internal_Base(std::string  terminationId);
  virtual ~SmallMGC_Internal_Base();
  virtual Poco::UInt32 process(class H248STACK_StdCallControl* callHandler);
  virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
  virtual bool isForCallControl();
  virtual void sendToMessageHandler();

protected:
private:

};

class SmallMGC_Internal_OutOfService : public SmallMGC_Internal_Base
{
public:
  SmallMGC_Internal_OutOfService(std::string  terminationId);
  virtual ~SmallMGC_Internal_OutOfService();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
     virtual bool isForCallControl();
     virtual void sendToMessageHandler();
private:

};

class SmallMGC_Internal_AssociationLost : public SmallMGC_Internal_Base
{
public:
  SmallMGC_Internal_AssociationLost();
  virtual ~SmallMGC_Internal_AssociationLost();
     virtual Poco::UInt32 process(class SmallMGC_StdCallControl* callHandler);
     virtual Poco::UInt32 process(class H248STACK_GatewayControl* gatewayHandler);
     virtual bool isForCallControl();
     virtual void sendToMessageHandler();
private:

};


#endif

