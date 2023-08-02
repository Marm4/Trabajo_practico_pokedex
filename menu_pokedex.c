#include "pokedex.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define EXITO 0
#define ERROR -1

bool mostrar_especies_menu(void* pokemon, void* contador){
	contador = contador;
	if(!pokemon)
		return true;
	printf("%i) %s\n", ((especie_pokemon_t*)pokemon)->numero, ((especie_pokemon_t*)pokemon)->nombre);
	return false;
}

especie_pokemon_t* especie(pokedex_t* pokedex){
	int variable = 0;
	printf("Seleccion el numero de la especie. Para salir presione 0\n");
	abb_con_cada_elemento(pokedex->pokemones, ABB_RECORRER_INORDEN,	mostrar_especies_menu, NULL);
	scanf("%i", &variable);
	
	if(variable == 0)
		return NULL;

	especie_pokemon_t* aux = calloc(1,sizeof(particular_pokemon_t));
	
	if(!aux)
		return NULL;
	
	aux->numero = variable;
	especie_pokemon_t* especie = arbol_buscar(pokedex->pokemones, aux);
	free(aux);
	return especie;
}

int pokedex_mostrar_pokemon_particular(pokedex_t* pokedex){
	int variable = 0;

	especie_pokemon_t* especie_pokemon = especie(pokedex);
	if(!especie_pokemon){
		printf("Hubo un problema. Regresando al menu \n\n");
		return EXITO;
	}
	printf("Seleccione un pokemon: \n");
	lista_iterador_t* it = lista_iterador_crear(especie_pokemon->pokemones);
	particular_pokemon_t* pokemon;
	particular_pokemon_t* pokemones[lista_elementos(especie_pokemon->pokemones)];
	while(lista_iterador_tiene_siguiente(it)){
		pokemon = lista_iterador_siguiente(it);
		pokemones[variable] = pokemon;
		printf("%i)%s\n", variable, pokemon->nombre);
		variable++;
	}
	scanf("%i", &variable);
	printf("Pokemon: %s. Nivel: %i. Capturado: %s\n", pokemones[variable]->nombre, pokemones[variable]->nivel, (pokemones[variable]->capturado==true?"SI":"NO"));

	free(it);
	return EXITO;
}

int pokedex_informacion_especie(pokedex_t* pokedex){
	especie_pokemon_t* especie_pokemon = especie(pokedex);
	if(!especie_pokemon){
		printf("Hubo un problema. Regresando al menu \n\n");
		return EXITO;
	}

	printf("Nombre especie: %s. Descripcion: %s", especie_pokemon->nombre, especie_pokemon->descripcion);
	printf("\n\n");
	return EXITO;
}

void informacion_comandos(pokedex_t* pokedex, bool ultimos_vistos, bool ultimos_capturados, bool pokedex_iniciada, bool mostrar_info){
	printf("Los comandos que se pueden utilizar son los siguiente: ");
	if(!pokedex_iniciada || !mostrar_info)
		printf("\nIniciar pokedex(I) ");
	if(pokedex_iniciada)
		printf("\nApagar pokedex(G) ");
	
	printf("\nSalir del programa(S)");
	
	if(pokedex_iniciada)
		printf("\nAvistar un pokemon(A) \nEvolucionar un pokemon(E)");
	if(!ultimos_vistos)
		printf("\nMostrar pokemoens vistos(V) ");
	if(!ultimos_capturados)
		printf("\nMostrar pokemones capturados(C) ");
	if(mostrar_info)
		printf("\nMostrar informacion de una especie(M) \nMostrar pokemon particular(P)");
	
	printf("\n\n");
}

