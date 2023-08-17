#ifndef GRAFO_H
#define GRAFO_H

#include <iostream>
#include "nodoA.h"
#include "nodoV.h"
#include <algorithm>
#include <vector>
#include <list>
#include <queue>
using namespace std;
template<class T,class C = int>
class Grafo
{
	//atributos
	protected:
		NodoV<T,C> *P,*U;
		int vertices,arcos;

	//metodos
	public:
		inline Grafo(NodoV<T,C> *p = NULL, NodoV<T,C> *u = NULL, int v = 0, int a = 0): P(p), U(u), vertices(v), arcos(a) {}
		inline Grafo(const Grafo& g) {this->P = NULL; this->U = NULL; this->vertices = 0; this->arcos = 0; *this = g;}

		
		/* ->>>>>>>modificadores<<<<<<<- */
		void agregarVertice(const T &e);
		void agregarArco(const T &v, const T &w, const C& c = *(new C));
		void eliminarVertice(const T &v);
		void eliminarArco(const T &v, const T &w);
		void agregarANoDirigido(const T &v,const T w);
		
		/* ->>>>>>>Observadores<<<<<<<- */
		inline bool esVacio() const {return this->P == NULL;}
		T datoRaiz();
		inline int cardinalidadArcos() const {return this->arcos;}
		inline int orden() const {return this->vertices;}
		inline bool esAislado(const T &v){return this->esSumidero(v) and this->esFuente(v);}
		bool existeVertice(const T &e);
		bool existeArco(const T &v, const T &w);
		C costoArco(const T &v, const T &w);
		bool esFuente(const T &v);
		bool esSumidero(const T &v);
		int gradoInterno(const T &v);
		int gradoExterno(const T &v);
		int grado(const T &v);//devuelve la cantidad de aristas que van hacia el y las que el apunta hacia otros
		
		/*->>>>>>>Recorridos<<<<<<<-*/
		list<T> bfs(const T& v);//busqueda en anchuras
		list<T> dfs(const T& v);//el backtraking que no es backtraking xdxd
		list<T> dijkstra(T v, T w,Grafo<T,C> &g);//por si acaso v: 
		list<T> sucesores(const T &v);//lista de sucesores
		list<T> predecesores(const T &v);//lista de predecesores
		list<T> vert();//lista vertices
		list<T> listaDeFuentes();//lista de los nodo de nivel 1 v:
		list<T> listaDeSumideros();//lista de nodos que todos le llegan but el no le llega a otros
		list<T> odenamientoTopologico();//toma los nodos fuente, los guarda eliminando el vertice y sus aristas y sigue con los demas (unicamente si el nodo no recibe ninguna arista)
		int vertToLabel(const T &v);
		T labelToVert(const int &v);
		list<T> caminoHamiltoniano();//esta en proceso aun no lo e terminado
		list<T> getVertices();
		
		
		
		void vaciarGrafo();
		Grafo<T,C>& operator=(const Grafo<T,C>& x);
		void liberacion(NodoV<T,C> *x);
		~Grafo();
		

		template<class P,class U>
		friend std::ostream& operator << (std::ostream& o,const Grafo<P,U>&g);

	protected:
		void actLabel();
		void dfs(NodoV<T,C> *v,std::vector<bool>& color,list<T> &recorrido,bool &fuente,list<string> &anterior);
		void camHam(T v, bool &encontrado, list<T> &sol);
};

template<class T, class C>
void Grafo<T,C>::agregarANoDirigido(const T &v,const T w)
{
	
	this->agregarArco(v,w);
	this->agregarArco(w,v);
	this->arcos-=1;
}

template<class T, class C>
T Grafo<T,C>::datoRaiz()
{
	T hola;
	hola=this->P->obtInfo();
	return hola;
}
template<class T, class C>
list<T> Grafo<T,C>::caminoHamiltoniano()
{
	list<T> cam;
	bool enc = false;
	list<T> vert = this->getVertices();
	cam.clear();
	while(!vert.empty() && !enc)
	{
		cam.push_back(vert.front());
		this->camHam(vert.front(), enc, cam);
		vert.pop_front();
		if(!enc)
			cam.clear();
	}
	return cam;
}

template <class T, class C>
list<T> Grafo<T,C>::getVertices()
{
	list<T> l;
	NodoV<T,C> *vact = this->P;
	while(vact != NULL)
	{
		l.push_back(vact->obtInfo());
		vact = vact->obtSig();
	}
	return(l);
}

