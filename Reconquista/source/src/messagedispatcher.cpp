/*
 * messagedispatcher.cpp
 *
 *  Created on: 15/11/2016
 *      Author: francis
 */

#include "messagedispatcher.h"
#include "objeto.h"
#include <SFML/Graphics.hpp>

using std::set;

//uncomment below to send message info to the debug window
#define SHOW_MESSAGING_INFO

//--------------------------- Instance ----------------------------------------
//
//   this class is a singleton
//-----------------------------------------------------------------------------
MessageDispatcher* MessageDispatcher::Instance()
{
  static MessageDispatcher instance;

  return &instance;
}

//----------------------------- Dispatch ---------------------------------
//
//  see description in header
//------------------------------------------------------------------------
void MessageDispatcher::Discharge(Objeto* pReceptor, const Telegrama& telegram)
{
	if (!pReceptor->manejarMensaje(telegram))
	{
		//telegram could not be handled
		#ifdef SHOW_MESSAGING_INFO
		std::cout << "Message not handled";
		#endif
	}
}

//---------------------------- DispatchMsg ---------------------------
//
//  given a message, a receiver, a sender and any time delay, this function
//  routes the message to the correct agent (if no delay) or stores
//  in the message queue to be dispatched at the correct time
//------------------------------------------------------------------------
void MessageDispatcher::DispatchMsg(double       delay,
                                    Objeto*      pSender,
                                    Objeto*      pReceiver,
                                    int          msg,
                                    void*        AdditionalInfo = NULL)
{
	//make sure the receiver is valid
	if (pReceiver == NULL)
	{
		#ifdef SHOW_MESSAGING_INFO
		std::cout << "\nWarning! No Receiver found " << pReceiver << "";
		#endif

		return;
	}

	//create the telegram
	Telegrama telegram(0, pSender, pReceiver, msg, AdditionalInfo);

	//if there is no delay, route telegram immediately
	if (delay <= 0.0)
	{
		#ifdef SHOW_MESSAGING_INFO
		std::cout << "\nTelegram dispatched at time: " << this->msgClock.getElapsedTime().asMilliseconds()
        				 << " by " << pSender << " for " << pReceiver << ". Msg is " << msg << "";
		#endif

		//send the telegram to the recipient
		Discharge(pReceiver, telegram);
	}

	//else calculate the time when the telegram should be dispatched
	else
	{
		double CurrentTime = this->msgClock.getElapsedTime().asMilliseconds();

		telegram._dispatchTime = CurrentTime + delay;

		//and put it in the queue
		PriorityQ.insert(telegram);

		#ifdef SHOW_MESSAGING_INFO
		std::cout << "\nDelayed telegram from " << pSender << " recorded at time "
				<< this->msgClock.getElapsedTime().asMilliseconds() << " for " << pReceiver << ". Msg is " << msg << "";
		#endif
	}
}

//---------------------- DispatchDelayedMessages -------------------------
//
//  This function dispatches any telegrams with a timestamp that has
//  expired. Any dispatched telegrams are removed from the queue.
//	Este procedimiento se debe colocar en el update del bucle principal del juego.
//------------------------------------------------------------------------
void MessageDispatcher::DispatchDelayedMessages()
{
	//first get current time
	double CurrentTime = this->msgClock.getElapsedTime().asMilliseconds();

	//now peek at the queue to see if any telegrams need dispatching.
	//remove all telegrams from the front of the queue that have gone
	//past their sell by date
	while( !PriorityQ.empty() &&
			(PriorityQ.begin()->_dispatchTime < CurrentTime) &&
			(PriorityQ.begin()->_dispatchTime > 0) )
	{
		//read the telegram from the front of the queue
		const Telegrama& telegram = *PriorityQ.begin();

		//find the recipient
		//Objeto* pReceiver = EntityMgr->GetEntityFromID(telegram.Receiver);

		#ifdef SHOW_MESSAGING_INFO
		std::cout << "\nQueued telegram ready for dispatch: Sent to "
				<< telegram._receptor << ". Msg is "<< telegram._msg << "";
		#endif

		//send the telegram to the recipient
		Discharge(telegram._receptor, telegram);

		//remove it from the queue
		PriorityQ.erase(PriorityQ.begin());
	}
}
