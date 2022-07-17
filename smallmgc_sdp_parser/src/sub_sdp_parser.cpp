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

#include "sub_sdp_parser.h"
#include "sub_h248_basenotify.h"
#include "sub_h248_database.h"

#include "sdp_parser.h"
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


void SUB_SDP_Parser::parser(std::string& message)
{
  m_parser->setMessage((char*)message.c_str());
  Poco::UInt32 ret = parseSDP((char*)message.c_str());
}
extern "C"
{
  #include "Apg.h"
  #include "SDP.h"
  static void* vpMemCtx = NULL;
  static SEM_CALLBACK saSemantic[RULE_COUNT_SDP];
  static SYN_CALLBACK saSyntactic[RULE_COUNT_SDP];
  static ulong        ulLevel = 6;
  static ulong        ulaAst[RULE_COUNT_SDP];
  static void*        vpParserCtx = NULL;
  // initializes the semantic callback functions and a corresponding AST
   // initialize the syntactic callback functions with the handwritten optimizing callbacks
  void sdpSyntaxCallbackInit_ALL(SYN_CALLBACK* spSyntax);
  void vSdpAstInit(ulong* ulpAst);
  void vDisplayAST(void* vpCtx, ulong ulLineLen, ulong ulAscii, ulong ulHex);
  void vPrintASTNodes(void* vpParserCtx);

}
void sdpSemanticCallbackInit_SDP(SEM_CALLBACK* spSemantic, ulong* ulpAst);

void SUB_SDP_Parser::initStack()
{
  ulong        ulTest;
  vpMemCtx = vpMemCtor();
  // initialize the parser(s)
  if(!vpMemCtx){return;}

  // initialize the parser
  vpParserCtx = vpParserCtor(vpParserInit_SDP, vpMemCtx);
  if(!vpParserCtx){return;}

  // initialize the handwritten, sytactic callback functions
  sdpSyntaxCallbackInit_ALL(saSyntactic);
  ulTest = ulParserSyntaxInit(vpParserCtx, saSyntactic);
  if(!ulTest){return;}

  vSdpAstInit(ulaAst);
  ulTest = ulParserAstInit(vpParserCtx, ulaAst);
  if(!ulTest){return;}

  // initialize the semantic callback functions and the corresponding AST
  sdpSemanticCallbackInit_SDP(saSemantic, ulaAst);
  ulTest = ulParserSemanticInit(vpParserCtx, saSemantic);
  if(!ulTest){return;}


}



Poco::UInt32 SUB_SDP_Parser::parseSDP(char* mesg)
{
  ulong         ulTest;
  ulong         ulTrace = APG_FALSE;
  void*         vpData = NULL;
  APG_PARSER_STATS  sStats;
  ulong ulSize = sizeof(APG_PARSER_STATS) / sizeof(ulong);
  APG_PARSER_STATE  sState;

  ulong  len = strlen(mesg);
  ulTest = ulParserSyntaxAnalysis(vpParserCtx, RULE_SDP_ANNOUNCEMENT, (Poco::UInt8*)mesg, len, NULL);
  if(!ulTest){return 1;}
  ulTest = ulParserState(vpParserCtx, &sState);
  if(!ulTest){return 1;}
  //if(sState.ulState != MATCH){return 1;}
  // translate the message (capture the parsed phrases)

  vPrintASTNodes(vpParserCtx);
  vDisplayAST(vpParserCtx,32, APG_TRUE, APG_TRUE);

  ulTest = ulParserSemanticAnalysis(vpParserCtx, NULL);
  if(!ulTest){return 1;}

  if(sState.ulState == NOMATCH)
  {
    Application::instance().logger().error("\n");
    Application::instance().logger().error("*** PARSE FAILED FOR THIS MESSAGE ***\n");
    std::string msg(mesg);
    Application::instance().logger().error(msg + "\n");
    //H248STACK_Commands* receivedMesg = H248STACK_Parser::getCommand();
    return 1;
  }

  if(sState.ulState == MATCH)
  {
    return 0;
  }
  return 2;
}


SUB_SDP_Parser::SUB_SDP_Parser()
{
  initStack();
  m_parser   = new SDPSTACK_Parser();
}

SUB_SDP_Parser::~SUB_SDP_Parser()
{
  delete m_parser;
}


void SUB_SDP_Parser::initialize(Application& self)
{
   Application::instance().logger().information("starting up");
   // create the receiving task
   //SUB_H248_Database base = Application::instance().getSubsystem();
   //m_database = &base;
}

void SUB_SDP_Parser::uninitialize()
{
  Application::instance().logger().information("shutting down");
  //m_task->cancel();
}

const char * SUB_SDP_Parser::name() const
{
  return ("SUB_SDP_Parser");
}

void SUB_SDP_Parser::defineOptions(OptionSet& options)
{
}


void SUB_SDP_Parser::handleHelp(const std::string& name, const std::string& value)
{
  m_helpRequested = true;
  displayHelp();
  //stopOptionsProcessing();
}

void SUB_SDP_Parser::displayHelp()
{
  //HelpFormatter helpFormatter(options());
  //helpFormatter.setCommand(commandName());
  //helpFormatter.setUsage("OPTIONS");
  //helpFormatter.setHeader("The subsytem for transmitting and receiving H248 packets.");
  //helpFormatter.format(std::cout);
}

void SUB_SDP_Parser::switchToTestMode()
{

}

void SUB_SDP_Parser::switchToNormalMode()
{

}





