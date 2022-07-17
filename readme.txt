20.02.2011 Version  0.043.000:
Improved ISDN Pri Support, correct issue with ISDN BRI due 
to changes made for ISDN Pri.
IUA adaption Layer now also supports the Integer id.
If you want to use Integer values leave in the configuration file or database
the text iua id empty.

27.12.2010 Version 0.042.000:
Support for ISDN PRI subscribers
Corrected IUA adaption Layer  in case of padding bytes
Improved stability in ISDN Q931 part

21.11.2010 Version 0.041.000:
For Version missed to commit smallmgc_udp_sip subsystem, this is now also commited

SDP parsing is also working now.
At the moment only the possibility to parse SIP and SDP exists, but this functionality is
not used in SmallMGC.
Improved the package concept somehow
Started with the ETSI callflow, which will be the main focus for the next releases.


14.08.2010 Version 0.040.000:
Corrected some files so that calls are possible if the long form is received.
Added new subsystems for SIP and SDP parsing.
SIP Parsing is only working that the header are interpreted correctly but it is not further integrated
in the SmallMGC application
SDP parsing is not working at the moment
Added a new package profile xml file. To be able to configure the different packages for different Gateways.

07.07.2010 Version 0.036.000:
Added new build configuration so that smallmgc could be built without the sctp library. But if that is done 
no ISDN subscriber are allowed in the configuration. If there still ISDN subscriber in the behaviour is at
the moment undefined.
Added that the MG id in header can be the ip address or the mg name (domain name).


30.06.2010 Version 0.035.000:
Improved integration of the ISDN stack, so that even special configuration are working now
There are still some segementation faults in thetimerhandling. Which I am still in trying to reproduce and
search for the reason why this happens.

18.06.2010 Version 0.034.000:
Improved on test mode.
Corrected some issue with sctp establishment if the peer is not reachable.
Compiled with new compiler from Ubuntu 10.04.

02.06.2010 Version 0.033.000:
Added the possibility for an reduced testmode. Testmode like Sipp is stopped for the moment
(If somebody needs this mode then a participation would be very welcome)
Clean up the logging so that only errors are in the logs.
Worked still on the ISDN stability
Do some improvements regarding the performance of writing the call database.
This version should be released again in Subversion as tag and as file download
Access to the database is done through an active object, so only a thread should have access to the SQLite connection

14.03.2010 Version 0.032.000:
Enhance functionality that SmallMGC can be configured from  external programs
Added the functionality to switch to a test mode
Added the possibility that several H248 transacions can be handled in one IP packet (only receiving side)
for sending it is not intented to support this.
Working on ISDN improvments, again a case seen where the message flow doesn't work, crashes in the ISDN part.


31.01.2010 Version 0.031.000:
Added a new version of the APG parser.
Thanks to Lowell D. Thomas for providing me this parser improvements
This should parse a Megaco message about 2-4 times faster.
Regression tests not completed, but even though do a tag so it is stored in Subversion


29.01.2010 Version 0.030.000:
In this Version the following is changed:

Important Note: From this Version up the Poco Full libraries are needed because starting from
this version also the Data library is used by SmallMGC. At the moment SQLIte is used

Included a configuration Database
Included a Call Database (for some callflows still not updated correctly)
Added files to be able to implement test scenarios (nothing finished, files from Sipp are included)
Updated to poco libraries 1.3.6
Improved stability after updating to poco libary version 1.3.6


17.11.2009 Version 0.021.002 :
Corrected crashes which were introduced with the callflow n.
Access to a variable is necessary for callflow n, but for the other callflows the call object is already deleted.
Introduced a guard that this situation doesn't lead to a crash.
There are still problems with the release version. There I have optimized the code and it seems that  then SmallMGC is much more instable compared to the unoptimized Debug version.

I have also started with the etsi call flow, but only the files copied until now the call flows not adapted. At the moment this part stays the same. I shifted to work to another feature.

I have started to include a scenario execution module. I reuse for that functionality the sipp concept.
At the moment the files are integrated in thebuild and compile. The next steps will be to integrate the functionality in SmallmGC and adpate it for H248.
  

19.10.2009 Version 0.021.000 :
If Termination is blocked clean up call, support forced and graceful blocking of the termination.
If Association is lost clean up internally and do clean up if reason is not cold boot after association is back.

07.10.2009 Version 0.020.000 :
Cleaned up the call control parts of smallmgc, all previously available call control state machine now are done in the new concept. The old callflows are removed.

