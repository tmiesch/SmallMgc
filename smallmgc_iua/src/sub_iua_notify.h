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


#ifndef SUBIUANOTIFY_INCLUDED
#define SUBIUANOTIFY_INCLUDED

#include <string>
#include <vector>
#include <algorithm>
#include "Poco/Types.h"
#include "Poco/Notification.h"


class SUB_IUA_Base: public Poco::Notification
{
public:
  SUB_IUA_Base(std::string  ipAddress): m_ipAddress(ipAddress) {};
  SUB_IUA_Base(std::vector<Poco::UInt8>& message);
  SUB_IUA_Base() {};
  virtual ~SUB_IUA_Base();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual void setIpAddress(std::string ipAddress);
  virtual void setIpPort(Poco::UInt32 ipPort);
  virtual void setTextIfId(std::string& ifId);
  virtual void setIntegerIfId(Poco::UInt32 intIfId);
  virtual Poco::UInt32 getIntegerIfId() { return m_integerIfId; };
  virtual void setStreamId(Poco::UInt32 streamId);
  virtual void setTei(Poco::UInt8 tei);
  virtual Poco::UInt8 getTei() { return m_tei; };
  virtual void setLayer3(std::vector<Poco::UInt8>& layer3);
  virtual std::vector<Poco::UInt8>& getLayer3() { return m_layer3; };
  virtual Poco::UInt32 process(class SUB_IUA_Task* task);
  virtual Poco::UInt32 process(class SUB_Q931_Task* task);
protected:
  std::vector<Poco::UInt8> m_message;
  std::string m_ipAddress;
  Poco::UInt32 m_ipPort;
  Poco::UInt32 m_streamId;
  Poco::UInt32 m_integerIfId;
  std::string m_textIfId;
  std::vector<Poco::UInt8> m_layer3;
  Poco::UInt8 m_tei;
};

class SUB_IUA_Notify: public SUB_IUA_Base
{
public:
  SUB_IUA_Notify(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_Notify();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_IUA_Task* task);

protected:

};

class SUB_IUA_Error: public SUB_IUA_Base
{
public:
  SUB_IUA_Error(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_Error();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_IUA_Task* task);

protected:

};

class SUB_IUA_UnitData: public SUB_IUA_Base
{
public:
  SUB_IUA_UnitData(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_UnitData();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_IUA_Task* task);
protected:

};

class SUB_IUA_UnitDataReq: public SUB_IUA_Base
{
public:
  SUB_IUA_UnitDataReq(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_UnitDataReq();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_IUA_Task* task);
protected:

};

class SUB_IUA_DataReq: public SUB_IUA_Base
{
public:
  SUB_IUA_DataReq(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_DataReq();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_IUA_Task* task);
protected:

};


class SUB_IUA_Data: public SUB_IUA_Base
{
public:
  SUB_IUA_Data(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_Data();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_IUA_Task* task);
  virtual Poco::UInt32 process(class SUB_Q931_Task* task);

protected:

};

class SUB_IUA_ReleaseConf: public SUB_IUA_Base
{
public:
  SUB_IUA_ReleaseConf(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_ReleaseConf();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_IUA_Task* task);
  virtual Poco::UInt32 process(class SUB_Q931_Task* task);
protected:

};

class SUB_IUA_ReleaseInd: public SUB_IUA_Base
{
public:
  SUB_IUA_ReleaseInd(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_ReleaseInd();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_IUA_Task* task);
  virtual Poco::UInt32 process(class SUB_Q931_Task* task);
protected:

};
class SUB_IUA_ReleaseReq: public SUB_IUA_Base
{
public:
  SUB_IUA_ReleaseReq(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_ReleaseReq();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_IUA_Task* task);
protected:

};

class SUB_IUA_EstablishConf: public SUB_IUA_Base
{
public:
  SUB_IUA_EstablishConf(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_EstablishConf();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_IUA_Task* task);
  virtual Poco::UInt32 process(class SUB_Q931_Task* task);

protected:

};

class SUB_IUA_EstablishInd: public SUB_IUA_Base
{
public:
  SUB_IUA_EstablishInd(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_EstablishInd();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_IUA_Task* task);
  virtual Poco::UInt32 process(class SUB_Q931_Task* task);

protected:

};


class SUB_IUA_EstablishReq: public SUB_IUA_Base
{
public:
  SUB_IUA_EstablishReq(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_EstablishReq();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_IUA_Task* task);
protected:

};

class SUB_IUA_AspUpAck: public SUB_IUA_Base
{
public:
  SUB_IUA_AspUpAck(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_AspUpAck();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_IUA_Task* task);
protected:

};

class SUB_IUA_AspDownAck: public SUB_IUA_Base
{
public:
  SUB_IUA_AspDownAck(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_AspDownAck();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_IUA_Task* task);
protected:

};

class SUB_IUA_HeartBeatAck: public SUB_IUA_Base
{
public:
  SUB_IUA_HeartBeatAck(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_HeartBeatAck();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_IUA_Task* task);
protected:

};

