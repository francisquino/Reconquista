/*
 * messagedispatcher.h
 *
 *  Created on: 15/11/2016
 *      Author: francis
 */

#ifndef MESSAGEDISPATCHER_H_
#define MESSAGEDISPATCHER_H_

//------------------------------------------------------------------------
//
//  Name:   MessageDispatcher.h
//
//  Desc:   A message dispatcher. Manages messages of the type Telegram.
//          Instantiated as a singleton.
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <set>
#include <string>

#include <SFML/System.hpp>

#include "telegrama.h"

class Objeto;

//to make life easier...
#define Dispatcher MessageDispatcher::Instance()

//to make code easier to read
const double  SEND_MSG_IMMEDIATELY = 0.0;
const int     NO_ADDITIONAL_INFO   = 0;
//const Objeto* SENDER_IRRELEVANT = NULL;


class MessageDispatcher
{
	private:
		sf::Clock msgClock;    //Inicializar reloj mensajes

		//a std::set is used as the container for the delayed messages
		//because of the benefit of automatic sorting and avoidance
		//of duplicates. Messages are sorted by their dispatch time.
		std::set<Telegrama> PriorityQ;

		//this method is utilized by DispatchMsg or DispatchDelayedMessages.
		//This method calls the message handling member function of the receiving
		//entity, pReceiver, with the newly created telegram
		void Discharge(Objeto* pReceptor, const Telegrama& msg);

		MessageDispatcher(){}

		//copy ctor and assignment should be private
		MessageDispatcher(const MessageDispatcher&);
		MessageDispatcher& operator=(const MessageDispatcher&);

	public:

		static MessageDispatcher* Instance();

		//send a message to another agent. Receiving agent is referenced by ID.
		void DispatchMsg(double      delay,
				Objeto*     pSender,
				Objeto*     pReceiver,
				int         msg,
				void*       ExtraInfo);

		//send out any delayed messages. This method is called each time through
		//the main game loop.
		void DispatchDelayedMessages();
};

#endif /* MESSAGEDISPATCHER_H_ */
