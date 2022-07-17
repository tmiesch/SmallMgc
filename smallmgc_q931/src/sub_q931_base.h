//
// SUB_Q931_BASE.h
//


#ifndef SUBQ931BASE_INCLUDED
#define SUBQ931BASE_INCLUDED

#include <string>
#include "sub_iua_notify.h"


class SUB_Q931_Base: public SUB_IUA_Base
{
public:
  SUB_Q931_Base(std::vector<Poco::UInt8>& message);
  virtual ~SUB_Q931_Base();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual void setIpAddress(std::string& ipAddress);
  virtual Poco::UInt32 process(class SUB_IUA_Task* task);
  virtual Poco::UInt32 process(class SUB_Q931_Task* task);
protected:
  std::vector<Poco::UInt8> m_message;
  std::string m_ipAddress;
};

class SUB_Q931_SetupReq: public SUB_Q931_Base
{
public:
  SUB_Q931_SetupReq(std::vector<Poco::UInt8>& message);
  virtual ~SUB_Q931_SetupReq();
  virtual std::vector<Poco::UInt8>& getMessage();
  virtual void setMessage(std::vector<Poco::UInt8>& message);
  virtual Poco::UInt32 process(class SUB_Q931_Task* task);
protected:

};

/*class SUB_IUA_Error: public SUB_IUA_Base
{
public:
  SUB_IUA_Error(std::string& message);
  virtual ~SUB_IUA_Error();
  virtual std::string& getMessage();
  virtual void setMessage(std::string& message);

protected:

};

class SUB_IUA_UnitData: public SUB_IUA_Base
{
public:
  SUB_IUA_UnitData(std::string& message);
  virtual ~SUB_IUA_UnitData();
  virtual std::string& getMessage();
  virtual void setMessage(std::string& message);

protected:

};

class SUB_IUA_UnitDataReq: public SUB_IUA_Base
{
public:
  SUB_IUA_UnitDataReq(std::string& message);
  virtual ~SUB_IUA_UnitDataReq();
  virtual std::string& getMessage();
  virtual void setMessage(std::string& message);

protected:

};

class SUB_IUA_Data: public SUB_IUA_Base
{
public:
  SUB_IUA_Data(std::string& message);
  virtual ~SUB_IUA_Data();
  virtual std::string& getMessage();
  virtual void setMessage(std::string& message);

protected:

};

class SUB_IUA_Release: public SUB_IUA_Base
{
public:
  SUB_IUA_Release(std::string& message);
  virtual ~SUB_IUA_Release();
  virtual std::string& getMessage();
  virtual void setMessage(std::string& message);

protected:

};

class SUB_IUA_Establish: public SUB_IUA_Base
{
public:
  SUB_IUA_Establish(std::string& message);
  virtual ~SUB_IUA_Establish();
  virtual std::string& getMessage();
  virtual void setMessage(std::string& message);

protected:

};

class SUB_IUA_EstablishReq: public SUB_IUA_Base
{
public:
  SUB_IUA_EstablishReq(std::string& message);
  virtual ~SUB_IUA_EstablishReq();
  virtual std::string& getMessage();
  virtual void setMessage(std::string& message);

protected:

};

class SUB_IUA_AspUpAck: public SUB_IUA_Base
{
public:
  SUB_IUA_AspUpAck(std::string& message);
  virtual ~SUB_IUA_AspUpAck();
  virtual std::string& getMessage();
  virtual void setMessage(std::string& message);
  virtual void process(class SUB_IUA_Task* task);
protected:

};

class SUB_IUA_AspActiveAck: public SUB_IUA_Base
{
public:
  SUB_IUA_AspActiveAck(std::string& message);
  virtual ~SUB_IUA_AspActiveAck();
  virtual std::string& getMessage();
  virtual void setMessage(std::string& message);
protected:

};
*/


#endif // SUBQ931BASE_INCLUDED