template <class T, class C>
void Grafo<T,C>::camHam(T v, bool &encontrado, list<T> &sol)
{
	T w;
	list<T> suc = this->sucesores(v);
	int aux;
	bool esta;
	while(!suc.empty() && !encontrado)
	{
		w = suc.front();
		suc.pop_front();
		esta = false;
		if(sol.begin() == find(sol.begin(), sol.end(), w))
			esta = true;

		if(!esta)
		{
			sol.push_back(w);
			aux=sol.size();
			if(aux == this->vertices)//algo anda mal, hacer casos de prueba para verificar que pasa
				encontrado = true;
			else
				this->camHam(w, encontrado, sol);

			if(!encontrado)
			{	
				sol.pop_back();
			}
		}
	}
}

template<class T, class C>
int Grafo<T,C>::grado(const T &v)
{
	NodoV<T,C> *desp;
	NodoA<T,C> *despa;
	int i = 0;

	desp = this->P;

	if(this->existeVertice(v))
	{
		while(desp != NULL)
		{

				despa = desp->obtAbajo();
				while(despa!=NULL)
				{
					if(despa->obtVert()->obtInfo() == v or desp->obtInfo() == v)
					{
						i++;
					}
					despa = despa->obtSig();
				}
			
			desp = desp->obtSig();
		}
	}
	else
	{
		i = -1;
	}

	return i;
}

template<class T, class C>
int Grafo<T,C>::gradoExterno(const T &v)
{
	NodoV<T,C> *desp;
	NodoA<T,C> *despa;
	int i = 0;

	desp = this->P;

	if(this->existeVertice(v))
	{
		while(desp != NULL and desp->obtInfo()!= v)
		{
			desp = desp->obtSig();
		}

		if(desp != NULL)
		{
			despa = desp->obtAbajo();

			while(despa != NULL)
			{
				i++;
				despa = despa->obtSig();
			}
		}
	}
	else
	{
		i = -1;
	}

	return i;
}
template<class T, class C>
list<T> Grafo<T,C>::dijkstra(T v, T w,Grafo<T,C> &g)
{
	list<T> camino,sucesores;
	queue<T> veri;
	std::vector<bool> visitado(g.orden(),false);
	std::vector<C> distancia(g.orden(),9999);
	std::vector<T> predecesores(g.orden(),*(new T));
	C costo;
	T va,vs;
	bool encontrado = false;
	if(!esSumidero(v))
	{
		visitado[g.vertToLabel(v) - 1] = true;
		distancia[g.vertToLabel(v) - 1] = 0;

		veri.push(v);

		while(!veri.empty())
		{
			va = veri.front();
			veri.pop();
			sucesores = g.sucesores(va);
			while(!sucesores.empty())
			{
				vs = sucesores.front();
				sucesores.pop_front();
				costo = g.costoArco(va,vs);
				if((costo = costo + distancia[g.vertToLabel(va) - 1]) < distancia[g.vertToLabel(vs) - 1])
				{
					distancia[g.vertToLabel(vs) - 1] = costo;
					predecesores[g.vertToLabel(vs) - 1] = va;
				}
	
				if(vs == w)
				{
					encontrado = true;
				}
	
				if(!visitado[g.vertToLabel(vs) - 1])
				{
					veri.push(vs);
					visitado[g.vertToLabel(vs) - 1] = true;
				}
			}
		}
	
		if(encontrado)
		{
			std::cout<<std::endl;
			va = w;
			while(va != v)
			{
				va = predecesores[g.vertToLabel(va)-1];
				camino.push_front(va);
			}
			camino.push_back(w);
		}
	}
	else
		cout<<"ERROR brother es un sumidero el primer nodo que ingresaste \npor lo tanto no se puede conseguir un camino hacia otro nodo desde el";

	return camino;
}
template<class T, class C>
int Grafo<T,C>::gradoInterno(const T &v)
{
	NodoV<T,C> *desp;
	NodoA<T,C> *despa;
	int i = 0;

	desp = this->P;

	if(this->existeVertice(v))
	{
		while(desp != NULL)
		{

			if(desp->obtInfo() != v)
			{
				despa = desp->obtAbajo();
				while(despa!=NULL)
				{
					if(despa->obtVert()->obtInfo() == v)
					{
						i++;
					}
					despa = despa->obtSig();
				}
			}
			desp = desp->obtSig();
		}
	}
	else
	{
		i = -1;
	}

	return i;
}

