/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EcoCityMoto.h
 * Author: Samuel
 *
 * Created on 20 de octubre de 2019, 16:29
 */

#ifndef ECOCITYMOTO_H
#define ECOCITYMOTO_H
#include "Cliente.h"
#include "Moto.h"
#include <vector>
#include <map>
#include "Estado.h"

class Cliente;
class Moto;

class EcoCityMoto {
public:
    EcoCityMoto();
    EcoCityMoto(const EcoCityMoto& orig);
    virtual ~EcoCityMoto();
    Moto* localizaMotoCercana(UTM& ubicacion);
    void desbloqueaMoto(Moto& m, Cliente* cli);
    Cliente* buscarCliente(string& dni);
    unsigned int getIdUltimo() const;
    void exportaClientesCSV();
    Moto* localizaMoto(Cliente* p);
    void muestraArbolClientes();
    int alturaArbolClientes();
    vector<Moto> localizasSinBateria(float porctBateria);
    bool nuevoCliente(Cliente cli);
    bool eliminarCliente(string dni);
    

private:
    unsigned int idUltimo = 0;
    vector<Moto> motos;
    map<string, Cliente> clientes;
    double calculaDistancia(UTM& orig, UTM& destino);
    void anadirMotos(string nombrefich);
    void anadirClientes(string nombrefich);
    void crearItinerarios();
    void guardarClientesItinerarios(string nombrefich);
};

#endif /* ECOCITYMOTO_H */

