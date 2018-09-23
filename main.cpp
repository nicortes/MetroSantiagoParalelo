/* 
 * File:   main.cpp
 * Author: nicolas
 *
 * Created on 8 de septiembre de 2018, 16:35
 */

    /*Cantidad de vértices: 107 estaciones reales + 11 repetidas = 118.
     Linea 1: 27 estaciones.
     Linea 2: 22 estaciones.
     Linea 4: 23 estaciones.
     Linea 4a: 6 estaciones.
     Linea 5: 30 estaciones.
     Linea 6: 10 estaciones.

    Cantidad de aristas: 101 aristas reales + 11 para las combinaciones = 112.
     Linea 1: 26 aristas.
     Linea 2: 21 aristas.
     Linea 4: 22 aristas.
     Linea 4a: 5 aristas.
     Linea 5: 29 aristas.
     Linea 6: 9 aristas.

    Cantidad de combinaciones: 5 + 4 + 3 + 2 + 5 + 3 combinaciones reales + 11 repetidas = 33.
     Linea 1: San Pablo, Los Héroes, Baquedano, Los Leones, Tobalaba (5).
     Linea 2: La Cisterna, Franklin, Los Héroes, Santa Ana (4).
     Linea 4: Vicente Valdés, Vicuña Mackenna, Tobalaba (3).
     Linea 4a: La Cisterna, Vicuña Mackenna (2).
     Linea 5: Vicente Valdés, Ñuble, Baquedano, Santa Ana, San Pablo (5).
     Linea 6: Franklin, Ñuble, Los Leones (3).
     * Únicos: San Pablo, Los Héroes, Baquedano, Los Leones, Tobalaba, La Cisterna, Franklin
     * Santa Ana, Vicente Valdés, Vicuña Mackenna, Ñuble (11).
     */

#include "Funciones.h"
#include </usr/include/mpi/mpi.h>

//lista de adyacencia. 
//Node es un par <int numero_estacion, peso_estacion>. first = numero; second = peso
//vector< Node > ady[ max ];

/* ENTENDIENDO EL CÓDIGO:
 * LOS VECTORES SON ARREGLOS DINÁMICOS, DONDE NO SE ESPECIFICA EL TAMAÑO POR DEFECTO
 * vector<int> a : DECLARA UN VECTOR DE ENTEROS LLAMADO a
 * vector<int> a(10) : DECLARA UN VECTOR DE ENTEROS LLAMADO a CON 10 ESPACIOS INICIALIZADOS EN CERO
 * vector<int> a[10] : DECLARA UN ARREGLO LLAMADO a CONFORMADO POR 10 VECTORES DE ENTEROS*/

// REEMPLAZAMOS EL ARREGLO DE VECTORES DE NODOS POR UNA MATRIZ
/* EJEMPLO:
 *    0 1  2  3
 * 0|
 * 1| 2 87
 * 2| 3
 * 3| 4
 * LA FILA INDICA LA ESTACIÓN, LAS COLUMNAS INDICAN SUS ADYACENCIAS
 EN EL EJEMPLO: LA ESTACIÓN 1 ES ADYACENTE CON 2 Y 87
 LA ESTACIÓN 2 ES ADYACENTE CON 3 Y LA ESTACIÓN 3 ES ADYACENTE CON 4 
 */

int matriz_ady[cantidad_estaciones][3];
//int **matriz_ady;

//distancia[ u ] distancia de vértice inicial a vértice con ID = u
int distancia[ max ];

//para vértices visitados
bool visitado[ max ];

//priority queue propia del c++, usamos el comparador definido 
//para que el de menor valor este en el tope
priority_queue< Node , vector<Node> , cmp > Q;
//priority_queue< int, vector<int>, cmp > Q;

//numero de vertices
int V = cantidad_estaciones;

//para la impresion de caminos
int previo[ max ];

// VARIABLES PARA USAR LOS ARGUMENTOS FUERA DEL MAIN
int cantidad_argumentos;
char** argumentos;

// VARIABLES PARA MPI
int rango;
int procesos;
int rango_origen;
int rango_destino;
int etiqueta;
MPI_Status estado;

/**
 * Función de inicialización.
 */
void init(){
    for( int i = 0 ; i <= V ; ++i ){
        distancia[ i ] = INF;  //inicializamos todas las distancias con valor infinito
        visitado[ i ] = false; //inicializamos todos los vértices como no visitados
        previo[ i ] = -1;      //inicializamos el previo del vertice i con -1
    }
}

/**
 * Paso de relajación. Prioriza la distancia más corta entre un vértice y su adyacente.
 * @param actual Vértice actual.
 * @param adyacente Vértice adyacente.
 * @param peso Peso de la arista.
 */
