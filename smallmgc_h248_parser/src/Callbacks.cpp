#include "Apg.h"
#include "MEGACO.h"
#include <stdio.h>
#include "h248stack_parser.h"

// dummy H248 class
/*class MegacoMessage
{
  public:
  static char at(int){return 0;}
};

class H248STACK_Parser
{
  public:
  static void writeCommand(){}
  static void createPending(){}
  static void createError(){}
  static void setErrorDescriptor(ulong ulOffset, ulong ulLen){}
  static void setTransactionId(ulong ulOffset, ulong ulLen){}
  static void setContextId(ulong ulOffset, ulong ulLen){}
  static void setIPAddress(ulong ulOffset, ulong ulLen){}
  static void setIPPort(ulong ulOffset, ulong ulLen){}
  static void setTerminationId(ulong ulOffset, ulong ulLen){}
  static void setLocalDescriptor(ulong ulOffset, ulong ulLen){}
  static void setRemoteDescriptor(ulong ulOffset, ulong ulLen){}
  static void setObservedEvent(ulong ulOffset, ulong ulLen){}
  static void setObservedParameter(ulong ulOffset, ulong ulLen){}
  static void setEventId(ulong ulOffset, ulong ulLen){}
  static void setSvcMethod(ulong ulOffset, ulong ulLen){}
  static void setSvcReason(ulong ulOffset, ulong ulLen){}
  static void setVersion(ulong ulOffset, ulong ulLen){}
  static void createAdd(){}
  static void createNotify(){}
  static void createAuditValue(){}
  static ulong isEvent(ulong ulOffset, ulong ulLen){return 0;}
  static void createModify(){}
  static void setTransactionType(ulong ulOffset, ulong ulLen){}
  static void createServiceChange(){}
  static void createSubtract(ulong ulOffset, ulong ulLen){}

  static MegacoMessage megacoMesg;
//megacoMesg.at(i)
};

MegacoMessage H248STACK_Parser::megacoMesg;
*/
// call back function for rule "NotifyToken"
static ulong pfn_NotifyToken(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_NotifyToken
if (SEM_POST  == ulState)
 {
    //Application::instance().logger().trace("notify token\n");
    int i = ulOffset+1;
    char c = H248STACK_Parser::megacoMesg.at(i);
    if ((ulLen == 1) && ((c == ' ') || (c == '=')))
      H248STACK_Parser::createNotify();
    else
    {
        if (ulLen == 6)
        {
          H248STACK_Parser::createNotify();
        }
        else
         printf(" Notify character %c", c);
    }

 }

//}}CallBack.pfn_NotifyToken
  return ulReturn;
}

// call back function for rule "megacoMessage"
static ulong pfn_megacoMessage(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_megacoMessage
  if (SEM_POST  == ulState)
 {
  // Application::instance().logger().trace("megaco message parsed \n");
   H248STACK_Parser::writeCommand();
}

//}}CallBack.pfn_megacoMessage
  return ulReturn;
}

// call back function for rule "megacoMid"
static ulong pfn_megacoMid(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_megacoMessage
  if (SEM_POST  == ulState)
 {
  // Application::instance().logger().trace("megaco message parsed \n");
  H248STACK_Parser::setMid(ulOffset, ulLen);
}

//}}CallBack.pfn_megacoMid
  return ulReturn;
}

// call back function for rule "deviceName"
static ulong pfn_deviceName(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_megacoMessage
  if (SEM_POST  == ulState)
 {
  // Application::instance().logger().trace("megaco message parsed \n");
  H248STACK_Parser::setMgName(ulOffset, ulLen);
}

//}}CallBack.pfn_deviceName
  return ulReturn;
}

// call back function for rule "transactionRequest"
static ulong pfn_transactionRequest(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_transactionRequest
if (SEM_POST  == ulState)
 {
//  H248STACK_Parser::setTransactionType(ulOffset, ulLen);
  //Application::instance().logger().trace("this a request interpreted\n");
}
  else
  { //Application::instance().logger().trace("this is a request\n");
  }
//}}CallBack.pfn_transactionRequest
  return ulReturn;
}

// call back function for rule "transactionPending"
static ulong pfn_transactionPending(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_transactionPending
if (SEM_POST  == ulState)
 {
  H248STACK_Parser::createPending();
  //Application::instance().logger().trace("this a request interpreted\n");
}
  else
  { //Application::instance().logger().trace("this is a request\n");
  }


//}}CallBack.pfn_transactionPending
  return ulReturn;
}

