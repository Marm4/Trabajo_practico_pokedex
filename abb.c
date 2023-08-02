#include "abb.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define ERROR -1
#define EXITO 0

abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){
	abb_t *arbol_crear = calloc(1,sizeof(abb_t));
	arbol_crear->comparador = comparador;
	arbol_crear->destructor = destructor;
	return arbol_crear;
}

nodo_abb_t* crear_nodo(void* elemento){
	if(!elemento)
		return NULL;
		
	nodo_abb_t *nuevo_nodo = calloc (1,sizeof(nodo_abb_t));
	nuevo_nodo->elemento = elemento;
	
	return nuevo_nodo;	
}

void insertar_nodo(abb_t *arbol, nodo_abb_t *nodo_raiz, nodo_abb_t *padre, void* elemento, int *es_derecha){
	if(!nodo_raiz && padre){
		if(*es_derecha == 1)
			padre->derecha = crear_nodo(elemento);

		else if(*es_derecha == 0)
			padre->izquierda = crear_nodo(elemento);
	
		padre = NULL;
		return;
	}
	
	padre = nodo_raiz;
	
	if(padre && ((*arbol->comparador)(elemento, nodo_raiz->elemento) == 0 || (*arbol->comparador)(elemento, nodo_raiz->elemento) == 1)){
		*es_derecha = 1;
		insertar_nodo(arbol, nodo_raiz->derecha, padre, elemento, es_derecha);	
	}
	
	else if(padre && ((*arbol->comparador)(elemento, nodo_raiz->elemento) == -1)){
		*es_derecha = 0;
		insertar_nodo(arbol, nodo_raiz->izquierda, padre, elemento, es_derecha);
	}
}

int arbol_insertar(abb_t *arbol, void* elemento){
		if(!arbol || !elemento)
		return ERROR;
	
	if(!arbol->nodo_raiz)
		arbol->nodo_raiz = crear_nodo(elemento);
		
	else{
		nodo_abb_t* nodo_raiz = arbol->nodo_raiz; 
		nodo_abb_t* padre = nodo_raiz;
		int es_derecha = 0;
		insertar_nodo(arbol, nodo_raiz, padre, elemento, &es_derecha);
	}
	return EXITO;
}

int destruir_nodo(abb_t *arbol, nodo_abb_t *raiz, nodo_abb_t *padre){
	if(arbol->nodo_raiz == raiz){
		bool ultimo_elemento = false;
		if(!raiz->izquierda && !raiz->derecha)
			ultimo_elemento = true;
			
		if(raiz->izquierda){
			nodo_abb_t *izquierda = raiz->izquierda;
			nodo_abb_t *anterior;
			if(izquierda->derecha){
				while(izquierda->derecha){
					anterior = izquierda;
					izquierda = izquierda->derecha;
					}
				anterior->derecha = NULL;
				izquierda->izquierda = arbol->nodo_raiz->izquierda;
			}
			arbol->nodo_raiz = izquierda;
			}
		if(raiz->derecha){
			nodo_abb_t *derecha = raiz->derecha;
			if(!raiz->izquierda)
				arbol->nodo_raiz = derecha;
			
			else if(raiz->izquierda){
				arbol->nodo_raiz->derecha = derecha;
					
			}
		}	
		(*arbol->destructor)(raiz->elemento);
		free(raiz);
		if(ultimo_elemento == true)
			arbol->nodo_raiz = NULL;
		return EXITO;
	}
	if(!raiz->izquierda && !raiz->derecha){
		if(padre->derecha == raiz)
			padre->derecha = NULL;
		if(padre->izquierda == raiz)
			padre->izquierda = NULL;
			
		(*arbol->destructor)(raiz->elemento);
		free(raiz);
		return EXITO;
	}
	else if(raiz->izquierda || raiz->derecha){
		int es_derecha = 0, hijo_derecho = 0, hijo_izquierdo = 1;
		if(padre->derecha == raiz)
			es_derecha = 1;
		
		if(raiz->derecha)
			hijo_derecho = 1;
		
		if(raiz->izquierda)
			hijo_izquierdo = 1;
		
		if(es_derecha == 1){
			if(hijo_derecho == 1 && hijo_izquierdo == 1){
				padre->derecha = raiz->derecha;
				padre->derecha->izquierda = raiz->izquierda;
				}
			else if(hijo_derecho == 1 && hijo_izquierdo == 0)
				padre->derecha = raiz->derecha;

			else if(hijo_derecho == 0 && hijo_izquierdo == 1)
				padre->derecha = raiz->izquierda;
			
		}
		else if(es_derecha == 0){
			if(hijo_derecho == 1 && hijo_izquierdo == 1){
				padre->izquierda = raiz->derecha;
				padre->izquierda->izquierda = raiz->izquierda;
				}
				
			else if(hijo_derecho == 1 && hijo_izquierdo == 0)
				padre->izquierda = raiz->derecha;

			else if(hijo_derecho == 0 && hijo_izquierdo == 1)
				padre->izquierda = raiz->izquierda;
			
		
		}
		(*arbol->destructor)(raiz->elemento);
		free(raiz);
		return EXITO;
	}
	return ERROR;
}

