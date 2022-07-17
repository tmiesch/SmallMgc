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
#include "Poco/Util/HelpFormatter.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/NumberFormatter.h"

#include "Poco/Net/NetException.h"
#include "Poco/Timespan.h"

#include <iostream>
#include "sub_smallmgc_test.h"
#include "sub_h248_database.h"
#include "h248stack_database.h"
#include "sub_h248_basenotify.h"
#include "h248stack_commands.h"
#include "call.hpp"
#include "smallmgc.h"

using Poco::Util::Application;
using Poco::Util::Subsystem;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;
using Poco::Task;
using Poco::TaskManager;
using Poco::DateTimeFormatter;
using Poco::Net::Socket;
using Poco::Net::DatagramSocket;
using Poco::Net::SocketAddress;
using Poco::Net::IPAddress;
using Poco::NumberFormatter;

#define T_UDP                      0
#define T_TCP                      1
#define T_TLS                      2
#define DEFAULT_RATE                 10.0
#define DEFAULT_RATE_SCALE           1.0
#define DEFAULT_RATE_PERIOD_MS       1000
#define DEFAULT_TRANSPORT            T_UDP
#define DEFAULT_PORT                 5060
#define DEFAULT_MEDIA_PORT           6000
#define DEFAULT_3PCC_PORT            6060
#define DEFAULT_SERVICE              ((char *)"service")
#define DEFAULT_AUTH_PASSWORD        ((char *)"password")
#define DEFAULT_REPORT_FREQ          1000
#define DEFAULT_REPORT_FREQ_DUMP_LOG 60000
#define DEFAULT_TIMER_RESOLUTION     1
#define DEFAULT_FREQ_DUMP_RTT        200
#define DEFAULT_MAX_MULTI_SOCKET     50000
#define DEFAULT_CTRL_SOCKET_PORT     8888
#define DEFAULT_DEADCALL_WAIT	     33000

#define DISPLAY_SCENARIO_SCREEN    3

#define MAX_RECV_LOOPS_PER_CYCLE   1000
#define MAX_SCHED_LOOPS_PER_CYCLE  1000
#define NB_UPDATE_PER_CYCLE        1




#define SIPP_OPTION_HELP	   1
#define SIPP_OPTION_INT		   2
#define SIPP_OPTION_SETFLAG	   3
#define SIPP_OPTION_UNSETFLAG	   4
#define SIPP_OPTION_STRING	   5
#define SIPP_OPTION_ARGI 	   6
#define SIPP_OPTION_TIME_SEC	   7
#define SIPP_OPTION_FLOAT	   8
#define SIPP_OPTION_BOOL	  10
#define SIPP_OPTION_VERSION	  11
#define SIPP_OPTION_TRANSPORT	  12
#define SIPP_OPTION_NEED_SSL	  13
#define SIPP_OPTION_IP		  14
#define SIPP_OPTION_MAX_SOCKET	  15
#define SIPP_OPTION_CSEQ	  16
#define SIPP_OPTION_SCENARIO	  17
#define SIPP_OPTION_RSA		  18
#define SIPP_OPTION_LIMIT	  19
#define SIPP_OPTION_USERS	  20
#define SIPP_OPTION_KEY		  21
#define SIPP_OPTION_3PCC	  22
#define SIPP_OPTION_TDMMAP	  23
#define SIPP_OPTION_TIME_MS	  24
#define SIPP_OPTION_SLAVE_CFG     25
#define SIPP_OPTION_3PCC_EXTENDED 26
#define SIPP_OPTION_INPUT_FILE	  27
#define SIPP_OPTION_TIME_MS_LONG  28
#define SIPP_OPTION_LONG          29
#define SIPP_OPTION_LONG_LONG     30
#define SIPP_OPTION_DEFAULTS      31
#define SIPP_OPTION_OOC_SCENARIO  32
#define SIPP_OPTION_INDEX_FILE    33
#define SIPP_OPTION_VAR		  34
#define SIPP_OPTION_RTCHECK	  35
#define SIPP_OPTION_LFNAME	  36
#define SIPP_OPTION_LFOVERWRITE	  37
#define SIPP_OPTION_PLUGIN	  38

// globals from sipp.cpp
bool               use_tdmmap;
unsigned int       tdm_map_a;
unsigned int       tdm_map_b;
unsigned int       tdm_map_c;
unsigned int       tdm_map_x;
unsigned int       tdm_map_y;
unsigned int       tdm_map_z;
unsigned int       tdm_map_h;
bool               tdm_map[1024];

