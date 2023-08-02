// ** EN LISTA.C A LISTA_DESTRURIR AGREGAR FORMA DE DESTRURIR MEMORIA RESERVADA POR MI. GREGUE
// ** TAMBIEN UNA FORMA DE BUSQUEDA DE UN ELEMENTO. SE AGREGO LISTA_BUSCAR_ELEMENTO al .h
// ** AGREGUE EN LISTA UNA FUNCION QUE BORRA UN ELEMENTO ESPECIFICO.c


#include "pokedex.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ERROR -1
#define EXITO 0

int comparador_pokemon(void* pokemon1, void* pokemon2){
	if(!pokemon1 || !pokemon2)
		return -1;

	if(strcmp(((particular_pokemon_t*)pokemon1)->nombre, ((particular_pokemon_t*)pokemon2)->nombre) == 0){
		if(((particular_pokemon_t*)pokemon1)->capturado)
			return 0;
	}

	return -1;
}

int comparador_nivel(void* pokemon1, void* pokemon2){
	if(!pokemon1 || !pokemon2)
		return 0;

	if(((especie_pokemon_t*)pokemon1)->numero > ((especie_pokemon_t*)pokemon2)->numero)
		return 1;
	
	if(((especie_pokemon_t*)pokemon1)->numero < ((especie_pokemon_t*)pokemon2)->numero)
		return -1;
	
	return 0;
}

void no_destruye(void* elemento){
	return;
}

void destruir_pokemon(particular_pokemon_t* pokemon){
	if(pokemon)
		free(pokemon); 
}

void destructor_pokemon(void* pokemon){
	if(!pokemon)
		return;
 	destruir_pokemon(pokemon);
}

void destruir_especie(especie_pokemon_t* especie){
	if(especie)
		free(especie);
}

void desturctor_especie(void* elemento){
	if(!elemento)
		return;
	destruir_especie(elemento);
}

bool guardar_pokemones(void* pokemon, void* archivo){
	if(!pokemon || !archivo)
		return true;

	fprintf(archivo, "%s%s%s%i%s%s%s", "E;", ((especie_pokemon_t*)pokemon)->nombre, ";", 
	((especie_pokemon_t*)pokemon)->numero, ";", ((especie_pokemon_t*)pokemon)->descripcion, "\n");
	
	while(lista_vacia((((especie_pokemon_t*)(particular_pokemon_t*)pokemon))->pokemones) == false){
		particular_pokemon_t* pokemon_a_guardar = lista_tope((((especie_pokemon_t*)(particular_pokemon_t*)pokemon))->pokemones);
		char capturado [MAX_NOMBRE]; 
		if(pokemon_a_guardar->capturado)
			strcpy(capturado, "SI");
		else
			strcpy(capturado, "NO");
		fprintf(archivo,"%s%s%s%i%s%s%s", "P;", pokemon_a_guardar->nombre, ";", pokemon_a_guardar->nivel, ";", capturado, "\n");
		lista_borrar_especial(((especie_pokemon_t*)pokemon)->pokemones);
	}

	return false;
}

pokedex_t* pokedex_crear(char entrenador[MAX_NOMBRE]){
	pokedex_t* pokedex = calloc(1,sizeof(pokedex_t));
	if(!pokedex)
		return NULL;
	
	memcpy(pokedex->nombre_entrenador, entrenador,(MAX_NOMBRE)*sizeof(char));
	return pokedex;
}

pokedex_t* inicializar_pokedex(pokedex_t* pokedex){
	abb_t *pokemones = arbol_crear(comparador_nivel, desturctor_especie);
	if(!pokemones)
		return NULL;
	
	lista_t *ultimos_capturados = lista_crear(comparador_pokemon, no_destruye);
	if(!ultimos_capturados){
		free(pokemones);
		return NULL;
	}
	lista_t *ultimos_vistos = lista_crear(comparador_pokemon, no_destruye);
	if(!ultimos_vistos){
		free(pokemones);
		free(ultimos_capturados);
		return NULL;
	}
	pokedex->pokemones = pokemones;
	pokedex->ultimos_capturados = ultimos_capturados;
	pokedex->ultimos_vistos = ultimos_vistos;

	return pokedex;
}

