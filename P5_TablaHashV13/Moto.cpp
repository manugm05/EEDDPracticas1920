/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Moto.cpp
 * Author: Samuel
 * 
 * Created on 20 de octubre de 2019, 16:31
 */


#include "UTM.h"
#include "Estado.h"
#include "EcoCityMoto.h"
#include "Cliente.h"


Moto::Moto() {
}

Moto::Moto(const Moto& orig) : estatus(orig.estatus), id(orig.id), posicion(orig.posicion),
pcliente(orig.pcliente), porcentajeBateria(orig.porcentajeBateria) {
}

UTM Moto::getPosicion() const {
    return posicion;
}

int Moto::getEstatus() const {
    return estatus;
}

/**
 * @brief Constructor parametrizado de Moto
 * @param mat - Id de la moto, matricula
 * @param estatus - Estado de la moto: {bloqueada, activa, sinbateria, rota}
 * @param lat - latitud de la posicion de la moto
 * @param longt - longitud de la posicion de la moto
 * @param bateria - bateria de la moto
 * @pre El estado de la moto debe pertenecer al intervalo [0,4]
 */
Moto::Moto(string mat, int estatus, double lat, double longt) : id(mat) {
    if (estatus < 0 || estatus > 4) throw std::invalid_argument("error");
    else {
        switch (estatus) {
            case 0:
                this->estatus = bloqueada;
                break;
            case 1:
                this->estatus = activa;
                break;
            case 2:
                this->estatus = sinbateria;
                break;
            case 3:
                this->estatus = rota;
                break;
        }
    }

    //Generar Bateria
    float bateria;
    int dato0 = rand() % 101;
    if (estatus == 2) {
        bateria = 1 + rand() % 14 + (dato0) / 100.0;
    } else {
        bateria = 15 + rand() % 86 + (dato0) / 100.0;
    }
    porcentajeBateria = bateria;
    posicion = UTM(lat, longt);

}

std::ostream& operator<<(std::ostream& os, const Moto& obj) {
    os << obj.id << ";" << obj.estatus << ";" << obj.posicion.latitud << ";" << obj.posicion.longitud << ";" << obj.porcentajeBateria << "%";
    return os;
}

/**
 * @brief Activa la moto que ha solicitado un cliente, por lo que la moto queda asociada a ese cliente
 * @param cli - Cliente que solicita la moto
 * @pre La moto debe estar previamente bloqueada
 * @return bool - Si la moto se ha activado con exito devuelve true
 */
bool Moto::seActiva(Cliente* cli) {
    if (estatus == bloqueada) {
        estatus = activa;
        pcliente = cli;
        return true;
    } else return false;

}

/**
 * @brief Cambiada el estado de la moto a bloqueada
 * @pre La moto debe estar previamente activada
 */
void Moto::seDesactiva() {
    if (estatus == activa) {
        estatus = bloqueada;
        pcliente = nullptr;
    }
    if (porcentajeBateria < 15) {
        estatus = sinbateria;
    }
}

void Moto::setPosicion(UTM posicion) {
    this->posicion = posicion;
}

Moto::~Moto() {
}

string Moto::getId() const {
    return id;
}

Cliente* Moto::getPcliente() const {
    return pcliente;
}

float Moto::getPorcentajeBateria() const {
    return porcentajeBateria;
}

void Moto::setPorcentajeBateria(float porcentajeBateria) {
    if(porcentajeBateria <= 0) this->porcentajeBateria = 0;
    else this->porcentajeBateria = porcentajeBateria;
}

void Moto::darAviso() {
    cout << "[!] ATENCION: Bateria baja -> " << this->porcentajeBateria <<" %" << endl;
}