/************ User controls and command line options ***********/
int                duration;
double             rate;
double             rate_scale;
int	          rate_increase;
int	          rate_max;
bool	          rate_quit;
int                users ;
int               rate_period_ms;
int                sleeptime;
unsigned long      defl_recv_timeout;
unsigned long      defl_send_timeout;
unsigned long      global_timeout;
int                transport;
bool               retrans_enabled;
int                rtcheck;
int                max_udp_retrans;
int                max_invite_retrans;
int                max_non_invite_retrans;
unsigned long      default_behaviors;
unsigned long	  deadcall_wait;
bool               pause_msg_ign;
bool               auto_answer;
int                multisocket;
int                compression;
int                peripsocket;
int                peripfield;
bool               bind_local;
void             * monosocket_comp_state;
char             * service;
char             * auth_password;
unsigned long      report_freq;
unsigned long      report_freq_dumpLog;
bool		  periodic_rtd;
char		* stat_delimiter;
bool               timeout_exit;
bool               timeout_error;

unsigned long      report_freq_dumpRtt;

int		  max_multi_socket;
bool		  skip_rlimit;

unsigned int       timer_resolution;
int                max_recv_loops;
int                max_sched_loops;

char               local_ip[40];
char               local_ip_escaped[42];
bool               local_ip_is_ipv6;
int                local_port;
char               control_ip[40];
int                control_port;
int                buff_size ;
int                tcp_readsize;
#ifdef PCAPPLAY
int                hasMedia;
#endif
bool               rtp_echo_enabled;
char               media_ip[40];
char               media_ip_escaped[42];
int                user_media_port;
int                media_port ;
size_t             media_bufsize ;
bool               media_ip_is_ipv6;
char               remote_ip[40];
char               remote_ip_escaped[42];
int                remote_port;
unsigned int       pid;
bool               print_all_responses;
unsigned long      stop_after;
int                quitting;
int                interrupt;
bool               paused;
int                lose_packets;
double             global_lost;
char               remote_host[255];
char               twinSippHost[255];
char               twinSippIp[40];
char             * master_name;
char             * slave_number;
int                twinSippPort ;
bool               twinSippMode ;
bool               extendedTwinSippMode;
bool               nostdin;
bool               backgroundMode;
bool               signalDump;
int                currentScreenToDisplay;
int                currentRepartitionToDisplay;
unsigned int       base_cseq;
char             * auth_uri;
char             * call_id_string;
char             **generic[100];

/************************ Statistics **************************/

unsigned long last_report_calls;
unsigned long nb_net_send_errors;
unsigned long nb_net_cong;
unsigned long nb_net_recv_errors;
bool          cpu_max;
bool          outbound_congestion;
int           open_calls_user_setting;
int           resynch_send;
int           resynch_recv;
unsigned long rtp_pckts;
unsigned long rtp_bytes;
unsigned long rtp_pckts_pcap;
unsigned long rtp_bytes_pcap;
unsigned long rtp2_pckts;
unsigned long rtp2_bytes;
unsigned long rtp2_pckts_pcap;
unsigned long rtp2_bytes_pcap;

/************* Rate Control & Contexts variables **************/

int           last_running_calls;
int           last_woken_calls;
int           last_paused_calls;
unsigned int  open_calls_allowed;
unsigned long last_report_time;
unsigned long last_dump_time;

/********************** Clock variables ***********************/

unsigned long clock_tick;
unsigned long scheduling_loops;
unsigned long last_timer_cycle;

// extern field file management
//typedef std::map<string, FileContents *> file_map;
file_map inFiles;
//typedef std::map<string, str_int_map *> file_index;
char *ip_file;
char *default_file;

// free user id list
list<int> freeUsers;
list<int> retiredUsers;
AllocVariableTable *globalVariables ;
AllocVariableTable *userVariables;
//typedef std::map<int, VariableTable *> int_vt_map;
int_vt_map          userVarMap;

bool   useCallDebugf;
bool   useShortMessagef;
bool   dumpInFile;
bool   dumpInRtt;

char * scenario_file;
char * slave_cfg_file;

static unsigned char tolower_table[256];
void init_tolower_table() {
  for (int i = 0; i < 256; i++) {
    tolower_table[i] = tolower(i);
  }
}
char* remove_pattern(char* P_buffer, char* P_extensionPattern) {

  char *L_ptr = P_buffer;

  if (P_extensionPattern == NULL) {
    return P_buffer ;
  }

  if (P_buffer == NULL) {
    return P_buffer ;
  }

  L_ptr = strstr(P_buffer, P_extensionPattern) ;
  if (L_ptr != NULL) {
    *L_ptr = '\0' ;
  }

  return P_buffer ;
}


