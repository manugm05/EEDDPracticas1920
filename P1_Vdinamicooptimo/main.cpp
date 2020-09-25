/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: lidia
 *
 */

#include <cstdlib>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <complex>
#include "VDinamico.h"
#include "Cliente.h"
#include "UTM.h"


using namespace std;

void leeClientes(string fileNameClientes, VDinamico<Cliente>& v) {
    ifstream fe; //Flujo de entrada
    string linea; //Cada línea tiene un clienete
    int total = 0; //Contador de líneas o clientes

    //Variables auxiliares para almacenar los valores leídos
    string dni, nombre, pass, direccion, latitud, longitud;
    double dlat, dlon;



    //Asociamos el flujo al fichero 
    fe.open(fileNameClientes);

    if (fe.good()) {
        //Mientras no se haya llegado al final del fichero
        while (!fe.eof()) {
            getline(fe, linea); //Toma una línea del fichero
            stringstream ss; //Stream que trabaja sobre buffer interno         

            if (linea != "") {
                ++total;
            }

            if (total > 1) {
                //Inicializamos el contenido de ss
                ss << linea;

                //Leemos el NIF
                getline(ss, dni, ';'); //El carácter ; se lee y se elimina de ss

                //Leemos el pass
                getline(ss, pass, ';'); //El caráter ; se lee y se elimina de ss

                //Leemos el nombre
                getline(ss, nombre, ';'); //El caráter ; se lee y se elimina de ss

                //Leemos la dirección
                getline(ss, direccion, ';'); //El caráter ; se lee y se elimina de ss

                //Leemos la latitud y longitud
                getline(ss, latitud, ';'); //El caráter ; se lee y se elimina de ss
                getline(ss, longitud, ';'); //El caráter ; se lee y se elimina de ss

                dlat = stod(latitud);
                dlon = stod(longitud);

                //con todos los atributos leídos, se crea el cliente
                v.insertar(Cliente(dni, pass, nombre, direccion, dlat, dlon));
                cout << "leido cliente " << total << "  ";
            }
        }

        cout << "Total de clientes en el fichero: " << total << endl;
        fe.close(); //Cerramos el flujo de entrada
    } else {
        cerr << "No se puede abrir el fichero" << endl;
    }
}

/**
 * @Brief busqueda de elementos segun nombre.
 * @param nombre - string
 * @param v - VDinamico
 * @pre v debe estar instanciado en T=Cliente
 * @return Número de veces que se borra un cliente con el nombre dado
 */
int busquedaPorNombre(std::string nombre, VDinamico<Cliente>& v){
    Cliente aux = Cliente(nombre);
    int pos = 0;
    int cont = 0;
    pos=v.busquedaBin(aux);
    while (pos != -1){
            v.borrar(pos);
            cont++;
            pos=v.busquedaBin(aux);
    }
    return cont;
}

/**
 * @brief Cálculo de la distancia máxima entre dos clientes de un vector.
 * @param v - Objeto de tipo VDinamico<Cliente>
 * @return distanciaMax - Dato de doble precisión que almacena la distancia pedida.
 */
double distanciaMaxima(VDinamico<Cliente>& v) {
    double distanciaMax = 0;
    for (int i = 0; i < v.getTam(); i++) {
        for (int j = i + 1; j < v.getTam(); j++) {
            double distancia = v[i].distancia(v[j]);
            if (distanciaMax <= distancia) distanciaMax = distancia;
        }
    }
    return distanciaMax;
}


int main(int argc, char** argv) {
    VDinamico<Cliente> v;
    string nombre = "Francesco";

    try{
    leeClientes("clientes_v2.csv", v);
    cout << "==LECTURA INICIAL==\n";
    v.lee();

    cout << "\n==LECTURA DESPUES DE BORRADO==\n";
    v.lee();

    VDinamico<Cliente> ordenado = v.ordenar();
    cout << "\n==LECTURA DEL VECTOR ORDENADO==\n";
    ordenado.lee();

    cout << "\n\n==== BUSQUEDA DE DISTANCIA MAXIMA ====\n";
    cout << "La maxima distancia entre 2 clientes del vector es: " << distanciaMaxima(ordenado);

    cout << "\n\n==== TEST DE BORRADO POR NOMBRE ====\n";
    cout << "Tamaño anterior al borrado: " << ordenado.getTam();
    cout << "\nSe han eliminado " << busquedaPorNombre(nombre,ordenado) << " clientes cuyo nombre es: " << nombre;
    cout << "\nTamaño despues del borrado: " << ordenado.getTam();
    } catch (std::invalid_argument& ex) {
        std::cerr << ex.what();
    } catch (std::out_of_range& ex){
        std::cerr << ex.what();
    } catch (std::exception& ex){
        std::cerr << "Se ha producido una excepcion";
    }
    



    return 0;
}

