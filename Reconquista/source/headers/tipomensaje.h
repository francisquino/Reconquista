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
	_msjIrA,
	_msjRecolectar,
	_msjNoHayRecurso,
	_msjDestinoFijado,
	_msjDesplazarseARecurso,
	_msjRecursoFijado
};


inline std::string msjToStr(int msg)
{
	switch (msg)
	{
		case _msjIrA:
			return "IrA";

		case _msjRecolectar:
			return "Recolectar";

		case _msjNoHayRecurso:
			return "NoHayRecurso";

		case _msjDestinoFijado:
			return "DestinoFijado";

		case _msjDesplazarseARecurso:
			return "DesplazarseARecurso";

		case _msjRecursoFijado:
			return "RecursoFijado";

		default:
			return "No reconocido!";
	}
}



#endif /* TIPOMENSAJE_H */
