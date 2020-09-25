/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Moto.h
 * Author: Samuel
 *
 * Created on 20 de octubre de 2019, 16:31
 */

#ifndef MOTO_H
#define MOTO_H

class EcoCityMoto;
class Cliente;

#include "Estado.h"
#include "UTM.h"
#include "PuntoRecarga.h"
#include <iostream>

using namespace std;

class Moto {
public:
    Moto();
    Moto(const Moto& orig);
    Moto(string mat, int estatus, double lat, double longt, float bat=-1);
    virtual ~Moto();
    bool seActiva(Cliente* cli);
    void seDesactiva();
    UTM getPosicion() const;
    int getEstatus() const;
    friend std::ostream& operator<<(std::ostream& os, const Moto& obj);
    void setPosicion(UTM posicion);
    string getId() const;
    Cliente* getPcliente() const;
    void setPorcentajeBateria(float porcentajeBateria);
    float getPorcentajeBateria() const;
    void darAviso();
    void recargar(PuntoRecarga *pr);
    void terminarRecarga();


private:
    string id;
    UTM posicion;
    Estado estatus; //Estado: {bloqueada, activa, sinbateria, rota}
    Cliente *pcliente;
    float porcentajeBateria=100;
    PuntoRecarga* precarga;
};

#endif /* MOTO_H */

