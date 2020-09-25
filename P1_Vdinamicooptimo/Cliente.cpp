/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Cliente.cpp
 * Author: admin
 * 
 * Created on 26 de septiembre de 2019, 17:08
 */

#include "Cliente.h"
#include <sstream>
#include <iostream>
#include <cmath>

Cliente::Cliente(string _dni, string _pass, string _nombre, string _direccion, double _latitud, double _longitud) :
dni(_dni), pass(_pass), nombre(_nombre), direccion(_direccion), posicion(_latitud, _longitud) {
}

Cliente::Cliente() {

}

Cliente::~Cliente() {

}

string Cliente::getNombre() const {
    return nombre;
}

Cliente& Cliente::operator=(const Cliente& right) {
    // Check for self-assignment!
    if (this == &right) return *this;
    direccion = right.direccion;
    dni = right.dni;
    nombre = right.nombre;
    pass = right.pass;
    posicion = right.posicion;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Cliente& obj) {
    os << obj.nombre << ";" << obj.dni;
    return os;
}

/**
 * @brief Operador < enfocado a nombre.
 * @param right
 * @return true - si alfabeticamente el nombre del objeto que llama al operador es menor y viceversa
 */
bool Cliente::operator<(const Cliente& right) const {
    if (nombre < right.nombre) return true;
    return false;
}
/**
 * @breif Operador==. Enfocado a primer nombre.
 * @param right - Objeto de clase cliente..
 * @return True si el primer nombre coincide , false si no.
 */
bool Cliente::operator==(const Cliente& right) const {
    stringstream ss;
    std::string corte;
    ss << right.getNombre();
    getline(ss, corte, ' ');
    if(corte == nombre) return true;
    return false;
}

/**
 * @brief Calculo de la distancia entre 2 clientes mediante el vector formado por la cordenadas
 * @param cli - Objeto tipo cliente
 * @return double - Módulo del vector (cordX,cordY).
 */
double Cliente::distancia(const Cliente& cli) {
    double cordX = posicion.latitud - cli.posicion.latitud;
    double cordY = posicion.longitud - cli.posicion.longitud;
    double distancia = sqrt(pow(cordX, 2) + pow(cordY, 2));
    return distancia;
}

Cliente::Cliente(std::string nombre): nombre(nombre) {
}
