#ifndef SMALLMGC_INCLUDED
#define SMALLMGC_INCLUDED

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


#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/AutoPtr.h"
#include "Poco/Task.h"
#include "Poco/TaskManager.h"
#include "Poco/NotificationQueue.h"
#include <iostream>


using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Util::OptionCallback;
using Poco::AutoPtr;
using Poco::TaskManager;

class SmallMgc: public Application
{
public:
  SmallMgc();
  void displayQueueInfo();
  void displayTaskInfo();
  void sendTo(std::string& ipAddress, std::string& message);
    class SUB_H248_UDP *getSub_udp() const
    {
        return m_sub_udp;
    }

    class SUB_H248_Parser *getSub_parser() const
    {
        return m_sub_parser;
    }

    class SUB_SDP_Parser *getSub_sdp_parser() const
    {
        return m_sub_sdp_parser;
    }

    class SUB_H248_SCallControl *getSub_callControl() const
    {
        return m_sub_callControl;
    }

    class SUB_H248_GatewayControl *getSub_gatewayControl() const
    {
        return m_sub_gatewayControl;
    }

    class SUB_H248_CLI *getSub_cli() const
    {
        return m_sub_cli;
    }

    void setSub_cli(class SUB_H248_CLI *m_sub_cli)
    {
        this->m_sub_cli = m_sub_cli;
    }

    Poco::NotificationQueue* getIuaPackets()
    {
        return &iuaPackets;
    }

    class Poco::NotificationQueue* getParserQueue()
  {
    return &packets;
  }

  class Poco::NotificationQueue* getSipParserQueue()
  {
    return &sipQueue;
  }

  class SUB_H248_SCTP* getSubSctp() {
    return m_sub_sctp;
  }

  class SUB_API* getSubApi() {
	  return m_sub_api;
  }
protected:

  void initialize(Application& self);

  void uninitialize();

  void reinitialize(Application& self);
  void defineOptions(OptionSet& options);

  void handleHelp(const std::string& name, const std::string& value);

  void handleDefine(const std::string& name, const std::string& value);

  void handleConfig(const std::string& name, const std::string& value);

  void displayHelp();

  void defineProperty(const std::string& def);

  int main(const std::vector<std::string>& args);

  void printProperties(const std::string& base);



private:
  bool _helpRequested;
  TaskManager m_tm;
  Poco::NotificationQueue packets;
  Poco::NotificationQueue iuaPackets;
  Poco::NotificationQueue isdnPackets;
  Poco::NotificationQueue calls;
  Poco::NotificationQueue gatecalls;
  Poco::NotificationQueue cliQueue;
  Poco::NotificationQueue apiQueue;
  Poco::NotificationQueue testQueue;
  Poco::NotificationQueue sipQueue;

  class SUB_H248_UDP*            m_sub_udp;
  class SUB_H248_SCTP*           m_sub_sctp;
  class SUB_H248_Parser*         m_sub_parser;
  class SUB_H248_Database*       m_sub_database;
  class SUB_H248_SCallControl*   m_sub_callControl;
  class SUB_H248_GatewayControl* m_sub_gatewayControl;
  class SUB_H248_CLI*            m_sub_cli;
  class SUB_Timer*               m_sub_timer;
  class SUB_IUA*                 m_sub_iua;
  class SUB_Q931*                m_sub_q931;
  class SUB_API*                 m_sub_api;
  class SUB_SmallMGC_Test*       m_sub_test;
  class SUB_SIP_UDP*             m_sub_sip_udp;
  class SUB_SIP_Parser*          m_sub_sip_parser;
  class SUB_SDP_Parser*          m_sub_sdp_parser;
};

#endif // SMALLMGC_INCLUDED
