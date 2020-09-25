/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Cliente.h
 * Author: admin
 *
 * Created on 26 de septiembre de 2019, 17:08
 */

#ifndef CLIENTE_H
#define CLIENTE_H
#include <iostream>
#include "UTM.h"
using namespace std;

class Cliente {
    string dni = "00000000A";
    string pass = "admin";
    string nombre = "admin";
    string direccion = "aa";
    UTM posicion;


public:
    Cliente& operator=(const Cliente& right);
    friend std::ostream& operator<<(std::ostream& os, const Cliente& obj);
    bool operator<(const Cliente& right) const;
    bool operator==(const Cliente& right) const;
    double distancia(const Cliente& cli);
    Cliente();
    Cliente(std::string nombre);
    Cliente(string _dni, string _pass, string _nombre, string _direccion, double _latitud, double _longitud);
    virtual ~Cliente();
    string getNombre() const;

};

#endif /* CLIENTE_H */