int insertar_especie(pokedex_t* pokedex, FILE* archivo){
	int cantidad_leidos = 0;

	int numero_pokemon;
	int nivel;
	char sin_uso[MAX_NOMBRE];
	char descripcion_especie[MAX_DESCRIPCION];
	char nombre_pokemon[MAX_NOMBRE];
	char capturado[1];
	char nombre_especie_pokemon[MAX_NOMBRE];
	
	cantidad_leidos = fscanf(archivo, "%i;%[^;];%[^;];%[^;];%i;%s", &numero_pokemon, nombre_especie_pokemon, 
	descripcion_especie, nombre_pokemon, &nivel, capturado);

	if(cantidad_leidos != 6 || cantidad_leidos == -1)
		return cantidad_leidos;
	
	especie_pokemon_t* nueva_especie = calloc(1,sizeof(especie_pokemon_t));
	if(!nueva_especie)
		return cantidad_leidos;

	particular_pokemon_t* nuevo_pokemon = calloc(1,sizeof(particular_pokemon_t));	
	if(!nuevo_pokemon){
		free(nueva_especie);
		return cantidad_leidos	;
	}
													// Sin esto, el programa no guarda una variable. Pj, si borran "sin_uso", 
	memcpy(sin_uso, "", (MAX_NOMBRE)*sizeof(char));	// no guarda la descripcion	. No entiendo el porque. Si me lo dicen en la 
													// devolucion joya.				
	
	memcpy(nueva_especie->nombre, nombre_especie_pokemon, (MAX_NOMBRE)*sizeof(char));
	memcpy(nueva_especie->descripcion, descripcion_especie, (MAX_DESCRIPCION)*sizeof(char));
	memcpy(nuevo_pokemon->nombre, nombre_pokemon, (MAX_NOMBRE)*sizeof(char));
	nueva_especie->numero = numero_pokemon;
	nuevo_pokemon->nivel = nivel;
	
	if(strcmp(capturado, "S") == 0){
		nuevo_pokemon->capturado = true;
		lista_insertar(pokedex->ultimos_capturados, nuevo_pokemon);
	}
	else{ 
		nuevo_pokemon->capturado = false; 
		lista_insertar(pokedex->ultimos_vistos, nuevo_pokemon);
	}
	especie_pokemon_t* especie_existente = arbol_buscar(pokedex->pokemones, nueva_especie);

	if(especie_existente){
		free(nueva_especie);
		lista_insertar(especie_existente->pokemones, nuevo_pokemon);
		return cantidad_leidos;
	}
	lista_t* nueva_lista_pokemon = lista_crear(comparador_pokemon, destructor_pokemon);
	nueva_especie->pokemones = nueva_lista_pokemon;
	lista_insertar(nueva_especie->pokemones, nuevo_pokemon);
	arbol_insertar(pokedex->pokemones, nueva_especie);

	return cantidad_leidos;

}

int pokedex_avistar(pokedex_t* pokedex, char ruta_archivo[MAX_RUTA]){
	if(!pokedex)
		return ERROR;
	
	FILE *archivo;
	archivo = fopen(ruta_archivo, "r");
	if(!archivo)
		return ERROR;	

	int cantidad_leidos = 6;

	while(cantidad_leidos == 6 && cantidad_leidos != -1){
		cantidad_leidos = insertar_especie(pokedex, archivo);
		if(cantidad_leidos != 6 && cantidad_leidos != -1){
			fclose(archivo);
			return ERROR;
		}
	}
	fclose(archivo);
	return EXITO;
}