// call back function for rule "serviceChangeRequest"
static ulong pfn_serviceChangeRequest(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_serviceChangeRequest
 if (SEM_POST  == ulState)
 {
  //Application::instance().logger().trace("this is a service change\n");
  //Application::instance().logger().trace("Data: %s", (char*)vpData);
 }
//}}CallBack.pfn_serviceChangeRequest
  return ulReturn;
}

// call back function for rule "errorDescriptor"
static ulong pfn_errorDescriptor(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
  if (SEM_POST  == ulState)

  {
    H248STACK_Parser::createError();
    H248STACK_Parser::setErrorDescriptor(ulOffset, ulLen);
    printf("error descriptor\n");
  }


//{{CallBack.pfn_errorDescriptor
//}}CallBack.pfn_errorDescriptor
  return ulReturn;
}

// call back function for rule "ErrorCode"
static ulong pfn_ErrorCode(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;

//{{CallBack.pfn_ErrorCode
  if (SEM_POST  == ulState)
  {
    printf("error code\n");
  }
//}}CallBack.pfn_ErrorCode
  return ulReturn;
}

// call back function for rule "TransactionID"
static ulong pfn_TransactionID(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_TransactionID
if (SEM_POST  == ulState)
 {
  H248STACK_Parser::setTransactionId(ulOffset, ulLen);
  //Application::instance().logger().trace("this is the transaction id\n");
}
//Application::instance().logger().trace("this is the transaction id\n");
 // Application::instance().logger().trace("Data: %d", *((int*)vpData));
//
//}}CallBack.pfn_TransactionID
  return ulReturn;
}

// call back function for rule "ContextID"
static ulong pfn_ContextID(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_ContextID
  if (SEM_POST  == ulState)
 {
  H248STACK_Parser::setContextId(ulOffset, ulLen);
  //Application::instance().logger().trace("this is the context id\n");
}

//}}CallBack.pfn_ContextID
  return ulReturn;
}

// call back function for rule "domainAddress"
static ulong pfn_domainAddress(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_domainAddress
  if (SEM_POST  == ulState)
 {
  H248STACK_Parser::setIPAddress(ulOffset, ulLen);
  //Application::instance().logger().trace("this is the ip address \n");
}
//  Application::instance().logger().trace("this is the domain address\n");
//  Application::instance().logger().trace("Data: %s\n", (char*)vpData);
//
//}}CallBack.pfn_domainAddress
  return ulReturn;
}

// call back function for rule "portNumber"
static ulong pfn_portNumber(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_portNumber
if (SEM_POST  == ulState)
 {
  H248STACK_Parser::setIPPort(ulOffset, ulLen);
  //Application::instance().logger().trace("this is the ip port \n");
}

// if(ulState == SYN_PRE)
  //{
  // APG_BSTR* spSrc = (APG_BSTR*)vpData;
  //  char* cpToken = (char*)spSrc->ucpStr + ulOffset;
  //  ulong ulIndex;
 //   ulong ulLength = spSrc->ulLen - ulOffset;
    //Application::instance().logger().trace("this is the port number\n");
    //Application::instance().logger().trace("Data: %s\n", (char*)vpData);
   //}
//
//}}CallBack.pfn_portNumber
  return ulReturn;
}

// call back function for rule "TerminationID"
static ulong pfn_TerminationID(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_TerminationID
 if (SEM_POST  == ulState)
 {
  H248STACK_Parser::setTerminationId(ulOffset, ulLen);
  //Application::instance().logger().trace("this is the termination id\n");
  //Application::instance().logger().trace("Data: %s", (char*)vpData);
 }

//}}CallBack.pfn_TerminationID
  return ulReturn;
}

// call back function for rule "localDescriptor"
static ulong pfn_localDescriptor(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_localDescriptor
  if (SEM_POST  == ulState)
 {
  H248STACK_Parser::setLocalDescriptor(ulOffset, ulLen);
  //Application::instance().logger().trace("this is the local parameter\n");
}

//}}CallBack.pfn_localDescriptor
  return ulReturn;
}

// call back function for rule "remoteDescriptor"
static ulong pfn_remoteDescriptor(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_remoteDescriptor
  if (SEM_POST  == ulState)
 {
  H248STACK_Parser::setRemoteDescriptor(ulOffset, ulLen);
// Application::instance().logger().trace("this is the remote_ parameter\n");
}

//}}CallBack.pfn_remoteDescriptor
  return ulReturn;
}