SUB_SmallMGC_Test_Task::SUB_SmallMGC_Test_Task(class SUB_H248_Database* database, Poco::NotificationQueue& queue): Task("H248 Receive Task"), m_database(database), m_queue(queue), m_stop(false)
{
  use_tdmmap = false;
  tdm_map_a  = 0;
  tdm_map_b  = 0;
  tdm_map_c  = 0;
  tdm_map_x  = 0;
  tdm_map_y  = 0;
  tdm_map_z  = 0;
  tdm_map_h  = 0;

  duration    =(0);
  rate         =(DEFAULT_RATE);
  rate_scale    =(DEFAULT_RATE_SCALE);
  rate_increase =(0);
  rate_max	 =(0);
  rate_quit	=(true);
  users           =(-1);
  rate_period_ms   = (DEFAULT_RATE_PERIOD_MS);
  sleeptime            =(0);
  defl_recv_timeout    =(0);
  defl_send_timeout   =(0);
  global_timeout     =(0);
  transport         =(DEFAULT_TRANSPORT);
  retrans_enabled = (1);
  rtcheck	= (RTCHECK_FULL);
  max_udp_retrans = (UDP_MAX_RETRANS);
  max_invite_retrans = (UDP_MAX_RETRANS_INVITE_TRANSACTION);
  max_non_invite_retrans = (UDP_MAX_RETRANS_NON_INVITE_TRANSACTION);
  default_behaviors = (DEFAULT_BEHAVIOR_ALL);
  deadcall_wait = (DEFAULT_DEADCALL_WAIT);
  pause_msg_ign = 0;
  auto_answer = false;
  multisocket = 0;
  compression = 0;
  peripsocket = 0;
  peripfield = 0;
  bind_local = false;
  monosocket_comp_state = 0;
  //char             * service                 _DEFVAL(DEFAULT_SERVICE);
  //char* auth_password           _DEFVAL(DEFAULT_AUTH_PASSWORD);
  report_freq = (DEFAULT_REPORT_FREQ);
  report_freq_dumpLog = (DEFAULT_REPORT_FREQ_DUMP_LOG);
  periodic_rtd = false;
  //char* stat_delimiter          _DEFVAL(";");
  timeout_exit  = false;
  timeout_error = false;

  report_freq_dumpRtt = DEFAULT_FREQ_DUMP_RTT;

  max_multi_socket = DEFAULT_MAX_MULTI_SOCKET;
  skip_rlimit = false;

  timer_resolution = DEFAULT_TIMER_RESOLUTION;
  max_recv_loops  = MAX_RECV_LOOPS_PER_CYCLE;
  max_sched_loops = MAX_SCHED_LOOPS_PER_CYCLE;
  local_port = 0;
  control_port = 0;
  buff_size = 65535;
  tcp_readsize = 65535;
  #ifdef PCAPPLAY
  hasMedia = 0;
  #endif
  rtp_echo_enabled = 0;
  user_media_port = 0;
  media_port = 0;
  media_bufsize = 2048;
  remote_port = DEFAULT_PORT;
  pid = 0;
  print_all_responses = false;
  stop_after = 0xffffffff;
  quitting  = 0;
  interrupt = 0;
  paused = false;
  lose_packets = 0;
  global_lost = 0.0;
  twinSippPort = DEFAULT_3PCC_PORT;
  twinSippMode = false;
  extendedTwinSippMode = false;
  nostdin = false;
  backgroundMode = false;
  signalDump = false;
  currentScreenToDisplay  = DISPLAY_SCENARIO_SCREEN;
  currentRepartitionToDisplay = 1;
  base_cseq = 0;
  auth_uri= 0;
 // char             * call_id_string          _DEFVAL("%u-%p@%s");
  /************************ Statistics **************************/

  last_report_calls       =(0);
  nb_net_send_errors      =(0);
  nb_net_cong             =(0);
  nb_net_recv_errors      =(0);
  cpu_max                 =(false);
  outbound_congestion     =(false);
  open_calls_user_setting =(0);
  resynch_send       =(0);
  resynch_recv       =(0);
  rtp_pckts          =(0);
  rtp_bytes          =(0);
  rtp_pckts_pcap     =(0);
  rtp_bytes_pcap     =(0);
  rtp2_pckts         =(0);
  rtp2_bytes         =(0);
  rtp2_pckts_pcap    =(0);
  rtp2_bytes_pcap    =(0);

  /************* Rate Control & Contexts variables **************/

  last_running_calls=(0);
  last_woken_calls  =(0);
  last_paused_calls =(0);
  open_calls_allowed=(0);
  last_report_time  =(0);
  last_dump_time    =(0);

  /********************** Clock variables ***********************/

  clock_tick    =(0);
  scheduling_loops =(0);
  last_timer_cycle =(0);
  // extern field file management
  ip_file=(NULL);
  default_file=(NULL);

  // free user id list
  globalVariables=(NULL);
  userVariables =(NULL);

  useCallDebugf = false;
  useShortMessagef = false;

  dumpInFile = false;
  dumpInRtt  = false;

}

