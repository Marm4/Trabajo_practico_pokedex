#ifndef __POKEDEX_H__
#define __POKEDEX_H__

#include "lista.h"
#include "abb.h"

#define MAX_NOMBRE 100
#define MAX_RUTA 100
#define MAX_DESCRIPCION 100

typedef struct especie_pokemon {
	int numero;
	char nombre[MAX_NOMBRE];
	char descripcion[MAX_DESCRIPCION];
	lista_t* pokemones;
} especie_pokemon_t;

typedef struct particular_pokemon {
	char nombre[MAX_NOMBRE];
	int nivel;
	bool capturado;
} particular_pokemon_t;

typedef struct pokedex {
	char nombre_entrenador[MAX_NOMBRE];
	lista_t* ultimos_capturados;
	lista_t* ultimos_vistos;
	abb_t* pokemones;
} pokedex_t;

/*
 * Crea la Pokedex, reservando la memoria necesaria para la misma.
 * Devolverá una referencia a la pokedex creada o NULL si no se pudo crear.
 */
pokedex_t* pokedex_crear(char entrenador[MAX_NOMBRE]);

/*
 * Función que dado un archivo, deberá cargar en la Pokedex a los
 * Pokémon que fueron tanto capturados como vistos. No verifica si
 * dos Pokémon de la misma especie tienen nombre repetido.
 *
 * Agrega a la pila correspondiente los Pokémon capturados.
 * Agrega a la cola correspondiente los Pokémon avistados.
 *
 * Devuelve 0 si tuvo éxito o -1 si se encuentra algún error durante el proceso.
 */
int pokedex_avistar(pokedex_t* pokedex, char ruta_archivo[MAX_RUTA]);

/*
 * Función que dado un archivo, deberá cargar en la Pokedex a los
 * Pokémon que evolucionaron.
 *
 * La evolución de Pokémon no afecta a la pila de capturados ni a la
 * cola de vistos. Un Pokémon que no está marcado como capturado no
 * puede evolucionar.
 *
 * Devuelve 0 si tuvo éxito o -1 si se encuentra algún error durante
 * el proceso (por ejemplo si no existe la especie, el Pokémon
 * específico o si el Pokémon no fue capturado).
 */
int pokedex_evolucionar(pokedex_t* pokedex, char ruta_archivo[MAX_RUTA]);

/*
 * Procedimiento que muestra los últimos Pokémon que fueron
 * capturados. Dichos Pokémon deberán ser listados del mas reciente
 * al menos reciente.
 *
 * Luego de invocar esta función, la pila de ultimos capturados queda
 * vacía.
 */
void pokedex_ultimos_capturados(pokedex_t* pokedex);

/*
 * Procedimiento que muestra los últimos Pokémon que fueron
 * vistos. Dichos Pokémon deberán ser listados del mas antiguo al
 * mas reciente.
 *
 * Luego de invocar esta función, la cola de últimos vistos queda
 * vacía.
 */
void pokedex_ultimos_vistos(pokedex_t* pokedex);

/*
 * Muestra la información del Pokémon recibido por parametros (especie
 * y nombre).  En caso de no existir el Pokémon o la especie, se
 * deberá mostrar un mensaje informando que es una especie o Pokémon
 * desconocidos..
 *
 * En caso de recibir un nombre vacío, se deben listar a todos los
 * Pokémon de esa especie.
 *
 */
void pokedex_informacion(pokedex_t* pokedex, int numero_pokemon, char nombre_pokemon[MAX_NOMBRE]);

/*
 * Destruye la estructura de la Pokedex, liberando la memoria que fue
 * reservada para la misma.
 */
void pokedex_destruir(pokedex_t* pokedex);

/*
 * Guarda la información de la pokedex a archivo pokedex.txt. La
 * información debe ser guardada en el formato especificado en el
 * enunciado del TP y adicionalmente deben guardarse de tal forma que
 * al prender la pokedex, la forma de árbol de especies tenga la misma
 * forma del árbol original.
 *
 * Devuelve 0 en caso de éxito o -1 si hubo algún error.
 */
int pokedex_apagar(pokedex_t* pokedex);

/*
 * Carga la información de la pokedex del archivo pokedex.txt.
 *
 * No se piden validaciones sobre el formato del archivo ya que
 * suponemos que fueron guardados correctamente mediante la función
 * pokedex_apagar.
 *
 * Devuelve la pokedex creada desde el archivo o NULL en caso de error.
 */
pokedex_t* pokedex_prender();

#endif /* __POKEDEX_H__ */
