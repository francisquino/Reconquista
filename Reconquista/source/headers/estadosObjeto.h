/*
 * estadosObjeto.h
 *
 *  Created on: 10/11/2016
 *      Author: francis
 */

#ifndef ESTADOSOBJETO_H
#define ESTADOSOBJETO_H

#include "estado.h"

class Objeto;


//------------------------------------------------------------------------
//
//  En este estado el Objeto (Campesino) esta en un estado inactivo, en
//	espera de una orden.
//------------------------------------------------------------------------
class estadoInactivo : public Estado
{
    public:
		//this is a singleton
		static estadoInactivo* Instance();
    	virtual void entrar(Objeto* pObjeto, Objeto* pDestino);
    	virtual void ejecutar(Objeto* pObjeto, Objeto *pDestino);
    	virtual void salir(Objeto* pObjeto);

    private:
        estadoInactivo() {}
        estadoInactivo(const estadoInactivo&);              // Don't Implement.
        estadoInactivo& operator=(const estadoInactivo&); // Don't implement
 };


//------------------------------------------------------------------------
//
//  En este estado el Objeto (Campesino) caminara a un recurso (mina o
//  bosque) y recolectara la materia del recurso.
//  Si el campesino alcanza el máximo de carga, cambiara su estado a
//  estadoVisitarAyuntamientoYDepositarMaterial.
//------------------------------------------------------------------------
class estadoRecolectar : public Estado
{
    public:
		//this is a singleton
		static estadoRecolectar* Instance();
    	virtual void entrar(Objeto* pObjeto, Objeto* pDestino);
    	virtual void ejecutar(Objeto* pObjeto, Objeto *pDestino);
    	virtual void salir(Objeto* pObjeto);

    private:
        estadoRecolectar() {}
        estadoRecolectar(const estadoRecolectar&);              // Don't Implement.
        estadoRecolectar& operator=(const estadoRecolectar&); // Don't implement
 };


//------------------------------------------------------------------------
//
//  Entidad ira al Ayuntamiento y depositara el material que transporta.
//  Luego volvera a por mas material.
//------------------------------------------------------------------------
class estadoVisitarAyuntamientoYDepositarMaterial : public Estado
{
	public:
		//this is a singleton
		static estadoVisitarAyuntamientoYDepositarMaterial* Instance();
		virtual void entrar(Objeto* pObjeto, Objeto* pDestino);
		virtual void ejecutar(Objeto* pObjeto, Objeto *pDestino);
		virtual void salir(Objeto* pObjeto);

	private:
		estadoVisitarAyuntamientoYDepositarMaterial() {}

		//copy ctor and assignment should be private
		estadoVisitarAyuntamientoYDepositarMaterial(const estadoVisitarAyuntamientoYDepositarMaterial&);
		estadoVisitarAyuntamientoYDepositarMaterial& operator=(const estadoVisitarAyuntamientoYDepositarMaterial&);
};

#endif /* ESTADOSOBJETO_H */
