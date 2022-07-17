/*
 * q931message.cpp
 *
 *  Created on: 08.04.2009
 *      Author: thomas
 */

#include "q931message.h"
#include "Q931_Ies.h"
#include "Q931_ie.h"

Q931_Message::Q931_Message(class Q931_DataLink* dlc): m_dlc(dlc), refcnt(1) {
	// TODO Auto-generated constructor stub
	ies = new Q931Ies();
	rawlen = 0;
	//__u8 raw_message_type;
	//enum q931_message_type message_type;
	callref = 0;
	callref_len = 1;
	// enum q931_callref_flag callref_direction;
        rawies = 0;
	rawies_len = 0;
}

Q931_Message::~Q931_Message() {
	// TODO Auto-generated destructor stub
        delete ies;
}

int Q931_Message::writeIes (class Q931Ies* user_ies)
{
  Q931Ies ies(*user_ies);
  ies.sort();

  int i;
  for (i=0; i<ies.get_count(); i++)
  {
    //assert(ies.ies[i]->cls->write_to_buf);
    if (ies.getIe(i)->getType() == Q931_IE_TYPE_VL)
    {
      /* Write container */
      *(__u8 *)(raw + rawlen) =  ies.getIe(i)->getId();
      rawlen++;
      /* Write payload before len */
      int ie_len = ies.getIe(i)->writeToBuf(raw + rawlen + 1, sizeof(raw) - rawlen);

      /* Now write len */
      *(__u8 *)(raw + rawlen) = ie_len;
      rawlen++;

      /* Finally jump len */
      rawlen += ie_len;

      // report_msg_cont(msg, LOG_DEBUG, "->  VL IE %d ===> %u (%s) --" " length %u\n",  i, ies.ies[i]->cls->id, ies.ies[i]->cls->name, ie_len);
    }
    else
    {
     // *(__u8 *)(raw + rawlen) =  ies.getIe(i)->getId();
      //rawlen++;
      int res = ies.getIe(i)->writeToBuf(raw + rawlen, sizeof(raw) - rawlen);
      //*(__u8 *)(raw + rawlen) = res;
      rawlen++;
      //assert(res > 0);
      //rawlen++;
      /* Finally jump len */
     // rawlen += res;
      //report_msg_cont(msg, LOG_DEBUG, "->  SO IE %d ===> %u (%s)\n", i, ies.ies[i]->cls->id, ies.ies[i]->cls->name);
    }
    /*if (ies.ies[i]->cls->dump)
      ies.ies[i]->cls->dump(ies.ies[i], q931_report, "->    ");
    */
  }
}

