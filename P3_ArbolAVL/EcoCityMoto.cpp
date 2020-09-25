/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EcoCityMoto.cpp
 * Author: Samuel
 * 
 * Created on 20 de octubre de 2019, 16:29
 */

#include "fecha.h"
#include "EcoCityMoto.h"
#include "Moto.h"
#include "Cliente.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

EcoCityMoto::EcoCityMoto() : idUltimo(0) {
}

EcoCityMoto::EcoCityMoto(const EcoCityMoto& orig) {
}

EcoCityMoto::~EcoCityMoto() {
}

/**
 * @brief Añade datos de motos desde un fichero a un vector dinamico
 * @param nombrefich - Fichero que contiene los datos de motos
 */
void EcoCityMoto::anadirMotos(string nombrefich) {
    ifstream fich;
    string linea;
    string matricula, estado, lat, longit;
    int estadoint;
    double latd, longd;
    int total = 0;
    fich.open(nombrefich);
    if (fich.good()) {
        while (!fich.eof()) {
            getline(fich, linea);
            stringstream ss;

            if (linea != "") total++;
            if (total > 1) {
                ss << linea;
                getline(ss, matricula, ';');
                getline(ss, estado, ';');
                getline(ss, lat, ';');
                getline(ss, longit, ';');
                latd = stod(lat);
                longd = stod(longit);
                estadoint = stoi(estado);
                Moto nueva(matricula, estadoint, latd, longd);
                motos.insertar(nueva);
            }

        }
    }
    fich.close();
    cout << "\n=== MOTOS AÑADIDAS CORRECTAMENTE ===\nTotal: " << motos.getTam() << endl;
}

/**
 * @brief Añade datos de cliente desde un fichero a un arbol AVL
 * @param nombrefich - Fichero que contiene los datos de clientes
 */
void EcoCityMoto::anadirClientes(string nombrefich) {
    ifstream fich;
    string linea;
    string dni, nombre, pass, direccion, latitud, longitud;
    double latd, longd;
    int total = 0;
    fich.open(nombrefich);
    if (fich.good()) {
        while (!fich.eof()) {
            getline(fich, linea);
            stringstream ss;

            if (linea != "") total++;
            if (total > 1) {
                ss << linea;
                getline(ss, dni, ';');
                getline(ss, nombre, ';');
                getline(ss, pass, ';');
                getline(ss, direccion, ';');
                getline(ss, latitud, ';');
                getline(ss, longitud, ';');
                latd = stod(latitud);
                longd = stod(longitud);
                Cliente nuevo(dni, nombre, pass, direccion, latd, longd);
                clientes.inserta(nuevo);
            }

        }
    }
    fich.close();
    cout << "=== CLIENTES AÑADIDOS CORRECTAMENTE ===\nTotal: " << clientes.numElementos() << "\nAltura: " << clientes.altura() << endl;
}

/**
 * @brief Metodo que busca un cliente en el VDinamico segun su DNI
 * @param dni - string que sirve para identificar cada cliente
 * @return Cliente* - es el cliente buscado
 */
Cliente* EcoCityMoto::buscarCliente(string& dni) {
    Cliente *encuentra = new Cliente(dni, "", "", "", 0, 0);
    Cliente *resultado = new Cliente();

    if (clientes.busca(*encuentra, *resultado)) {
        cout << "\n== BUSQUEDA SATISFACTORIA ==\n";
        cout << dni << endl << *resultado;
        return resultado;
    } else cout << "\n[!] No se ha encontrado a ese cliente";
    return nullptr;
}

/**
 * @brief Calcula la distancia entre dos coordenadas
 * @param orig - Coordenada uno
 * @param destino - Coordenada dos
 * @return double - Distancia comprendida entre las dos coordenadas
 */
double EcoCityMoto::calculaDistancia(UTM& orig, UTM& destino) {
    double cordX = destino.latitud - orig.latitud;
    double cordY = destino.longitud - orig.longitud;
    double distancia = sqrt(pow(cordX, 2) + pow(cordY, 2));
    return distancia;
}

/**
 * @brief Busca la moto mas cercana a la ubicacion que se le pasa como parametro
 * @param ubicacion - UTM posicion formada por coordenadas de longitud y latitud
 * @return Moto* - devuelve la moto mas cercana dada la ubicacion introducida
 */
Moto* EcoCityMoto::localizaMotoCercana(UTM& ubicacion) {
    UTM auxpos = motos[0].getPosicion();
    double min = calculaDistancia(ubicacion, auxpos);
    double auxmin;
    int pos = -1;
    for (int i = 1; i < motos.getTam(); i++) {
        auxpos = motos[i].getPosicion();
        auxmin = calculaDistancia(ubicacion, auxpos);
        if (auxmin < min && motos[i].getEstatus() == 0) {
            min = auxmin;
            pos = i;
        }
    }
    if (pos != -1) {
        //        cout << "\n== MOTO MAS CERCANA ENCONTRADA ==\n";
        //        cout << "Distancia desde tu ubicacion hasta la moto: " << auxmin;
        //        cout << "\nDatos de la moto: " << motos[pos] << endl;
        //        cout << "Posicion actual: " << "( " << motos[pos].getPosicion().latitud <<", " << motos[pos].getPosicion().longitud << " )" << endl;
        return &(motos[pos]);
    } else return nullptr;
}

/**
 * @brief Desbloquea la moto que ha solicitado un cliente
 * @param m - Moto cercana al cliente que ha pedido que sea desbloqueada
 * @param cli - Cliente que solicita el desbloqueo
 */
void EcoCityMoto::desbloqueaMoto(Moto& m, Cliente* cli) {
    idUltimo++;
    m.seActiva(cli);
}

unsigned int EcoCityMoto::getIdUltimo() const {
    return idUltimo;
}

/**
 * @brief Metodo que exporta los datos de clientes en un fichero
 */
void EcoCityMoto::exportaClientesCSV() {
    clientes.exportaCSV();
}

/**
 * @brief Localiza una moto asociada a un cliente
 * @param p - Cliente que tiene asociada una moto
 * @return Moto* - Moto que esta asociada a un cliente
 */
Moto* EcoCityMoto::localizaMoto(Cliente *p) {
    for (int i = 0; i < motos.getTam(); i++) {
        if (motos[i].getPcliente() == p) {
            return &motos[i];
        }
    }
}

/**
 * @brief Muestra el arbol AVL de clientes
 */
void EcoCityMoto::muestraArbolClientes() {
    clientes.muestra();
}

/**
 * @brief Devuelve la altura del arbol AVL de clientes
 * @return int - Altura del arbol AVL
 */
int EcoCityMoto::alturaArbolClientes() {
    return clientes.altura();
}