template<class T, class C>
void Grafo<T,C>::liberacion(NodoV<T,C> *x)
{
	NodoV<T,C> *desp;
	NodoA<T,C> *ant,*act,*sig;
	bool encontrado = false;

	desp = this->P;
	act = NULL;
	while(desp != NULL)
	{
		encontrado = false;
		sig = desp->obtAbajo();
		act = NULL;
		while(sig != NULL and !encontrado)
		{
			ant = act;
			act = sig;
			sig = sig->obtSig();
			if(act->obtVert() == x)
			{
				if(ant == NULL)
				{
					desp->modAbajo(sig);
				}
				else
				{
					ant->modSig(sig);
				}
				delete act;
				this->arcos--;
				encontrado = true;
			}
		}
		desp = desp->obtSig();
	}
}

template<class T, class C>
Grafo<T,C>::~Grafo()
{
	NodoV<T,C> *desp,*depi;
	NodoA<T,C> *despa,*despp;

	desp = this->P;

	while(desp!=NULL)
	{
		depi = desp;
		despa = desp->obtAbajo();
		despp = NULL;

		while(despa!=NULL)
		{
			despp = despa;
			despa = despa->obtSig();
			delete despp;
		}

		desp = desp->obtSig();
		delete depi;
	}
	this->P = NULL;
	this->U = NULL;
	this->vertices = 0;
	this->arcos = 0;
}

template<class T, class C>
list<T> Grafo<T,C>::odenamientoTopologico()
{
	list<T> p,fuente,sus;

	fuente = this->listaDeFuentes();
	p = this->listaDeFuentes();
	Grafo<T,C> g(*this);

	while(!fuente.empty())
	{

		sus = g.sucesores(fuente.front());
		g.eliminarVertice(fuente.front());
		fuente.pop_front();

		while(!sus.empty())
		{
			if(g.esFuente(sus.front()))
			{
				p.push_back(sus.front());
				fuente.push_back(sus.front());
			}
			sus.pop_front();
		}
	}
	return p;
}

template<class T, class C>
void Grafo<T,C>::dfs(NodoV<T,C> *v,std::vector<bool>& color,list<T> &recorrido,bool &fuente,list<string> &anterior)
{

	
	NodoA<T,C> *despa;
	despa = v->obtAbajo();
	list<T> l;

	
	
	while(despa != NULL)
	{
		
		if(!color[despa->obtVert()->obtLabel()])
		{
			
			color[despa->obtVert()->obtLabel()] = true;recorrido.push_back(despa->obtVert()->obtInfo());
			
			this->dfs(despa->obtVert(),color,recorrido,fuente,anterior);
		}
		despa = despa->obtSig();
	}
}

template<class T, class C>
list<T> Grafo<T,C>::dfs(const T& v)
{	
	
	bool fuente=false;
	list<T> recorrido;
	std::vector<bool> color(this->vertices,false);
	string pasado;
	NodoV<T,C>* desp;
	list<string> anterior;

	if(this->P != NULL)
	{
		
		desp = this->P;
		while(desp!=NULL and desp->obtInfo() != v)
		{
			desp = desp->obtSig();
		}
		
		recorrido.push_back(desp->obtInfo());
		
		while(desp!=NULL)
		{
			if(!color[desp->obtLabel()])
			{
				color[desp->obtLabel()] = true;
				this->dfs(desp,color,recorrido,fuente,anterior);
			}
			desp = desp->obtSig();
		}
	}
	return recorrido;
}

template<class T, class C>
list<T> Grafo<T,C>::sucesores(const T &v)
{
	NodoV<T,C> *desp;
	NodoA<T,C> *despa;
	list<T> sus;

	desp=this->P;

	while( desp != NULL and desp->obtInfo() != v )
	{
		desp = desp->obtSig();
	}
	if( desp != NULL )
	{
		despa = desp->obtAbajo();

		while(despa != NULL)
		{
			desp=despa->obtVert();
			sus.push_front(desp->obtInfo());
			despa = despa->obtSig();
		}
	}

	return sus;
}

