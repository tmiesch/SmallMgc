/*
 * Q931_Ies.cpp
 *
 *  Created on: 10.03.2009
 *      Author: thomas
 */

#include "Q931_Ies.h"
#include "Q931_ie.h"



Q931Ies::Q931Ies() : count(0)
{
	// TODO Auto-generated constructor stub
  assigned = 0;
  merged = 0;
  ies.clear();
}
Q931Ies::Q931Ies(Q931Ies& copy)
{
  // TODO:
  assigned = 0;
  merged = 0;
  count = 0;
  for (int i= 0; i < copy.count; i++)
  {
    ies.push_back(copy.ies[i]->copy());
    count++;
  }
}

Q931Ies& Q931Ies::operator=(const Q931Ies& copy)
{
 //TODO:
  if (&copy != this)
  {
    if (count == 0)
    {
      count = 0;
      for (int i= 0; i < copy.count; i++)
      {
        ies.push_back(copy.ies[i]->copy());
        count++;
      }
      assigned++;
    }
    else
      {
        assigned++;
      }
  }
  return *this;
}



Q931Ies::~Q931Ies() {
	// TODO Auto-generated destructor stub
  flush();
  /*for(int i = 0; i < count; i++)
  {
    Q931Ie* ie = ies[i];
    ie->put();
    //delete ie;
  }*/
}

void Q931Ies::flush()
{
  for (int i=0; i<count; i++)
  {
    ies[i]->put();
  }
  ies.clear();
  count = 0;
}

void Q931Ies::destroy()
{

}

void Q931Ies::add(class Q931Ie *ie)
{
  ies.push_back(ie);
  ie->get();
  //ies[count] = ie;
  count++;
}

void Q931Ies::add_put(class Q931Ie *ie)
{
  add(ie);
  ie->put();
}

void Q931Ies::del(class Q931Ie *ie)
{
  int i;
  for (i=0; i<count; i++)
  {
     if (ies[i] == ie)
     {
       int j;
       for (j=i; j<count-1; j++)
          ies[i] = ies[i+1];
       ie->put();
       count--;
       break;
     }
  }
}

void Q931Ies::merge(Q931Ies *src_ies)
{
  if (!src_ies)
    return;
  merged++;
  int i;
  for (i=0; i<src_ies->get_count(); i++)
  {
    if (src_ies->getIe(i)->getId() == Q931_IE_CAUSE)
    {
       Q931IeCause *src_cause = (Q931IeCause*)src_ies->getIe(i);
       int j;
       for (j=0; j<count; j++)
       {
         if (ies[j]->getId() == Q931_IE_CAUSE)
         {
           Q931IeCause *cause = (Q931IeCause*)ies[j];
           if (cause->getValue() == src_cause->getValue())
             goto continue_to_next;
         }
       }
    }
    add(src_ies->getIe(i));

continue_to_next:;
  }

}

void Q931Ies::copy(Q931Ies *src_ies)
{
  int i;
  flush();
  count = 0;
  for (i=0; i<src_ies->get_count(); i++)
  {
    ies.push_back(src_ies->ies[i]->copy());
    count++;
  }
  //  add(src_ies->getIe(i));

}

void Q931Ies::sort()
{

}

int Q931Ies::compare(const void *a, const void *b)
{
  return q931_intcmp((*((Q931Ie **)a))->getId(),
                     (*((Q931Ie **)b))->getId());
}
