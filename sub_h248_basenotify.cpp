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

#include "sub_h248_basenotify.h"


SUB_H248_BaseNotify::SUB_H248_BaseNotify(std::string& message): m_message(message)
{

}

SUB_H248_BaseNotify::~SUB_H248_BaseNotify()
{
}

void SUB_H248_BaseNotify::setMessage(std::string& message)
{
  m_message.clear();
  m_message = message;
}
std::string& SUB_H248_BaseNotify::getMessage()
{
  return m_message;
}


SmallMGC_SippNotify::SmallMGC_SippNotify(std::string& option): m_option(option)
{

}

SmallMGC_SippNotify::~SmallMGC_SippNotify()
{
}

void SmallMGC_SippNotify::setOption(std::string& option)
{
  m_option.clear();
  m_option = option;
}

std::string& SmallMGC_SippNotify::getOption()
{
  return m_option;
}

void SmallMGC_SippNotify::setHelp(std::string& help)
{
  m_help.clear();
  m_help = help;
}

std::string& SmallMGC_SippNotify::getHelp()
{
  return m_help;
}

void SmallMGC_SippNotify::setFileName(std::string& name)
{
  m_fileName.clear();
  m_fileName = name;
}

std::string& SmallMGC_SippNotify::getFileName()
{
  return m_fileName;
}


void SmallMGC_SippNotify::setType(Poco::UInt32 type)
{
  m_type = type;
}

Poco::UInt32 SmallMGC_SippNotify::getType()
{
  return m_type;
}


void SmallMGC_SippNotify::setPass(Poco::UInt32 pass)
{
  m_pass = pass;
}

Poco::UInt32 SmallMGC_SippNotify::getPass()
{
  return m_pass;
}


void SmallMGC_SippNotify::setData(void* data)
{
  m_data = data;
}

void* SmallMGC_SippNotify::getData()
{
  return m_data;
}
