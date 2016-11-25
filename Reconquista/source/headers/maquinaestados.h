/*
 * maquinaestados.h
 *
 *  Created on: 14/11/2016
 *      Author: francis
 */

#ifndef MAQUINAESTADOS_H
#define MAQUINAESTADOS_H

#include <cassert>
#include <string>

#include "estado.h"
#include "telegrama.h"

template <class tipoEntidad>
class maquinaEstados
{
	private:
		//Puntero al agente que posee esta instancia
		tipoEntidad* opPropietario;

		Estado<tipoEntidad>* opEstadoActual;
		//Registro del ultimo estado en el que estaba el agente
		Estado<tipoEntidad>* opEstadoPrevio;
		//Estal logica de estados se llama cada vez que la FSM se actualiza (FSM Maquina de Estados Finitos)
		Estado<tipoEntidad>* opEstadoGlobal;

	public:
		maquinaEstados(tipoEntidad* owner):
				opPropietario(owner),
				opEstadoActual(NULL),
				opEstadoPrevio(NULL),
				opEstadoGlobal(NULL)
		{}

		//Usar estos metodos para inicializar la FSM
		void setEstadoActual(Estado<tipoEntidad>* s){opEstadoActual = s;}
		void setEstadoGlobal(Estado<tipoEntidad>* s){opEstadoGlobal = s;}
		void setEstadoPrevio(Estado<tipoEntidad>* s){opEstadoPrevio = s;}

		//Llamar a esto para actualizar la FSM
		void update()const
		{
			//Si un estado global existe, llamar a su metodo Ejecutar
			if (opEstadoGlobal) opEstadoGlobal->ejecutar(opPropietario);
			//Igual para el estado actual
			if (opEstadoActual) opEstadoActual->ejecutar(opPropietario);
		}

		bool manejarMensaje(const Telegrama& msg)const
		{
			//Primero se ve si el estado actual es válido y si puede manejar el mensaje
			if (opEstadoActual && opEstadoActual->OnMessage(opPropietario, msg)) {
				return true;
			}

			//Si no, y si se ha implementado un estado global, se envía el mensaje al estado global
			if (opEstadoGlobal && opEstadoGlobal->OnMessage(opPropietario, msg)) {
				return true;
			}

			return false;
		}

		//Cambiar a un nuevo estado
		void cambiarEstado(Estado<tipoEntidad>* pNuevoEstado)
		{
			assert(pNuevoEstado && "<maquinaEstados::cambiarEstado>: intentando cambiar a un Estado null");
			//Mantener un registro del Estado previo
			opEstadoPrevio = opEstadoActual;
			//Llamar al metodo salir del Estado existente
			opEstadoActual->salir(opPropietario);
			//Cambiar estado al nuevo estado
			opEstadoActual = pNuevoEstado;
			//Llamar al metodo entrar del nuevo Estado
			opEstadoActual->entrar(opPropietario);
		}

		//Cambiar el estado al estado previo
		void cambiarAEstadoPrevio()
		{
			cambiarEstado(opEstadoPrevio);
		}

		//Accessors
		Estado<tipoEntidad>* estadoActual() const{return opEstadoActual;}
		Estado<tipoEntidad>* estadoGlobal() const{return opEstadoGlobal;}
		Estado<tipoEntidad>* estadoPrevio() const{return opEstadoPrevio;}

		//Devuelve true si el tipo del estado actual es igual al tipo de la clase
		//pasada como parametro.
		bool estaEnEstado(const Estado<tipoEntidad>& st)const;
};


#endif /* MAQUINAESTADOS_H */
