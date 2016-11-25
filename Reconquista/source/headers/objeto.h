/*
 * objeto.h
 *
 *  Created on: 27/10/2016
 *      Author: francis
 */

#ifndef OBJETO_H
#define OBJETO_H

#include <cmath>
#include <vector>
#include <map>
#include <string>

#include <SFML/Graphics.hpp>

#include "animatedsprite.h"
#include "globals.h"
#include "maquinaestados.h"

template <class tipoEntidad> class Estado;

struct Telegrama;

class Graphics;
class Level;

//Los distintos entes
namespace tipoObjeto {
    enum TipoObjeto {
        Ayuntamiento,
        Barracones,
        Mina,
        Bosque,
        Campesino,
        Caballero
    };
}


inline std::string tipoObjetoToStr(tipoObjeto::TipoObjeto tObj)
{
	switch (tObj)
	{
	case tipoObjeto::Ayuntamiento:
		return "Ayuntamiento";
	case tipoObjeto::Barracones:
		return "Barracones";
	case tipoObjeto::Mina:
		return "Mina";
	case tipoObjeto::Bosque:
		return "Bosque";
	case tipoObjeto::Campesino:
		return "Campesino";
	case tipoObjeto::Caballero:
		return "Caballero";
	}
}

//Diferentes recursos materiales
namespace tipoMaterial {
	enum TipoMaterial {
		Oro,
		Madera
	};
}

inline std::string materialToStr(tipoMaterial::TipoMaterial mat)
{
	switch (mat)
	{
		case tipoMaterial::Oro:
			return "Oro";
		case tipoMaterial::Madera:
			return "Madera";
	}
}


class Objeto : public AnimatedSprite {
	public:
		Objeto();
		Objeto(Graphics &graphics, tipoObjeto::TipoObjeto tipo, std::string filePath, int sourceX, int sourceY,
				int width, int height, sf::Vector2i spawnPoint, int maxHealth, int timeToUpdate);
		virtual ~Objeto() {}

		const inline tipoObjeto::TipoObjeto getTipo() const { return this->_tipo; }

		virtual void update(int elapsedTime);
		virtual void draw(Graphics &graphics);

		//all entities can communicate using messages. They are sent
		//using the MessageDispatcher singleton class
		virtual bool manejarMensaje(const Telegrama& msg)=0;

		virtual void animationDone(std::string currentAnimation);
		virtual void setupAnimations();

		void moveLeft();
		void moveRight();
		void moveDown();
		void moveUp();
		void stopMoving();

		void setDestino(const float destinoX, const float destinoY);
		const float getDestinoX() const { return this->_destinoX; }
		const float getDestinoY() const { return this->_destinoY; }

		bool getSeleccionado() const { return this->_seleccionado; }
		void setSeleccionado(const bool seleccionado);

		bool checkColision(const Rectangle &other);
		bool chocaConObjeto(Objeto* pObjeto);
		Objeto* chocaConAlgunObjeto();

		const inline int getMaxHealth() const { return this->_maxHealth; }
		const inline int getCurrentHealth() const { return this->_currentHealth; }
		void gainHealth(int amount);

		void modificarCantidadMaterial(tipoMaterial::TipoMaterial material, int cantidad);
		//La cantidad máxima que puede cargar un Campesino
		int cargaMaterialMaxima(tipoMaterial::TipoMaterial material) { return this->_cargaMaxima[material]; }
		//Cantidad de material que se transporta o se almacena
		int getCantidadMaterial(tipoMaterial::TipoMaterial material) { return this->_materiales[material];}
		//El tipo de material que proporciona un recurso. Esta pensada para los recursos (mina y bosque)
		virtual tipoMaterial::TipoMaterial getTipoMaterial();

		void sumarUnidad(Objeto* unidad);

		std::vector<Objeto*> _unidades;

	protected:
		tipoObjeto::TipoObjeto _tipo;

		float _dx, _dy, _destinoX, _destinoY;

		Direction _facing;

		bool _seleccionado;

		int _maxHealth;
		int _currentHealth;

		std::map<tipoMaterial::TipoMaterial, int> _materiales;

		//Los campesinos pueden transportar una carga maxima
		std::map<tipoMaterial::TipoMaterial, int> _cargaMaxima;
}; //class Objeto

class Ayuntamiento : public Objeto {
	public:
		Ayuntamiento();
		Ayuntamiento(Graphics &graphics, sf::Vector2i spawnPoint);
		~Ayuntamiento(){delete this->_apMaquinaEstados;}

