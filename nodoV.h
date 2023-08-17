#ifndef NODOV_H
#define NODOV_H

#include <iostream>

template <class T,class C>
class NodoA;

template <class T,class C>
class NodoV
{
	//atributos
	protected:
		T info;
		int label;
		NodoV<T,C> *sig;
		NodoA<T,C> *abajo;

	//metodos 
	public:
		inline NodoV(const T &e = *(new T), int la = -1, NodoV<T,C> *s = NULL, NodoA<T,C> *a = NULL): info(e), label(la), sig(s), abajo(a) {}

		inline void modInfo(const T &e){this->info = e;}
		inline void modSig(NodoV<T,C> *s){this->sig = s;}
		inline void modAbajo(NodoA<T,C> *a){this->abajo = a;}
		inline void modLabel(int i){this->label = i;}

		inline T obtInfo() const {return this->info;}
		inline NodoV<T,C> * obtSig() const {return this->sig;}
		inline NodoA<T,C> * obtAbajo() const {return this->abajo;}
		inline int obtLabel() const {return this->label;}
};

#endif
