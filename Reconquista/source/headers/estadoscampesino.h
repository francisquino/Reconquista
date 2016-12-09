/*
 * estadoscampesino.h
 *
 *  Created on: 10/11/2016
 *      Author: francis
 */

#ifndef ESTADOSCAMPESINO_H
#define ESTADOSCAMPESINO_H

#include "estado.h"
#include "messagedispatcher.h"
#include "tipomensaje.h"

class Campesino;


//------------------------------------------------------------------------
//
//  Estado global de Campesino
//------------------------------------------------------------------------
class campesinoEstadoGlobal : public Estado<Campesino>
{
    public:
		//this is a singleton
		static campesinoEstadoGlobal* Instance();
    	virtual void entrar(Campesino* pCampesino);
    	virtual void ejecutar(Campesino* pCampesino);
    	virtual void salir(Campesino* pCampesino);
    	virtual bool OnMessage(Campesino* agent, const Telegrama& msg);
    private:
        campesinoEstadoGlobal() {}
        campesinoEstadoGlobal(const campesinoEstadoGlobal&);              // Don't Implement.
        campesinoEstadoGlobal& operator=(const campesinoEstadoGlobal&); // Don't implement
 };


//------------------------------------------------------------------------
//
//  En este estado el Campesino esta en un estado inactivo, en
//	espera de una orden.
//------------------------------------------------------------------------
class campesinoEstadoInactivo : public Estado<Campesino>
{
    public:
		//this is a singleton
		static campesinoEstadoInactivo* Instance();
    	virtual void entrar(Campesino* pCampesino);
    	virtual void ejecutar(Campesino* pCampesino);
    	virtual void salir(Campesino* pCampesino);
    	virtual bool OnMessage(Campesino* agent, const Telegrama& msg);
    private:
    	campesinoEstadoInactivo() {}
    	campesinoEstadoInactivo(const campesinoEstadoInactivo&);              // Don't Implement.
    	campesinoEstadoInactivo& operator=(const campesinoEstadoInactivo&); // Don't implement
 };


//------------------------------------------------------------------------
//
//  En este estado el Campesino se dirige a un destino.
//------------------------------------------------------------------------
class campesinoEstadoIrDestino : public Estado<Campesino>
{
    public:
		//this is a singleton
		static campesinoEstadoIrDestino* Instance();
    	virtual void entrar(Campesino* pCampesino);
    	virtual void ejecutar(Campesino* pCampesino);
    	virtual void salir(Campesino* pCampesino);
    	virtual bool OnMessage(Campesino* agent, const Telegrama& msg);

    private:
    	campesinoEstadoIrDestino() {}
    	campesinoEstadoIrDestino(const campesinoEstadoIrDestino&);              // Don't Implement.
    	campesinoEstadoIrDestino& operator=(const campesinoEstadoIrDestino&); // Don't implement
 };


//------------------------------------------------------------------------
//
//  En este estado el Campesino entrara en un recurso (mina o
//  bosque) y recolectara la materia del recurso.
//  Si el campesino alcanza el máximo de carga, cambiara su estado a
//  estadoVisitarAyuntamientoYDepositarMaterial.
//------------------------------------------------------------------------
class campesinoEstadoRecolectar : public Estado<Campesino>
{
    public:
		//this is a singleton
		static campesinoEstadoRecolectar* Instance();
    	virtual void entrar(Campesino* pCampesino);
    	virtual void ejecutar(Campesino* pCampesino);
    	virtual void salir(Campesino* pCampesino);
    	virtual bool OnMessage(Campesino* agent, const Telegrama& msg);

    private:
    	campesinoEstadoRecolectar() {}
    	campesinoEstadoRecolectar(const campesinoEstadoRecolectar&);              // Don't Implement.
    	campesinoEstadoRecolectar& operator=(const campesinoEstadoRecolectar&); // Don't implement
 };


#endif /* ESTADOSCAMPESINO_H */