template<class T, class C>
list<T> Grafo<T,C>::bfs(const T& v)
{
	std::vector<bool> color(this->vertices,false);
	std::vector<int> o(this->vertices,0);
	queue<NodoV<T,C> *> c;
	list<T> l;
	NodoV<T,C> *va, *vs;
	NodoA<T,C> *despa;

	o[this->vertToLabel(v)-1] = 0;
	color[this->vertToLabel(v)-1] = true;

	va = this->P;
	while(va!=NULL and va->obtInfo() != v)
	{
		va = va->obtSig();
	}

	if(va != NULL)
	{
		c.push(va);
		l.push_back(v);
		while(!c.empty())
		{
			despa = va->obtAbajo();

			while(despa != NULL)
			{
				vs = despa->obtVert();				
				if(!color[vs->obtLabel()-1])
				{
					color[vs->obtLabel()-1] = true;
					c.push(vs);
					o[vs->obtLabel()-1] = o[va->obtLabel()-1] +1;
					l.push_back(vs->obtInfo());
				}
				despa = despa->obtSig();
			}
			c.pop();
			if(!c.empty())
			{
				va = c.front();
			}
		}
	}
	return l;
}

template<class P,class U>
std::ostream& operator << (std::ostream& o,const Grafo<P,U>&g)
{
	queue<NodoA<P,U> *> c;
	NodoV<P,U> *desp;
	int i = 0;

	desp = g.P;

	while(desp!=NULL)
	{
		o<<desp->obtInfo()<<"->\t";
		c.push(desp->obtAbajo());
		if(desp->obtAbajo() != NULL)
		{
			i++;
		}
		desp = desp->obtSig();
	}
	o<<std::endl;
	while(i != 0)
	{
		for(int k = 0; k<g.vertices;k++)
		{
			o<<"|\t";
		}
		o<<std::endl;
		for(int k = 0; k<g.vertices;k++)
		{
			if(c.front() != NULL)
			{
				o<<"("<<c.front()->obtVert()->obtInfo()<<","<<c.frente()->obtCosto()<<")\t";
			}
			else
			{
				o<<"( , )\t";
			}
			if(c.front() == NULL)
			{  
				c.push(NULL);
			}
			else
			{
				if(c.front()->obtSig() == NULL)
				{
					i--;
				}
				c.push(c.front()->obtSig());
			} 
			c.pop();
		}
		o<<std::endl;
	}

	return o;

}

template<class T, class C>
Grafo<T,C>& Grafo<T,C>::operator=(const Grafo<T,C>& x)
{
	NodoV<T,C> *desp;
	NodoA<T,C> *despa;

	this->~Grafo();

	desp = x.P;

	while(desp!=NULL)
	{
		this->agregarVertice(desp->obtInfo());
		desp = desp->obtSig();
	}

	desp = x.P;

	while(desp!=NULL)
	{
		despa = desp->obtAbajo();
		while(despa!=NULL)
		{
			this->agregarArco(desp->obtInfo(),despa->obtVert()->obtInfo(),despa->obtCosto());
			despa = despa->obtSig();
		}
		desp = desp->obtSig();
	}

	return *this;
}

template<class T, class C>
list<T> Grafo<T,C>::listaDeSumideros()
{
	NodoV<T,C> *desp;
	list<T> sumidero;

	if(this->P != NULL)
	{
		desp = this->P;

		while(desp != NULL)
		{
			if(desp->obtAbajo() == NULL)
			{
				sumidero.push_back(desp->obtInfo());
			}
			desp = desp->obtSig();
		}
	}
	return sumidero;
}

template<class T, class C>
list<T> Grafo<T,C>::listaDeFuentes()
{
	NodoV<T,C> *desp;
	NodoA<T,C> *despa;
	int i = 0;
	std::vector<bool> vc (this->vertices,true);
	std::vector<NodoV<T,C> *> vn (this->vertices,NULL);
	list<T> fuente;

	if(this->P != NULL)
	{
		desp = this->P;

		while(desp != NULL)
		{
			despa = desp->obtAbajo();
			vn[i] = desp;
			while(despa != NULL)
			{
				vc[despa->obtVert()->obtLabel() -1] = false;
				despa = despa->obtSig();
			}
			desp = desp->obtSig();
			i++;
		}

		for(i = 0;i < this->vertices;i++)
		{
			if(vc[i])
			{
				fuente.push_back(vn[i]->obtInfo());
			}
		}

	}
	return fuente;
}

template<class T, class C>
bool Grafo<T,C>::esSumidero(const T &v)
{
	NodoV<T,C> *desp;
	NodoA<T,C> *despa;
	bool sumidero = true;

	if(this->P != NULL)
	{
		desp = this->P;

		while(desp != NULL and desp->obtInfo() != v)
		{
			desp = desp->obtSig();
		}
		if(desp != NULL and desp->obtAbajo() != NULL)
		{
			sumidero = false;
		}
	}
	return sumidero;

}

