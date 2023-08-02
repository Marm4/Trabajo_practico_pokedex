#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ERROR -1
#define EXITO 0

typedef int (*lista_comparador)(void*, void*);

typedef void (*lista_destructor)(void*);

typedef struct lista{
	struct nodo* primero;
	struct nodo* ultimo;
	lista_comparador comparador;
	lista_destructor destructor;
}lista_t;

typedef struct nodo{
	void* elemento;
	struct nodo* siguiente;
}nodo_t;

typedef struct lista_iterador{
	size_t indice;
	struct lista* lista;
}lista_iterador_t;


lista_t* lista_crear(lista_comparador comparador, lista_destructor destructor){
	lista_t *lista_crear = calloc(1,(size_t)sizeof(lista_t));
	if(!lista_crear)
		return NULL;
	lista_crear->comparador = comparador;
	lista_crear->destructor = destructor;
	return lista_crear;
}

nodo_t* nuevo_nodo(void* elemento){
	nodo_t* nuevo_nodo = calloc(1, sizeof(nodo_t)); 
	nuevo_nodo->elemento = elemento;
	return nuevo_nodo;

}

int lista_insertar(lista_t* lista, void* elemento){
	if(!lista || !elemento)
		return ERROR;	
	
	if(!lista->ultimo)
		lista->ultimo = lista->primero = nuevo_nodo(elemento);
	else{
		lista->ultimo->siguiente = nuevo_nodo(elemento);
		lista->ultimo = lista->ultimo->siguiente;			
	}
	return EXITO;	
}

size_t lista_elementos(lista_t* lista){
	size_t devolver = 0;
	if(!lista || !lista->primero)
		return devolver;		
	if(lista->primero == lista->ultimo){
		devolver++;
		return devolver;
	}	
	
	nodo_t *nodo_aux = lista->primero;			
	while(nodo_aux != lista->ultimo){
		nodo_aux = nodo_aux->siguiente;
		devolver++;
		}
	devolver++;
	return devolver;
}

int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion_buscada){
	if(!lista)
		return ERROR;
	
	if(!lista->ultimo){
		lista->ultimo = lista->primero = nuevo_nodo(elemento);
		return EXITO;
	}
	
	nodo_t *nodo_aux1 = lista->primero;
	nodo_t *nodo_aux2 = nodo_aux1->siguiente;
	
	if(posicion_buscada <= 0){		
		nodo_t *nodo_nuevo = nuevo_nodo(NULL);
		nodo_nuevo->elemento = nodo_aux1->elemento;
		nodo_nuevo->siguiente = nodo_aux2;
		nodo_aux1->elemento = elemento;
		nodo_aux1->siguiente = nodo_nuevo;
	}
	else if(posicion_buscada > 0 && posicion_buscada < lista_elementos(lista)){
		for(int i=0; i<posicion_buscada; i++){
			nodo_aux2 = nodo_aux1;
			nodo_aux1 = nodo_aux1->siguiente;
		}
		nodo_aux2->siguiente = nuevo_nodo(elemento);
		nodo_aux2 = nodo_aux2->siguiente;
		nodo_aux2->siguiente = nodo_aux1;
	}	
	else if(posicion_buscada >= lista_elementos(lista)){
		lista_insertar(lista, elemento);
			
	}
	return EXITO;		
}

void* buscar(lista_t* lista, nodo_t* nodo, nodo_t* nodo_padre, void* elemento){
	if(!nodo)
		return NULL;
	if((*lista->comparador)(nodo->elemento, elemento) == 0){
		void* elemento_encontrado = nodo->elemento;
		nodo_padre->siguiente = nodo->siguiente;
		
		if(lista->primero == lista->ultimo){
			lista->ultimo = NULL;
			return elemento_encontrado;
		}
		else if(lista->ultimo == nodo){
			if(lista->primero == nodo_padre)
				lista->ultimo = lista->primero;
			else
				lista->ultimo = nodo_padre;
			}
		else if(lista->primero == nodo)
			lista->primero = nodo->siguiente;

		free(nodo);
		return elemento_encontrado;
	}
	else{
		nodo_padre = nodo;
		nodo = nodo->siguiente;	
	}
	return buscar(lista, nodo, nodo_padre, elemento);
}

void* lista_buscar_elemento(lista_t* lista, void* elemento){
	if(!lista || !elemento)
		return NULL;
		
	nodo_t* nodo = lista->primero;
	nodo_t* nodo_padre = nodo;
	return buscar(lista, nodo, nodo_padre, elemento);
}

int lista_borrar_especial(lista_t* lista){
	if(!lista || !lista->primero)
		return ERROR;
		
	if(lista->primero == lista->ultimo){
		lista->ultimo = NULL;
		return EXITO;
	}
	nodo_t *nodo_aux = lista->primero;	
	while(nodo_aux->siguiente != lista->ultimo)
		nodo_aux = nodo_aux->siguiente;
	
	(*lista->destructor)(lista->ultimo->elemento);
	free(lista->ultimo);
	lista->ultimo = nodo_aux;
	lista->ultimo->siguiente = NULL;
	return EXITO;
}