// call back function for rule "pkgdName"
static ulong pfn_pkgdName(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_pkgdName
    if (SEM_POST  == ulState)
 {
  //H248STACK_Parser::setObservedEvent(ulOffset, ulLen);
  //Application::instance().logger().trace("this is the observed event\n");
}

//}}CallBack.pfn_pkgdName
  return ulReturn;
}

// call back function for rule "observedEvent"
static ulong pfn_observedEvent(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_observedEvent
  if (SEM_POST  == ulState)
  {
    H248STACK_Parser::setObservedEvent(ulOffset, ulLen);
    //Application::instance().logger().trace("this is the observed event\n");
  }
//}}CallBack.pfn_observedEvent
  return ulReturn;
}

// call back function for rule "observedEventParameter"
static ulong pfn_observedEventParameter(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_observedEventParameter
        if (SEM_POST  == ulState)
 {
  H248STACK_Parser::setObservedParameter(ulOffset, ulLen);
  //Application::instance().logger().trace("this is the observed event parameter\n");
}

//}}CallBack.pfn_observedEventParameter
  return ulReturn;
}

// call back function for rule "RequestID"
static ulong pfn_RequestID(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_RequestID
      if (SEM_POST  == ulState)
 {
  H248STACK_Parser::setEventId(ulOffset, ulLen);
  //Application::instance().logger().trace("this is the observed event\n");
}

//}}CallBack.pfn_RequestID
  return ulReturn;
}

// call back function for rule "serviceChangeMethod"
static ulong pfn_serviceChangeMethod(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_serviceChangeMethod
  if (SEM_POST  == ulState)
   {
    H248STACK_Parser::setSvcMethod(ulOffset, ulLen);
    //Application::instance().logger().trace("this is the service change reason id\n");
  }
//}}CallBack.pfn_serviceChangeMethod
  return ulReturn;
}

// call back function for rule "serviceChangeReason"
static ulong pfn_serviceChangeReason(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_serviceChangeReason
if (SEM_POST  == ulState)
 {
  H248STACK_Parser::setSvcReason(ulOffset, ulLen);
  //Application::instance().logger().trace("this is the service change reason id\n");
}

//}}CallBack.pfn_serviceChangeReason
  return ulReturn;
}

// call back function for rule "Version"
static ulong pfn_Version(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_Version
  if (SEM_POST  == ulState)
 {
  H248STACK_Parser::setVersion(ulOffset, ulLen);
  //Application::instance().logger().trace("this is the version\n");
}

//}}CallBack.pfn_Version
  return ulReturn;
}

// call back function for rule "AddToken"
static ulong pfn_AddToken(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_AddToken
  if (SEM_POST  == ulState)
 {
    //Application::instance().logger().trace("add token\n");
    H248STACK_Parser::createAdd();
 }
//}}CallBack.pfn_AddToken
  return ulReturn;
}

// call back function for rule "AuditValueToken"
static ulong pfn_AuditValueToken(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_AuditValueToken
if (SEM_POST  == ulState)
 {
    //Application::instance().logger().trace("audit token\n");
    H248STACK_Parser::createAuditValue();
 }
//}}CallBack.pfn_AuditValueToken
  return ulReturn;
}

// call back function for rule "ErrorToken"
static ulong pfn_ErrorToken(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_ErrorToken
    if (SEM_POST  == ulState)

  {
    printf("error token\n");
  }
//}}CallBack.pfn_ErrorToken
  return ulReturn;
}

// call back function for rule "EventsToken"
static ulong pfn_EventsToken(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_EventsToken
if (SEM_POST  == ulState)
 {
   if (H248STACK_Parser::isEvent(ulOffset, ulLen) != 0)
   {
     ulReturn = PFN_SKIP;
   }

 }
//}}CallBack.pfn_EventsToken
  return ulReturn;
}

// call back function for rule "ModifyToken"
static ulong pfn_ModifyToken(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_ModifyToken
  if (SEM_POST  == ulState)
 {
    //Application::instance().logger().trace("modify token\n");
    H248STACK_Parser::createModify();
 }

//}}CallBack.pfn_ModifyToken
  return ulReturn;
}

// call back function for rule "ReplyToken"
static ulong pfn_ReplyToken(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_ReplyToken
 if (SEM_POST  == ulState)
 {
  H248STACK_Parser::setTransactionType(ulOffset, ulLen);
  //Application::instance().logger().trace("this a request interpreted\n");
}
//}}CallBack.pfn_ReplyToken
  return ulReturn;
}

