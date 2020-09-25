/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PuntoRecarga.h
 * Author: admin
 *
 * Created on 28 de noviembre de 2019, 16:33
 */

#ifndef PUNTORECARGA_H
#define PUNTORECARGA_H

#include <iostream>

#include "UTM.h"

using namespace std;

class PuntoRecarga {
public:
    PuntoRecarga();
    PuntoRecarga(string id, UTM posicion);
    PuntoRecarga(const PuntoRecarga& orig);
    virtual ~PuntoRecarga();
    double getX();
    double getY();
    string getId() const;
    void setX(double x);
    void setY(double y);
    
private:
    string id;
    UTM posicion;
};

#endif /* PUNTORECARGA_H */

