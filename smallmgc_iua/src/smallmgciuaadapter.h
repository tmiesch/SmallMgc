/*
 * smallmgciuaadapter.h
 *
 *  Created on: 18.08.2009
 *      Author: thomas
 */

#ifndef SMALLMGCIUAADAPTER_H_
#define SMALLMGCIUAADAPTER_H_

#include "Poco/Types.h"
#include "Poco/NotificationQueue.h"

class SmallMGC_IUA_Adapter
{
public:
  SmallMGC_IUA_Adapter(class Poco::NotificationQueue& iuaQueue);
  virtual
  ~SmallMGC_IUA_Adapter();
  virtual class TimerIua* startTimer(Poco::UInt32 duration, std::string ipAddress);

  virtual void stopTimer(class TimerIua* timer);
  void aspActive(class H248STACK_Gateway* gateway);
  void aspUp(class H248STACK_Gateway* gateway, Poco::UInt16 streamId);

private:
  class Poco::NotificationQueue& m_iuaQueue;
};

#endif /* SMALLMGCIUAADAPTER_H_ */