// call back function for rule "ServiceChangeToken"
static ulong pfn_ServiceChangeToken(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_ServiceChangeToken
   if (SEM_POST  == ulState)
 {
    //Application::instance().logger().trace("service change token\n");
    H248STACK_Parser::createServiceChange();
 }

//}}CallBack.pfn_ServiceChangeToken
  return ulReturn;
}

// call back function for rule "StatsToken"
static ulong pfn_StatsToken(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_StatsToken
 if (SEM_POST  == ulState)
 {
    //Application::instance().logger().trace("statistics token\n");
 }
//}}CallBack.pfn_StatsToken
  return ulReturn;
}

// call back function for rule "SubtractToken"
static ulong pfn_SubtractToken(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_SubtractToken
  if (SEM_POST  == ulState)
 {
    //Application::instance().logger().trace("subtract token\n");
    H248STACK_Parser::createSubtract(ulOffset, ulLen);
 }


//}}CallBack.pfn_SubtractToken
  return ulReturn;
}

// call back function for rule "TransToken"
static ulong pfn_TransToken(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_TransToken
  if (SEM_POST  == ulState)
 {
  H248STACK_Parser::setTransactionType(ulOffset, ulLen);
  //Application::instance().logger().trace("this a request interpreted\n");
 }
  return ulReturn;
}

// call back function for rule "ResponseAckToken"
static ulong pfn_ResponseAckToken(APG_CBDATA* spData, ulong ulState, ulong ulOffset, ulong ulLen)
{
  ulong ulReturn = PFN_OK;
//{{CallBack.pfn_ResponseAckToken
 //if (SEM_POST  == ulState)
// {
 // H248STACK_Parser::setImmediateAckResponseRequired (ulOffset, ulLen);
  //Application::instance().logger().trace("response ack token \n");
// }
//}}CallBack.pfn_ResponseAckToken
  return ulReturn;
}

