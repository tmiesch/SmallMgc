#ifndef SUBH248BASENOTIFY_INCLUDED
#define SUBH248BASENOTIFY_INCLUDED

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
#include "Poco/Notification.h"

class SUB_H248_BaseNotify: public Poco::Notification
{
public:
  SUB_H248_BaseNotify(std::string& message);
  virtual ~SUB_H248_BaseNotify();
  virtual std::string& getMessage();
  virtual void setMessage(std::string& message);

protected:
  std::string m_message;
};

class SmallMGC_SippNotify: public Poco::Notification
{
public:
	SmallMGC_SippNotify(std::string& option);
  virtual ~SmallMGC_SippNotify();
  virtual std::string& getOption();
  virtual void setOption(std::string& option);
  virtual std::string& getHelp();
  virtual void setHelp(std::string& help);
  virtual std::string& getFileName();
  virtual void setFileName(std::string& name);
  virtual Poco::UInt32 getType();
  virtual void setType(Poco::UInt32 type);
  virtual Poco::UInt32 getPass();
  virtual void setPass(Poco::UInt32 pass);
  virtual void* getData();
  virtual void setData(void* data);

protected:
	std::string m_option;
	std::string m_help;
	std::string m_fileName;
	Poco::UInt32 m_type;
	void *m_data;
	/* Pass 0: Help and other options that should exit immediately. */
	/* Pass 1: All other options. */
	/* Pass 2: Scenario parsing. */
	Poco::UInt32 m_pass;
};

#endif // SUBH248BASENOTIFY_INCLUDED