template<class T, class C>
bool Grafo<T,C>::esFuente(const T &v)
{
	NodoV<T,C> *desp;
	NodoA<T,C> *despa;
	bool fuente = true;

	if(this->P != NULL)
	{
		desp = this->P;

		while(desp != NULL and fuente)
		{
			despa = desp->obtAbajo();

			while(despa != NULL and fuente)
			{
				if(despa->obtVert()->obtInfo() == v)
				{
					fuente = false;
				}
				despa = despa->obtSig();
			}
			desp = desp->obtSig();
		}
	}

	return fuente;
}

template<class T, class C>
T Grafo<T,C>::labelToVert(const int &v)
{
	NodoV<T,C> *desp;

	if(this->P != NULL)
	{
		desp = this->P;

		for(int i=1;i<v;i++)
		{
			desp = desp->obtSig();
		}
	}
	return desp->obtInfo();
}

template<class T, class C>
int Grafo<T,C>::vertToLabel(const T &v)
{
	NodoV<T,C> *desp;

	if(this->P != NULL)
	{
		desp = this->P;

		while(desp != NULL and desp->obtInfo() != v)
		{
			desp = desp->obtSig();
		}
		if(desp != NULL)
		{
			return desp->obtLabel();
		}
	}
	return 0;
}

template<class T, class C>
C Grafo<T,C>::costoArco(const T &v, const T &w)
{
	NodoV<T,C> *desp;
	NodoA<T,C> *despa;
	C e = *(new C);

	if(this->P != NULL)
	{
		desp = this->P;

		while(desp != NULL and desp->obtInfo() != v)
		{
			desp = desp->obtSig();
		}
		if(desp != NULL and desp->obtAbajo() != NULL)
		{
			despa = desp->obtAbajo();

			while(despa != NULL and despa->obtVert()->obtInfo() != w)
			{
				despa = despa->obtSig();
			}
			if(despa != NULL)
			{
				e = despa->obtCosto();
			}
		}
	}

	return e;
}

template<class T, class C>
list<T> Grafo<T,C>::vert()
{
	NodoV<T,C> *desp;
	list<T> vert;

	if(this->P != NULL)
	{
		desp = this->P;

		while(desp != NULL)
		{
			vert.push_back(desp->obtInfo());
			desp = desp->obtSig();
		}	
	}
	return vert;
}

template<class T, class C>
list<T> Grafo<T,C>::predecesores(const T &v)
{
	NodoV<T,C> *desp;
	NodoA<T,C> *despa;
	list<T> pre;
	bool encontrado = false;

	if(this->P != NULL)
	{
		desp = this->P;

		while(desp != NULL)
		{
			despa = desp->obtAbajo();

			if(desp->obtInfo() != v)
			{
				while(despa != NULL and !encontrado)
				{
					if(despa->obtVert()->obtInfo() == v)
					{
						pre.push_front(desp->obtInfo());
						encontrado = true;
					}
					despa = despa->obtSig();
				}
			}
			encontrado = false;
			desp = desp->obtSig();
		}
	}

	return pre;
}

template<class T,class C>
void Grafo<T,C>::actLabel()
{
	NodoV<T,C> *desp;
	int i = 1;

	if(this->P != NULL)
	{
		desp = this->P;

		while(desp != NULL)
		{
			desp->modLabel(i);
			desp = desp->obtSig();
		}
	}
}

template<class T,class C>
void Grafo<T,C>::vaciarGrafo()
{
	NodoV<T,C> *vertice,*aux;
	vertice=this->P;
	while(vertice!=NULL)
	{
		aux=vertice;
		vertice=vertice->obtSig();
		this->liberacion(aux);
		this->eliminarVertice(aux->obtInfo());
		
	}
}

