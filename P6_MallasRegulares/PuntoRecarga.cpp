/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PuntoRecarga.cpp
 * Author: admin
 * 
 * Created on 28 de noviembre de 2019, 16:33
 */

#include "PuntoRecarga.h"
#include "CasillaMalla.h"

PuntoRecarga::PuntoRecarga() {
}

PuntoRecarga::PuntoRecarga(const PuntoRecarga& orig) {
    id = orig.id;
    posicion = orig.posicion;
}

PuntoRecarga::~PuntoRecarga() {
}

PuntoRecarga::PuntoRecarga(string id, UTM posicion) : id(id), posicion(posicion) {

}

double PuntoRecarga::getX() {
    return posicion.latitud;
}

double PuntoRecarga::getY() {
    return posicion.longitud;
}

string PuntoRecarga::getId() const {
    return id;
}

void PuntoRecarga::setX(double x) {
    this->posicion.latitud = x;
}

void PuntoRecarga::setY(double y) {
    this->posicion.longitud = y;
}

