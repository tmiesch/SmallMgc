/*
 * smallmgciuaassociation.h
 *
 *  Created on: 18.08.2009
 *      Author: thomas
 */

#ifndef SMALLMGCIUAASSOCIATION_H_
#define SMALLMGCIUAASSOCIATION_H_

#include <string>
#include <map>
#include "Poco/Types.h"


enum IUA_Association_State
{
  IUA_ASSOC_WAITINGUP,
  IUA_ASSOC_UP,
  IUA_ASSCO_ACTIVE,
  IUA_LOST_CONNECTION,
  IUA_REGAINED_CONNECTION
};

class IUA_Association_Data
{
public:
  Poco::UInt32 assocId;
  Poco::UInt16 streamId;
  std::string  ipAddress;
  class TimerIua* timer;
  IUA_Association_State state;
  IUA_Association_Data() : assocId(0), streamId(0), timer(0), ipAddress(""), state(IUA_ASSOC_WAITINGUP) {};
  ~IUA_Association_Data() {};
};

class SmallMGC_IUA_Association
{
public:
  SmallMGC_IUA_Association(class H248STACK_Database* db, class SmallMGC_IUA_Adapter* adapter);
  virtual
  ~SmallMGC_IUA_Association();

  Poco::UInt32 init();
  Poco::UInt32 aspUpAck(std::string ipAddress);
  Poco::UInt32 aspActiveAck(std::string ipAddress, Poco::UInt32 assocId);
  Poco::UInt32 assocInterrupted(std::string ipAddress, Poco::UInt32 assocId);
  Poco::UInt32 assocDown(std::string ipAddress);
  Poco::UInt32 timerExpired(std::string ipAddress);
private:
  class H248STACK_Database* m_db;
  class SmallMGC_IUA_Adapter* m_adapter;
  std::map<std::string, IUA_Association_Data* > m_assocDataMap;
  std::map<int, IUA_Association_Data* > m_assocDataMapAssocId;
};

#endif /* SMALLMGCIUAASSOCIATION_H_ */