template<class T,class C>
void Grafo<T,C>::eliminarVertice(const T &v)
{
	NodoV<T,C> *despant,*despact,*despsig;
	NodoA<T,C> *depi,*depi2;

	if(this->P != NULL)
	{
		despant = NULL;
		despact = this->P;
		despsig = despact->obtSig();

		while(despsig != NULL and despact->obtInfo() != v)
		{
			despant = despact;
			despact = despsig;
			despsig = despsig->obtSig();
		}
		if(despact->obtInfo() == v)
		{
			if(this->P == despact)
			{
				if(this->P == this->U)
				{
					this->P = NULL;
					this->U = NULL;
				}
				else
				{
					this->P = despsig;
				}
			}
			else if(this->U == despact)
			{
				this->U = despant;
				despant->modSig(despsig);
			}
			else
			{
				despant->modSig(despsig);
			}
			depi2 = despact->obtAbajo();
			this->liberacion(despact);
			delete despact;
			this->vertices--;
			this->actLabel();

			depi = NULL;
			while(depi2 != NULL)
			{
				depi = depi2;
				depi2 = depi2->obtSig();
				delete depi;
				this->arcos--;
			}
		}
	}
}

template<class T,class C>
void Grafo<T,C>::eliminarArco(const T &v, const T &w)
{
	NodoV<T,C> *desp;
	NodoA<T,C> *despant,*despact,*despasig;

	if(this->P != NULL)
	{
		desp = this->P;

		while(desp != NULL and desp->obtInfo() != v)
		{
			desp = desp->obtSig();
		}
		if(desp != NULL and desp->obtAbajo() != NULL)
		{
			despact = desp->obtAbajo();
			despasig = despact->obtSig();
			despant = NULL;

			while(despasig != NULL and despact->obtVert()->obtInfo() != w)
			{
				despant = despact;
				despact = despasig;
				despasig = despasig->obtSig();
			}
			if(despact->obtVert()->obtInfo() == w)
			{
				
				if(despant != NULL)
				{
					despant->modSig(despasig);
				}
				else
				{
					desp->modAbajo(despasig);
				}
				
				delete despact;
				this->arcos--;
			}
		}
	}
}

template<class T,class C>
bool Grafo<T,C>::existeVertice(const T &e)
{
	NodoV<T,C> *desp;
	bool existe;

	existe = false;

	if(this->P != NULL)
	{
		desp = this->P;

		while(desp != NULL and desp->obtInfo() != e)
		{
			desp = desp->obtSig();
		}
		if(desp != NULL)
		{
			existe = true;
		}
	}
	return existe;
}

template<class T,class C>
bool Grafo<T,C>::existeArco(const T &v, const T &w)
{
	NodoV<T,C> *desp;
	NodoA<T,C> *despa;
	bool existe; 

	existe = false;
	if(this->P != NULL)
	{
		desp = this->P;

		while(desp != NULL and desp->obtInfo() != v)
		{
			desp = desp->obtSig();
		}

		if(desp != NULL)
		{
			if(desp->obtAbajo() != NULL)
			{
				despa = desp->obtAbajo();

				while(despa != NULL and despa->obtVert()->obtInfo() != w )
				{
					despa = despa->obtSig();
				}
				if(despa != NULL)
				{
					existe = true;
				}
			}
		}
	}

	return existe;
}

template<class T, class C>
void Grafo<T,C>::agregarArco(const T &v, const T &w, const C &c)
{
	NodoA<T,C> *nuevo,*despa,*despant;
	NodoV<T,C> *despv,*despw;
	bool existe;

	existe = false;

	if(this->P != NULL)
	{
		despv = this->P;
		despw = this->P;

		while(despv != NULL and despv->obtInfo() != v)
		{
			despv = despv->obtSig();
		}

		while(despw != NULL and despw->obtInfo() != w)
		{
			despw = despw->obtSig();
		}

		if(despv != NULL and despw != NULL)
		{
			nuevo = new NodoA<T,C>(c);
			if(despv->obtAbajo() == NULL)
			{
				despv->modAbajo(nuevo);
			}
			else
			{
				despa = despv->obtAbajo();
				despant = despa;

				while(despa != NULL && !existe)
				{
					if(!existe && despa->obtVert()->obtInfo() == w)
					{
						existe = true;
					}
					despant = despa;
					despa = despa->obtSig();
				}

				if(!existe)
				{
					despant->modSig(nuevo);
				}
			}
			if(!existe)
			{
				nuevo->modVert(despw);
				this->arcos++;
			}
		}
	}
}

template<class T, class C>
void Grafo<T,C>::agregarVertice(const T &e)
{
	NodoV<T,C> *nuevo;

	nuevo = new NodoV<T,C>(e,this->vertices+1);

	if(this->P == NULL)
	{
		this->P = nuevo;
		this->U = nuevo;
	}
	else
	{
		this->U->modSig(nuevo);
		this->U = nuevo;
	}

	this->vertices++;
}

#endif
