Algoritmos y programacion II - Catedra Mendez.
Marco Masciulli. DNI; 39804927. Padron: 105145.

podekex.c: En su totalidad, lo que hace es guardar en un unico archivo TXT (pokedex.txt) los distintos archivos que 
se le pasa (avistamientos.txt y evolucionar.txt).

linea 86 - pokedex_crear: Recibe como parametro el nombre del entrenador dueño de la pokedex. Reservamemoria para la pokedex, y le 
asigna el nombre. Devuelve la pokedex, de lo contrario NULL.

linea 95 - inicializar_pokedex: Recibe la pokedex. Reserva la memoria para los distintos campos del mismo(listas: ultimos_capturados, ultimos_vistos. Un arbol para los pokemones avistados). A su vez, inicilizalos comparadores y destructores de los mismos, que paso 
a explicar a continuacion:

linea 14 - comparador_pokemon: Recibe 2 elementos, la comparacion es del tipo "particular_pokemon_t".evalua que los nombre de ambos 
elementos recibidos sean iguales. Devuelve 0 si lo son, de lo contrario -1.

linea 26 - comparador_nivel: Recibe 2 elementos, la comparacion es del tipo "especie_pokemon_t". Compara losnumeros de los dos elementos recibidos. Devuelve 0 si son iguales, -1 si el pokemon1 es mayor al pokemon2.y 1 si pokemon2 es mayor a pokemon1.

linea 39 - no_destruye: Se creo para asignar un destructor que no libere la memoria de los elementos del programa.

linea 48 - destructor_pokemon: Recibe un pokemon. De existir, llama a la funcion "destruir_pokemon". Libera la memoria del mismo.

linea 59 - destructor_especie: Recibe una especie. De existir, llama a la funcion "destruir_especie". Libera la memoria del mismo.

linea 188 - pokedex_avistar: Llama a la funcion "insertar_especie" mientras la cantidad de elementos leidos sea igual a 6
y distinto de -1. De lo contrario, devuelve ERROR. - Insertar_especie: Reserva la memoria para la especie y pokemon que lee del 
archivo "avistamientos.txt". Si no lee 6 elementos o lee -1, devuelve la cantidad de elementos leidos. De lo contrario continua
con el programa, asignando los distintos datos a la especie y el pokemon. Si la especie no existia, reserva memoria para la
lista de pokemones del mismo. Si la especie ya existia, libera la memoria reservada para la especie.
Retorna la cantidad de leidos.	

linea 262 - pokedex_evolucionar: Llama a la funcion "evolucionar" mientras la cantidad de elementos leidos sea igual a 5 o 
distinto de -1. De lo contrario, devuelve ERROR. - Evolucionar: Lee el archivo. Si no lee 6 elementos, o lee -1. Devuelve la cantidad
de elementos leidos. Comienza buscando la especie, en caso de no ser encontrada, imprimi por pantalla "No se encontro a la especie"
y devuelve la cantidad de elementos leidos. Si encontro la especie, pasa a buscar al pokemon. En caso de el pokemon no sea encontrado,
o bien el pokemon no haya sido capturado, imprime por pantalla "No se puede evolucionar al pokemon". Luego el programa se fija
si la especie a la evoluciona el pokemon ya existe. De existir, agrega el pokemon a la lista. En caso de no existir, crea la especie,
reserva memoria para la lista y asigna al pokemon. Retorna la cantidad de elementos leidos.

linea 283 - pokedex_ultimos_capturados: Muestra la pila de los ultimos pokemones capturados. Imprime por pantalla los nombre, mientras que
"lista_vacia" sea false.

linea 298 - pokedex_ultimos_vistos: Muestra la cola de los ultimos pokemones vistos. Imprime por pantalla los nombres, mientras que 
"lista_via" sea false.

