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

#include "h248stack_message.h"


H248STACK_Message::H248STACK_Message()
{

}

std::string H248STACK_Message::get_version() const
{
  return version_;
}

std::string H248STACK_Message::get_address() const
{
  return address_;
}

std::string H248STACK_Message::get_port() const
{
  return port_;
}

std::string H248STACK_Message::get_transactionId() const
{
  return transaction_id_;
}

std::string H248STACK_Message::get_contextId() const
{
  return context_id_;
}

std::string H248STACK_Message::get_terminationId() const
{
  return termination_id_;
}

std::string H248STACK_Message::get_transaction() const
{
  return transaction_;
}

void H248STACK_Message::set_version(std::string version)
{
    version_ = version;
}

void H248STACK_Message::set_address(std::string address)
{
    address_ = address;
}

void H248STACK_Message::set_port(std::string port)
{
    port_ = port;
}

void H248STACK_Message::set_transactionId(std::string transactionId)
{
    transaction_id_ = transactionId;
}

void H248STACK_Message::set_contextId(std::string contextId)
{
    context_id_ = contextId;
}

void H248STACK_Message::set_terminationId(std::string terminationId)
{
    termination_id_ = terminationId;
}

void H248STACK_Message::set_transaction(std::string transaction)
{
    transaction_ = transaction;
}