int evolucionar(pokedex_t* pokedex, FILE* archivo){
	int numero_especie;
	int numero_especie_evolucion;
	int cantidad_leidos = 0;
	char nombre_pokemon[MAX_NOMBRE];
	char nombre_especie[MAX_NOMBRE];
	char descripcion[MAX_DESCRIPCION];

	cantidad_leidos = fscanf(archivo, "%i;%[^;];%i;%[^;];%[^\n]", &numero_especie, nombre_pokemon, 
		&numero_especie_evolucion, nombre_especie, descripcion);
	
	if(cantidad_leidos == -1 || cantidad_leidos != 5)
		return cantidad_leidos;
	
	especie_pokemon_t* nueva_especie = calloc(1,sizeof(especie_pokemon_t));
	if(!nueva_especie)
		return ERROR;
	
	nueva_especie->numero = numero_especie;
	strcpy(nueva_especie->nombre, nombre_especie);
	especie_pokemon_t* especie_pokemon_buscado = arbol_buscar(pokedex->pokemones, nueva_especie);
	
	if(!especie_pokemon_buscado){
		printf("\n\nNo se encontre la especie pokemon: %s", nueva_especie->nombre);
		free(nueva_especie);
		return cantidad_leidos;
	}

	particular_pokemon_t* pokemon_buscado = calloc(1,sizeof(particular_pokemon_t));
	if(!pokemon_buscado){
		free(nueva_especie);
		return cantidad_leidos;
	}
	strcpy(pokemon_buscado->nombre, nombre_pokemon);
	particular_pokemon_t* pokemon_evoluciona = lista_buscar_elemento(especie_pokemon_buscado->pokemones, pokemon_buscado);
	if(!pokemon_evoluciona){
		printf("\n\nNo se puede evolucionar al siguiente pokemon: %s", pokemon_buscado->nombre);
		lista_insertar(especie_pokemon_buscado->pokemones, pokemon_evoluciona);
		free(nueva_especie);
		free(pokemon_buscado);
		return cantidad_leidos;
	}
	
	free(pokemon_buscado);
	nueva_especie->numero = numero_especie_evolucion;
	especie_pokemon_t* especie_evoluciona = arbol_buscar(pokedex->pokemones, nueva_especie);
	
	if(especie_evoluciona){
		lista_insertar(especie_evoluciona->pokemones, pokemon_evoluciona);
		free(nueva_especie);
	}
	else{
		lista_t* pokemones = lista_crear(comparador_pokemon, destructor_pokemon);
		lista_insertar(pokemones, pokemon_evoluciona);
		nueva_especie->pokemones = pokemones;
		strcpy(nueva_especie->nombre, nombre_especie);
		strcpy(nueva_especie->descripcion, descripcion);
		arbol_insertar(pokedex->pokemones, nueva_especie);
	}
return cantidad_leidos;
}

int pokedex_evolucionar(pokedex_t* pokedex, char ruta_archivo[MAX_RUTA]){
	if(!pokedex)
		return ERROR;
	
	FILE* archivo = fopen(ruta_archivo, "r");
	if(!archivo)
		return ERROR;
	int retorno = 5;

	while(retorno != -1 && retorno == 5){
		retorno = evolucionar(pokedex, archivo);
		
		if(retorno !=-1 && retorno != 5){
			fclose(archivo);
			return ERROR;
		}
	}
	printf("\n\n");
	fclose(archivo);
	return EXITO;
}

void pokedex_ultimos_capturados (pokedex_t* pokedex){
	if(!pokedex)
		return;
	
	lista_t* capturados = pokedex->ultimos_capturados;
	printf("\n\nUltimos pokemones capturados: ");
	
	while(!lista_vacia(capturados)){
		printf("%s ", ((particular_pokemon_t*)lista_tope(capturados))->nombre);
		lista_desapilar(capturados);
	}
	
	printf("\n\n");
}

void pokedex_ultimos_vistos(pokedex_t* pokedex){
	if(!pokedex)
		return;
	
	lista_t* vistos = pokedex->ultimos_vistos;
	printf("\n\nUltimos pokemones vistos: ");
	
	while(!lista_vacia(vistos)){
		printf("%s ",((particular_pokemon_t*)lista_primero(vistos))->nombre);
		lista_desencolar(vistos);
	}
	
	printf("\n\n");
}

void pokemon_no_existe(especie_pokemon_t* especie, particular_pokemon_t* particular){
	printf("El pokemon buscado no existe");
	if(especie)
		free(especie);
	if(particular)
		free(particular);
}