linea 337 - pokedex_informacion: Comienza buscando la especie, si no la encuetra, imprime por pantalla "No se encontro la especie" y retorna. En caso de encontrar la especie y que no se haya
pasado ningun nombre por parametro, el programa imprimira todos los pokemones encontrados en la lista de esa especie pokemon atraves de la 
funcion Especie_pokemones. Esta funcion recibe por parametro una especie, utliza un iterador interno el cual itera mientras que 
"lista_iterador_tiene_sigueinte" sea true. En caso de que sea haya pasado el nombre de un pokemon por parametro, lo buscara. En caso de no
encontrarlo, imprime por pantalla "No se encontro el pokemon" y  retorna. Por ultimo, si encuentra al pokemon, imprime los datos del mismo.

linea 383 - pokedex_destruir: Libera toda la memoria que se haya utlizado para crear el programa. Para las listas de los pokemones, implemente
una lista de listas. Osea, una lista que en cada nodo, apunta a una lista. Esto para liberar la memoria de las mismas. El resto, son "free"'s
y arbol_destruir.

linea 411 - pokedex_apagar: Crea el archivo "pokedex.txt", en caso de existir, lo sobreescribe. Llama a la funcion "abb_con_cada_elemento", al
cual se le pasa por parametro la funcion "guardar_pokemones". linea 65 - Guardar_pokemones: comienza guardando los datos de la especie. Luego
con un while, guardo los datos de la lista de pokemones de esa especie, hasta que la misma se quede sin pokemones por guardar. Devuelve false
en caso de tener exito, true en caso de no existir alguno de los datos pasado por parametro.

linea 502 - pokedex_prender: A partir de un archivo "pokedex.txt", crea una pokedex en memoria, con los datos que tiene el mismo, para esto
llama a la funcion "pokedex_iniciar_datos". linea 452 - Pokedex_iniciar_datos: recibe una pokedex y un archivo(Los cuales ya se sabe que existen, porque de lo contrario no se pudiera haber llamado a la funcion), lee el archivo, comienza leyendo la primera especie. En caso de no
leer 4 elementos, retorna ERROR. Luego, entra en un while, el cual funciona mientras que no sea el final del archivo. Se reserva memoria
para la especie, y se le pasan los datos de la misma por parametro a la funcion "iniciar_datos_especie", luego crea la lista para la misma.
Lee los datos de los pokemones del archivo, y por ultimo inserta la especie. En caso de haber un error, devolvera ERROR.

menu_pokedex.c: Menu iteractivo con el usuario para poder utilizar las distintas funciones de la pokedex, en el cual se utiizan todas las funciones de la pokedex. (A excepcion de pokedex_informacion, el cual hice mis propias funciones por una cuestion de gusto. De todas maneras
pokedex_informacion esta testeado y funciona correctamente).

linea 505- main: El main, crea las distntos booleanos que seran usados luego en el menu_pokedex: Recibe como parametros a la pokedex en cuestion y 4 booleanos, los cuales serviran a la hora de mostrar que funciones estan disponibles para usar, a su vez, sirven para que no se puedan utilizar las funciones fuera de momento, sino que solamente cuando estan hablitados. Asi corro menos riesgo de errores. De principio, el menu solamente deja utilizar dos opciones "iniciar pokedex" y "salir del programa". Una vez iniciada la pokedex, se activan las opciones de "guarda pokedex", "salir de la pokedex" (funcion la cual sale, liberando todo, pero no guarda nada. En caso de salir, se perderan todos los datos), "avistar un pokemon" el cual llama a pokedex_avistar. Una vez avistados los pokemones, se habilitan el resto de las opciones, "evolucionar" se habilita al mismo tiempo que avistar, "mostrar ultimos capturados", "mostrar ultimos vistos", "mostrar un pokemon particular", y "mostrar una especie". Una vez iniciada la pokedex, no se puede volver a iniciar. Una vez se hallan mostrado los capturados o bien los vistos, tampoco se podran volver a mostrar (A no ser que se avisten pokemones nuevamente). En caso de haber algun problema en algun momento del programa,este avisara el mismo y regresara al menu.