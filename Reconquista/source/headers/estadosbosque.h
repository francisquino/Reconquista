/*
 * estadosbosque.h
 *
 *  Created on: 10/11/2016
 *      Author: francis
 */

#ifndef ESTADOSBOSQUE_H
#define ESTADOSBOSQUE_H

#include "estado.h"

class Bosque;


//------------------------------------------------------------------------
//
//  Estado global de Bosque
//------------------------------------------------------------------------
class bosqueEstadoGlobal : public Estado<Bosque>
{
    public:
		//this is a singleton
		static bosqueEstadoGlobal* Instance();
    	virtual void entrar(Bosque* pBosque);
    	virtual void ejecutar(Bosque* pBosque);
    	virtual void salir(Bosque* pBosque);
    	virtual bool OnMessage(Bosque* agent, const Telegrama& msg);
    private:
        bosqueEstadoGlobal() {}
        bosqueEstadoGlobal(const bosqueEstadoGlobal&);              // Don't Implement.
        bosqueEstadoGlobal& operator=(const bosqueEstadoGlobal&); // Don't implement
 };


//------------------------------------------------------------------------
//
//  En este estado el Bosque esta en un estado inactivo, en
//	espera de una orden.
//------------------------------------------------------------------------
class bosqueEstadoInactivo : public Estado<Bosque>
{
    public:
		//this is a singleton
		static bosqueEstadoInactivo* Instance();
    	virtual void entrar(Bosque* pBosque);
    	virtual void ejecutar(Bosque* pBosque);
    	virtual void salir(Bosque* pBosque);
    	virtual bool OnMessage(Bosque* agent, const Telegrama& msg);
    private:
    	bosqueEstadoInactivo() {}
    	bosqueEstadoInactivo(const bosqueEstadoInactivo&);              // Don't Implement.
    	bosqueEstadoInactivo& operator=(const bosqueEstadoInactivo&); // Don't implement
 };

#endif /* ESTADOSBOSQUE_H */
