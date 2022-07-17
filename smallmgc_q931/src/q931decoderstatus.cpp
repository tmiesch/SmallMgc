/*
 * q931decoderstatus.cpp
 *
 *  Created on: 13.04.2009
 *      Author: thomas
 */
#include <limits.h>
#include <stdio.h>
#include "q931decoderstatus.h"
#include "Q931_ie.h"
#include "q931message.h"
#include "q931interface.h"

Q931_DecoderStatus::Q931_DecoderStatus() {
	// TODO Auto-generated constructor stub
  active_codeset = 0;

  curie = 0;

  mandatory_ies.clear();

  missing_mand_ies.clear();
  invalid_mand_ies.clear();
  invalid_opt_ies.clear();
  invalid_access_ies.clear();
  unrecognized_ies.clear();

  mandatory_ies_usage.clear();

}

Q931_DecoderStatus::~Q931_DecoderStatus() {
	// TODO Auto-generated destructor stub
}

__u8 Q931_DecoderStatus::q931DecodeShiftIe(class Q931_Message *msg,	__u8 ie_id)
{
  __u8 codeset;
  if (q931_get_so_ie_type2_value(ie_id) & 0x08)
  {
	// Non-Locking shift
	codeset = q931_get_so_ie_type2_value(ie_id) & 0x07;

	//report_msg_cont(msg, LOG_DEBUG,"<-  Non-Locking shift from codeset %u to codeset %u\n",ds->active_codeset,codeset);
  }
  else
  {
	// Locking shift
	codeset = q931_get_so_ie_type2_value(ie_id) & 0x07;
	//report_msg_cont(msg, LOG_DEBUG,"<-  Locking shift from codeset %u to codeset %u\n",ds->active_codeset,codeset);
    active_codeset = codeset;
  }

  //assert(codeset < ARRAY_SIZE(ds->previous_ie_id));
  return codeset;
}

Q931Ie *Q931_DecoderStatus::q931GetIeClass(__u8 codeset, __u8 id)
{

	int i;
	for (i=0; i<Q931Ie::q931IePrototypes.size(); i++) {
		if(Q931Ie::q931IePrototypes[i]->getCodeset() == codeset &&
				Q931Ie::q931IePrototypes[i]->getId() == id)
		{
			return Q931Ie::q931IePrototypes[i]->copy();
		}
	}
	return NULL;
}

const struct Q931_Ie_Usage *Q931_DecoderStatus::q931_get_ie_usage(enum q931_message_type message_type,__u8 codeset,	enum q931_ie_id ie_id)
{
	int i;
	for (i=0; i<Q931Ie::numberOfIeInUsages(); i++) {
		if(Q931Ie::q931IeUsages[i].codeset == codeset &&
		   Q931Ie::q931IeUsages[i].ie_id == ie_id)
			return &Q931Ie::q931IeUsages[i];
	}

	return NULL;
}



