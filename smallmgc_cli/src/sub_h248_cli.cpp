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
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include <termios.h>

#include "sub_h248_database.h"
#include "h248_cli_stimulis.h"

#include "version.h"

void do_cd(char *argv[]);
void do_help(char *argv[]);
void execute_new(char *argv[]);
void do_mgc_status(char *argv[]);

int mygetch( ) {
struct termios oldt,
newt;
int ch;
tcgetattr( STDIN_FILENO, &oldt );
newt = oldt;
newt.c_lflag &= ~( ICANON | ECHO );
tcsetattr( STDIN_FILENO, TCSANOW, &newt );
ch = getchar();
tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
return ch;
}

//tets
#include "Poco/Util/HelpFormatter.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Net/NetException.h"
#include "Poco/Timespan.h"

#include <iostream>
#include "sub_h248_cli.h"
#include "sub_h248_basenotify.h"


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

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #define strcasecmp stricmp
#endif

volatile unsigned char running = 1;

/*void Command(const char *cmd) {
	char buf[SCT_CMDLINE_BUFFER_SIZE], *tok;
	SCT_Printf("Received: \"%s\"\n", cmd);

	strcpy(buf, cmd);
	tok = strtok(buf, " ");
	if (!strcasecmp(tok, "quit"))
        running = 0;
	else if (!strcasecmp(tok, "cls"))
		SCT_ClearScreen();
	else if (!strcasecmp(tok, "setprompt"))
		SCT_SetPrompt(buf + strlen(tok) + 1);
	else if (!strcasecmp(tok, "clhist"))
		SCT_ClearHistory();
	else if (!strcasecmp(tok, "help"))
		SCT_Printf("Available commands:\n"
			"clhist         - clears command history\n"
			"cls            - clears the output screen\n"
			"help           - displays this message\n"
			"quit           - quits the program\n"
			"setprompt      - sets the new prompt to the command's arguments\n");
	memset(buf, 0, sizeof(buf));
}
*/


SUB_H248_CLI_Task::SUB_H248_CLI_Task(Poco::NotificationQueue& queue, SUB_H248_Database* database, class SmallMgc* app): Task("H248 Receive Task"), m_queue(queue), m_database(database), m_stop(false)
{
  //const std::string command("root");
  const std::string command("");
  m_rootStimuli = new H248_CLI_Stimuli_Root(command, 0, database);
  const std::string command1("database");
  H248_CLI_Stimuli_Database* database_stimuli = new H248_CLI_Stimuli_Database(command1, m_rootStimuli, database);
  const std::string command1a("gateways");
  H248_CLI_Stimuli_Db_Gateways* db_gateway_stimuli = new H248_CLI_Stimuli_Db_Gateways(command1a, database_stimuli, database);
  const std::string command2("queues");
  H248_CLI_Stimuli_QueueInfo* queue_stimuli = new H248_CLI_Stimuli_QueueInfo(command2, m_rootStimuli, database, app);
  const std::string command3("send");
  H248_CLI_Stimuli_SendMessages* send_stimuli = new H248_CLI_Stimuli_SendMessages(command3, m_rootStimuli, database, app);
  const std::string command4("hreplies");
  H248_CLI_Stimuli_SendMessagesH* send_stimuliH = new H248_CLI_Stimuli_SendMessagesH(command4, send_stimuli, database, app);
  const std::string command5("hcommands");
  H248_CLI_Stimuli_SendMessagesHR* send_stimuliHR = new H248_CLI_Stimuli_SendMessagesHR(command5, send_stimuli, database, app);
  const std::string command5a("ereplies");
  H248_CLI_Stimuli_SendMessagesER* send_stimuliER = new H248_CLI_Stimuli_SendMessagesER(command5a, send_stimuli, database, app);

  const std::string command6("iua");
  H248_CLI_Stimuli_SendIUAMessages* send_stimuliIUA = new H248_CLI_Stimuli_SendIUAMessages(command6, send_stimuli, database, app);
  const std::string command7("iuaq931");
  H248_CLI_Stimuli_SendIUAQ931Messages* send_stimuliIUAQ931 = new H248_CLI_Stimuli_SendIUAQ931Messages(command7, send_stimuli, database, app);
  const std::string command8("megaco");
  H248_CLI_Stimuli_SendMegacoTestMessages* send_stimuliMegaco = new H248_CLI_Stimuli_SendMegacoTestMessages(command8, send_stimuli, database, app);
  const std::string command9("sip");
   H248_CLI_Stimuli_SendMessagesSip* send_stimuliSip = new H248_CLI_Stimuli_SendMessagesSip(command9, send_stimuli, database, app);
  const std::string command10("sdp");
  H248_CLI_Stimuli_SendMessagesSdp* send_stimuliSdp = new H248_CLI_Stimuli_SendMessagesSdp(command10, send_stimuli, database, app);

}

