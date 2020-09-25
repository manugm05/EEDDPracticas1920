/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Itinerario.h
 * Author: admin
 *
 * Created on 3 de octubre de 2019, 17:13
 */

#ifndef ITINERARIO_H
#define ITINERARIO_H
#include "UTM.h"
#include "fecha.h"
#include "Moto.h"
#include "Cliente.h"

class Cliente;
class Moto;


class Itinerario {
public:
    Itinerario();
    Itinerario(int id, int minutos, UTM inicio, UTM fin, Fecha fecha);
    Itinerario(const Itinerario& orig);
    virtual ~Itinerario();
    void SetFecha(Fecha fecha);

    void SetFin(UTM& fin);

    void SetId(int id);

    void SetInicio(UTM& inicio);

    void SetMinutos(int minutos);
    friend std::ostream& operator<<(std::ostream& os, const Itinerario& obj);
    
    void SetVehiculo(Moto* vehiculo);
    Moto* GetMoto();



private:
    int id;
    UTM inicio;
    UTM fin;
    Fecha fecha;
    int minutos;
    Moto *vehiculo;

};

#endif /* ITINERARIO_H */

