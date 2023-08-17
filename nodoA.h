#ifndef NODOA_H
#define NODOA_H

#include <iostream>

template<class T, class C>
class NodoV;

template<class T, class C>
class NodoA
{
	//atributos
	protected:
		C costo;
		NodoA<T,C> *sig;
		NodoV<T,C> *vert;

	//metodos
	public:
		inline NodoA(const C &p = *(new C), NodoA<T,C> *s = NULL, NodoV<T,C> *a = NULL): costo(p), sig(s), vert(a) {}

		inline void modCosto(const C &c) {this->costo = c;}
		inline void modSig(NodoA<T,C> *s) {this->sig = s;}
		inline void modVert(NodoV<T,C> *v) {this->vert = v;}

		inline C obtCosto() const {return this->costo;}
		inline NodoA<T,C> * obtSig() const {return this->sig;}
		inline NodoV<T,C> * obtVert() const {return this->vert;}
};

#endif