SUB_H248_CLI_Task::~SUB_H248_CLI_Task()
{
}


void SUB_H248_CLI_Task::runTask()
{
  Application& app = Application::instance();
  Application::instance().logger().information("Received Task started.. " + DateTimeFormatter::format(app.uptime()));
  /*		if (!SCT_Init()) { // initialize scotty
        fApplication::instance().logger().trace(stderr, "libscotty error: %i\n", SCT_GetError());
        exit(1);
	    }
	SCT_SetCommandHandlerFunction(Command); // use the Command() function as the command parser
	SCT_SetHistorySize(32); // make libscotty keep record of the last 32 commands
	SCT_AddCompletionEntry("clhist");
	SCT_AddCompletionEntry("cls");
	SCT_AddCompletionEntry("help");
	SCT_AddCompletionEntry("quit");
	SCT_AddCompletionEntry("setprompt");
	SCT_AddCompletionEntry("whoa, now you have to choose!");
	SCT_AddCompletionEntry("whoa, now you have to choose out of two!");

	SCT_Application::instance().logger().trace("Hello world!\nI will echo all the commands you type in, and keep\n"
		"track of the last 32! Use the vertical arrow keys to cycle through my\n"
		"memory. Also, try the command completion feature - just type something and\n"
		"press tab!\n"
		"Type \"help\" to list available commands, \"quit\" to exit.\n");
	SCT_SetPrompt("] "); // set a simple prompt
    while (running)
	   SCT_Input();

   SCT_Close();
  */
   char *cmd=(char *)malloc(256*sizeof(char));
   char *cmd_arg[10];
   int cmdlen,i,j,tag;
   char prompt[30];
   strncpy(prompt, PRODUCT_NAME, sizeof(PRODUCT_NAME));
   strncpy(&(prompt[sizeof(PRODUCT_NAME)-1]), " ", 1);
   strncpy(&(prompt[sizeof(PRODUCT_NAME)]), PRODUCT_VERSION, sizeof(PRODUCT_VERSION));
   //printf("Prompt : %s\n", prompt);

   do{
      /* init cmd */
      for(i=0;i<255;i++) cmd[i]='\0';

      printf("|*-=%s=-*| ", prompt);
      /*cmd[0]=mygetch();
      switch(cmd[0])
      {
        case 72: Application::instance().logger().trace(" code for up"); break;
        default: Application::instance().logger().trace ("The char %c has the ASCII code %d\n", cmd[0], cmd[0]);
      }*/
      fgets(&cmd[0],256,stdin);

      cmdlen=strlen(cmd);
      if (cmdlen < 2)
        continue;
      cmdlen--;
      cmd[cmdlen]='\0';

      for(i=0;i<10;i++) cmd_arg[i]=NULL;
      i=0; j=0; tag=0;
      while(i<cmdlen && j<10){
         if(cmd[i]==' '){
            cmd[i]='\0';
            tag=0;
         }else{
            if(tag==0)
               cmd_arg[j++]=cmd+i;
            tag=1;
         }
         i++;
      }

      if(j>=10 && i<cmdlen){
         printf("TOO MANY ARGUMENTS\n");
         continue;
      }

      /* cmd quit: exit Mini Shell */
      if(strcmp(cmd_arg[0],"quit")==0)
         break;

      /* cmd cd */
      if(strcmp(cmd_arg[0],"cd")==0){
         do_cd(cmd_arg);
         continue;
      }
      /* cmd help */
      /*if(strcmp(cmd_arg[0],"help")==0){
         do_help(cmd_arg);
         continue;

      } */

      /* cmd help */
      if(strcmp(cmd_arg[0],"mgcstatus")==0){
         do_mgc_status(cmd_arg);
         continue;
      }

      m_rootStimuli->doAction(cmd_arg);
      /* other cmd */
      //execute_new(cmd_arg);
   }while(1);

}

