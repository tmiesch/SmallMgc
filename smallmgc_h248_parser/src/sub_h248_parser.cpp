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

#include "Poco/Notification.h"

#include "sub_h248_parser.h"
#include "sub_h248_basenotify.h"
#include "sub_h248_database.h"

#include "h248stack_parser.h"
#include "h248stack_commands.h"
//test

using Poco::Util::Application;
using Poco::Util::Subsystem;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;
using Poco::Task;
using Poco::TaskManager;
using Poco::DateTimeFormatter;
using Poco::Notification;
using Poco::NotificationQueue;
using Poco::NumberFormatter;

void SUB_H248_Parser_Task::runTask()
{
  Poco::AutoPtr<Poco::Notification> pNf = m_queue.waitDequeueNotification();
  while (true)
  {
    if (pNf)
	{
	  Poco::AutoPtr<SUB_H248_BaseNotify> receivedMessage = pNf.cast<SUB_H248_BaseNotify>();
      //SUB_H248_BaseNotify* receivedMessage= 0;
      //receivedMessage = (SUB_H248_BaseNotify*)m_queue.waitDequeueNotification(); // wait max 1s for the next message
      if (receivedMessage)
      {
        Application::instance().logger().warning("Packet Received.. " + receivedMessage->getMessage());
        parser(receivedMessage->getMessage());
        //delete receivedMessage;
        //receivedMessage->release();
      }
      else
      {
        Application::instance().logger().debug("No Packet Received.. " /*+ DateTimeFormatter::format(app.uptime())*/);
      }
	}
    pNf = m_queue.waitDequeueNotification();
  }
}void SUB_H248_Parser_Task::switchToTestMode()
{
	m_testMode = false;
}

void SUB_H248_Parser_Task::switchToNormalMode()
{
	m_testMode = false;
}

void SUB_H248_Parser_Task::parser(std::string& message)
{
  m_parser->setMessage((char*)message.c_str());
  Poco::UInt32 ret = parse248((char*)message.c_str());

  if (ret == 0)
  {
    H248STACK_Commands* command = m_parser->getCommand();
    if (command != 0)
    {
      if (command->isForCallControl())
      {
        if (!m_testMode)
        {
          //Application::instance().logger().information("Enqueue for call control " /*+ DateTimeFormatter::format(app.uptime())*/);
          m_callQueue.enqueueNotification(command);
          Application::instance().logger().debug("Number of Messages in CallQueue: " + NumberFormatter::format(m_callQueue.size()));
        }
        else
        {
          m_testQueue.enqueueNotification(command);
          Application::instance().logger().debug("Number of Messages in TestQueue: " + NumberFormatter::format(m_testQueue.size()));
        }
      }
      else
      {
        //Application::instance().logger().debug("Enqueue for gateway control" /*+ DateTimeFormatter::format(app.uptime())*/);
        m_gateQueue.enqueueNotification(command);
        Application::instance().logger().debug("Number of Messages in GateQueueA: " + NumberFormatter::format(m_gateQueue.size()));
      }
    }
  //m_callControl->callControl(/*m_parser*/);
  }
  else
  {
    H248STACK_Commands* command = m_parser->getCommand();
    if (command != 0)
    {
      //Application::instance().logger().debug("Enqueue for gateway control A" /*+ DateTimeFormatter::format(app.uptime())*/);
      m_gateQueue.enqueueNotification(command);
      Application::instance().logger().debug("Number of Messages in GateQueueB: " + NumberFormatter::format(m_gateQueue.size()));
    }
    else
    {
      //printf("message %s", message.c_str());
    }
  }
}
#include "Apg.h"
/*#include "ApgUtilities.h"*/
#include "MEGACO.h"
static void* vpMemCtx = NULL;
static SEM_CALLBACK saSemantic[RULE_COUNT_MEGACO];
static SYN_CALLBACK saSyntactic[RULE_COUNT_MEGACO];
static ulong        ulLevel = 6;
static ulong        ulaAst[RULE_COUNT_MEGACO];
static void*        vpParserCtx = NULL;
extern "C"
{
	// initializes the semantic callback functions and a corresponding AST
	void vCallbackInit(SEM_CALLBACK* spSemantic, ulong* ulpAst);

	// initialize the syntactic callback functions with the handwritten optimizing callbacks
	void vSyntaxCallbackInit(SYN_CALLBACK* spSyntax, ulong ulLevel);

	// copied from ApgUtilities
	void vDisplayAST(void* vpCtx, ulong ulLineLen, ulong ulAscii, ulong ulHex);

}

void SUB_H248_Parser_Task::initStack()
{
	ulong        ulTest;
	vpMemCtx = vpMemCtor();
    // initialize the parser(s)
    if(!vpMemCtx){return;}

    // initialize the parser
    vpParserCtx = vpParserCtor(vpParserInit_MEGACO, vpMemCtx);
    if(!vpParserCtx){return;}

    // initialize the handwritten, sytactic callback functions
    vSyntaxCallbackInit(saSyntactic, ulLevel);
    ulTest = ulParserSyntaxInit(vpParserCtx, saSyntactic);
    if(!ulTest){return;}

    // initialize the semantic callback functions and the corresponding AST
    vCallbackInit(saSemantic, ulaAst);
    ulTest = ulParserSemanticInit(vpParserCtx, saSemantic);
    if(!ulTest){return;}
    ulTest = ulParserAstInit(vpParserCtx, ulaAst);
    if(!ulTest){return;}

   //unsigned int ulTemp = ulParserInit(vpMEGACOInit, vpMemCtx);
}

