#ifndef SUBSDPPARSER_INCLUDED
#define SUBSDPPARSER_INCLUDED

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

class SUB_SDP_Parser: public Poco::Util::Subsystem
	/// Sending and Receiving H248 Packets.
{
public:
	SUB_SDP_Parser();
		/// Creates the SIP receiving thread.

	virtual ~SUB_SDP_Parser();
		/// Destroys the SIP receiving thread.

	void run();
		/// Does the work.
	void initialize(Poco::Util::Application& self);

	void uninitialize();

        const char * name() const;

	void defineOptions(Poco::Util::OptionSet& options);
        virtual void switchToTestMode();
	virtual void switchToNormalMode();

	virtual void parser(std::string& message);
	virtual Poco::UInt32 parseSDP(char* mesg);
	void initStack();

protected:
	void handleHelp(const std::string& name, const std::string& value);

	void displayHelp();


private:
	bool         m_stop;
        bool m_helpRequested;
        class SDPSTACK_Parser* m_parser;

};




#endif // SUBSDPBASENOTIFY_INCLUDED