/* cd cmd */
void do_mgc_status(char *argv[])
{
   if(argv[1]!=NULL){
     if(strcmp(argv[1],"root")==0){
         printf(" Get MGC status\n");
         //do_mgc_status(cmd_arg);
         return;
     }
   }
}

/* cd cmd */
void do_cd(char *argv[])
{
   if(argv[1]!=NULL){
      if(chdir(argv[1])<0)
         switch(errno){
         case ENOENT:
            printf("DIRECTORY NOT FOUND\n");
            break;
         case ENOTDIR:
            printf("NOT A DIRECTORY NAME\n");
            break;
         case EACCES:
            printf("YOU DO NOT HAVE RIGHT TO ACCESS\n");
            break;
         default:
            printf("SOME ERROR HAPPENED IN CHDIR\n");
         }
   }

}

/* cd cmd */
void do_help(char *argv[])
{
   printf(" This is the help function\n");
}
/* other cmd */
void execute_new(char *argv[])
{
   pid_t pid;

   pid=fork();
   if(pid<0){
      printf("SOME ERROR HAPPENED IN FORK\n");
      exit(2);
   }else if(pid==0){
      if(execvp(argv[0],argv)<0)
         switch(errno){
         case ENOENT:
               printf("COMMAND OR FILENAME NOT FOUND\n");
               break;
            case EACCES:
               printf("YOU DO NOT HAVE RIGHT TO ACCESS\n");
               break;
                           default:
                                   printf("SOME ERROR HAPPENED IN EXEC\n");
         }
         exit(3);
      }else
         wait(NULL);
}

SUB_H248_CLI::SUB_H248_CLI(TaskManager& tm, Poco::NotificationQueue& queue, SUB_H248_Database* database): Subsystem(), m_tm(tm), m_queue(queue),
     m_database(database), m_helpRequested(false)
{
}

SUB_H248_CLI::~SUB_H248_CLI()
{
}

void SUB_H248_CLI::initialize(Application& self)
{
   Application::instance().logger().information("starting up");
   class SmallMgc* app = (class SmallMgc*)(&Application::instance());
   // create the receiving task
   m_task = new SUB_H248_CLI_Task(m_queue, m_database, app);
   m_tm.start(m_task);//m_task->runTask();
}

void SUB_H248_CLI::uninitialize()
{
  Application::instance().logger().information("shutting down");
  m_task->cancel();
}

const char * SUB_H248_CLI::name() const
{
  return ("SUB_H248_CLI");
}

void SUB_H248_CLI::defineOptions(OptionSet& options)
{
}


void SUB_H248_CLI::handleHelp(const std::string& name, const std::string& value)
{
  m_helpRequested = true;
  displayHelp();
  //stopOptionsProcessing();
}

void SUB_H248_CLI::displayHelp()
{
  //HelpFormatter helpFormatter(options());
  //helpFormatter.setCommand(commandName());
  //helpFormatter.setUsage("OPTIONS");
  //helpFormatter.setHeader("The subsytem for transmitting and receiving H248 packets.");
  //helpFormatter.format(std::cout);
}


