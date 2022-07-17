#ifndef PARSER_H
#define PARSER_H

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

#include <vector>
#include <string>

#include "h248stack_message.h"

class H248STACK_Parser
{
 public:
  H248STACK_Parser();

  bool decode(char const* message);

  H248STACK_Message* get_message() const;
  void setMessage(char* mesg);
  static void setImmediateAckResponseRequired(int start, int len);
  static void setTerminationId(int start, int len);
  static void setTransactionId(int start, int len);
  static void setSvcReason(int start, int len);
  static void setSvcMethod(int start, int len);
  static void setContextId(int start, int len);
  static void setIPPort(int start, int len);
  static void setIPAddress(int start, int len);
  static void setVersion(int start, int len);
  static void setTransactionType(int start, int len);
  static void setCommand(int start, int len);
  static void setObservedEvent(int start, int len);
  static void setObservedParameter(int start, int len);
  static void setEventId(int start, int len);
  static void setLocalDescriptor(int start, int len);
  static void setRemoteDescriptor(int start, int len);
  static void setErrorDescriptor(int start, int len);
  static void setMgName(int start, int len);
  static void setMid(int start, int len);
  static void writeCommand();
  static void createAdd();
  static void createModify();
  static void createSubtract(int start, int len);
  static void createMove();
  static void createAuditValue();
  static void createAuditCapabilities();
  static void createNotify();
  static void createServiceChange();
  static void createError();
  static void createPending();
  static int  isEvent(int start, int len);
  static class H248STACK_Commands* getCommand();
  static std::string megacoMesg;
  private:
  H248STACK_Message* message_;
  static std::string m_terminationId;
  static std::string m_transactionId;
  static std::string m_contextId;
  static std::string m_ipPort;
  static std::string m_ipAddress;
  static std::string m_mgName;
  static std::string m_mid;
  static std::string m_version;
  static std::string m_transactionType;
  static std::string m_svcReason;
  static std::string m_svcMethod;
  static std::string m_observedEvent;
  static std::string m_observedEventParameter;
  static std::string m_eventId;
  static std::string m_localDescriptor;
  static std::string m_remoteDescriptor;
  static std::string m_errorDescriptor;

  static class H248STACK_Commands* s_command;
  static class H248STACK_Commands* s_firstCommand;
};

#endif