void relajacion( int actual , int adyacente , int peso ){
    //Si la distancia entre el origen y el vertice actual
    //+ el peso del vertice adyacente
    //es menor a la distancia del origen al vertice adyacente
    if( distancia[ actual ] + peso < distancia[ adyacente ] ){
        distancia[ adyacente ] = distancia[ actual ] + peso;  //relajamos el vertice actualizando la distancia
        previo[ adyacente ] = actual;                         //a su vez actualizamos el vertice previo
        Q.push( Node( adyacente , distancia[ adyacente ] ) ); //agregamos adyacente a la cola de prioridad
    }
}

/**
 * Impresion del camino mas corto desde el vertice inicial y final ingresados
 * @param destino Vértice de destino.
 * @param estaciones Arreglo con las struct de las estaciones.
 * @param ruta Arreglo que almacenará los nombre de las estaciones.
 * @param index Contador de estaciones en la ruta.
 * @param previo2 Arreglo con las estaciones previas de cada estación.
 * @return 
 */
int print(int destino, Estacion estaciones[], string ruta[], int index, int previo2[]){
    
    ruta[index] = estaciones[destino].nombre;
    index++;
    //cout << "previo2[destino]=" << previo2[destino] << endl;
    if( previo2[ destino ] != -1 ){//si aun poseo un vertice previo
        //cout << "ENTRÉ AL IF" << endl;
        return 1 + print(previo2[destino],estaciones,ruta,index,previo2);//recursivamente sigo explorando
    //printf("%d " , destino );//terminada la recursion imprimo los vertices recorridos
    }
}

/**
 * Aplicación del algoritmo de dijkstra.
 * @param inicial Vértice inicial.
 * @param rec_ady Matriz de adyacencias y combinaciones con los números de los vértices.
 */
void dijkstra(int inicial, int rec_ady[][3]){
    
    bool primera_vez = true;
    init(); //inicializamos nuestros arreglos
    //Q.push( Node( inicial , 0 ) ); //Insertamos el vértice inicial en la Cola de Prioridad
    Q.push( Node(inicial,0) );
    distancia[ inicial ] = 0; //Este paso es importante, inicializamos la distancia del inicial como 0
    int actual , adyacente , peso;
    while( !Q.empty() ){
        //cout << "rango: " <<  rango << endl; // CON ESTO COMPROBAMOS QUE LOS PROCESADORES 1 Y 2 ESTÁN EJECUTANDO DIJKSTRA AL MISMO TIEMPO
        actual = Q.top().first; //Obtengo de la cola el nodo con menor peso, en un comienzo será el inicial
        Q.pop(); //Sacamos el elemento de la cola
        //cout << "actual=" << actual << endl;
        //cout << "visitado[actual]=" << visitado[actual] << endl;
        if( visitado[ actual ] ) continue; //Si el vértice actual ya fue visitado entonces sigo sacando elementos de la cola
        
        visitado[ actual ] = true; //Marco como visitado el vértice actual

        for( int i = 0 ; i < 3; ++i ){ //reviso sus adyacentes del vertice actual
            // EL PROCESADOR 1 EMPIEZA CON 0 Y 2 (ADYACENTE POR UN LADO Y COMBINACIÓN)
            if( (rango == 1) and (i == 1) and (primera_vez == true) ){ 
                continue;
            }
            // EL PROCESADOR 2 EMPIEZA CON 1 (ADYACENTE POR EL OTRO LADO)
            if( (rango == 2) and ( (i == 0) or (i == 2) ) and (primera_vez == true) ){ 
                continue;
            }
            adyacente = rec_ady[ actual ][ i ]; //id del vertice adyacente
            //cout << "adyacente=" << adyacente << endl;
            if(adyacente != 0){
                peso = 1; //peso de la arista que une actual con adyacente ( actual , adyacente )
                if( !visitado[ adyacente ] ){ //si el vertice adyacente no fue visitado
                    relajacion( actual , adyacente , peso ); //realizamos el paso de relajacion
                }
            }
        }
        primera_vez = false;
    }
}

/**
 * Función asigna los arreglos para que sean contiguos
 * @param rows Filas de la matriz
 * @param cols Columnas de la matriz
 * @return  Arreglo con memoria asignada
 */
int **alloc_2d_int(int rows, int cols) {
    int *data = (int *)malloc(rows*cols*sizeof(int));
    int **array= (int **)malloc(rows*sizeof(int*));
    for (int i=0; i<rows; i++)
        array[i] = &(data[cols*i]);

    return array;
}

