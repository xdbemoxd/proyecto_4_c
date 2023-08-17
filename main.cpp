#include <iostream>
#include "grafo.h"
#include <list>
using namespace std;

//funcion para ingrear los datos en el grafo
void leerEntrada( Grafo<int> &g );
//funcion que resuelve el algoritmo
int ciudadesOcupadas( Grafo<int> g );
//esta determina cuantos arcos nuevos pueden ser agregados a la lista arcos
int sucesoresNuevos( list<int> arcos, list<int> arcosNuevos,int i );
//verifica que no se repita la ciudad
bool repetido( list<int> ciudades, int ciudadActual );
//retorna una lista con los arcos nuevos, o sea, quita los repetidos
list<int> asignarArcos( list<int> arcos, list<int> sucesores, int i );

//Benjamin Marroquin 27445292

int main()
{
	Grafo<int> g;

	leerEntrada( g );

	cout<< ciudadesOcupadas( g ) <<endl;

	return 0;
}

void leerEntrada( Grafo<int> &g )
{
	int a,b;

	while( cin>>a && cin>>b )
	{
		if ( ! g.existeVertice( a ) )
		{
			g.agregarVertice( a );
		}

		if ( ! g.existeVertice( b ) )
		{
			g.agregarVertice( b );
		}

		if ( ! g.existeArco( a, b) )
		{
			g.agregarANoDirigido( a, b );
		}
	}
}

int ciudadesOcupadas( Grafo<int> g )
{
	list<int> sucesores,ciudad,arcos,arcosNuevos;
	int i=0,ciudadActual;
	bool listo=false;

	//aqui agrego la primera ciudad, obteniendo la ciudad con mas adyacencias
	while( i <= g.orden() )
	{
		i++;
		sucesores=g.sucesores( i );

		if ( arcosNuevos.empty() )
		{
			arcosNuevos=sucesores;
			ciudadActual=i;	
		}else
		{
			if ( arcosNuevos.size() < sucesores.size() )
			{
				arcosNuevos=sucesores;
				ciudadActual=i;
			}
		}
	}

	//guardo la ciudad
	ciudad.push_back( ciudadActual );

	//agrego los arcos
	while( ! arcosNuevos.empty() )
	{
		arcos.push_front( ciudadActual );
		arcos.push_back( arcosNuevos.front() );
		arcosNuevos.pop_front();
	}

	arcosNuevos.clear();

	//simepre buscara la ciudad con la mayor cantidad de adyacencias "no repetidas"
	while( !listo )
	{
		i=1;
		
		while( i <= g.orden() )
		{

			sucesores=g.sucesores( i );
			
			if (  arcosNuevos.empty() && !repetido( ciudad, i ) ) 
			{

				arcosNuevos=asignarArcos( arcos, sucesores, i );
				ciudadActual=i;

			}else
			{
				if ( arcosNuevos.size() < sucesoresNuevos( arcos, sucesores, i ) && !repetido( ciudad, i ) )
				{

					arcosNuevos=asignarArcos( arcos, sucesores, i );
					ciudadActual=i;

				}
			}

			i++;
		}

		//guardo la ciudad, para no repetirla
		ciudad.push_back( ciudadActual );

		while( ! arcosNuevos.empty() )
		{
			/*guardo los arco pero en este estilo 1-1-2-3, donde uno los lee 1-2, 1-3*/
			arcos.push_front( ciudadActual );
			arcos.push_back( arcosNuevos.front() );
			arcosNuevos.pop_front();

		}

		//mi libreria cuenta 2 veces los arcos, lo cual me resulta comodo a la hora de la condicion de parada
		if ( arcos.size() == g.cardinalidadArcos() )
		{
			listo=true;
		}
	}
	
	//retorna el numero de las ciudades seleccionadas
	return ciudad.size();
}

int sucesoresNuevos( list<int> arcos, list<int> arcosNuevos, int i )
{
	int j=0,ciudad1,ciudad2,ciudad3;
	list<int> aux;
	bool sirveArco;

	while( !arcosNuevos.empty() )
	{
		aux=arcos;
		sirveArco=true;
		ciudad3=arcosNuevos.front();

		while( !aux.empty() && sirveArco )
		{
			ciudad1=aux.front();
			ciudad2=aux.back();
			
			//con estos condicionales, determino si la carreta en cuestion es la mejor opcion
			if ( ciudad1 == i && ciudad2 == ciudad3 )
			{
				sirveArco=false;
			}

			if (ciudad1 == ciudad3 && ciudad2 == i && sirveArco )
			{
				sirveArco=false;
			}

			aux.pop_front();
			aux.pop_back();
		}

		//en caso de serlo, cuento cuantas se podrian agregar
		if ( sirveArco )
		{
			j++;
		}
		
		arcosNuevos.pop_front();
	}

	return j;
}

bool repetido( list<int> ciudades, int ciudadActual )
{
	bool aux=false;

	while( !ciudades.empty() && !aux )
	{
		if ( ciudades.front() == ciudadActual )
		{
			aux=true;
		}

		ciudades.pop_front();
	}

	return aux;
}

//ya una vez seleccionada la mejor opcion, se pasa a esta funcion, para determinar la lista de carreteras no agregadas
list<int> asignarArcos( list<int> arcos, list<int> sucesores, int i )
{
	list<int> arcosNuevos, aux;
	int ciudad1,ciudad2,ciudad3;
	bool sirveArco;

	while( !sucesores.empty() )
	{

		aux=arcos;
		sirveArco=true;
		ciudad3=sucesores.front();

		while( !aux.empty() && sirveArco )
		{
			ciudad1=aux.front();
			ciudad2=aux.back();

			if ( ciudad2 == i && ciudad1 == ciudad3 )
			{
				sirveArco=false;
			}

			if ( ciudad1 == i && ciudad2 == ciudad3 && sirveArco ) 
			{
				sirveArco=false;
			}

			aux.pop_front();
			aux.pop_back();
		}
		
		if (sirveArco)
		{
			arcosNuevos.push_back( ciudad3 );
		}

		sucesores.pop_front();

	}

	return arcosNuevos;
}