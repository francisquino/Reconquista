/*
 * telegrama.h
 *
 *  Created on: 15/11/2016
 *      Author: francis
 */

#ifndef TELEGRAMA_H
#define TELEGRAMA_H

//------------------------------------------------------------------------
//
//  Name:   Telegram.h
//
//  Desc:   This defines a telegram. A telegram is a data structure that
//          records information required to dispatch messages. Messages
//          are used by game agents to communicate with each other.
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <iostream>
#include <math.h>

class Objeto;


struct Telegrama
{
	//the entity that sent this telegram
	//int          Sender;
	Objeto* _emisor;

	//the entity that is to receive this telegram
	//int          Receiver;
	Objeto* _receptor;

	//the message itself. These are all enumerated in the file
	//"MessageTypes.h"
	int _msg;

	//messages can be dispatched immediately or delayed for a specified amount
	//of time. If a delay is necessary this field is stamped with the time
	//the message should be dispatched.
	double _dispatchTime;

	//any additional information that may accompany the message
	void* _extraInfo;


	Telegrama():_dispatchTime(-1),
			_emisor(NULL),
			_receptor(NULL),
			_msg(-1),
			_extraInfo(NULL)
	{}


	Telegrama(double time, Objeto* emisor, Objeto* receptor, int msg, void* info = NULL):
					_dispatchTime(time),
					_emisor(emisor),
					_receptor(receptor),
					_msg(msg),
					_extraInfo(info)
	{}

};


//these telegrams will be stored in a priority queue. Therefore the >
//operator needs to be overloaded so that the PQ can sort the telegrams
//by time priority. Note how the times must be smaller than
//SmallestDelay apart before two Telegrams are considered unique.
const double SmallestDelay = 0.25;


inline bool operator==(const Telegrama& t1, const Telegrama& t2)
{
	return ( fabs(t1._dispatchTime-t2._dispatchTime) < SmallestDelay) &&
			(t1._emisor == t2._emisor)        &&
			(t1._receptor == t2._receptor)    &&
			(t1._msg == t2._msg);
}

inline bool operator<(const Telegrama& t1, const Telegrama& t2)
{
	if (t1 == t2)
	{
		return false;
	}

	else
	{
		return  (t1._dispatchTime < t2._dispatchTime);
	}
}

inline std::ostream& operator<<(std::ostream& os, const Telegrama& t)
{
	os << "Hora: " << t._dispatchTime << "  Emisor: " << t._emisor
			<< "   Receptor: " << t._receptor << "   Msg: " << t._msg;

	return os;
}

//handy helper function for dereferencing the ExtraInfo field of the Telegram
//to the required type.
template <class T>
inline T DereferenceToType(void* p)
{
	return *(T*)(p);
}

#endif /* TELEGRAMA_H */
