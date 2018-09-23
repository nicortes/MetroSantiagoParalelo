/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Funciones.cpp
 * Author: nicolas
 * 
 * Created on 8 de septiembre de 2018, 23:17
 */

#include "Funciones.h"

void mostrar_vector_enteros(int vector[cantidad_estaciones], int columnas){
    for(int i=0; i< columnas;i++){
        cout << vector[i] << "  ";
    }
}

void mostrar_matriz_enteros(int matriz[][3]/*int** matriz*/, int filas, int columnas){
    for (int i=0;i<filas;i++){
        for(int j=0;j<columnas;j++){
            cout<<matriz[i][j]<<"\t";
        }
        cout<<endl;
    }
}

void inicializar_vector_enteros(int vector[], int columnas){
    for (int i=0; i<columnas; i++){
        vector[i] = 0;
    }
}

void inicializar_matriz_enteros(int matriz[][cantidad_estaciones], int filas, int columnas){
    for (int i=0; i<filas-1; i++)
    {
        for (int j=0; j<columnas; j++){
            matriz[i][j] = 0;
        }
     }
}

void mostrar_vector_strings(string vector[cantidad_estaciones], int columnas){
    for(int i = 0; i< columnas;i++){
        cout << vector[i] << " - ";
    }
}

void split(string linea, char delimitador, string arreglo[]){
    int contador;
    string aux;
    contador = 0;
        aux = "";
        for(int i = 0; i < linea.length(); i++){
            
            if(linea[i] != delimitador){
                aux += linea[i];
            }
            else{
                switch(contador){
                    case 0:
                        arreglo[0] = aux;
                        break;
                    case 1:
                        arreglo[1] = aux;
                        break;
                    case 2:
                        arreglo[2] = aux;
                        break;
                    case 3:
                        arreglo[3] = aux;
                        break;
                }
                aux = "";
                contador++;
            }
        }
        arreglo[4] = aux;
}                

int cargar_archivo(string nombre, Estacion estaciones[]){

    int cantidad_lineas = 0;
    Estacion estacion;
    ifstream archivo;
    archivo.open(nombre);
    if (!archivo) {                                                                                                                                              
            cerr << "No se pudo abrir el archivo " << nombre << endl;
            exit(1);   // detenerse y salir
        }
    string linea;
    string datos[5];
    while(getline(archivo,linea)){
        
        if(linea[0] == '#'){
            continue;
        }
        
        cantidad_lineas++;
        split(linea,';',datos);
        
        estacion.numero = stoi(datos[0]);
        estacion.codigo = datos[1];
        estacion.nombre = datos[2];
        estacion.adyacente = stoi(datos[3]);
        estacion.combinacion = stoi(datos[4]);

        estaciones[estacion.numero] = estacion;
    }
    archivo.close();
    return cantidad_lineas;
}

void mostrar_estaciones(Estacion estacion[], int largo){
    
    for (int i = 1; i <= largo; i++){
        if(estacion[i].nombre != ""){
            cout << estacion[i].numero << ".-";
            cout << estacion[i].codigo << "->";
            cout << estacion[i].nombre << " " << endl;
            cout << "Adyacente: " << estacion[i].adyacente << endl;
            cout << "Combinacion: " << estacion[i].combinacion;
            cout << endl;
        }
    }
}

void imprimir_autores(){
    cout << endl;
    cout << "Aplicación desarrollada por:" << endl;
    cout << "- Nicolás Cortés." << endl;
    cout << "- Rodrigo Echeverría." << endl;
    cout << "- Daniel Espinoza." << endl;
    cout << endl;
}

int buscar_indice(string codigo, Estacion estaciones[], int largo){
    for (int i = 1; i <= largo; i++){
        if(estaciones[i].codigo == codigo){
            return estaciones[i].numero;
        }
    }
    return -1;
}

void copiar_arreglo_strings(string recipiente[],string arreglo[],int columnas){
    for(int i = 0 ; i < columnas ; i++){
        recipiente[i] = arreglo[i];
    }
}
