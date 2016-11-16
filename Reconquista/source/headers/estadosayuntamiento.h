/*
 * estadosayuntamiento.h
 *
 *  Created on: 10/11/2016
 *      Author: francis
 */

#ifndef ESTADOSAYUNTAMIENTO_H
#define ESTADOSAYUNTAMIENTO_H

#include "estado.h"

class Ayuntamiento;


//------------------------------------------------------------------------
//
//  Estado global de Ayuntamiento
//------------------------------------------------------------------------
class ayuntamientoEstadoGlobal : public Estado<Ayuntamiento>
{
    public:
		//this is a singleton
		static ayuntamientoEstadoGlobal* Instance();
    	virtual void entrar(Ayuntamiento* pAyuntamiento);
    	virtual void ejecutar(Ayuntamiento* pAyuntamiento);
    	virtual void salir(Ayuntamiento* pAyuntamiento);
    	virtual bool OnMessage(Ayuntamiento* agent, const Telegrama& msg);
    private:
        ayuntamientoEstadoGlobal() {}
        ayuntamientoEstadoGlobal(const ayuntamientoEstadoGlobal&);              // Don't Implement.
        ayuntamientoEstadoGlobal& operator=(const ayuntamientoEstadoGlobal&); // Don't implement
 };


//------------------------------------------------------------------------
//
//  En este estado el Ayuntamiento esta en un estado inactivo, en
//	espera de una orden.
//------------------------------------------------------------------------
class ayuntamientoEstadoInactivo : public Estado<Ayuntamiento>
{
    public:
		//this is a singleton
		static ayuntamientoEstadoInactivo* Instance();
    	virtual void entrar(Ayuntamiento* pAyuntamiento);
    	virtual void ejecutar(Ayuntamiento* pAyuntamiento);
    	virtual void salir(Ayuntamiento* pAyuntamiento);
    	virtual bool OnMessage(Ayuntamiento* agent, const Telegrama& msg);
    private:
    	ayuntamientoEstadoInactivo() {}
    	ayuntamientoEstadoInactivo(const ayuntamientoEstadoInactivo&);              // Don't Implement.
    	ayuntamientoEstadoInactivo& operator=(const ayuntamientoEstadoInactivo&); // Don't implement
 };

#endif /* ESTADOSAYUNTAMIENTO_H */
