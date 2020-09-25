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
#include <stdlib.h>
#include <list>
#include "Moto.h"
#include "Itinerario.h"

using namespace std;

const int MAX = 100;
class EcoCityMoto;

class Cliente {
    string dni = "00000000A";
    string pass = "admin";
    string nombre = "admin";
    string direccion = "aa";
    string display = "";
    UTM posicion;
    list<Itinerario> itirenario;
    EcoCityMoto *acceso;
    unsigned int puntos;


public:
    Cliente& operator=(const Cliente& right);
    friend std::ostream& operator<<(std::ostream& os, const Cliente& obj);
    bool operator<(const Cliente& right) const;
    bool operator>(const Cliente& right) const;
    bool operator==(const Cliente& right) const;
    double distancia(const Cliente& cli);
    Cliente();
    Cliente(std::string nombre);
    Cliente(string _dni, string _pass, string _nombre, string _direccion, double _latitud, double _longitud);
    virtual ~Cliente();
    string getNombre() const;
    void crearItinerarios(unsigned int num, int idUltimo, UTM min, UTM max);
    void iterenariosCSV();
    UTM getPosicion() const;
    Moto* buscarMotoCercana();
    void desbloquearMoto (Moto &m);
    void terminarTrayecto();
    void setAcceso(EcoCityMoto* acceso);
    void anadeIti(Itinerario iti);
    string getDni() const;
    void mostrarMensaje(string texto);
    void setPosicion(UTM posicion);
    void decrementarPunto();
    void incrementarPunto();
    PuntoRecarga* puntoRecargaCercano();
    void recargarMoto(PuntoRecarga* pr);
    void setPuntos(int puntos);
    unsigned int getPuntos() const;

};

#endif /* CLIENTE_H */

