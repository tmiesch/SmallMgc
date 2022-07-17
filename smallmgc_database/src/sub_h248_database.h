#ifndef SUBH248DATABASE_INCLUDED
#define SUBH248DATABASE_INCLUDED

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
#include "Poco/Util/Subsystem.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"

#include "Poco/Task.h"
#include "Poco/TaskManager.h"
#include "Poco/NotificationQueue.h"
#include <iostream>


class SUB_H248_Database: public Poco::Util::Subsystem
	/// Sending and Receiving H248 Packets.
{
public:
	SUB_H248_Database();
		/// Creates the H248 receiving thread.

	virtual ~SUB_H248_Database();
		/// Destroys the H248 receiving thread.

	void run();
		/// Does the work.
	void initialize(Poco::Util::Application& self);

	void uninitialize();

        const char * name() const;

	void defineOptions(Poco::Util::OptionSet& options);

	class H248STACK_Gateway* getGateway(std::string ipAddress);
    class H248STACK_Call*    getCallObjectTerm(std::string& ipAddress, std::string& terminationId);
    class H248STACK_Call*    getCallObjectContext(std::string& ipAddress, std::string& context);
	void                     searchIncoming(std::string& ipAddress, H248STACK_Call* call);
	Poco::UInt32             addGateway(std::string ipAddress);
	Poco::UInt32             addSubscriber(std::string ipAddress, std::string terminationId, std::string subscriberNumber);
    const std::string&       getTypeOfCallControl();
	/**
     * Adds a data pair of context id and call object.
     * So that with the context id the correct call object can be found
     *
     * @param string         context  The context id
     * @param H248STACK_Call call     The call object
     * @return                        0 possible to write data
     *                                1 not possible to write data
	 */
     Poco::UInt32 addUsedContext(class H248STACK_Gateway* gateway, std::string& context, class H248STACK_Call* call);

     /**
      * Adds a data pair of termination id and call object.
      * So that with the termination id the correct call object can be found
      *
      * @param string         termination  The termination id
      * @param H248STACK_Call call     The call object
      * @return                        0 possible to write data
      *                                1 not possible to write data
	  */
	Poco::UInt32 addUsedTermination(class H248STACK_Gateway* gateway, std::string& termination, class H248STACK_Call* call);

     /**
      * Removes a data pair of context id and call object.
      * That is normally done after the call is in the release phase
      * For the clean up remove data pair because it is not used anymore
      *
      * @param string         context  The context id
      * @return                        0 possible to write data
      *                                1 not possible to write data
	  */
	Poco::UInt32 removeUsedContext(class H248STACK_Gateway* gateway, std::string& context);

	/**
     * Removes a data pair of termination id and call object.
     * That is normally done after the call is in the release phase
     * For the clean up remove data pair because it is not used anymore
     *
     * @param string         termination  The termination id
     * @return                        0 possible to write data
     *                                1 not possible to write data
	 */
    Poco::UInt32 removeUsedTermination(class H248STACK_Gateway* gateway, std::string& termination);

    class H248STACK_Database* getDatabase() { return m_database; };
protected:
	void handleHelp(const std::string& name, const std::string& value);

	void displayHelp();


private:
	class H248STACK_Database* m_database;
    bool m_helpRequested;

};




#endif // SUBH248DATABASE_INCLUDED
