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

class Itinerario {
public:
    Itinerario();
    Itinerario(const Itinerario& orig);
    virtual ~Itinerario();
    void SetFecha(Fecha fecha);

    void SetFin(UTM fin);

    void SetId(int id);

    void SetInicio(UTM inicio);

    void SetMinutos(int minutos);
    friend std::ostream& operator<<(std::ostream& os, const Itinerario& obj);


private:
    int id;
    UTM inicio;
    UTM fin;
    Fecha fecha;
    int minutos;

};

#endif /* ITINERARIO_H */

