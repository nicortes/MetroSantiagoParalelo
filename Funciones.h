/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Funciones.h
 * Author: nicolas
 *
 * Created on 8 de septiembre de 2018, 23:17
 */

#include <iostream>
#include <stdio.h>
#include <vector>
#include <queue>
#include <fstream>
#include <string>

using namespace std;

#define cantidad_estaciones 119
#define max 10005 //maximo numero de vértices
#define Node pair< int , int > //definimos el nodo como un par( first , second ) donde first es el vertice adyacente y second el peso de la arista
#define INF 1<<30 //definimos un valor grande que represente la distancia infinita inicial, basta conque sea superior al maximo valor del peso en alguna de las aristas

/* La cola de prioridad de C++ por default es un max-Heap 
 * (elemento de mayor valor en el tope)
 * por ello es necesario realizar nuestro comparador para que sea un min-Heap
*/ 
struct cmp {
    bool operator() ( const Node &a , const Node &b ) {
        return a.second > b.second;
    }
};

struct comparador{
    bool operator() (const int &a, const int &b){
        return a > b;
    }
};

// Estructura de una estación
struct Estacion{
    int numero;
    string codigo;
    string nombre;
    int adyacente;
    int combinacion;
};

#ifndef FUNCIONES_H
#define FUNCIONES_H

/**
 * Muestra los contenidos de un arreglo unidimensional de enteros.
 * @param vector Arreglo unidimensional de enteros.
 * @param columnas Cantidad de casillas a mostrar.
 */
void mostrar_vector_enteros(int[], int);

/**
 * Muestra los contenidos de una matriz de enteros.
 * @param matriz Arreglo bidimensional de enteros.
 * @param filas Cantidad de filas.
 * @param columnas Cantidad de columnas.
 */
void mostrar_matriz_enteros(int[][3]/*int***/, int, int);

/**
 * Rellena un arreglo unidimensional de enteros con ceros.
 * @param vector Arreglo unidimensional de enteros.
 * @param columnas Cantidad de casillas a inicializar.
 */
void inicializar_vector_enteros(int[], int);

/**
 * Rellena una matriz de enteros con ceros.
 * @param matriz Arreglo bidimensional de enteros.
 * @param filas Cantidad de filas a inicializar.
 * @param columnas Cantidad de columnas a inicializar.
 */
void inicializar_matriz_enteros(int[][cantidad_estaciones], int, int);

/**
 * Muestra el contenido de un arreglo de strings.
 * @param vector Arreglo de strings.
 * @param columnas Cantidad de casillas a imprimir.
 */
void mostrar_vector_strings(string[], int);

/**
 * Lee una línea del archivo y guarda los datos de la estación en un arreglo.
 * @param linea Línea leída del archivo.
 * @param delimitador Caracter que separa los datos.
 * @param arreglo Arreglo que contendrá los datos separados en casillas.
 */
void split(string, char, string[]);

/**
 * Lee un archivo con los datos de las estaciones y las ingresa a un arreglo.
 * @param archivo Nombre del archivo.
 * @param estaciones Arreglo de las estaciones.
 * @return Cantidad de estaciones ingresadas.
 */
int cargar_archivo(string, Estacion[]);

/**
 * Muestra la información de cada estación.
 * @param estaciones Arreglo con las estaciones.
 * @param largo Cantidad de estaciones a imprimir.
 */
void mostrar_estaciones(Estacion[], int);

/**
 * Muestra en pantalla los nombres de los desarrolladores.
 */
void imprimir_autores();

/**
 * Utiliza el código de una estación para retornar su posición en el arreglo.
 * @param codigo Código a buscar en el arreglo.
 * @param estaciones Arreglo con las estaciones.
 * @param largo Cantidad de estaciones.
 * @return Posición de la estación en el arreglo.
 */
int buscar_indice(string, Estacion[], int);

/**
 * Copia el contenido de un arreglo de strings a otro.
 * @param recipiente Arreglo que será modificado.
 * @param arreglo Arreglo a copiar.
 * @param columnas Cantidad de strings a copiar de un arreglo a otro.
 */
void copiar_arreglo_strings(string[],string[],int);

#endif /* FUNCIONES_H */
