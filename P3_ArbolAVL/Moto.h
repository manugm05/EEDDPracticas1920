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
#include <iostream>
#include "UTM.h"
#include "Cliente.h"
#include "Itinerario.h"
#include "EcoCityMoto.h"

class EcoCityMoto;
class Itinerario;
class Cliente;
#include "Estado.h"

class Moto {
public:
    Moto();
    Moto(const Moto& orig);
    Moto(string mat, int estatus, double lat, double longt);
    virtual ~Moto();
    bool seActiva(Cliente* cli);
    void seDesactiva();
    UTM getPosicion() const;
    int getEstatus() const;
    friend std::ostream& operator<<(std::ostream& os, const Moto& obj);
    void setPosicion(UTM posicion);
    string getId() const;
    Cliente* getPcliente() const;

private:
    string id;
    UTM posicion;
    Estado estatus; //Estado: {bloqueada, activa, sinbateria, rota}
    Cliente *pcliente;
};

#endif /* MOTO_H */

