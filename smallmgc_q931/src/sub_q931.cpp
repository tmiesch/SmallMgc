//
// sub_q931.cpp
//
// Subsystem for receiving h248 packets

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/sctp.h>
#include <sys/uio.h>

#define BUFLEN  100


//tets
#include "Poco/Util/HelpFormatter.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Net/NetException.h"
#include "Poco/Timespan.h"

#include <iostream>
#include "sub_q931.h"
#include "sub_h248_database.h"
#include "h248stack_database.h"
#include "sub_iua_notify.h"
//#include "Q931.h"
#include "q931adapter.h"

using Poco::Util::Application;
using Poco::Util::Subsystem;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;
using Poco::Task;
using Poco::TaskManager;
using Poco::DateTimeFormatter;





Q931_Adapter* SUB_Q931_Task::m_q931Adapter = 0;

Q931_Adapter* SUB_Q931_Task::getAdapter()
   {
     return m_q931Adapter;
  }


void SUB_Q931_Task::handleEvent(void *buf)
{
}

void* SUB_Q931_Task::receiveMessage(int fd, struct msghdr *msg, void *buf, size_t *buflen, ssize_t *nrp, size_t cmsglen)
{
}


SUB_Q931_Task::SUB_Q931_Task(class SUB_H248_Database* database, Poco::NotificationQueue& isdnQueue, Poco::NotificationQueue& iuaQueue, Poco::NotificationQueue& callQueue): Task("Q931 Task"),
        m_database(database), m_isdnQueue(isdnQueue), m_iuaQueue(iuaQueue), m_callQueue(callQueue), m_stop(false)
{
  m_q931Adapter = new Q931_Adapter(database, iuaQueue, callQueue, isdnQueue);
  Q931Ie::createPrototypes();
}

SUB_Q931_Task::~SUB_Q931_Task()
{
}

void SUB_Q931_Task::initIsdnStack()
{

}

void SUB_Q931_Task::timerExpired(class Q931_Call *call, Q931Timers timerId)
{
  call->q931CallClearTimer(timerId);
  switch (timerId)
  {
    case Q931_T301:
      call->q931_timer_T301();
      break;
    case Q931_T302:
      call->q931_timer_T302();
      break;
    case Q931_T303:
      call->q931_timer_T303();
      break;
    case Q931_T304:
      call->q931_timer_T304();
      break;
    case Q931_T305:
      call->q931_timer_T305();
      break;
    case Q931_T306:
      call->q931_timer_T306();
      break;
    case Q931_T308:
      call->q931_timer_T308();
      break;
    case Q931_T309:
      call->q931_timer_T309();
      break;
    case Q931_T310:
      call->q931_timer_T310();
      break;
    case Q931_T312:
      call->q931_timer_T312();
      break;
    case Q931_T313:
      call->q931_timer_T313();
      break;
    case Q931_T314:
      call->q931_timer_T314();
      break;
    case Q931_T316:
      call->q931_timer_T316();
      break;
    case Q931_T318:
      call->q931_timer_T318();
      break;
    case Q931_T319:
      call->q931_timer_T319();
      break;
    case Q931_T320:
      call->q931_timer_T320();
      break;
    case Q931_T321:
      call->q931_timer_T321();
      break;
    case Q931_T322:
      call->q931_timer_T322();
      break;

  }

}

void SUB_Q931_Task::sendTo(std::string& ipAddress, std::string& message)
{
  //SocketAddress destinationAddress(ipAddress, 2944);
  //m_h248SocketUdp->sendTo(message.c_str(), message.length(), destinationAddress);
}

void SUB_Q931_Task::runTask()
{
  Application& app = Application::instance();
  Application::instance().logger().information("Q931 Task started.. " + DateTimeFormatter::format(app.uptime()));

  Poco::AutoPtr<Poco::Notification> pNf = m_isdnQueue.waitDequeueNotification();
  while (true)
  {
    if (pNf)
	{
	  Poco::AutoPtr<SUB_IUA_Base> receivedMessage = pNf.cast<SUB_IUA_Base>();
      //SUB_IUA_Base* receivedMessage= 0;
      //receivedMessage = (SUB_IUA_Base*)m_isdnQueue.waitDequeueNotification(); // wait max 1s for the next message
      if (receivedMessage)
      {
        Application::instance().logger().trace(" Q931;  Received Message\n");
        receivedMessage->process(this);
        //delete receivedMessage;
        //receivedMessage->release();
      }
	}
    pNf = m_isdnQueue.waitDequeueNotification();
  }

}



SUB_Q931::SUB_Q931(class SUB_H248_Database* database, TaskManager& tm,  Poco::NotificationQueue& isdnQueue, Poco::NotificationQueue& iuaQueue, Poco::NotificationQueue& callQueue):
	   Subsystem(), m_database(database), m_tm(tm), m_isdnQueue(isdnQueue), m_iuaQueue(iuaQueue), m_callQueue(callQueue), m_helpRequested(false)
{
}

SUB_Q931::~SUB_Q931()
{
}

void SUB_Q931::initialize(Application& self)
{
   Application::instance().logger().information("starting up");
   // create the receiving task
   m_task = new SUB_Q931_Task(m_database, m_isdnQueue, m_iuaQueue, m_callQueue);
   m_tm.start(m_task);//m_task->runTask();
}

void SUB_Q931::uninitialize()
{
  Application::instance().logger().information("shutting down");
  m_task->cancel();
}

const char * SUB_Q931::name() const
{
  return ("SUB_Q931");
}

void SUB_Q931::defineOptions(OptionSet& options)
{
}


void SUB_Q931::handleHelp(const std::string& name, const std::string& value)
{
  m_helpRequested = true;
  displayHelp();
  //stopOptionsProcessing();
}

void SUB_Q931::displayHelp()
{
  //HelpFormatter helpFormatter(options());
  //helpFormatter.setCommand(commandName());
  //helpFormatter.setUsage("OPTIONS");
  //helpFormatter.setHeader("The subsytem for transmitting and receiving H248 packets.");
  //helpFormatter.format(std::cout);
}


void SUB_Q931::sendTo(std::string& ipAddress, std::string& message)
{
  m_task->sendTo(ipAddress, message);
}
