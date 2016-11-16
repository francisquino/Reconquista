/*
 * estado.h
 *
 *  Created on: 10/11/2016
 *      Author: francis
 */

#ifndef ESTADO_H
#define ESTADO_H

struct Telegrama;

template <class tipoEntidad>
class Estado {
	public:
		virtual ~Estado(){}

		//Se ejecuta al entrar en un estado
		virtual void entrar(tipoEntidad*)=0;

		//Se llama por la funcion update del objeto cada paso de update
		virtual void ejecutar(tipoEntidad*)=0;

		//Se ejecuta al salir de un estado
		virtual void salir(tipoEntidad*)=0;

		//this executes if the agent receives a message from the
		//message dispatcher
		virtual bool OnMessage(tipoEntidad*, const Telegrama&)=0;
};


#endif /* ESTADO_H */
