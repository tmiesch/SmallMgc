#ifndef H248STACK_TEAM_H
#define H248STACK_TEAM_H
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

using Poco::UInt32;
/**
 * This class implements the body of the team. It is used to decouple the
 * team interface from the team implementation. All team members are aggregated
 * to it as a composition. It has all knowledge about the team internals and is
 * responsible to create and delete the team members.
 * If task context switching is used, the functionality after the context switch
 * (so-called "handle" functions) is implemented in this class.
 *
 * See the "team" design pattern document for more explanations.
 */

class H248STACK_Team
{
public: // mandatory
  /**
   * Create the whole infrastructure of the team, including the standard
   * base infrastructure for a standard team.
   *
   * @param xcode           see service interface.
   * @param setupDefinition see service interface.
   * @param adapter         see service interface.
   * @param mds             see service interface
   * @param teamName        see service interface.
   * @param serviceSocket   see service interface
   */
  H248STACK_Team();

  /**
   * Delete all the resources allocated for this object.
   */
  virtual ~H248STACK_Team();


public:
  //static class H248STACK_Team* getTeam() { return s_team; };
  //class H248STACK_Service*  getService() {  return m_service; };
  //class H248STACK_StdCallControl* getCallHandler() { return m_callControl; };
  //class H248STACK_Gateway* getGateway(std::string ipAddress);
  void   sendToMg(std::string ipAddress, std::string message);
  void assocInService(std::string ipAddress);
  UInt32 getNextTransactionId() { return m_transactionId++; };
  UInt32 getEventRequestId()    { return m_eventRequestId++; };
private:


  UInt32 handleParse (std::string message);
  UInt32 handleAddUser();
//std::string const& name,  std::string const& gateway, std::string number);
  UInt32 handleAddGateway();
  UInt32 handleHeartbeatTimeout();

          // std::string const& name,  IP4Address const& addr);



private: // mandatory

  void initStack();
  void parse248(char * mesg);

  private: // optional
  /**
   * Declaration of the member attributes. All those attributes are
   * optional and could be deleted if the implementation doesn't
   * need them.
   */
  bool                        m_enabled;
  UInt32                 m_transactionId;
  UInt32                 m_eventRequestId;
 class H248STACK_Parser* m_parser;
 //class H248STACK_Database* m_database;
  //class H248STACK_StdCallControl* m_callControl;
  //class H248STACK_GatewayControl* m_gatewayControl;
   //class H248STACK_TeamMember* m_teamMember;



private: // inhibit access to not implemented default C++ operators
  H248STACK_Team            (const H248STACK_Team&);  // copy constructor
  H248STACK_Team& operator= (const H248STACK_Team&);  // copy assignment
};


#endif

