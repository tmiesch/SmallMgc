//
// sub_iua_notify.cpp
//
// Subsystem for receiving h248 packets


#include "sub_q931_base.h"
#include "sub_q931.h"

SUB_Q931_Base::SUB_Q931_Base(std::vector<Poco::UInt8>& message): SUB_IUA_Base(message)
{

}

SUB_Q931_Base::~SUB_Q931_Base()
{
}

Poco::UInt32 SUB_Q931_Base::process(class SUB_IUA_Task* task)
{
  return 0;
}

Poco::UInt32 SUB_Q931_Base::process(class SUB_Q931_Task* task)
{
return 0;
}

void SUB_Q931_Base::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}

void SUB_Q931_Base::setIpAddress(std::string& ipAddress)
{
  m_ipAddress.clear();
  m_ipAddress = ipAddress;
}


std::vector<Poco::UInt8>& SUB_Q931_Base::getMessage()
{
  return m_message;
}


//----------------------------------------------------------------------------------------------
SUB_Q931_SetupReq::SUB_Q931_SetupReq(std::vector<Poco::UInt8>& message): SUB_Q931_Base(message)
{

}

SUB_Q931_SetupReq::~SUB_Q931_SetupReq()
{
}

void SUB_Q931_SetupReq::setMessage(std::vector<Poco::UInt8>& message)
{
  m_message.clear();
  m_message = message;
}
std::vector<Poco::UInt8>& SUB_Q931_SetupReq::getMessage()
{
  return m_message;
}

Poco::UInt32 SUB_Q931_SetupReq::process(class SUB_Q931_Task* task)
{
  return 0;
}


/*
//--------------------------------------------------------------------------------------
SUB_IUA_Error::SUB_IUA_Error(std::string& message): SUB_IUA_Base(message)
{

}

SUB_IUA_Error::~SUB_IUA_Error()
{
}

void SUB_IUA_Error::setMessage(std::string& message)
{
  m_message.clear();
  m_message = message;
}

std::string& SUB_IUA_Error::getMessage()
{
  return m_message;
}

//----------------------------------------------------------------------------------------------
SUB_IUA_UnitData::SUB_IUA_UnitData(std::string& message): SUB_IUA_Base(message)
{

}

SUB_IUA_UnitData::~SUB_IUA_UnitData()
{
}

void SUB_IUA_UnitData::setMessage(std::string& message)
{
  m_message.clear();
  m_message = message;
}

std::string& SUB_IUA_UnitData::getMessage()
{
  return m_message;
}

//----------------------------------------------------------------------------------------------
SUB_IUA_UnitDataReq::SUB_IUA_UnitDataReq(std::string& message): SUB_IUA_Base(message)
{

}

SUB_IUA_UnitDataReq::~SUB_IUA_UnitDataReq()
{
}

void SUB_IUA_UnitDataReq::setMessage(std::string& message)
{
  m_message.clear();
  m_message = message;
}

std::string& SUB_IUA_UnitDataReq::getMessage()
{
  return m_message;
}
//----------------------------------------------------------------------------------------------
SUB_IUA_Data::SUB_IUA_Data(std::string& message): SUB_IUA_Base(message)
{

}

SUB_IUA_Data::~SUB_IUA_Data()
{
}

void SUB_IUA_Data::setMessage(std::string& message)
{
  m_message.clear();
  m_message = message;
}

std::string& SUB_IUA_Data::getMessage()
{
  return m_message;
}

//----------------------------------------------------------------------------------------------

SUB_IUA_Release::SUB_IUA_Release(std::string& message): SUB_IUA_Base(message)
{

}

SUB_IUA_Release::~SUB_IUA_Release()
{
}

void SUB_IUA_Release::setMessage(std::string& message)
{
  m_message.clear();
  m_message = message;
}

std::string& SUB_IUA_Release::getMessage()
{
  return m_message;
}

//----------------------------------------------------------------------------------------------

SUB_IUA_Establish::SUB_IUA_Establish(std::string& message): SUB_IUA_Base(message)
{

}

SUB_IUA_Establish::~SUB_IUA_Establish()
{
}

void SUB_IUA_Establish::setMessage(std::string& message)
{
  m_message.clear();
  m_message = message;
}

std::string& SUB_IUA_Establish::getMessage()
{
  return m_message;
}

//----------------------------------------------------------------------------------------------

SUB_IUA_EstablishReq::SUB_IUA_EstablishReq(std::string& message): SUB_IUA_Base(message)
{

}

SUB_IUA_EstablishReq::~SUB_IUA_EstablishReq()
{
}

void SUB_IUA_EstablishReq::setMessage(std::string& message)
{
  m_message.clear();
  m_message = message;
}

std::string& SUB_IUA_EstablishReq::getMessage()
{
  return m_message;
}

//----------------------------------------------------------------------------------------------

SUB_IUA_AspUpAck::SUB_IUA_AspUpAck(std::string& message): SUB_IUA_Base(message)
{

}

SUB_IUA_AspUpAck::~SUB_IUA_AspUpAck()
{
}

void SUB_IUA_AspUpAck::process(class SUB_IUA_Task* task)
{
  task->aspActive(task->getGateway(m_ipAddress));
}

void SUB_IUA_AspUpAck::setMessage(std::string& message)
{
  m_message.clear();
  m_message = message;
}

std::string& SUB_IUA_AspUpAck::getMessage()
{
  return m_message;
}

//----------------------------------------------------------------------------------------------

SUB_IUA_AspActiveAck::SUB_IUA_AspActiveAck(std::string& message): SUB_IUA_Base(message)
{

}

SUB_IUA_AspActiveAck::~SUB_IUA_AspActiveAck()
{
}

void SUB_IUA_AspActiveAck::setMessage(std::string& message)
{
  m_message.clear();
  m_message = message;
}

std::string& SUB_IUA_AspActiveAck::getMessage()
{
  return m_message;
}

//----------------------------------------------------------------------------------------------
*/
