#ifndef SUBH248CLISTIMULIS_INCLUDED
#define SUBH248CLISTIMULIS_INCLUDED

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
#include <list>


class H248_CLI_Stimuli
{
public:
  H248_CLI_Stimuli(std::string command, class H248_CLI_Stimuli* parent, class SUB_H248_Database* database);
  virtual ~H248_CLI_Stimuli();
  virtual void doAction(char *argv[]);
  virtual void help(char *argv[]);

protected:
  virtual void addChild(class H248_CLI_Stimuli* child);

  class SUB_H248_Database* m_database;
  std::string              m_command;
  std::list<class H248_CLI_Stimuli*>  m_childs;

};


class H248_CLI_Stimuli_Root : public H248_CLI_Stimuli
{
public:
  H248_CLI_Stimuli_Root(std::string command, class H248_CLI_Stimuli* parent, class SUB_H248_Database* database);
  virtual ~H248_CLI_Stimuli_Root();
  virtual void doAction(char *argv[]);
  virtual void help(char *argv[]);

private:


};

class H248_CLI_Stimuli_Database : public H248_CLI_Stimuli
{
public:
  H248_CLI_Stimuli_Database(std::string command, class H248_CLI_Stimuli* parent, class SUB_H248_Database* database);
  virtual ~H248_CLI_Stimuli_Database();
  virtual void doAction(char *argv[]);
  virtual void help(char *argv[]);

private:


};


class H248_CLI_Stimuli_Db_Gateways: public H248_CLI_Stimuli
{
public:
  H248_CLI_Stimuli_Db_Gateways(std::string command, class H248_CLI_Stimuli* parent, class SUB_H248_Database* database);
  virtual ~H248_CLI_Stimuli_Db_Gateways();
  virtual void doAction(char *argv[]);
  virtual void help(char *argv[]);

private:
};

class H248_CLI_Stimuli_QueueInfo : public H248_CLI_Stimuli
{
public:
  H248_CLI_Stimuli_QueueInfo(std::string command, class H248_CLI_Stimuli* parent, class SUB_H248_Database* database, class SmallMgc* app);
  virtual ~H248_CLI_Stimuli_QueueInfo();
  virtual void doAction(char *argv[]);
  virtual void help(char *argv[]);

private:
  class SmallMgc* m_app;


};


class H248_CLI_Stimuli_SendMessages : public H248_CLI_Stimuli
{
public:
  H248_CLI_Stimuli_SendMessages(std::string command, class H248_CLI_Stimuli* parent, class SUB_H248_Database* database, class SmallMgc* app);
  virtual ~H248_CLI_Stimuli_SendMessages();
  virtual void doAction(char *argv[]);
  virtual void help(char *argv[]);

private:
  class SmallMgc* m_app;


};

class H248_CLI_Stimuli_SendMessagesH : public H248_CLI_Stimuli
{
public:
  H248_CLI_Stimuli_SendMessagesH(std::string command, class H248_CLI_Stimuli* parent, class SUB_H248_Database* database, class SmallMgc* app);
  virtual ~H248_CLI_Stimuli_SendMessagesH();
  virtual void doAction(char *argv[]);
  virtual void help(char *argv[]);

private:
  class SmallMgc* m_app;


};

class H248_CLI_Stimuli_SendMessagesHR : public H248_CLI_Stimuli
{
public:
  H248_CLI_Stimuli_SendMessagesHR(std::string command, class H248_CLI_Stimuli* parent, class SUB_H248_Database* database, class SmallMgc* app);
  virtual ~H248_CLI_Stimuli_SendMessagesHR();
  virtual void doAction(char *argv[]);
  virtual void help(char *argv[]);

private:
  class SmallMgc* m_app;


};

class H248_CLI_Stimuli_SendMessagesER : public H248_CLI_Stimuli
{
public:
  H248_CLI_Stimuli_SendMessagesER(std::string command, class H248_CLI_Stimuli* parent, class SUB_H248_Database* database, class SmallMgc* app);
  virtual ~H248_CLI_Stimuli_SendMessagesER();
  virtual void doAction(char *argv[]);
  virtual void help(char *argv[]);

private:
  class SmallMgc* m_app;


};


class H248_CLI_Stimuli_SendIUAMessages : public H248_CLI_Stimuli
{
public:
  H248_CLI_Stimuli_SendIUAMessages(std::string command, class H248_CLI_Stimuli* parent, class SUB_H248_Database* database, class SmallMgc* app);
  virtual ~H248_CLI_Stimuli_SendIUAMessages();
  virtual void doAction(char *argv[]);
  virtual void help(char *argv[]);

private:
  class SmallMgc* m_app;


};

class H248_CLI_Stimuli_SendIUAQ931Messages : public H248_CLI_Stimuli
{
public:
  H248_CLI_Stimuli_SendIUAQ931Messages(std::string command, class H248_CLI_Stimuli* parent, class SUB_H248_Database* database, class SmallMgc* app);
  virtual ~H248_CLI_Stimuli_SendIUAQ931Messages();
  virtual void doAction(char *argv[]);
  virtual void help(char *argv[]);

private:
  class SmallMgc* m_app;


};

class H248_CLI_Stimuli_SendMessagesSip : public H248_CLI_Stimuli
{
public:
  H248_CLI_Stimuli_SendMessagesSip(std::string command, class H248_CLI_Stimuli* parent, class SUB_H248_Database* database, class SmallMgc* app);
  virtual ~H248_CLI_Stimuli_SendMessagesSip();
  virtual void doAction(char *argv[]);
  virtual void help(char *argv[]);

private:
  class SmallMgc* m_app;


};

class H248_CLI_Stimuli_SendMegacoTestMessages : public H248_CLI_Stimuli
{
public:
  H248_CLI_Stimuli_SendMegacoTestMessages(std::string command, class H248_CLI_Stimuli* parent, class SUB_H248_Database* database, class SmallMgc* app);
  virtual ~H248_CLI_Stimuli_SendMegacoTestMessages();
  virtual void doAction(char *argv[]);
  virtual void help(char *argv[]);

private:
  class SmallMgc* m_app;
  std::string m_ipAddress;
  std::string m_contextId;
  std::string m_terminationId;


};

class H248_CLI_Stimuli_SendMessagesSdp : public H248_CLI_Stimuli
{
public:
  H248_CLI_Stimuli_SendMessagesSdp(std::string command, class H248_CLI_Stimuli* parent, class SUB_H248_Database* database, class SmallMgc* app);
  virtual ~H248_CLI_Stimuli_SendMessagesSdp();
  virtual void doAction(char *argv[]);
  virtual void help(char *argv[]);

private:
  class SmallMgc* m_app;


};

#endif // SUBH248CLISTIMULIS_INCLUDED