class SUB_IUA_AspActiveAck: public SUB_IUA_Base
{
public:
  SUB_IUA_AspActiveAck(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_AspActiveAck();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual void setAssocId(Poco::UInt32 assocId)
  {
    m_assocId = assocId;
  }
  virtual Poco::UInt32 process(class SUB_IUA_Task* task);
protected:
  Poco::UInt32 m_assocId;

};

class SUB_IUA_AspInactiveAck: public SUB_IUA_Base
{
public:
  SUB_IUA_AspInactiveAck(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_AspInactiveAck();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
protected:

};

class SUB_IUA_AspInterrupted: public SUB_IUA_Base
{
public:
  SUB_IUA_AspInterrupted(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_AspInterrupted();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_IUA_Task* task);
  virtual void setAssocId(Poco::UInt32 assocId)
      {
        m_assocId = assocId;
      }
protected:
    Poco::UInt32 m_assocId;
};


class SUB_IUA_TeiConf: public SUB_IUA_Base
{
public:
  SUB_IUA_TeiConf(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_TeiConf();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
protected:

};

class SUB_IUA_TeiIndication: public SUB_IUA_Base
{
public:
  SUB_IUA_TeiIndication(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_TeiIndication();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
protected:

};


class SUB_IUA_TeiRequest: public SUB_IUA_Base
{
public:
  SUB_IUA_TeiRequest(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_TeiRequest();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
protected:


};

class SUB_IUA_Q931_Base: public SUB_IUA_Base
{
public:
  SUB_IUA_Q931_Base(std::vector<Poco::UInt8>& message);
  SUB_IUA_Q931_Base();
  virtual ~SUB_IUA_Q931_Base();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_Q931_Task* task);
    class Q931_Interface *getInterface() const
    {
        return m_interface;
    }

    void setInterface(class Q931_Interface *m_interface)
    {
        this->m_interface = m_interface;
    }
    class Q931_Call *getIsdnCall() const
    {
      return m_isdnCall;
    }

    void setIsdnCall(class Q931_Call *m_call)
    {
      this->m_isdnCall = m_call;
    }

protected:
  class Q931_Interface* m_interface;
  class Q931_Call*      m_isdnCall;

};

class SUB_IUA_Q931_SetupAck: public SUB_IUA_Q931_Base
{
public:
  SUB_IUA_Q931_SetupAck(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_Q931_SetupAck();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_Q931_Task* task);
protected:


};


class SUB_IUA_Q931_SetupReq: public SUB_IUA_Q931_Base
{
public:
  SUB_IUA_Q931_SetupReq(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_Q931_SetupReq();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_Q931_Task* task);
    std::string getSelectedBChannel() const
    {
        return selectedBChannel;
    }

    void setSelectedBChannel(std::string selectedBChannel)
    {
        this->selectedBChannel = selectedBChannel;
    }

    std::string getCalledPartyNumber() const
    {
        return calledPartyNumber;
    }

    void setCalledPartyNumber(std::string calledPartyNumber)
    {
        this->calledPartyNumber = calledPartyNumber;
    }

    std::string getCallingPartyNumber() const
    {
        return callingPartyNumber;
    }

    void setCallingPartyNumber(std::string callingPartyNumber)
    {
        this->callingPartyNumber = callingPartyNumber;
    }

protected:
  std::string calledPartyNumber;
  std::string callingPartyNumber;
  std::string selectedBChannel;


};

class SUB_IUA_Q931_CallProceeding: public SUB_IUA_Q931_Base
{
public:
  SUB_IUA_Q931_CallProceeding(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_Q931_CallProceeding();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_Q931_Task* task);
protected:


};

class SUB_IUA_Q931_AlertingReq: public SUB_IUA_Q931_Base
{
public:
  SUB_IUA_Q931_AlertingReq(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_Q931_AlertingReq();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_Q931_Task* task);
protected:


};

class SUB_IUA_Q931_ReleaseReq: public SUB_IUA_Q931_Base
{
public:
  SUB_IUA_Q931_ReleaseReq(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_Q931_ReleaseReq();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_Q931_Task* task);
protected:


};

class SUB_IUA_Q931_ReleaseCompleteReq: public SUB_IUA_Q931_Base
{
public:
  SUB_IUA_Q931_ReleaseCompleteReq(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_Q931_ReleaseCompleteReq();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_Q931_Task* task);
protected:


};

class SUB_IUA_Q931_DisconnectReq: public SUB_IUA_Q931_Base
{
public:
  SUB_IUA_Q931_DisconnectReq(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_Q931_DisconnectReq();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_Q931_Task* task);
protected:


};


class SUB_IUA_Q931_ConnectAckReq: public SUB_IUA_Q931_Base
{
public:
  SUB_IUA_Q931_ConnectAckReq(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_Q931_ConnectAckReq();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_Q931_Task* task);
protected:


};


class SUB_IUA_Q931_ConnectReq: public SUB_IUA_Q931_Base
{
public:
  SUB_IUA_Q931_ConnectReq(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_Q931_ConnectReq();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_Q931_Task* task);
protected:


};

class SUB_IUA_Q931_FreeCallResources: public SUB_IUA_Q931_Base
{
public:
  SUB_IUA_Q931_FreeCallResources(std::vector<Poco::UInt8>& message);
  virtual ~SUB_IUA_Q931_FreeCallResources();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_Q931_Task* task);
protected:


};



#endif // SUBIUANOTIFY_INCLUDED