SUB_SmallMGC_Test_Task::~SUB_SmallMGC_Test_Task()
{
}

void SUB_SmallMGC_Test_Task::sendTo(std::string& ipAddress, std::string& message, std::string portNr)
{
}

/*
void SUB_SmallMGC_Test_Task::runTask()
{
  Application& app = Application::instance();
  Application::instance().logger().information("Received Task started.. " + DateTimeFormatter::format(app.uptime()));
  std::string* buff = new std::string();
  // Initialize the tolower table.
  init_tolower_table();

  // Initialize our global variable structure.
  globalVariables = new AllocVariableTable(NULL);
  userVariables = new AllocVariableTable(globalVariables);

  // copy calls from sipp.cpp main
  Poco::UInt32 xcode = 0;
  while (true)
  {
	SmallMGC_SippNotify* receivedMessage= 0;
    receivedMessage = (SmallMGC_SippNotify*)m_queue.waitDequeueNotification(); // wait max 1s for the next message
    Application::instance().logger().debug("Number of Messages in Reading CallQueue: " + NumberFormatter::format(m_queue.size()));
    if (receivedMessage != 0)
    {
       Application::instance().logger().debug("Packet Received.. A Call Control");
       switch (receivedMessage->getType())
       {

          case SIPP_OPTION_HELP:  break;
          case SIPP_OPTION_INT: break;
          case SIPP_OPTION_SETFLAG: break;
          case SIPP_OPTION_UNSETFLAG: break;
          case SIPP_OPTION_STRING: break;
          case SIPP_OPTION_ARGI: break;
          case SIPP_OPTION_TIME_SEC: break;
          case SIPP_OPTION_FLOAT: break;
          case SIPP_OPTION_BOOL: break;
          case SIPP_OPTION_VERSION: break;
          case SIPP_OPTION_TRANSPORT: break;
          case SIPP_OPTION_NEED_SSL: break;
          case SIPP_OPTION_IP: break;
          case SIPP_OPTION_MAX_SOCKET: break;
          case SIPP_OPTION_CSEQ: break;
          case SIPP_OPTION_SCENARIO:
        	if (receivedMessage->getOption().find("-sf") != std::string::npos)
        	{
        	  scenario_file = new char [receivedMessage->getFileName().size()] ;
        	  sprintf(scenario_file,"%s", receivedMessage->getFileName().c_str());
        	  scenario_file = remove_pattern (scenario_file, (char*)".xml");
        	  //if (useLogf == 1)
        	  //{
        	  //  rotate_logfile();
        	  //}
        	  main_scenario = new scenario((char*)receivedMessage->getFileName().c_str(), 0);
        	  main_scenario->stats->setFileName(scenario_file, (char*)".csv");
        	} else if (receivedMessage->getOption().find("-sn") != std::string::npos)
        	{
        	  int i = find_scenario(receivedMessage->getFileName().c_str());

        	  main_scenario = new scenario(0, i);
        	  scenario_file = new char [receivedMessage->getFileName().size()] ;
        	  sprintf(scenario_file,"%s", receivedMessage->getFileName().c_str());
        	  main_scenario->stats->setFileName((char*)receivedMessage->getFileName().c_str(), (char*)".csv");
        	} else if (receivedMessage->getOption().find("-sd") != std::string::npos)
        	{
        	  int i = find_scenario(receivedMessage->getFileName().c_str());
        	  fprintf(stdout, "%s", default_scenario[i]);
        	  //exit(EXIT_OTHER);
        	} else
        	{
        	  //ERROR("Internal error, I don't recognize %s as a scenario option\n", argv[argi] - 1);
            }
            break;
          case SIPP_OPTION_RSA: break;
          case SIPP_OPTION_LIMIT: break;
          case SIPP_OPTION_USERS: break;
          case SIPP_OPTION_KEY: break;
          case SIPP_OPTION_3PCC: break;
          case SIPP_OPTION_TDMMAP: break;
          case SIPP_OPTION_TIME_MS: break;
          case SIPP_OPTION_SLAVE_CFG: break;
          case SIPP_OPTION_3PCC_EXTENDED: break;
          case SIPP_OPTION_INPUT_FILE: break;
          case SIPP_OPTION_TIME_MS_LONG: break;
          case SIPP_OPTION_LONG: break;
          case SIPP_OPTION_LONG_LONG: break;
          case SIPP_OPTION_DEFAULTS:
      	    break;
          case SIPP_OPTION_OOC_SCENARIO: break;
            if (receivedMessage->getOption().find("-oocsf") != std::string::npos)
            {
          	  ooc_scenario = new scenario((char*)receivedMessage->getFileName().c_str(), 0);
          	} else if (receivedMessage->getOption().find("-oocsn") != std::string::npos)
          	{
          	  int i = find_scenario(receivedMessage->getFileName().c_str());
          	  ooc_scenario = new scenario(0, i);
          	} else
          	{
          	  //ERROR("Internal error, I don't recognize %s as a scenario option\n", argv[argi] - 1);
          	}

          case SIPP_OPTION_INDEX_FILE: break;
          case SIPP_OPTION_VAR: break;
          case SIPP_OPTION_RTCHECK: break;
          case SIPP_OPTION_LFNAME: break;
          case SIPP_OPTION_LFOVERWRITE: break;
          case SIPP_OPTION_PLUGIN: break;
       }
       //delete receivedMessage;
       receivedMessage->release();

    }
    else
    {
      Application::instance().logger().debug("No Packet Received.. " + DateTimeFormatter::format(app.uptime()));
    }
  }

  //buff->resize(2000);
}*/