int menu_pokedex(pokedex_t* pokedex, bool ultimos_vistos, bool ultimos_capturados, bool pokedex_iniciada, bool mostrar_info){
	char variable[1];
	char ruta_archivo[MAX_NOMBRE];
	printf("*****Menu pokedex***** \n\n\tI)Prender pokedex. \n\tA)Avistar Pokemon. \n\tE)Evolucionar Pokemones. \n\tH)Ayuda. \n\tV)Mostrar ultimos pokemones vistos. \n\tC)Mostrar pokemones capturados recientemente. \n\tM)Informacion de una especie. \n\tP)Informacion de un pokemon. \n\tG)Apagar pokedex.	\n\tS)Salir del programa.\n");

	scanf("%s",variable);

	
	if(((strcmp(variable, "I") == 0) || (strcmp(variable, "i") == 0)) && !pokedex_iniciada){
		if(pokedex)
			pokedex_destruir(pokedex);

		pokedex_t* nueva_pokedex = pokedex_prender();
		if(nueva_pokedex){
			printf("\nSe prendio con exito la pokedex\n");
			pokedex_iniciada = true;
			mostrar_info = true;
		}
		else
			printf("\nHubo un problema al intentar prender la pokedex\n");
		return menu_pokedex(nueva_pokedex, ultimos_vistos, ultimos_capturados, pokedex_iniciada, mostrar_info);
	}
		
	else if(((strcmp(variable, "G") == 0) || (strcmp(variable, "g") == 0)) && (mostrar_info || pokedex_iniciada)){
		if(pokedex_apagar(pokedex) == EXITO){
			printf("\nSe apago con exito la pokedex\n");
			pokedex_iniciada = false;
			mostrar_info = false;
		}
		else
			printf("\nHubo un problema la intentar apagar la pokedex\n");
		return menu_pokedex(pokedex, ultimos_vistos, ultimos_capturados, pokedex_iniciada, mostrar_info);
	}
		
	else if((strcmp(variable, "S") == 0) || (strcmp(variable, "s") == 0)){
		pokedex_destruir(pokedex);
		return EXITO;
	}
		
	else if((strcmp(variable, "H") == 0) || (strcmp(variable, "h") == 0)){
		informacion_comandos(pokedex, ultimos_vistos, ultimos_capturados, pokedex_iniciada, mostrar_info);
		return menu_pokedex(pokedex, ultimos_vistos, ultimos_capturados, pokedex_iniciada, mostrar_info);
	}
	
	else if(((strcmp(variable, "A") == 0) || (strcmp(variable, "a") == 0)) && pokedex_iniciada){
		strcpy(ruta_archivo, "avistamientos.txt");
		if(pokedex_avistar(pokedex, ruta_archivo) == EXITO)
			printf("\nSe avistaron con exito los pokemones\n");
		else
			printf("\nHubo un error al cargar los avistamientos\n");
		ultimos_vistos = false;
		ultimos_capturados = false;
		mostrar_info = true;
		return menu_pokedex(pokedex, ultimos_vistos, ultimos_capturados, pokedex_iniciada, mostrar_info);
	
	return EXITO;
	}
		
	else if(((strcmp(variable, "E") == 0) || (strcmp(variable, "e") == 0)) && pokedex_iniciada){
		strcpy(ruta_archivo, "evoluciones.txt");
		if(pokedex_evolucionar(pokedex, ruta_archivo) == EXITO)
			printf("\nSe han cargado las evoluciones con exito\n");
		else
			printf("\nHubo un error al intentar evolucionar los pokemones\n");
		return menu_pokedex(pokedex, ultimos_vistos, ultimos_capturados, pokedex_iniciada, mostrar_info);
	}
		
	else if(((strcmp(variable, "C") == 0) || (strcmp(variable, "c") == 0)) && !ultimos_capturados){
		pokedex_ultimos_capturados(pokedex);
		ultimos_capturados = true;
		return menu_pokedex(pokedex, ultimos_vistos, ultimos_capturados, pokedex_iniciada, mostrar_info);
	}
	
	else if(((strcmp(variable, "V") == 0) || (strcmp(variable, "v") == 0)) && !ultimos_vistos){
		pokedex_ultimos_vistos(pokedex);
		ultimos_vistos = true;
		return menu_pokedex(pokedex, ultimos_vistos, ultimos_capturados, pokedex_iniciada, mostrar_info);
	}
		
	else if(((strcmp(variable, "M") == 0) || (strcmp(variable, "m") == 0)) && mostrar_info){
		pokedex_informacion_especie(pokedex);
		return menu_pokedex(pokedex, ultimos_vistos, ultimos_capturados, pokedex_iniciada, mostrar_info);
	}

	else if(((strcmp(variable, "P") == 0) || (strcmp(variable, "p") == 0)) && mostrar_info){
		pokedex_mostrar_pokemon_particular(pokedex);
		return menu_pokedex(pokedex, ultimos_vistos, ultimos_capturados, pokedex_iniciada, mostrar_info);
	}

	else{
		printf("\n\nError al inresar la tecla. Para saber los comandos permitidos presione H\n\n");
		return menu_pokedex(pokedex, ultimos_vistos, ultimos_capturados, pokedex_iniciada, mostrar_info);
	}
	printf("\n\n");
	return EXITO;
}

int main(){
	pokedex_t* pokedex = pokedex_crear("Marco");
	bool ultimos_vistos = true;
	bool ultimos_capturados = true;
	bool pokedex_iniciada = false;
	bool mostrar_info = false;
	menu_pokedex(pokedex, ultimos_vistos, ultimos_capturados, pokedex_iniciada, mostrar_info);
	return 0;
}