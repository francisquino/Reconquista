/*
 * tipomensaje.h
 *
 *  Created on: 15/11/2016
 *      Author: francis
 */

#ifndef TIPOMENSAJE_H
#define TIPOMENSAJE_H

#include <string>

enum tipoMensaje
{
	_msjRecolectarRecurso,
	_msjNoHayRecurso
};


inline std::string msjToStr(int msg)
{
	switch (msg)
	{
		case _msjRecolectarRecurso:
			return "RecolectarRecurso";

		case _msjNoHayRecurso:
			return "NoHayRecurso";
		default:
			return "No reconozido!";
	}
}



#endif /* TIPOMENSAJE_H */
