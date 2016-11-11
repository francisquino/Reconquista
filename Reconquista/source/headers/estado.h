/*
 * estado.h
 *
 *  Created on: 10/11/2016
 *      Author: francis
 */

#ifndef ESTADO_H
#define ESTADO_H

class Objeto;

class Estado {
public:
	virtual ~Estado(){}

	//Se ejecuta al entrar en un estado
	virtual void entrar(Objeto*, Objeto*)=0;

	//Se llama por la funcion update del objeto cada paso de update
	virtual void ejecutar(Objeto*, Objeto*)=0;

	//Se ejecuta al salir de un estado
	virtual void salir(Objeto*)=0;
};


#endif /* ESTADO_H */
