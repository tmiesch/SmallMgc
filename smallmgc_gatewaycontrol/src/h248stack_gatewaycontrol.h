#ifndef H248STACK_GATEWAYCONTROL_H
#define H248STACK_GATEWAYCONTROL_H
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

#include "Poco/Types.h"
#include <string>
#include <map>

/**
 * This is an example for a team member class. This example is used to store
 * some data. The team members are a collaboration of classes that constitute
 * the team's functionality.
 * Please notice that if the implementation of member classes is small and
 * simple enough, it is allowed to declare and define them merely in the
 * implementation of the team (h248stack_team.cpp).
 *
 * See the "team" design pattern document for more explanations.
 */

enum H248STACK_GatewayControlTimerIds
{
  H248STACK_GatewayHeartbeat = 1,
  H248STACK_GatewayHeartbeatRepeation,
  H248STACK_GatewayTimeout,
  H248STACK_GatewayStartup,
  H248STACK_ESAStartup,
  H248STACK_ESAServiceChangeTimeout
};

enum H248STACK_TypeOfMgcBehaviour
{
  H248STACK_ControlS = 1,
  H248STACK_ControlA,
  H248STACK_ControlE,
  H248STACK_ControlH,
  H248STACK_ControlX,
  H248STACK_ControlNone
};

class H248STACK_GatewayControl
{
public :
  H248STACK_GatewayControl(class H248STACK_GatewayAdapter*  adapter, class H248STACK_Database* database, class SUB_H248_GatewayControl_Task* task);
  virtual ~H248STACK_GatewayControl();


public :
  Poco::UInt32 gatewayControl(class H248STACK_Commands* command);
  Poco::UInt32 assocInService(std::string ipAddress, Poco::UInt32 transactionId, std::string terminationId);
  Poco::UInt32 serviceChangeReply(std::string ipAddress, Poco::UInt32 transactionId, std::string terminationId);
  Poco::UInt32 terminationInService(std::string ipAddress, Poco::UInt32 transactionId, std::string terminationId, std::string contextId);
  Poco::UInt32 terminationOutService(std::string ipAddress, Poco::UInt32 transactionId, std::string terminationId, std::string contextId, bool isForced);
  Poco::UInt32 heartbeatTimeout(std::string ipAddress);
  Poco::UInt32 heartbeatTimeoutRepeation(std::string ipAddress);
  Poco::UInt32 startupTimeout(std::string ipAddress);
  Poco::UInt32 timerExpiry(std::string ipAddress, Poco::UInt32 transactionId, std::string terminationId);
  Poco::UInt32 auditReplyReceived(std::string ipAddress, Poco::UInt32 transactionId, std::string terminationId);
  Poco::UInt32 modifyReply(std::string ipAddress, Poco::UInt32 transactionId, std::string terminationId, bool immediaAckRequired = false);
  Poco::UInt32 pendingReceived(std::string ipAddress, Poco::UInt32 transactionId);
  Poco::UInt32 handleError(std::string ipAddress, Poco::UInt32 transactionId);
  Poco::UInt32 esaStartupTimeout();
  Poco::UInt32 esaTimeoutRepeation();

  bool         hasOutstandingMessage() {
    return m_outstandingMessage;
  }
private:
  class H248STACK_GatewayAdapter* m_adapter;
  class H248STACK_Database* m_database;
  bool  m_outstandingMessage;
  H248STACK_TypeOfMgcBehaviour m_typeOfMgcBehaviour;
  class SUB_H248_GatewayControl_Task* m_task;
  class Timer*                        m_esaServiceChangeTimer;
  class Timer*                        m_esaServiceChangeRepeatitionTimer;
  Poco::UInt32                        m_esaServiceChangeCounter;
  Poco::UInt32                        m_esaMode;
  std::string                         m_esaServiceChangeCommand;
  std::string                         m_rootId;

private: // inhibit access to not implemented default C++ operators
  H248STACK_GatewayControl            (const H248STACK_GatewayControl&);  // copy constructor
  H248STACK_GatewayControl& operator= (const H248STACK_GatewayControl&);  // copy assignment
};

#endif
