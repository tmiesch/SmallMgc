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

#include <stdio.h>
#include <string.h>


#include "h248stack_team.h"
//#include "h248stack_stdcallcontrol.h"
//#include "h248stack_commands.h"
//#include "h248stack_gatewaycontrol.h"
//#include "h248stack_database.h"


#include "h248stack_parser.h"
#include <errno.h>

#include <string>
#include <map>

using Poco::UInt8;

//----------------------------------------------------------------------------
// Constant definition
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Implementation of the H248STACK_Team
//----------------------------------------------------------------------------

H248STACK_Team::H248STACK_Team () :
  m_enabled(true),
  m_transactionId(10000),
  m_eventRequestId(0)
  //m_callControl(0),
{
      //m_database = new H248STACK_Database(xcode, this, teamName);
      //m_callControl = new H248STACK_StdCallControl(xcode, this, m_database,  teamName);
      //m_gatewayControl = new H248STACK_GatewayControl(xcode, this, m_database, teamName);
      initStack();

}

void H248STACK_Team::initStack()
{
}

void H248STACK_Team::parse248(char* mesg)
{
}

//----------------------------------------------------------------------------
H248STACK_Team::~H248STACK_Team()
{
}



/*BTXCode H248STACK_Team::handleHeartbeatTimeout(void* request, void* response, BTUInt32 &responseSize)
{
}
*/



UInt32 H248STACK_Team::handleParse(std::string message)
{
  // m_parser->decode(message);
  m_parser->setMessage((char*)message.c_str());
  parse248((char*)message.c_str());
  //m_callControl->callControl(/*m_parser*/);
  return 0;
}





//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
/*H248STACK_Gateway* H248STACK_Team::getGateway(std::string ipAddress)
{
   return m_database->getGateway(ipAddress);
}
*/
void H248STACK_Team::assocInService(std::string ipAddress)
{
    //m_gatewayControl->assocInService(ipAddress);
}

void  H248STACK_Team::sendToMg(std::string ipAddress, std::string message)
{
   /*int rc;
   struct sockaddr_in remoteServAddr;
   remoteServAddr.sin_family = AF_INET;
   remoteServAddr.sin_addr.s_addr = inet_addr (ipAddress.c_str());
   remoteServAddr.sin_port = htons (2944);

    printf("Message to be sent: %s\n to address :%s\n", message.c_str(),ipAddress.c_str());

    rc = sendto (m_udpSocket, message.c_str(), message.size(), 0,
                 (struct sockaddr *) &remoteServAddr,
                 sizeof (remoteServAddr));
   */
}