Poco::UInt32 SUB_H248_Parser_Task::parse248(char* mesg)
{
  ulong         ulTest;
  ulong         ulTrace = APG_FALSE;
  void*         vpData = NULL;
  APG_PARSER_STATS  sStats;
  ulong ulSize = sizeof(APG_PARSER_STATS) / sizeof(ulong);
  APG_PARSER_STATE  sState;

  ulong  len = strlen(mesg);
  ulTest = ulParserSyntaxAnalysis(vpParserCtx, RULE_MEGACO_MEGACOMESSAGE,(Poco::UInt8*)mesg, len, NULL);
  if(!ulTest){return 1;}
  ulTest = ulParserState(vpParserCtx, &sState);
  if(!ulTest){return 1;}
  if(sState.ulState != MATCH){return 1;}
  // translate the message (capture the parsed phrases)
  ulTest = ulParserSemanticAnalysis(vpParserCtx, NULL);
  if(!ulTest){return 1;}

    //ulTest = ulParserSyntaxAnalysis(m_vpParserCtx, 0, (Poco::UInt8*)mesg, len, vpData, ulTrace);
    //ulTest = ulParserState(m_vpParserCtx, &sState);
    if(sState.ulState == NOMATCH)
    {
      Application::instance().logger().error("\n");
      Application::instance().logger().error("*** PARSE FAILED FOR THIS MESSAGE ***\n");
      std::string msg(mesg);
      Application::instance().logger().error(msg + "\n");
      H248STACK_Commands* receivedMesg = H248STACK_Parser::getCommand();
      return 1;
    }

    if(sState.ulState == MATCH)
    {
      //H248STACK_Commands* receivedMesg = H248STACK_Parser::getCommand();
      //if (0 != receivedMesg)
      //{
         //printf("Message received\n");
         //receivedMesg->process();
         //delete receivedMesg;
         //receivedMesg = 0;
         return 0;
      //}

	}
    return 2;
    // clear the error message queue
    //ulTest = ulErrCount(vpMemCtx);
}

SUB_H248_Parser_Task::SUB_H248_Parser_Task(Poco::NotificationQueue& queue,Poco::NotificationQueue& callQueue, Poco::NotificationQueue& gateQueue, class Poco::NotificationQueue& testQueue):
       Task("H248 Parse Task"), m_queue(queue), m_callQueue(callQueue), m_gateQueue(gateQueue), m_testQueue(testQueue), m_testMode(false)
{
  initStack();
  // get a parser context
  //m_vpParserCtx = vpParserCtor(vpMEGACOInit, vpMemCtx);
  m_parser   = new H248STACK_Parser();
}

SUB_H248_Parser_Task::~SUB_H248_Parser_Task()
{
  delete m_parser;
}

SUB_H248_Parser::SUB_H248_Parser(TaskManager& tm , NotificationQueue& queue, Poco::NotificationQueue& callQueue, Poco::NotificationQueue& gateQueue, class Poco::NotificationQueue& testQueue): Subsystem(), m_helpRequested(false),
m_tm(tm), m_queue(queue), m_callQueue(callQueue), m_gateQueue(gateQueue), m_testQueue(testQueue)
{

}

SUB_H248_Parser::~SUB_H248_Parser()
{
}


void SUB_H248_Parser::initialize(Application& self)
{
   Application::instance().logger().information("starting up");
   // create the receiving task
   //SUB_H248_Database base = Application::instance().getSubsystem();
   //m_database = &base;
   m_task = new SUB_H248_Parser_Task(m_queue, m_callQueue, m_gateQueue, m_testQueue);
   m_tm.start(m_task);//m_task->runTask();
}

void SUB_H248_Parser::uninitialize()
{
  Application::instance().logger().information("shutting down");
  m_task->cancel();
}

const char * SUB_H248_Parser::name() const
{
  return ("SUB_H248_Parser");
}

void SUB_H248_Parser::defineOptions(OptionSet& options)
{
}


void SUB_H248_Parser::handleHelp(const std::string& name, const std::string& value)
{
  m_helpRequested = true;
  displayHelp();
  //stopOptionsProcessing();
}

void SUB_H248_Parser::displayHelp()
{
  //HelpFormatter helpFormatter(options());
  //helpFormatter.setCommand(commandName());
  //helpFormatter.setUsage("OPTIONS");
  //helpFormatter.setHeader("The subsytem for transmitting and receiving H248 packets.");
  //helpFormatter.format(std::cout);
}

void SUB_H248_Parser::switchToTestMode()
{
	m_task->switchToTestMode();
}

void SUB_H248_Parser::switchToNormalMode()
{
	m_task->switchToNormalMode();
}





