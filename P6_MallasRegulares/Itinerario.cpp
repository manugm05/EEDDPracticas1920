/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Itinerario.cpp
 * Author: admin
 * 
 * Created on 3 de octubre de 2019, 17:13
 */

#include "Itinerario.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include "UTM.h"
#include "fecha.h"
#include "Moto.h"

Itinerario::Itinerario() {
}

Itinerario::Itinerario(int id, int minutos, UTM inicio, UTM fin, Fecha fecha): id(id), minutos(minutos), inicio(inicio), fin(fin), fecha(fecha){

}

Itinerario::Itinerario(const Itinerario& orig) : fecha(orig.fecha), fin(orig.fin), id(orig.id), inicio(orig.inicio), minutos(orig.minutos), vehiculo(orig.vehiculo) {
}

Itinerario::~Itinerario() {
}

std::ostream& operator<<(std::ostream& os, const Itinerario& obj) {
    os << obj.id << ";" << obj.fecha << ";" << obj.minutos << ";"  << obj.inicio.latitud << ";" << obj.inicio.longitud << ";" <<  obj.fin.latitud << ";" << obj.fin.longitud;
    return os;
}

void Itinerario::SetFecha(Fecha fecha) {
    this->fecha = fecha;
}

void Itinerario::SetFin(UTM& fin) {
    this->fin = fin;
}

void Itinerario::SetId(int id) {
    this->id = id;
}

void Itinerario::SetInicio(UTM& inicio) {
    this->inicio = inicio;
}

void Itinerario::SetMinutos(int minutos) {
    this->minutos = minutos;
}

void Itinerario::SetVehiculo(Moto* vehiculo) {
    this->vehiculo = vehiculo;
}

Moto* Itinerario::GetMoto() {
    return vehiculo;
}