int borrar_elemento(abb_t* arbol, nodo_abb_t *raiz, nodo_abb_t *padre, void* elemento){
	if((*arbol->comparador)(elemento, raiz->elemento) == 0){
		return destruir_nodo(arbol, raiz, padre);
	}
	
	else if((*arbol->comparador)(elemento, raiz->elemento) == 1)
		return borrar_elemento(arbol, raiz->derecha, raiz, elemento);
		
	
	else if((*arbol->comparador)(elemento, raiz->elemento) == -1)
		return borrar_elemento(arbol, raiz->izquierda, raiz, elemento);
	
	return ERROR;
}

int arbol_borrar(abb_t* arbol, void* elemento){
	if(!arbol)
		return ERROR;
	
	if(!elemento)
		return ERROR;
	
	nodo_abb_t *nodo_raiz = arbol->nodo_raiz;
	return borrar_elemento(arbol, nodo_raiz, nodo_raiz, elemento);
			
}

void* buscar_elemento(abb_t* arbol, nodo_abb_t* raiz, void* elemento){
	if(!raiz)
		return NULL;
			
	if((*arbol->comparador)(elemento, raiz->elemento) == 0){
		return raiz->elemento;
	}
	
	else if((*arbol->comparador)(elemento, raiz->elemento) == 1)
		return buscar_elemento(arbol, raiz->derecha, elemento);
		
	else if((*arbol->comparador)(elemento, raiz->elemento) == -1)
		return buscar_elemento(arbol, raiz->izquierda, elemento);

	return NULL;	
}


void* arbol_buscar(abb_t* arbol, void* elemento){
	if(!arbol || !elemento || !arbol->nodo_raiz)
		return NULL;
		
	nodo_abb_t *raiz_aux = arbol->nodo_raiz;
	return buscar_elemento(arbol, raiz_aux, elemento);
}

void* arbol_raiz(abb_t* arbol){
	if(!arbol || !arbol->nodo_raiz)
		return NULL;
		
	nodo_abb_t *raiz_aux = arbol->nodo_raiz;
	return raiz_aux->elemento;
}

bool arbol_vacio(abb_t* arbol){
	if(!arbol)
		return true;
		
	else if(arbol->nodo_raiz)
		return false;
	
	return true;
}	
int cantidad_de_elementos(nodo_abb_t* raiz_aux, int cantidad_elementos){
	if(!raiz_aux)
		return cantidad_elementos;
	
	cantidad_elementos = cantidad_de_elementos(raiz_aux->derecha, cantidad_elementos);
	cantidad_elementos++;
	cantidad_elementos = cantidad_de_elementos(raiz_aux->izquierda, cantidad_elementos);
	return cantidad_elementos;
}

int arbol_cantidad_elementos(abb_t* arbol){
	if(!arbol)
		return ERROR;
	
	nodo_abb_t* raiz_aux = arbol->nodo_raiz;
	int cantidad_elementos = 0;
	return cantidad_de_elementos(raiz_aux, cantidad_elementos);
}

int inorden(nodo_abb_t* raiz_aux, void** array, int *tamanio_array, int indice){
	if(!raiz_aux)
		return indice;
	
	indice = inorden(raiz_aux->izquierda, array, tamanio_array, indice);
	if(*tamanio_array > indice){
		array[indice] = raiz_aux->elemento;
		indice++;
	}
	indice = inorden(raiz_aux->derecha, array, tamanio_array, indice);
	return indice;
}

int arbol_recorrido_inorden(abb_t* arbol, void** array, int tamanio_array){
	if(!arbol || !arbol->nodo_raiz->elemento || tamanio_array == 0)
		return 0;
		
	int indice = 0;
	nodo_abb_t *raiz_aux = arbol->nodo_raiz;
	return inorden(raiz_aux, array, &tamanio_array, indice);
}