// Begin C-language linkage specification
#if defined(__cplusplus)
extern "C"
{
#endif

void vCallbackInit(SEM_CALLBACK* spSemantic, ulong* ulpAst)
{
  memset((void*)spSemantic, 0, sizeof(SEM_CALLBACK) * RULE_COUNT_MEGACO);
  memset((void*)ulpAst, 0, sizeof(ulong) * RULE_COUNT_MEGACO);
  spSemantic[RULE_MEGACO_DEVICENAME] = pfn_deviceName;
  ulpAst[RULE_MEGACO_DEVICENAME] = APG_TRUE;
  spSemantic[RULE_MEGACO_MID] = pfn_megacoMid;
  ulpAst[RULE_MEGACO_MID] = APG_TRUE;
  spSemantic[RULE_MEGACO_NOTIFYTOKEN] = pfn_NotifyToken;
  ulpAst[RULE_MEGACO_NOTIFYTOKEN] = APG_TRUE;
  spSemantic[RULE_MEGACO_MEGACOMESSAGE] = pfn_megacoMessage;
  ulpAst[RULE_MEGACO_MEGACOMESSAGE] = APG_TRUE;
  spSemantic[RULE_MEGACO_TRANSACTIONREQUEST] = pfn_transactionRequest;
  ulpAst[RULE_MEGACO_TRANSACTIONREQUEST] = APG_TRUE;
  spSemantic[RULE_MEGACO_TRANSACTIONPENDING] = pfn_transactionPending;
  ulpAst[RULE_MEGACO_TRANSACTIONPENDING] = APG_TRUE;
  spSemantic[RULE_MEGACO_SERVICECHANGEREQUEST] = pfn_serviceChangeRequest;
  ulpAst[RULE_MEGACO_SERVICECHANGEREQUEST] = APG_TRUE;
  spSemantic[RULE_MEGACO_ERRORDESCRIPTOR] = pfn_errorDescriptor;
  ulpAst[RULE_MEGACO_ERRORDESCRIPTOR] = APG_TRUE;
  spSemantic[RULE_MEGACO_ERRORCODE] = pfn_ErrorCode;
  ulpAst[RULE_MEGACO_ERRORCODE] = APG_TRUE;
  spSemantic[RULE_MEGACO_TRANSACTIONID] = pfn_TransactionID;
  ulpAst[RULE_MEGACO_TRANSACTIONID] = APG_TRUE;
  spSemantic[RULE_MEGACO_CONTEXTID] = pfn_ContextID;
  ulpAst[RULE_MEGACO_CONTEXTID] = APG_TRUE;
  spSemantic[RULE_MEGACO_DOMAINADDRESS] = pfn_domainAddress;
  ulpAst[RULE_MEGACO_DOMAINADDRESS] = APG_TRUE;

  spSemantic[RULE_MEGACO_PORTNUMBER] = pfn_portNumber;
  ulpAst[RULE_MEGACO_PORTNUMBER] = APG_TRUE;
  spSemantic[RULE_MEGACO_TERMINATIONID] = pfn_TerminationID;
  ulpAst[RULE_MEGACO_TERMINATIONID] = APG_TRUE;
  spSemantic[RULE_MEGACO_LOCALDESCRIPTOR] = pfn_localDescriptor;
  ulpAst[RULE_MEGACO_LOCALDESCRIPTOR] = APG_TRUE;
  spSemantic[RULE_MEGACO_PKGDNAME] = pfn_pkgdName;
  ulpAst[RULE_MEGACO_PKGDNAME] = APG_TRUE;
  spSemantic[RULE_MEGACO_OBSERVEDEVENT] = pfn_observedEvent;
  ulpAst[RULE_MEGACO_OBSERVEDEVENT] = APG_TRUE;
  spSemantic[RULE_MEGACO_OBSERVEDEVENTPARAMETER] = pfn_observedEventParameter;
  ulpAst[RULE_MEGACO_OBSERVEDEVENTPARAMETER] = APG_TRUE;
  spSemantic[RULE_MEGACO_REQUESTID] = pfn_RequestID;
  ulpAst[RULE_MEGACO_REQUESTID] = APG_TRUE;
  spSemantic[RULE_MEGACO_SERVICECHANGEMETHOD] = pfn_serviceChangeMethod;
  ulpAst[RULE_MEGACO_SERVICECHANGEMETHOD] = APG_TRUE;
  spSemantic[RULE_MEGACO_SERVICECHANGEREASON] = pfn_serviceChangeReason;
  ulpAst[RULE_MEGACO_SERVICECHANGEREASON] = APG_TRUE;
  spSemantic[RULE_MEGACO_VERSION] = pfn_Version;
  ulpAst[RULE_MEGACO_VERSION] = APG_TRUE;

  spSemantic[RULE_MEGACO_ADDTOKEN] = pfn_AddToken;
  ulpAst[RULE_MEGACO_ADDTOKEN] = APG_TRUE;
  spSemantic[RULE_MEGACO_AUDITVALUETOKEN] = pfn_AuditValueToken;
  ulpAst[RULE_MEGACO_AUDITVALUETOKEN] = APG_TRUE;
  spSemantic[RULE_MEGACO_ERRORTOKEN] = pfn_ErrorToken;
  ulpAst[RULE_MEGACO_ERRORTOKEN] = APG_TRUE;
  spSemantic[RULE_MEGACO_EVENTSTOKEN] = pfn_EventsToken;
  ulpAst[RULE_MEGACO_EVENTSTOKEN] = APG_TRUE;
  spSemantic[RULE_MEGACO_MODIFYTOKEN] = pfn_ModifyToken;
  ulpAst[RULE_MEGACO_MODIFYTOKEN] = APG_TRUE;
  spSemantic[RULE_MEGACO_REPLYTOKEN] = pfn_ReplyToken;
  ulpAst[RULE_MEGACO_REPLYTOKEN] = APG_TRUE;
  spSemantic[RULE_MEGACO_SERVICECHANGETOKEN] = pfn_ServiceChangeToken;
  ulpAst[RULE_MEGACO_SERVICECHANGETOKEN] = APG_TRUE;
  spSemantic[RULE_MEGACO_STATSTOKEN] = pfn_StatsToken;
  ulpAst[RULE_MEGACO_STATSTOKEN] = APG_TRUE;
  spSemantic[RULE_MEGACO_SUBTRACTTOKEN] = pfn_SubtractToken;
  ulpAst[RULE_MEGACO_SUBTRACTTOKEN] = APG_TRUE;
  spSemantic[RULE_MEGACO_RESPONSEACKTOKEN] = pfn_ResponseAckToken;
  ulpAst[RULE_MEGACO_RESPONSEACKTOKEN] = APG_TRUE;

  spSemantic[RULE_MEGACO_TRANSTOKEN] = pfn_TransToken;
  ulpAst[RULE_MEGACO_TRANSTOKEN] = APG_TRUE;
  spSemantic[RULE_MEGACO_REMOTEDESCRIPTOR] = pfn_remoteDescriptor;
  ulpAst[RULE_MEGACO_REMOTEDESCRIPTOR] = APG_TRUE;
}
// End C-language linkage specification
#if defined(__cplusplus)
}
#endif