void SUB_SmallMGC_Test_Task::runTask()
{
  Application& app = Application::instance();
  Application::instance().logger().information("Received Task started.. " + DateTimeFormatter::format(app.uptime()));
  std::string* buff = new std::string();

  Poco::UInt32 xcode = 0;
  while (true)
  {
	H248STACK_Commands* receivedMessage= 0;
    receivedMessage = (H248STACK_Commands*)m_queue.waitDequeueNotification(); // wait max 1s for the next message
    Application::instance().logger().debug("Number of Messages in Reading Queue: " + NumberFormatter::format(m_queue.size()));
    if (receivedMessage != 0)
    {

        Application::instance().logger().debug("Packet Received.. A Call Control" /*+ DateTimeFormatter::format(app.uptime())*/);

        receivedMessage->process(((class SmallMgc*)&app)->getSubApi());
       //H248STACK_Commands* com = receivedMessage->getSubCommand();
       //while (com != 0)
       //{
     	//  xcode = com->process(((class SmallMgc*)&app)->getSubApi());
    	//  com = com->getSubCommand();
       //}

       receivedMessage->release();
    }
    else
    {
      Application::instance().logger().debug("No Packet Received.. " /*+ DateTimeFormatter::format(app.uptime())*/);
    }
  }

  //buff->resize(2000);
}



SUB_SmallMGC_Test::SUB_SmallMGC_Test(class SUB_H248_Database* database, TaskManager& tm, Poco::NotificationQueue& queue): Subsystem(), m_database(database), m_tm(tm), m_queue(queue), m_helpRequested(false)
{

}

SUB_SmallMGC_Test::~SUB_SmallMGC_Test()
{
}

Poco::UInt16 SUB_SmallMGC_Test::port() const
{
}

//Poco::Net::SocketAddress SUB_SmallMGC_Test::address() const
//{
//}

void SUB_SmallMGC_Test::initialize(Application& self)
{
   Application::instance().logger().information("starting up");
   // create the receiving task
   m_task = new SUB_SmallMGC_Test_Task(m_database, m_queue);
   m_tm.start(m_task);//m_task->runTask();
}

void SUB_SmallMGC_Test::uninitialize()
{
  Application::instance().logger().information("shutting down");
  m_task->cancel();
}

const char * SUB_SmallMGC_Test::name() const
{
  return ("SUB_SmallMGC_Test");
}

void SUB_SmallMGC_Test::defineOptions(OptionSet& options)
{
}


void SUB_SmallMGC_Test::handleHelp(const std::string& name, const std::string& value)
{
  m_helpRequested = true;
  displayHelp();
  //stopOptionsProcessing();
}

void SUB_SmallMGC_Test::displayHelp()
{
  //HelpFormatter helpFormatter(options());
  //helpFormatter.setCommand(commandName());
  //helpFormatter.setUsage("OPTIONS");
  //helpFormatter.setHeader("The subsytem for transmitting and receiving H248 packets.");
  //helpFormatter.format(std::cout);
}


void SUB_SmallMGC_Test::sendTo(std::string& ipAddress, std::string& message, std::string portNr)
{
  m_task->sendTo(ipAddress, message, portNr);
}