void especie_pokemones(especie_pokemon_t* pokemon_buscado){
	if(!pokemon_buscado)
		return;
	
	printf("\nEspecie pokemon '%s'. Descripcion: '%s'. Pokemones: \n",pokemon_buscado->nombre, pokemon_buscado->descripcion);
	lista_iterador_t* it = lista_iterador_crear(pokemon_buscado->pokemones);
  	
  	while(lista_iterador_tiene_siguiente(it)){
    	particular_pokemon_t* pokemon = lista_iterador_siguiente(it);
    	printf("%s nivel: %i, capturado: %s\n", pokemon->nombre, pokemon->nivel, (pokemon->capturado == true?"SI":"NO"));
  	}
	
 	printf("\n\n");
 	free(it);
}

void pokedex_informacion(pokedex_t* pokedex , int numero_pokemon , char nombre_pokemon[MAX_NOMBRE]){
	if(!pokedex)
		return;
	
	especie_pokemon_t* aux_especie = calloc(1,sizeof(especie_pokemon_t));
	if(!aux_especie)
		return;

	aux_especie->numero = numero_pokemon;
	especie_pokemon_t* pokemon_buscado = arbol_buscar(pokedex->pokemones, aux_especie);
	
	if(!pokemon_buscado){
		printf("\nNo se encontro la especie buscada");
		pokemon_no_existe(pokemon_buscado, NULL);
		return;
	}
	
	if(!nombre_pokemon){
		especie_pokemones(pokemon_buscado);
 		free(aux_especie);
 		return;
	}
	
	particular_pokemon_t* aux_particular = calloc(1,sizeof(particular_pokemon_t));
	if(!aux_particular){
		free(aux_especie);
		return;
	}

	strcpy(aux_particular->nombre, nombre_pokemon);
	lista_t* pokemon_particular = lista_buscar_elemento(pokemon_buscado->pokemones, aux_particular);
	
	if(!pokemon_particular){
		printf("\nNo se encontro al pokemon buscado");
		pokemon_no_existe(pokemon_buscado, aux_particular);
		return;
	}

	printf("Especie pokemon '%s'. Descripcion: '%s'. Pokemon:'%s' . Nivel: '%i'. Capturado: '%s\n'",
	pokemon_buscado->nombre, pokemon_buscado->descripcion, ((particular_pokemon_t*)pokemon_particular)->nombre, 
	((particular_pokemon_t*)pokemon_particular)->nivel, (((particular_pokemon_t*)pokemon_particular)->capturado == true? "SI":"NO"));
	printf("\n\n");
	free(aux_especie);
	free(aux_particular);
}

void pokedex_destruir(pokedex_t* pokedex){
	if(!pokedex)
		return;
	if(!pokedex->pokemones && !pokedex->ultimos_capturados && !pokedex->ultimos_vistos){
		free(pokedex);
		return;
	}


	int cantidad_elementos = arbol_cantidad_elementos(pokedex->pokemones);
	lista_t** pokemones = calloc(1,(size_t)(cantidad_elementos)*sizeof(lista_t*));
	if(!pokemones)
		return;
	if(cantidad_elementos>0)
		arbol_recorrido_inorden(pokedex->pokemones, (void**)pokemones, cantidad_elementos);

	for(int i=0; i<cantidad_elementos; i++){
		lista_destruir(((especie_pokemon_t*)pokemones[i])->pokemones);
	}
	
	lista_destruir(pokedex->ultimos_capturados); 
	lista_destruir(pokedex->ultimos_vistos);
	
	free(pokemones);
	arbol_destruir(pokedex->pokemones);
	free(pokedex);
}

int pokedex_apagar(pokedex_t* pokedex){
	if(!pokedex)
		return ERROR;
	
	FILE* archivo = fopen("pokedex.txt", "w+");
	if(!archivo)
			return ERROR;
	
	fprintf(archivo, "%s%s", pokedex->nombre_entrenador, "\n");
	abb_con_cada_elemento(pokedex->pokemones, ABB_RECORRER_INORDEN, guardar_pokemones, archivo);
	
	fclose(archivo);
	
	return EXITO;
}