void Q931_DecoderStatus::decodeIe(class Q931_Message *msg,class Q931_Interface *intf,
	__u8 codeset,__u8 ie_id,__u8 *buf, __u8 len, bool is_vl)
{
  Q931Ie *ie_class;
  ie_class = q931GetIeClass(codeset, ie_id);

  if (!ie_class)
  {
    if (q931_ie_comprehension_required(ie_id))
    {
      //q931_ieid_add_to_list(msg, &ds->unrecognized_ies, codeset, ie_id);
      //unrecognized_ies.push_back(ie_class);
    }
    else
      {
      //delete ie_class;
      //ie_class->put();
      }
    return;
  }

  if(!ie_class->readFromBuf(buf, len, intf))
  {
    //q931_ie_has_content_errors(msg, ds, ie_class, ie_usage);
    int i;
    char hexdump[512];
    for (i=0; i<len; i++)
      sprintf(hexdump + i * 2, "%02x", *(buf + i));
    ie_class->put();
    //delete ie_class;
    return;
  }

  const struct Q931_Ie_Usage *ie_usage;
  ie_usage = q931_get_ie_usage(msg->getMessage_type(), codeset, (enum q931_ie_id)ie_id);

  if (!ie_usage)
  {
    if (q931_ie_comprehension_required(ie_id))
    {
      //q931_ieid_add_to_list(msg, &ds->unrecognized_ies,	codeset, ie_id);
      ie_class->get();
      unrecognized_ies.push_back(ie_class);

    }
    else
      //delete ie_class;
      ie_class->put();
    return;
  }

  // Ignore IEs appearing more than "max_occur" times
  if (ie_class->getMax_occur() > 0 && ie_class->getMax_occur() != INT_MAX)
  {
    int i;
    int count = 0;
    for (i=0; i<msg->getIes()->get_count(); i++)
    {
      if (msg->getIes()->getIe(i)->getId() == ie_class->getId() &&
          msg->getIes()->getIe(i)->getCodeset() == ie_class->getCodeset())
	count++;
    }
    if (count > ie_class->getMax_occur())
    {
      //delete ie_class;
      ie_class->put();
      return;
    }
  }

  /*if (!ie_class->alloc ||
	    !ie_class->read_from_buf) {
		if (q931_ie_comprehension_required(ie_id)) {

			q931_ieid_add_to_list(msg, &ds->unrecognized_ies,
				ie_class->codeset, ie_class->id);
		}

		return;
	}*/

  if (q931_check_ie_size(msg, ie_class, len) < 0)
  {
    if (ie_class->getId() == Q931_IE_USER_USER ||
	ie_class->getId() == Q931_IE_LOW_LAYER_COMPATIBILITY ||
	ie_class->getId() == Q931_IE_HIGH_LAYER_COMPATIBILITY ||
	ie_class->getId() == Q931_IE_CALLED_PARTY_SUBADDRESS)
    {
      //q931_ieid_add_to_list(msg, &ds->invalid_access_ies,	ie_class->codeset, ie_class->id);
      ie_class->get();
      invalid_access_ies.push_back(ie_class);
      return;
    }
    else if (ie_class->getId() == Q931_IE_CALL_IDENTITY)
    {
      /* Truncate it */
      len = ie_class->getMax_len();
    }
    else
    {
      //q931_ie_has_content_errors(msg, ds, ie_class, ie_usage);
      //delete ie_class;
      ie_class->put();
      return;
    }
  }

  if (is_vl)
  {
    if (ie_class->getId() < previous_ie_id[ie_class->getCodeset()])
    {
      /* This implementation processes out of sequence IEs but * complains nonetheless
      */
    }
    ie_class->dump("ie:");
    previous_ie_id[ie_class->getCodeset()] = ie_class->getId();
  }
  msg->getIes()->add_put(ie_class);
  return;
}

int Q931_DecoderStatus::q931_check_ie_size(Q931_Message *msg, Q931Ie *ie_class, int ie_len)
{
	if (ie_class->getMax_len() != INT_MAX &&
	    ie_len > ie_class->getMax_len()) {

		return -1;
	}

	return 0;
}



