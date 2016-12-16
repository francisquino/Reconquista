/*
 * estadoscaballero.h
 *
 *  Created on: 10/11/2016
 *      Author: francis
 */

#ifndef ESTADOSCABALLERO_H
#define ESTADOSCABALLERO_H

#include "estado.h"
#include "messagedispatcher.h"
#include "tipomensaje.h"

class Caballero;


//------------------------------------------------------------------------
//
//  Estado global de Caballero
//------------------------------------------------------------------------
class caballeroEstadoGlobal : public Estado<Caballero>
{
    public:
		//this is a singleton
		static caballeroEstadoGlobal* Instance();
    	virtual void entrar(Caballero* pCaballero);
    	virtual void ejecutar(Caballero* pCaballero);
    	virtual void salir(Caballero* pCaballero);
    	virtual bool OnMessage(Caballero* agent, const Telegrama& msg);
    private:
        caballeroEstadoGlobal() {}
        caballeroEstadoGlobal(const caballeroEstadoGlobal&);              // Don't Implement.
        caballeroEstadoGlobal& operator=(const caballeroEstadoGlobal&); // Don't implement
 };


//------------------------------------------------------------------------
//
//  En este estado el Caballero esta en un estado inactivo, en
//	espera de una orden.
//------------------------------------------------------------------------
class caballeroEstadoInactivo : public Estado<Caballero>
{
    public:
		//this is a singleton
		static caballeroEstadoInactivo* Instance();
    	virtual void entrar(Caballero* pCaballero);
    	virtual void ejecutar(Caballero* pCaballero);
    	virtual void salir(Caballero* pCaballero);
    	virtual bool OnMessage(Caballero* agent, const Telegrama& msg);
    private:
    	caballeroEstadoInactivo() {}
    	caballeroEstadoInactivo(const caballeroEstadoInactivo&);              // Don't Implement.
    	caballeroEstadoInactivo& operator=(const caballeroEstadoInactivo&); // Don't implement
 };


//------------------------------------------------------------------------
//
//  En este estado el Caballero se dirige a un destino.
//------------------------------------------------------------------------
class caballeroEstadoIrDestino : public Estado<Caballero>
{
    public:
		//this is a singleton
		static caballeroEstadoIrDestino* Instance();
    	virtual void entrar(Caballero* pCaballero);
    	virtual void ejecutar(Caballero* pCaballero);
    	virtual void salir(Caballero* pCaballero);
    	virtual bool OnMessage(Caballero* agent, const Telegrama& msg);

    private:
    	caballeroEstadoIrDestino() {}
    	caballeroEstadoIrDestino(const caballeroEstadoIrDestino&);              // Don't Implement.
    	caballeroEstadoIrDestino& operator=(const caballeroEstadoIrDestino&); // Don't implement
 };


#endif /* ESTADOSCABALLERO_H */