especie_pokemon_t* iniciar_datos_especie(char nombre[MAX_NOMBRE], int numero, char descripcion[MAX_DESCRIPCION]){
	especie_pokemon_t* nueva_especie = calloc(1,sizeof(especie_pokemon_t));
	if(!nueva_especie)
		return NULL;

	memcpy(nueva_especie->nombre, nombre, (MAX_NOMBRE)*sizeof(char));
	memcpy(nueva_especie->descripcion, descripcion, (MAX_DESCRIPCION)*sizeof(char));
	nueva_especie->numero = numero;	

	return nueva_especie;
}

particular_pokemon_t* iniciar_datos_pokemon(char nombre[MAX_NOMBRE], int numero, char descripcion[MAX_DESCRIPCION]){
	particular_pokemon_t* nuevo_pokemon = calloc(1,sizeof(especie_pokemon_t));
	if(!nuevo_pokemon)
		return NULL;

	memcpy(nuevo_pokemon->nombre, nombre, (MAX_NOMBRE)*sizeof(char));
	nuevo_pokemon->nivel = numero;
	if(strcmp(descripcion, "SI"))
		nuevo_pokemon->capturado = true;
	else
		nuevo_pokemon->capturado = false;

	return nuevo_pokemon;
}

int pokedex_iniciar_datos(pokedex_t* pokedex, FILE* archivo){
	if(feof(archivo))
		return EXITO;

	char especie_pokemon[MAX_NOMBRE];
	char nombre[MAX_NOMBRE];
	char descripcion[MAX_DESCRIPCION];
	int numero;

	if(fscanf(archivo, "%[^;];%[^;];%i;%[^\n]\n", especie_pokemon, nombre, &numero, descripcion) !=4){
		return ERROR;
	}	
	while(!feof(archivo)){
		if(strcmp(especie_pokemon,"E") == 0){
			especie_pokemon_t* especie = iniciar_datos_especie(nombre, numero, descripcion);	
			if(!especie)
				return ERROR;

			lista_t* lista_pokemones = lista_crear(comparador_pokemon, destructor_pokemon);
			if(!lista_pokemones){
				free(especie);
				return ERROR;
			}
			
			especie->pokemones = lista_pokemones;
			if(fscanf(archivo, "%[^;];%[^;];%i;%[^\n]\n", especie_pokemon, nombre, &numero, descripcion) !=4){
				free(especie);
				free(lista_pokemones);
				return ERROR;
			}

			while(strcmp(especie_pokemon, "P") == 0 && (!feof(archivo))){
				particular_pokemon_t *pokemon = iniciar_datos_pokemon(nombre, numero, descripcion);
				if(!pokemon)
					return ERROR;
				if(lista_insertar(especie->pokemones, pokemon) == ERROR)
					return ERROR;
				if(fscanf(archivo, "%[^;];%[^;];%i;%[^\n]\n", especie_pokemon, nombre, &numero, descripcion) !=4)
					return ERROR;
			}	

			if(arbol_insertar(pokedex->pokemones, especie) == ERROR)
				return ERROR;

			if(strcmp(especie_pokemon, "P") != 0 && strcmp(especie_pokemon, "E") != 0)
				return ERROR;			
		}
		else
			return ERROR;
	}
	return EXITO;
}

pokedex_t* pokedex_prender(){
	FILE* archivo;
	archivo = fopen("pokedex.txt", "r");
	if(!archivo){
		printf("\nNo se encontro el archivo");
		return NULL;
	}
	
	char nombre_entrenador[MAX_NOMBRE];
	if(fscanf(archivo, "%s\n", nombre_entrenador) !=1){
		printf("\nEl archivo se encuentra vacio. Si desea crear uno nuevo, escriba el nombre del entrenador.\nPara volver al menu presiones 0\n");
		scanf("%s", nombre_entrenador);
		if(strcmp(nombre_entrenador, "0") == 0){
			fclose(archivo);
			return NULL;
		}
	}
	pokedex_t* pokedex = pokedex_crear(nombre_entrenador);
	inicializar_pokedex(pokedex);

	if(pokedex_iniciar_datos(pokedex, archivo) == ERROR){
		fclose(archivo);
		pokedex_destruir(pokedex);
		return NULL;
	}
	fclose(archivo);
	return pokedex;	
}