void Q931_DecoderStatus::decodeIes(class Q931_Message *msg, class Q931_Interface* interface)
{
  /* Fill the list of mandatory IEs for this message type */
  curie = 0;
  int i;
  for (i=0; i<Q931Ie::numberOfIeInUsages(); i++)
  {
	if (Q931Ie::q931IeUsages[i].message_type == msg->getMessage_type())
	{
          if (Q931Ie::q931IeUsages[i].presence != Q931_IE_MANDATORY)
            continue;
          mandatory_ies_usage.push_back(&Q931Ie::q931IeUsages[i]);
	  //q931_ieid_add_to_list(msg, &ds->mandatory_ies, q931_ie_usages[i].codeset,	q931_ie_usages[i].ie_id);
	}
  }

  if (msg->getMessage_type() == Q931_MT_SETUP &&  interface->getRole() == LAPD_INTF_ROLE_TE)
  {
     /* Channel ID is mandatory in SETUP n->u */
     for(i=0; i<Q931Ie::numberOfIeInUsages(); i++)
     {
       if (Q931Ie::q931IeUsages[i].message_type == Q931_MT_SETUP)
       {
         if (Q931Ie::q931IeUsages[i].ie_id != Q931_IE_CHANNEL_IDENTIFICATION)
         {
            mandatory_ies_usage.push_back(&Q931Ie::q931IeUsages[i]);
            //q931_ieid_add_to_list(msg,&ds->mandatory_ies,    0, Q931_IE_CHANNEL_IDENTIFICATION);
         }
       }
     }

  }

  int rawies_curpos = 0;

	/* Go through the IEs buffer and decode each of them */
  __u8 codeset = active_codeset;
  while(rawies_curpos < msg->getRawies_len())
  {
    __u8 ie_id = *(__u8 *)(msg->getRawies() + rawies_curpos);
    curie++;
    if (q931_is_so_ie(ie_id))
	{
	  if (q931_get_so_ie_id(ie_id) == Q931_IE_SHIFT)
	  {
	    codeset = q931DecodeShiftIe(msg, ie_id);
            rawies_curpos++;
            continue;
	  }
          //decodee(msg, ds, intf, codeset, q931_get_so_ie_id(ie_id),msg->rawies + rawies_curpos, 1,false);
          decodeIe(msg, interface, codeset, q931_get_so_ie_id(ie_id),msg->getRawies() + rawies_curpos, 1,false);

          rawies_curpos++;
	}
	else
	{
	  __u8 ie_len = *(__u8 *)(msg->getRawies() + rawies_curpos + 1);
	  ///q931_decode_ie(msg, ds, intf, codeset, ie_id,msg->rawies + rawies_curpos + 2,ie_len,true);
         decodeIe(msg, interface, codeset, ie_id,msg->getRawies() + rawies_curpos+2, ie_len,true);

	  rawies_curpos = rawies_curpos + ie_len + 2;
	  if(rawies_curpos > msg->getRawies_len())
	  {
            //report_msg(msg, LOG_ERR,	"<-  MALFORMED FRAME\n");
            break;
	  }
	}

    /* Remove IE from mandatory IEs */
    std::list<struct Q931_Ie_Usage*>::iterator iter;
    for (iter = mandatory_ies_usage.begin(); iter != mandatory_ies_usage.end(); iter++)
    {
	if (((*iter)->codeset == codeset) && ((*iter)->ie_id == ie_id))
	{
	  mandatory_ies_usage.remove(*iter);
	  break;
	}
        /*struct q931_ieid_entry *entry, *tpos;
	list_for_each_entry_safe(entry, tpos, &ds->mandatory_ies,node) {
	  if (entry->codeset == codeset && entry->id == ie_id)
	  {
		list_del(&entry->node);
		free(entry);
                break;
	  }
	}*/
     }
     codeset = active_codeset;
  }

}

void Q931_DecoderStatus::setDiagnosticsMandatoryIe(class Q931IeCause* ie)
{
  ie->setDiagnostics_len();
  int max_len = ie->getDiagnostics_len();
  int diagnostics_len = 0;

  std::list<class Q931Ie*>::iterator iter;
  for (iter = mandatory_ies.begin(); iter != mandatory_ies.end(); iter++)
  {
    if (diagnostics_len >= max_len)
      return;
    if((*iter)->getCodeset() != 0)
    {
      ie->getDiagnostics()[diagnostics_len] =  Q931_IE_SHIFT | 0x08 | (*iter)->getCodeset();
      diagnostics_len++;
    }
    if (diagnostics_len >= max_len)
      return;
    ie->getDiagnostics()[diagnostics_len] = (*iter)->getId();

    diagnostics_len++;

  }
}

void Q931_DecoderStatus::setDiagnosticsInvalidMandatoryIe(class Q931IeCause* ie)
{
  ie->setDiagnostics_len();
  int max_len = ie->getDiagnostics_len();
  int diagnostics_len = 0;

  std::list<class Q931Ie*>::iterator iter;
  for (iter = invalid_mand_ies.begin(); iter != invalid_mand_ies.end();  iter++)
  {
    if (diagnostics_len >= max_len)
      return;
    if((*iter)->getCodeset() != 0)
    {
      ie->getDiagnostics()[diagnostics_len] =  Q931_IE_SHIFT | 0x08 | (*iter)->getCodeset();
      diagnostics_len++;
    }
    if (diagnostics_len >= max_len)
      return;
    ie->getDiagnostics()[diagnostics_len] = (*iter)->getId();

    diagnostics_len++;
  }
}

