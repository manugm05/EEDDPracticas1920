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
#include "ListaDEnlazada.h"
#include "Iterator.h"

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
                Cliente c(dni, pass, nombre, direccion, dlat, dlon);
                v.insertar(c);
                //                cout << "leido cliente " << total << "  ";
            }
        }

        cout << "Total de clientes en el fichero: " << total << endl;
        fe.close(); //Cerramos el flujo de entrada
    } else {
        cerr << "No se puede abrir el fichero" << endl;
    }
}

/**
 * @brief busqueda de elementos segun nombre.
 * @param nombre - string
 * @param v - VDinamico
 * @pre v debe estar instanciado en T=Cliente
 * @return Número de veces que se borra un cliente con el nombre dado
 */
int busquedaPorNombre(std::string nombre, VDinamico<Cliente>& v) {
    Cliente aux = Cliente(nombre);
    int pos = 0;
    int cont = 0;
    pos = v.busquedaBin(aux);
    while (pos != -1) {
        v.borrar(pos);
        cont++;
        pos = v.busquedaBin(aux);
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

/**
 * @brief Búsqueda de latitudes y longitudes minimas y maximas.
 * @param v - Parámetro de entrada tipo VDinamico<Cliente>.
 * @param min - Parámetro de salida tipo UTM. Se guardan las coordenadas mínimas.
 * @param max - Parámetro de salida tipo UTM. Se guardan las coordenadas máximas.
 * @throw std::invalid_argument si el vector esta vacio.
 */
void limites(VDinamico<Cliente>& v, UTM& min, UTM& max) {
    if(v.getTam() == 0) throw std::invalid_argument("[!] Main.cpp limites(): El vector no tiene elementos.");
    double latmax, longmax, latmin, longmin;
    latmax = 0;
    longmax = 0;
    latmin = v[0].getPosicion().latitud;
    longmin = v[0].getPosicion().longitud;
    for (int i = 0; i < v.getTam(); i++) {
        if (latmax < v[i].getPosicion().latitud)
            latmax = v[i].getPosicion().latitud;
        if (longmax < v[i].getPosicion().longitud)
            longmax = v[i].getPosicion().longitud;
        if (latmin > v[i].getPosicion().latitud)
            latmin = v[i].getPosicion().latitud;
        if (longmin > v[i].getPosicion().longitud)
            longmin = v[i].getPosicion().longitud;
    }
    max = UTM(latmax, longmax);
    min = UTM(latmin, longmin);

}

int main(int argc, char** argv) {

    VDinamico<Cliente> vec;
    leeClientes("clientes_v2.csv", vec);
    UTM min, max;
    try {
        limites(vec, min, max);
        cout << "Los iterenarios se generan desde (" << min.latitud << "," << min.longitud << ") hasta (" << max.latitud << "," << max.longitud << ").\n";
        int id = 0, n = 20;
        for (int i = 0; i < vec.getTam(); i++) {
            id = n*i;
            vec[i].crearItinerarios(n, id, min, max);
        }
        vec.exportaCSV();
    } catch (std::out_of_range& err) {
        cerr << err.what();
    } catch (std::invalid_argument& err) {
        cerr << err.what();
    } catch (std::domain_error& err) {
        cerr << err.what();
    } catch (std::exception& err) {
        cerr << "Ha habido un error inseperado.";
    }
    return 0;
}