int lista_borrar(lista_t* lista){
	if(!lista || !lista->primero)
		return ERROR;
		
	if(lista->primero == lista->ultimo){
		(*lista->destructor)(lista->ultimo->elemento);
		lista->ultimo = NULL;
		return EXITO;
	}
	nodo_t *nodo_aux = lista->primero;	
	while(nodo_aux->siguiente != lista->ultimo)
		nodo_aux = nodo_aux->siguiente;
	(*lista->destructor)(lista->ultimo->elemento);
	free(lista->ultimo);
	lista->ultimo = nodo_aux;
	lista->ultimo->siguiente = NULL;
	return EXITO;
}

int lista_borrar_de_posicion(lista_t* lista, size_t posicion){
	if(!lista || !lista->primero)
		return ERROR;
		
	nodo_t *nodo_aux2 = lista->primero;
	nodo_t *nodo_aux1 = nodo_aux2->siguiente;
	
	if(posicion == 0){
		lista->primero = nodo_aux1;		
		(*lista->destructor)(nodo_aux2->elemento);
		free(nodo_aux2);
		return EXITO;
	}
	else if(posicion > 0 && posicion < lista_elementos(lista)){
		for(int i=1; i<posicion; i++){
			nodo_aux2 = nodo_aux1;
			nodo_aux1 = nodo_aux2->siguiente;
		}
		nodo_aux2->siguiente = nodo_aux1->siguiente;
		(*lista->destructor)(nodo_aux1->elemento);
		free(nodo_aux1);
	}
	
	else if(posicion >= lista_elementos(lista))
		lista_borrar(lista);
			
	return EXITO;		
}

void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){
	if(!lista || !lista->primero || (posicion > lista_elementos(lista)))
		return NULL;

	nodo_t *nodo_aux = lista->primero;
	
	for(size_t i=0; i<posicion; i++)
		nodo_aux = nodo_aux->siguiente;
	
	return nodo_aux->elemento;
}

void* lista_ultimo(lista_t* lista){
	if(!lista || !lista->primero)
		return NULL;

	return lista->ultimo->elemento;
}

bool lista_vacia(lista_t* lista){
	if(!lista || !lista->ultimo || !lista->ultimo->elemento)
		return true;
	return false;
}

int lista_apilar(lista_t* lista, void* elemento){
	return lista_insertar(lista, elemento);	
}

int lista_desapilar(lista_t* lista){
	if(!lista || !lista->primero)
		return ERROR;
	return lista_borrar(lista);	
}

void* lista_tope(lista_t* lista){
	return lista_ultimo(lista);	
}

int lista_encolar(lista_t* lista, void* elemento){
	return lista_insertar(lista, elemento);
}

int lista_desencolar(lista_t* lista){
	if(lista->ultimo == lista->primero){
		lista->ultimo = NULL;
		return EXITO;
		}
	return lista_borrar_de_posicion(lista,0);
}

void* lista_primero(lista_t* lista){
	if(!lista)
		return NULL;
		
	return lista->primero->elemento;
}


void destruir(lista_t* lista, nodo_t* nodo){
	if(!nodo)
		return;
	destruir(lista, nodo->siguiente);
	(*lista->destructor)(nodo->elemento);
	free(nodo);
}

void lista_destruir(lista_t* lista){
	if(!lista)
		return;
	nodo_t* nodo = lista->primero;
	destruir(lista, nodo);
	free(lista);
}

lista_iterador_t* lista_iterador_crear(lista_t* lista){
	if(!lista)
		return NULL;
	lista_iterador_t* it = calloc(1,sizeof(lista_iterador_t));
	if(!it)
		return NULL;
	it->lista = lista;
	return it;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
	if(!iterador)
		return false;
	return iterador->indice < lista_elementos(iterador->lista);
}

void* lista_iterador_siguiente(lista_iterador_t* iterador){	
	if(!iterador)
		return NULL;
	if(iterador->indice > lista_elementos(iterador->lista))
		return NULL;
	
	void* elemento = lista_elemento_en_posicion(iterador->lista, iterador->indice);
	iterador->indice++;
	return elemento;
}

void lista_iterador_destruir(lista_iterador_t* it){
	free(it);
}

void lista_con_cada_elemento(lista_t* lista, void (*funcion)(void*, void*), void *contexto){
	nodo_t *nodo_aux = lista->primero;
	void* elemento;
	for(size_t i = 0; i<lista_elementos(lista);i++){
		elemento = nodo_aux->elemento;
		funcion(elemento, contexto);
		if(nodo_aux->siguiente != NULL)
			nodo_aux = nodo_aux->siguiente;
	}
}