/**
 * Main
 * @param argc Cantidad de argumentos ingresados.
 * @param argv Arreglo con los argumentos.
 * @return 0.
 */
int main(int argc, char** argv) {
    
    cantidad_argumentos = argc;
    argumentos = argv;
    
    MPI_Init(&cantidad_argumentos,&argumentos);
    
    MPI_Comm_rank(MPI_COMM_WORLD,&rango);
    MPI_Comm_size(MPI_COMM_WORLD,&procesos);
    //MPI_Status estados[2];
    MPI_Status estado;  
    
    if(procesos > 3){
        cout << endl << "Este programa funciona con 3 procesadores: mpirun -np 2 ./programa -f origen destino" << endl;
        MPI_Finalize();
        exit(1);
    }
    
    /// PROCESO MAESTRO
    // LEE EL ARCHIVO, GENERA EL ARREGLO DE STRUCT
    // GENERA MATRIZ DE ADYACENCIAS Y  COMBINACIONES
    // ENVÍA LA MATRIZ Y A CAMBIO RECIBE RUTAS
    // EVALÚA CUÁL DE LAS DOS RUTAS LLEGA AL DESTINO Y ES MÁS CORTA
    // IMPRIME MENSAJES DE AYUDA, IMPRIME RUTA ÓPTIMA
    if(rango == 0){
        
        if (argc < 2){
	      cout << endl;
	      cout << "Forma de uso:" << endl;
	      cout << "Ver integrantes: ./programa -v" << endl;
	      cout << "Encontrar ruta más corta: mpirun -np 2 ./programa -f [CODIGO_ESTACION_ORIGEN] [CODIGO_ESTACION_DESTINO]" << endl;
              cout << endl;
	         exit(1);
        }
    
        string parametro = argv[1];

        if(parametro == "-v"){
            imprimir_autores();
        }

        else{

            if(parametro == "-f"){

                // Se declara el arreglo que contendrá las estaciones
                Estacion estaciones[cantidad_estaciones];

                // Se lee el archivo y se llena el arreglo
                int cantidad_lineas = cargar_archivo("archivo.txt",estaciones);

                int largo = cantidad_lineas;

                /* CÓDIGO PARA MOSTRAR LAS ESTACIONES
                 * mostrar_estaciones(estaciones,largo);
                */ 

                int peso = 1; // Se asume igual distancia entre estaciones

                // La cantidad de aristas es la cantidad de estaciones -1 por cada línea
                //int E = cantidad_estaciones - 6;

                int num_origen, num_destino;
                int num_inicial, num_final;
                
                //matriz_ady = alloc_2d_int(cantidad_estaciones,4);

                // INICIALIZANDO MATRIZ DE ADYACENCIAS
                for(int i = 0; i < cantidad_estaciones; i++){
                    for(int j = 0; j < 2; j++){
                        matriz_ady[i][j] = 0;
                    }
                }
                
                // Se ingresan las estaciones guardadas para poder aplicar dijkstra
                for (int i = 1; i <= cantidad_lineas; i++){

                    if(estaciones[i].adyacente != 0){

                        // Se guarda el número asignado a las estaciones
                        num_origen = estaciones[i].numero;
                        num_destino = estaciones[i].adyacente;

                        // Se ingresan al vector
                        //ady[ num_origen ].push_back( Node( num_destino , peso ) );
                        //ady[ num_destino ].push_back( Node( num_origen , peso ) );
                        
                        // matriz_ady[estacion_actual][estaciones_adyacentes]
                        // LA FILA INDICA LA ESTACIÓN
                        // LA COLUMNA 0 INDICA LA ESTACIÓN ADYACENTE EN UNA DIRECCIÓN
                        // LA COLUMNA 1 INDICA LA ESTACIÓN ADYACENTE EN LA OTRA DIRECCIÓN
                        // AMBAS ESTACIONES SON ADYACENTES ENTRE SÍ, POR ESO SE AGREGA A AMBAS
                        if(matriz_ady[num_origen][0] == 0){
                            matriz_ady[num_origen][0] = num_destino;
                        }
                        else{//if(matriz_ady[num_origen][0] != 0)
                            matriz_ady[num_origen][1] = num_destino;
                        }
                        
                        if(matriz_ady[num_destino][0] == 0){
                            matriz_ady[num_destino][0] = num_origen;
                        }
                        else{//if(matriz_ady[num_destino][0] != 0)
                            matriz_ady[num_destino][1] = num_origen;
                        }
                    }

                    if(estaciones[i].combinacion != 0){
                        num_origen = estaciones[i].numero;
                        num_destino = estaciones[i].combinacion;
                        //ady[ num_origen ].push_back( Node( num_destino , peso ) );
                        //ady[ num_destino ].push_back( Node( num_origen , peso ) );
                        
                        // LA FILA INDICA LA ESTACIÓN
                        // LA COLUMNA 2 INDICA LA COMBINACIÓN
                        // UNA COMBINACIÓN ES UNA ESTACIÓN DEL MISMO NOMBRE, PERO DE UNA LÍNEA DIFERENTE
                        // AMBAS ESTACIONES SON ADYACENTES ENTRE SÍ, POR ESO SE AGREGA A AMBAS
                        matriz_ady[num_origen][2] = num_destino;
                        matriz_ady[num_destino][2] = num_origen;
                    }
                }

                /* CÓDIGO PARA COMPROBAR QUE LA MATRIZ SE HAYA CREADO BIEN
                mostrar_matriz_enteros(matriz_ady,cantidad_estaciones,3);
                int a;
                cout << "ingrese 1 para salir" << endl;
                cin >> a;
                if(a==1){
                    MPI_Finalize();
                    exit(1);
                }*/
                
                num_inicial = buscar_indice(argv[2],estaciones,largo);
                num_final = buscar_indice(argv[3],estaciones,largo);

                // SE ENVÍA EL VÉRTICE INICIAL AL PROCESADOR 1, PARA QUE APLIQUE DIJKSTRA
                MPI_Send(&num_inicial,2,MPI_INT,1,0,MPI_COMM_WORLD);
                // SE ENVÍA EL VÉRTICE INICIAL AL PROCESADOR 2, PARA QUE APLIQUE DIJKSTRA
                MPI_Send(&num_inicial,2,MPI_INT,2,0,MPI_COMM_WORLD);
                
                // SE ENVÍA LA MATRIZ DE ADYACENCIAS Y COMBINACIONES AL PROCESADOR 1
                MPI_Send(&(matriz_ady[0][0]),cantidad_estaciones*3,MPI_INT,1,1,MPI_COMM_WORLD);
                // SE ENVÍA LA MATRIZ DE ADYACENCIAS Y COMBINACIONES AL PROCESADOR 2
                MPI_Send(&(matriz_ady[0][0]),cantidad_estaciones*3,MPI_INT,2,1,MPI_COMM_WORLD);
                
                int previo1[max];
                int previo2[max];
                //previo2 = (int [])malloc(max * sizeof(int));

                // RECIBE EL ARREGLO CON LOS VÉRTICES DE RUTA ÓPTIMA
                MPI_Recv(previo1,max,MPI_INT,1,2,MPI_COMM_WORLD,&estado);
                // RECIBE EL ARREGLO CON LOS VÉRTICES DE RUTA ÓPTIMA
                MPI_Recv(previo2,max,MPI_INT,2,2,MPI_COMM_WORLD,&estado);
                
                /*for(int h = 0; h < cantidad_estaciones; h++){
                    cout << "previo1[" << h << "]: " << previo1[h] << endl;
                }
                
                for(int h = 0; h < cantidad_estaciones; h++){
                    cout << "previo2[" << h << "]: " << previo2[h] << endl;
                }*/

                // RECIBE EL ok PARA CONTINUAR
                int ok1;
                int ok2;
                MPI_Recv(&ok1,1,MPI_INT,1,3,MPI_COMM_WORLD,&estado);
                MPI_Recv(&ok2,1,MPI_INT,2,3,MPI_COMM_WORLD,&estado);
                
                if(ok1 == 1 and ok2 == 1){
                    
                    string o = estaciones[num_inicial].nombre;
                    string d = estaciones[num_final].nombre;
                    
                    string ruta1[cantidad_estaciones];
                    string ruta2[cantidad_estaciones];
                    int index1 = 0;
                    int index2 = 0;
                
                    // AGREGA LOS NOMBRES DE LAS ESTACIONES A ruta1 Y RETORNA LA CANTIDAD DE ELEMENTOS
                    int indice1 = print(num_final,estaciones,ruta1,index1,previo1);
                    
                    // AGREGA LOS NOMBRES DE LAS ESTACIONES A ruta2 Y RETORNA LA CANTIDAD DE ELEMENTOS
                    int indice2 = print(num_final,estaciones,ruta2,index2,previo2);
                    
                    string ruta[cantidad_estaciones];
                    int indice;
                    
                    // CON ESTO COMPROBAMOS QUE LOS CAMINOS SON DISTINTOS, JUSTIFICANDO QUE SE COMPARE Y PREFIERA EL MÁS CORTO
                    //cout << "indice1=" << indice1 << endl;
                    //cout << "indice2=" << indice2 << endl;
                    
                    if(indice1 == -1){
                        indice1 = 999;
                    }
                    
                    if(indice2 == -1){
                        indice2 = 999;
                    }
                    
                    if(indice1 <= indice2){
                        indice = indice1;
                        copiar_arreglo_strings(ruta,ruta1,cantidad_estaciones);
                    }

                    if(indice2 <= indice1){
                        indice = indice2;
                        copiar_arreglo_strings(ruta,ruta2,cantidad_estaciones);
                    }
                    
                    cout << endl << "************** Impresión de camino más corto **************" << endl;

                    cout << "Origen: " << o << " -> " << "Destino: " << d << endl;

                    cout << endl << "Ruta más directa: ";

                    // IMPRIME RUTA ÓPTIMA
                    if(indice != 999){
                        for(int j = indice + 1; 0 <= j; j--){
                            //cout << "ruta[" << j << "]=" << ruta[j] << endl;
                            if(ruta[j] != ruta[j+1]){
                                cout << ruta[j];
                                if(j != 0){
                                    cout << " - ";
                                }
                            }
                        }  
                    }
                    
                    cout << endl;

                    /*for(int j = 0; j < 10; j++ ){
                        cout << endl << ruta[j] << endl;
                    }*/

                    cout << endl;
                    }
            }
            else{
                   cout << endl;
                   cout << "Argumentos incorrectos." << endl;
                   cout << endl;
                 }
        }
    }
    
    /// PROCESSO ESCLAVOS (1 Y 2)
    // APLICAN DIJKSTRA EN CAMINOS DIFERENTES
    // DEVUELVEN UN ARREGLO CADA UNO, CON LOS VÉRTICES DE SU RUTA ÓPTIMA
    if(rango != 0){
        if (argc < 2){
            MPI_Finalize();
            exit(1);
        }
        
        string parametro = argv[1];

        if(parametro == "-v"){
            MPI_Finalize();
            exit(1);
        }
        
        // COMIENZO MPI
        
        int rec_ini;
        //int** rec_matriz_ady;
        //rec_matriz_ady = alloc_2d_int(cantidad_estaciones,2);
        int rec_matriz_ady[cantidad_estaciones][3];
        
        // RECIBE EL VÉRTICE DE INICIO PARA APLICAR DIJKSTRA
        MPI_Recv(&rec_ini,2,MPI_INT,0,0,MPI_COMM_WORLD,&estado);
        
        // RECIBE LA MATRIZ DE ADYACENCIAS Y COMBINACIONES PARA APLICAR DIJKSTRA
        MPI_Recv(&(rec_matriz_ady[0][0]),cantidad_estaciones*3,MPI_INT,0,1,MPI_COMM_WORLD,&estado);
        
        /* CÓDIGO PARA COMPROBAR QUE LA MATRIZ SE HAYA CREADO BIEN
        mostrar_matriz_enteros(rec_matriz_ady,cantidad_estaciones,3);
        int a;
        cout << "ingrese 1 para salir" << endl;
        cin >> a;
        if(a==1){
            exit(1);
        }*/
        
        // APLICA EL ALGORITMO DE DIJKSTRA
        dijkstra(rec_ini,rec_matriz_ady);
        
        /*for(int z = 0; z < cantidad_estaciones; z++){
            cout << "previo[" << z << "]=" << previo[z] << endl;
        }*/
        
        
        /* ESTE ERROR NO SÉ EVITARLO, PERO SE SOLUCIONA MATANDO LOS PROCESOS ZOMBI (ps -fea | grep "[p]rograma Y DESPUÉS kill -9  A LOS PID")
         * In file included from <command-line>:0:0:                                                      
         * /usr/include/stdc-predef.h:1:0: fatal error: 
         * can’t create precompiled header programa: El archivo de texto está ocupado
         */
        
        /* CÓDIGO PARA VER SI SE CREÓ BIEN LA RUTA
        for(int h = 0; h < 3; h++){
            cout << "previo: " << previo[h] << endl;
        }
        int a;
        cout << "ingrese 1 para salir" << endl;
        cin >> a;
        if(a==1){
            MPI_Finalize();
            exit(1);
        }*/
        
        // LE ENVÍA AL PROCESADOR 0 EL ARREGLO previo[max]
        MPI_Send(previo,max,MPI_INT,0,2,MPI_COMM_WORLD);
        
        // LE DECIMOS AL PROCESADOR 0 QUE YA PUEDE IMPRIMIR LA RUTA
        int ok = 1;
        MPI_Send(&ok,1,MPI_INT,0,3,MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
    
    return 0;
}