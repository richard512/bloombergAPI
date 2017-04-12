
// bbAPI.cpp
// Bloomberg API C++ Program (c)2017 Lida Xu
// Portions (c) Bloomberg, LLC

// Bloomberg API Includes
#include <blpapi_defs.h>
#include <blpapi_correlationid.h>
#include <blpapi_element.h>
#include <blpapi_event.h>
#include <blpapi_exception.h>
#include <blpapi_message.h>
#include <blpapi_session.h> 
#include <blpapi_subscriptionlist.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>
using  namespace BloombergLP;
using  namespace blpapi;

class BloombergEventHandler : public EventHandler
{
public:
	// Empty constructor
	BloombergEventHandler()
	{
	}

	// The actual event handler method
	bool processEvent(const Event &event, Session *session)
	{
		// We are only interested in market data events related to
		// our subscription
		if (event.eventType() == Event::SUBSCRIPTION_DATA)
		{
			// Each event may have multiple messages. So
			// create a message iterator
			MessageIterator msgIter(event);
			// Iterate over each of the messages
			while (msgIter.next())
			{
				// Get one message and store it in msg
				Message msg = msgIter.message();
				// For Debugging use the msg.print method to
				// display all of the fields:
				// msg.print(std::cout);
				// std::cout << std::endl;
				// Declare a pointer to the topic of the message
				const char *topic = (char *)msg.correlationId().asPointer();
				// If the elements are present, then print out the message 
				if (msg.hasElement("LAST_TRADE", true) &&
					msg.hasElement("SIZE_LAST_TRADE", true))
				{
					// Show the topic
					std::cout << topic << ",";
					// Fetch the necessary elements from the message
					std::cout << msg.getElementAsFloat64("LAST_TRADE") << ",";
					std::cout << msg.getElementAsInt32("SIZE_LAST_TRADE");
					std::cout << std::endl;
				} // end if hasElement
			} // while loop over msgIter
		} // end if eventType is subscription
		return true;
	} // end bool processEvent
}; // end class BloombergEventHandler

class BLPInterface_Market{
public:
	BLPInterface_Market() {}

	void SendRequest() {
		int result = 0;
		// Define a security and a field
		const char *security = "IBM US Equity";
		const char *fields = "LAST_PRICE";
		// Define a pointer to our event handler based on our class
		BloombergEventHandler *sessionEventHandler;
		// Set of sessionOptions
		SessionOptions sessionOptions;
		sessionOptions.setServerHost("localhost");
		sessionOptions.setServerPort(8194);
		// Instantiate the new EventHandler object
		sessionEventHandler = new BloombergEventHandler();
		// Create a Session object using the sessionOptions
		// and event handler
		Session session(sessionOptions, sessionEventHandler);
		// Start the Session
		result = session.start();
		// Open up the Service to the market data feed
		result = session.openService("//blp/mktdata");

		// Create a list of subscriptions
		SubscriptionList subscriptions;
		// Add a subscription and create the CorrelationId on the fly
		subscriptions.add(security, fields, "",
			CorrelationId((char *)security));
		// Kick off the subscriptions
		session.subscribe(subscriptions);
		// At this point we are all done. Events should start
		// coming in and our event handler will get called one
		// time for each event.
		// The "main" thread will sit here below waiting for you
		// to type in a number and press enter.
		std::cin >> result;
		// Cancel our subscription 
		session.unsubscribe(subscriptions);
		// Stop this session
		session.stop();
	}
}; // end Main


class BLPInterface_Reference {

public:

	BLPInterface_Reference() {
	}

	void BDH(char *securities, char *fields, char *startdate, char *enddate, bool adjustmentNormal = true, bool adjustmentAbnormal = true, bool adjustmentSplit = true) {
		// BDH function returns the historical data for a selected security
		// It works same with BDH in Excel
		int done = 0;
		int result = 0;
		SessionOptions sessionOptions;
		// Create a SessionOptions object to hold the session parameters
		// Since this program will run on the same PC as the Bloomberg software,
		//  we use "localhost" as the host name and port 8194 as the default port.
		sessionOptions.setServerHost("localhost");
		sessionOptions.setServerPort(8194);
		// Create a Session object using the sessionOptions
		Session session(sessionOptions);
		// Start the Session
		result = session.start();
		// Note: Here we should check for success (omitted for brevity)
		// Open up the Reference data Service
		result = session.openService("//blp/refdata");
		// Get a reference to the service
		Service referenceService = session.getService("//blp/refdata");
		Request request = referenceService.createRequest("HistoricalDataRequest");
		// Specify the security we are interested in
		request.getElement("securities").appendValue(securities);
		// Specify the fields we are interested in 
		request.getElement("fields").appendValue(fields);
		// Set the start and ending dates and the max number of data points
		request.set("startDate", startdate);
		request.set("endDate", enddate);
		request.set("adjustmentNormal", adjustmentNormal);
		request.set("adjustmentAbnormal", adjustmentAbnormal);
		request.set("adjustmentSplit", adjustmentSplit);
		// Submit the request
		session.sendRequest(request);

		// Go into a loop and check each of the incoming events 
		// for the "response"
		done = 0;
		while (!done)
		{
			// Grab the next event coming in on this session
			Event event = session.nextEvent();
			// If this is the actual data response, show the messages
			if (event.eventType() == Event::RESPONSE)
			{
				// Create an Iterator for going over the messages within the event
				MessageIterator msgIter(event);
				// Loop over all of the messages
				while (msgIter.next()) {
					Message msg = msgIter.message();
					msg.print(std::cout);
					std::cout << std::endl;
				}
				// Set the flag as we are now done
				done = 1;
			}
		} // end while loop

		  // Pause so we can view the output
		std::cin >> done;
	} // end BDH

