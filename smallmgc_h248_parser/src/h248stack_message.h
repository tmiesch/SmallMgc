#ifndef MESSAGE_H
#define MESSAGE_H

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

#include <vector>
#include <string>

class H248STACK_Message
{

 public:
  H248STACK_Message();

  std::string get_version() const;
  std::string get_address() const;
  std::string get_port() const;
  std::string get_transactionId() const;
  std::string get_contextId() const;
  std::string get_terminationId() const;
  std::string get_transaction() const;
  void set_version(std::string version);
  void set_address(std::string address);
  void set_port(std::string port);
  void set_transactionId(std::string transactionId);
  void set_contextId(std::string contextId);
  void set_terminationId(std::string terminationId);
  void set_transaction(std::string transaction);

 private:
  std::string version_;
  std::string address_;
  std::string port_;
  std::string transaction_id_;
  std::string context_id_;
  std::string termination_id_;
  std::string transaction_;
};

#endif