int preorden(nodo_abb_t* raiz_aux, void** array, int *tamanio_array, int indice){
	if(!raiz_aux)
		return indice;
		
	if(*tamanio_array > indice){
		array[indice] = raiz_aux->elemento;
		(indice)++;
	}
	indice = preorden(raiz_aux->izquierda, array, tamanio_array, indice);
	indice = preorden(raiz_aux->derecha, array, tamanio_array, indice);
	return indice;
}

int arbol_recorrido_preorden(abb_t* arbol, void** array, int tamanio_array){
	if(!arbol || !arbol->nodo_raiz->elemento || tamanio_array == 0)
		return 0;
	
	int indice = 0;
	nodo_abb_t *raiz_aux = arbol->nodo_raiz;
	return preorden(raiz_aux, array, &tamanio_array, indice);
}

int postorden(nodo_abb_t* raiz_aux, void** array, int *tamanio_array, int indice){
	if(!raiz_aux)
		return indice;
		
	indice = postorden(raiz_aux->izquierda, array, tamanio_array, indice);
	indice = postorden(raiz_aux->derecha, array, tamanio_array, indice);
	if(*tamanio_array > indice){
		array[indice] = raiz_aux->elemento;
		indice++;
	}
	return indice;
}

int arbol_recorrido_postorden(abb_t* arbol, void** array, int tamanio_array){
	if(!arbol || !arbol->nodo_raiz->elemento || tamanio_array == 0)
		return 0;
	
	int indice = 0;
	nodo_abb_t *raiz_aux = arbol->nodo_raiz;
	return postorden(raiz_aux, array, &tamanio_array, indice);
}

void destruir_arbol(abb_t* arbol, nodo_abb_t* raiz_aux){
	if(!raiz_aux)
		return;
			
	else{
		destruir_arbol(arbol, raiz_aux->izquierda);
		destruir_arbol(arbol, raiz_aux->derecha);
		(*arbol->destructor)(raiz_aux->elemento);
		free(raiz_aux);
	}	
}

void arbol_destruir(abb_t* arbol){
	if(!arbol)
		return;
		
	nodo_abb_t *raiz_aux = arbol->nodo_raiz;
	
	if(raiz_aux)
		destruir_arbol(arbol, raiz_aux);
	
	free(arbol);
}

void imprimir_inorden(nodo_abb_t* raiz_aux, bool (*funcion)(void*, void*), void* extra, int *sigo){
	if(!raiz_aux)
		return;
	else{
		imprimir_inorden(raiz_aux->izquierda, (*funcion), extra, sigo);
		if(*sigo == 0){
			if(((*funcion)(raiz_aux->elemento, extra) == true)){
				*sigo = 1;
				return;
			}
		}
		imprimir_inorden(raiz_aux->derecha, (*funcion), extra, sigo);	
		
	}
}

void imprimir_preorden(nodo_abb_t* raiz_aux, bool (*funcion)(void*, void*), void* extra, int *sigo){
	if(!raiz_aux)
		return;
	else{
		if(*sigo == 0){
			if(((*funcion)(raiz_aux->elemento, extra) == true)){
				*sigo = 1;
				return;
			}
		}
		imprimir_preorden(raiz_aux->izquierda, (*funcion), extra, sigo);
		imprimir_preorden(raiz_aux->derecha, (*funcion), extra, sigo);		
	
	}
}

void imprimir_postorden(nodo_abb_t* raiz_aux, bool (*funcion)(void*, void*), void* extra, int *sigo){
	if(!raiz_aux)
		return;
	else{
		imprimir_postorden(raiz_aux->izquierda, (*funcion), extra, sigo);
		imprimir_postorden(raiz_aux->derecha, (*funcion), extra, sigo);

		if(*sigo == 0){
			if(((*funcion)(raiz_aux->elemento, extra) == true)){
				*sigo = 1;
				return;
			}
		}
	}
	return;
}

void abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra){
	if(!arbol)
		return;
	if(!arbol->nodo_raiz)
		return;
		
	nodo_abb_t *raiz_aux = arbol->nodo_raiz;
	int sigo = 0;
	
	if(recorrido == ABB_RECORRER_INORDEN)
		imprimir_inorden(raiz_aux, (*funcion), extra, &sigo);
	
	if(recorrido == ABB_RECORRER_PREORDEN)
		imprimir_preorden(raiz_aux, (*funcion), extra, &sigo);
	
	if(recorrido == ABB_RECORRER_POSTORDEN)
		imprimir_postorden(raiz_aux, (*funcion), extra, &sigo);
}