	void BDS(char *securities, char *fields, bool override = true, char *OverrideField = "END_DATE_OVERRIDE", char *OverrideValue = "20170131") {
		// BDS function returns the multi-cell descriptive data for a selected security
		// It works same with BDS in Excel
		int done = 0;
		int result = 0;
		SessionOptions sessionOptions;
		// Create a SessionOptions object to hold the session parameters
		// Since this program will run on the same PC as the Bloomberg software,
		//  we use "localhost" as the host name and port 8194 as the default port.
		sessionOptions.setServerHost("localhost");
		sessionOptions.setServerPort(8194);
		// Create a Session object using the sessionOptions
		Session session(sessionOptions);
		// Start the Session
		result = session.start();
		// Note: Here we should check for success (omitted for brevity)
		// Open up the Reference data Service
		result = session.openService("//blp/refdata");
		// Get a reference to the service
		Service referenceService = session.getService("//blp/refdata");
		Request request = referenceService.createRequest("ReferenceDataRequest");
		// Specify the security we are interested in
		request.getElement("securities").appendValue(securities);
		// Specify the fields we are interested in 
		request.getElement("fields").appendValue(fields);
		if (override) {
			request.set("strOverrideField", OverrideField);
			request.set("strOverrideValue", OverrideValue);
		}
		// Submit the request
		session.sendRequest(request);

		// Go into a loop and check each of the incoming events 
		// for the "response"
		done = 0;
		while (!done)
		{
			// Grab the next event coming in on this session
			Event event = session.nextEvent();
			// If this is the actual data response, show the messages
			if (event.eventType() == Event::RESPONSE)
			{
				// Create an Iterator for going over the messages within the event
				MessageIterator msgIter(event);
				// Loop over all of the messages
				while (msgIter.next()) {
					Message msg = msgIter.message();
					msg.print(std::cout);
					std::cout << std::endl;
				}
				// Set the flag as we are now done
				done = 1;
			}
		} // end while loop

		  // Pause so we can view the output
		std::cin >> done;
	} // end BDS
	void BDP(char *securities, char *fields) {
		// BDP return a single cell data
		// It is same with BDP
		int done = 0;
		int result = 0;
		SessionOptions sessionOptions;
		// Create a SessionOptions object to hold the session parameters
		// Since this program will run on the same PC as the Bloomberg software,
		//  we use "localhost" as the host name and port 8194 as the default port.
		sessionOptions.setServerHost("localhost");
		sessionOptions.setServerPort(8194);
		// Create a Session object using the sessionOptions
		Session session(sessionOptions);
		// Start the Session
		result = session.start();
		// Note: Here we should check for success (omitted for brevity)
		// Open up the Reference data Service
		result = session.openService("//blp/refdata");
		// Get a reference to the service
		Service referenceService = session.getService("//blp/refdata");
		Request request = referenceService.createRequest("ReferenceDataRequest");
		// Specify the security we are interested in
		request.getElement("securities").appendValue(securities);
		// Specify the fields we are interested in 
		request.getElement("fields").appendValue(fields);

		// Submit the request
		session.sendRequest(request);

		// Go into a loop and check each of the incoming events 
		// for the "response"
		done = 0;
		while (!done)
		{
			// Grab the next event coming in on this session
			Event event = session.nextEvent();
			// If this is the actual data response, show the messages
			if (event.eventType() == Event::RESPONSE)
			{
				// Create an Iterator for going over the messages within the event
				MessageIterator msgIter(event);
				// Loop over all of the messages
				while (msgIter.next()) {
					Message msg = msgIter.message();
					msg.print(std::cout);
					std::cout << std::endl;
				}
				// Set the flag as we are now done
				done = 1;
			}
		} // end while loop

		  // Pause so we can view the output
		std::cin >> done;
	} // End BDP 

}; // end class


int main() {

	BLPInterface_Reference testref;
	testref.BDH("AAPL US EQUITY", "PX_LAST","20110101","20110104");
//	testref.BDS("AAPL US EQUITY", "DVD_HIST_ALL", false);
//	testref.BDP("AAPL US EQUITY", "PX_LAST");
	
	return 0;
}