void Q931_DecoderStatus::setDiagnosticsInvalidOptionalIe(class Q931IeCause* ie)
{
	ie->setDiagnostics_len();
	int max_len = ie->getDiagnostics_len();

	int diagnostics_len = 0;

	std::list<class Q931Ie*>::iterator iter;
    for (iter = invalid_opt_ies.begin();
        iter != invalid_opt_ies.end();
        iter++)
    {
    	if (diagnostics_len >= max_len)
    		return;
        if((*iter)->getCodeset() != 0)
        {
           	ie->getDiagnostics()[diagnostics_len] =  Q931_IE_SHIFT | 0x08 | (*iter)->getCodeset();
           	diagnostics_len++;
        }
        if (diagnostics_len >= max_len)
   			return;
       	ie->getDiagnostics()[diagnostics_len] = (*iter)->getId();

   		diagnostics_len++;

    }
}

void Q931_DecoderStatus::setDiagnosticsInvalidAccessIe(class Q931IeCause* ie)
{
	ie->setDiagnostics_len();
	int max_len = ie->getDiagnostics_len();

	int diagnostics_len = 0;

	std::list<class Q931Ie*>::iterator iter;
    for (iter = invalid_access_ies.begin();
        iter != invalid_access_ies.end();
        iter++)
    {
    	if (diagnostics_len >= max_len)
    		return;
        if((*iter)->getCodeset() != 0)
        {
           	ie->getDiagnostics()[diagnostics_len] =  Q931_IE_SHIFT | 0x08 | (*iter)->getCodeset();
           	diagnostics_len++;
        }
        if (diagnostics_len >= max_len)
   			return;
       	ie->getDiagnostics()[diagnostics_len] = (*iter)->getId();

   		diagnostics_len++;

    }
}

void Q931_DecoderStatus::setDiagnosticsUnrecognizedIe(class Q931IeCause* ie)
{
	ie->setDiagnostics_len();
	int max_len = ie->getDiagnostics_len();

	int diagnostics_len = 0;

	std::list<class Q931Ie*>::iterator iter;
    for (iter = unrecognized_ies.begin();
        iter != unrecognized_ies.end();
        iter++)
    {
    	if (diagnostics_len >= max_len)
    		return;
        if((*iter)->getCodeset() != 0)
        {
           	ie->getDiagnostics()[diagnostics_len] =  Q931_IE_SHIFT | 0x08 | (*iter)->getCodeset();
           	diagnostics_len++;
        }
        if (diagnostics_len >= max_len)
   			return;
       	ie->getDiagnostics()[diagnostics_len] = (*iter)->getId();

   		diagnostics_len++;

    }
}


void Q931_DecoderStatus::addMandatoryIe(class Q931Ie* ie)
{
	mandatory_ies.push_back(ie);
}

void Q931_DecoderStatus::addMissingMandatoryIe(class Q931Ie* ie)
{
	missing_mand_ies.push_back(ie);
}

void Q931_DecoderStatus::addInvalidMandatoryIe(class Q931Ie* ie)
{
	invalid_mand_ies.push_back(ie);
}

void Q931_DecoderStatus::addInvalidOptionalIe(class Q931Ie* ie)
{
	invalid_opt_ies.push_back(ie);
}

void Q931_DecoderStatus::addInvalidAccessIe(class Q931Ie* ie)
{
	invalid_access_ies.push_back(ie);
}
void Q931_DecoderStatus::addUnrecognizedIe(class Q931Ie* ie)
{
	unrecognized_ies.push_back(ie);
}

int Q931_DecoderStatus::countMandatoryIe()
{
	return mandatory_ies.size();
}
int Q931_DecoderStatus::countMissingMandatoryIe()
{
	return missing_mand_ies.size();
}
int Q931_DecoderStatus::countInvalidMandatoryIe()
{
	return invalid_mand_ies.size();
}
int Q931_DecoderStatus::countInvalidOptionalIe()
{
	return invalid_opt_ies.size();
}
int Q931_DecoderStatus::countInvalidAccessIe()
{
	return invalid_access_ies.size();
}
int Q931_DecoderStatus::countUnrecognizedIe()
{
	return unrecognized_ies.size();
}
