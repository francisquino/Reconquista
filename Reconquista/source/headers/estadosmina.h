/*
 * estadosmina.h
 *
 *  Created on: 10/11/2016
 *      Author: francis
 */

#ifndef ESTADOSMINA_H
#define ESTADOSMINA_H

#include "estado.h"

class Mina;


//------------------------------------------------------------------------
//
//  Estado global de Mina
//------------------------------------------------------------------------
class minaEstadoGlobal : public Estado<Mina>
{
    public:
		//this is a singleton
		static minaEstadoGlobal* Instance();
    	virtual void entrar(Mina* pMina);
    	virtual void ejecutar(Mina* pMina);
    	virtual void salir(Mina* pMina);
    	virtual bool OnMessage(Mina* agent, const Telegrama& msg);
    private:
        minaEstadoGlobal() {}
        minaEstadoGlobal(const minaEstadoGlobal&);              // Don't Implement.
        minaEstadoGlobal& operator=(const minaEstadoGlobal&); // Don't implement
 };


//------------------------------------------------------------------------
//
//  En este estado el Mina esta en un estado inactivo, en
//	espera de una orden.
//------------------------------------------------------------------------
class minaEstadoInactivo : public Estado<Mina>
{
    public:
		//this is a singleton
		static minaEstadoInactivo* Instance();
    	virtual void entrar(Mina* pMina);
    	virtual void ejecutar(Mina* pMina);
    	virtual void salir(Mina* pMina);
    	virtual bool OnMessage(Mina* agent, const Telegrama& msg);
    private:
    	minaEstadoInactivo() {}
    	minaEstadoInactivo(const minaEstadoInactivo&);              // Don't Implement.
    	minaEstadoInactivo& operator=(const minaEstadoInactivo&); // Don't implement
 };

#endif /* ESTADOSMINA_H */