20.09.2009 Version 0.015.000 :
Implemented that both B channels in a ISDN Bra can be used.
Started to test that two calls to the same BRA can be done. There is still a crash after clearing of the call.
Nevertheless the tag was done so I can use the week in which I can't test anything so I can start with the clean up of the call control directories. 

Megaco:
Improved handling of the long form (Subtract and ServiceChange).
In the servicechange on root now also the method is checked. Previously only the reason was checked


13.09.2009 Version 0.014.000 :
Finished implementing timers for ISDN
Improved handling if SCTP connection is lost. SCTP will be reconnected as soon as information is detected

09.09.2009 Version 0.013.000 :
Fixed memory leaks for ISDN calls
Started to implement isdn timers
Continued to work on API implementation, API will be more important if a real database is introduced.
It is planned to use SQLite which is also supported by the poco framework.

29.08.2009 Version 0.012.000 :
Added a directory api which is under the Boost license so that guis for the configuration 
of SmallMGC can be written under a proprietary license.
Also a new subsystem is added to handle the configuration and status requests from the API.
The communication between the GUI process and the SmallmGC process will be done through 
Sockets.
This feature is not implemented first only the API functions are defined.
In the future this will be enhanced as necessary. So don't use it at the moment.

The rest is the same


22.08.2009 Version 0.011.000 :
Isdn is now working with different gateways, also the calling number is shown in the ISDN phone.
Implemented a first state machine for the IUA assocaition.
Worked on several memory leaks isdn and h248.

Open problems in ISDN:
There are still several memory leaks if calls are done.
ISDN timers are still not implemented.
Only B1 is used at the moment and therefore only one call is possible.
Several phones on the same So Bus.



13.08.2009 Version 0.010.000 :
Redesigned state machine to support ISDN. With the redesigned state machines now ISDN-ISDN, ISDN-PSTN, PSTN-ISDN
and PSTN-PSTN are possible. It is expected to include SIP only a small redesign will be necessary
ISDN calls are possible but there are still some things missing or erronous:
Stability is missing
State Machine contains error, so in cases of call error, calls are not released correctly
ISDN subsystem is at the moment implemented without timers
The IE must be checked if they are according to the standard, because the integration of the ISDN stack was only done according to examples


For Megaco the following issues are fixed:
BT2829756 Configured Termination ids are case sensitive
BT2829759 Digit string is not detected correctly

Furthermore the h callflow is implemented in the new concepts of different legs.
Only basic call is tested, even mgc controled fax switch over is implemented the
it is not tested and therefore it will not work as expected.
Because some SDP handling is missing.

  
There are now know several issues in the H.248 part:
Audit Capabilites can crash SmallMGC.
With the new concept the standard callflow which is implemented in smallmgc_callcontrol can not be used
anymore. In the future this will also be implemented according to the new concept. 

For IUA the status is there it is working to forward and receive the ISDN messages, but still only the things necessary to enable calls are implemented.

To enable the SCTP it is only tried after restart of SmallMGC and if this is not successful then SCTP connection will not be established. 

Even te ISDN is not at the level which I had considered to do a tag. I decided to do a tag
and make the ISDN stable and working with different gateways in the next release.
Because there a lot of code here and it is better to have once a state fixed.


Version 0.006.000 :
First published version
PSTN basic calls are possible between the same or different gateways
ISDN calls are still not possible SCTP/IUA and Q931 Stack are implemented as necessary for basic call but still not
working correctly.
The SmallMGC application can be configured with the configuration.xml file.
The own as the gateway IP addresses can be configured. To add a new gateway copy the configuration data of one gateway
and add it after the last gateway, then change ip address and the termination id as well as phone numbers.
This is only are very short information about the configuration possibilities. In the future there will be Manual published
what can be configured.
You will have to expect quite heavy changes in this region, because I add configuration information as needed.

The smallmgc.properties files is needed for the logging facility at the moment the logging output will be done
to the terminal as well as in a log file. Later I intend to support also syslog


My environment: Ubuntu 8.04
If your system is different then you will have to compile the application on yourself.
Hopefully it won't be too difficult.
I am using eclipse and CDT as development environment


Installation:
Install eclipse 3.4 or 3.5 and CDT Version 6.0 (at the moment milestone versions)
Install Poco library to /opt/poco-1.3.3p1       -> using the version 1.3.3p1 with header files etc.
Unzip folder smallmgc to /opt/workspace/  -> /opt/workspace/smallmgc
Install the poco shared libraries in /usr/lib
Install libsctp for the ISDN part

Now you should be able compile the smallmgc application.

If there are problems
thomas.miesch@smallmgc.org
support@smallmgc.org


