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

#ifndef SMALLMGCISDNCCFSM_H_
#define SMALLMGCISDNCCFSM_H_

#include "smallmgcccfsm.h"

class SmallMGC_IsdnCC_Fsm : public SmallMGC_CC_Fsm
{
public:
  SmallMGC_IsdnCC_Fsm(class H248STACK_Database* database, class H248STACK_CallAdapter* adapter):
      SmallMGC_CC_Fsm(database, adapter)
      {

      };


  SmallMGC_IsdnCC_Fsm();
  virtual
  ~SmallMGC_IsdnCC_Fsm();
};

#endif /* SMALLMGCIsdnCCFSM_H_ */