		void update(int elapsedTime);
		void draw(Graphics &graphics);
		virtual bool manejarMensaje(const Telegrama& msg);

		void animationDone(std::string currentAnimation);
		void setupAnimations();

		tipoMaterial::TipoMaterial getTipoMaterial();

		maquinaEstados<Ayuntamiento>* GetFSM() const{ return this->_apMaquinaEstados; }
	private:
		//Instancia de la clase Maquina de Estados
		maquinaEstados<Ayuntamiento>* _apMaquinaEstados; //Ayuntamiento puntero maquina de estados
}; //class Ayuntamiento

class Barracones : public Objeto {
	public:
		Barracones();
		Barracones(Graphics &graphics, sf::Vector2i spawnPoint);
		~Barracones(){delete this->_bapMaquinaEstados;}

		void update(int elapsedTime);
		void draw(Graphics &graphics);
		bool manejarMensaje(const Telegrama& msg);

		void animationDone(std::string currentAnimation);
		void setupAnimations();

		maquinaEstados<Barracones>* GetFSM() const{ return this->_bapMaquinaEstados; }
	private:
		//Instancia de la clase Maquina de Estados
		maquinaEstados<Barracones>* _bapMaquinaEstados; //Barracones puntero maquina de estados
}; //class Barracones

class Mina : public Objeto {
	public:
		Mina();
		Mina(Graphics &graphics, sf::Vector2i spawnPoint, int cantidad);
		~Mina(){delete this->_mpMaquinaEstados;}

		void update(int elapsedTime);
		void draw(Graphics &graphics);
		bool manejarMensaje(const Telegrama& msg);

		void animationDone(std::string currentAnimation);
		void setupAnimations();

		tipoMaterial::TipoMaterial getTipoMaterial() { return tipoMaterial::Oro; }

		maquinaEstados<Mina>* GetFSM() const{ return this->_mpMaquinaEstados; }
	private:
		//Instancia de la clase Maquina de Estados
		maquinaEstados<Mina>* _mpMaquinaEstados; //Mina puntero maquina de estados
}; //class Mina

class Bosque : public Objeto {
	public:
		Bosque();
		Bosque(Graphics &graphics, sf::Vector2i spawnPoint, int cantidad);
		~Bosque(){delete this->_bpMaquinaEstados;}

		void update(int elapsedTime);
		void draw(Graphics &graphics);
		bool manejarMensaje(const Telegrama& msg);

		void animationDone(std::string currentAnimation);
		void setupAnimations();

		tipoMaterial::TipoMaterial getTipoMaterial() { return tipoMaterial::Madera; }

		maquinaEstados<Bosque>* GetFSM() const{ return this->_bpMaquinaEstados; }
	private:
		//Instancia de la clase Maquina de Estados
		maquinaEstados<Bosque>* _bpMaquinaEstados; //Bosque puntero maquina de estados
}; //class Bosque

class Campesino : public Objeto {
	public:
		Campesino();
		Campesino(Graphics &graphics, sf::Vector2i spawnPoint);
		~Campesino(){delete this->_cpMaquinaEstados;}

		void update(int elapsedTime);
		void draw(Graphics &graphics);
		bool manejarMensaje(const Telegrama& msg);

		void animationDone(std::string currentAnimation);
		void setupAnimations();

		//Se guarda un recurso que se va a recolectar, bosque o mina
		void setRecursoRecolectar(Objeto* recurso) { this->_recursoRecoleccion = recurso; }
		//Se devuelve un puntero al recurso que se esta recolectando
		Objeto* getRecursoRecolectar() const { return this->_recursoRecoleccion; }

		tipoMaterial::TipoMaterial getTipoMaterial();

		/*
		void handleTileCollisions(std::vector<Rectangle> &others);
		void handleSlopeCollisions(std::vector<Slope> &others);
		void handleDoorCollisions(std::vector<Door> &others, Level &level, Graphics &graphics);
		void handleEnemyCollisions(std::vector<Enemy*> &others);
		*/

		maquinaEstados<Campesino>* GetFSM() const{ return this->_cpMaquinaEstados; }
	private:
		//Instancia de la clase Maquina de Estados
		maquinaEstados<Campesino>* _cpMaquinaEstados; //Campesino puntero maquina de estados

		Objeto* _recursoRecoleccion;	//Recurso que se está recolectando
}; //class Campesino


#endif /* OBJETO_H */
