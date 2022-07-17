#ifndef H248STACK_GATEWAYADAPTER_H
#define H248STACK_GATEWAYADAPTER_H
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
#include <map>
#include "Poco/Types.h"
#include "Poco/NotificationQueue.h"
#include "Poco/Timer.h"



class H248STACK_GatewayAdapter
{
public :
  H248STACK_GatewayAdapter(class Poco::NotificationQueue& gateQueue, class Poco::NotificationQueue& callQueue, class SUB_H248_UDP* udp);
  virtual ~H248STACK_GatewayAdapter();


public :

  Poco::UInt32 sendToMg(std::string& ipAddress, std::string& msg, std::string portNr=""); // for reply messages
  virtual  class Poco::Timer* startTimer(Poco::UInt32 timerId, Poco::UInt32 duration, std::string ipAddress,
               Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId);
  virtual  class Poco::Timer* startTimer(Poco::UInt32 duration, std::string ipAddress,
               Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId);
  virtual void stopTimer(class Poco::Timer* timer);

   virtual  class Timer* startTimer1(Poco::UInt32 timerId, Poco::UInt32 duration, std::string ipAddress,
               Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId);
  virtual  class Timer* startTimer1(Poco::UInt32 duration, std::string ipAddress,
               Poco::UInt32  transactionId,  std::string  contextId,  std::string  terminationId);
  virtual void stopTimer1(class Timer* timer);

  virtual  void takenOutOfService(std::string ipAddress, std::string  terminationId);
  virtual  void associationLost(std::string ipAddress);

private:
  class SUB_H248_UDP* m_udp;
  class Poco::NotificationQueue& m_gateQueue;
  class Poco::NotificationQueue& m_callQueue;
  typedef std::map<std::string, Poco::Timer*> ReplyWait;
  ReplyWait m_replyMap;

private: // inhibit access to not implemented default C++ operators
  H248STACK_GatewayAdapter            (const H248STACK_GatewayAdapter&);  // copy constructor
  H248STACK_GatewayAdapter& operator= (const H248STACK_GatewayAdapter&);  // copy assignment
};

#endif

