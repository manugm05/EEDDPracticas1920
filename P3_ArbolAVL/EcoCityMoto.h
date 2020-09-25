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
#include "ListaDEnlazada.h"
#include "VDinamico.h"
#include "AVL.h"
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
    void anadirMotos(string nombrefich);
    void anadirClientes(string nombrefich);
    unsigned int getIdUltimo() const;
    void exportaClientesCSV();
    Moto* localizaMoto(Cliente* p);
    void muestraArbolClientes();
    int alturaArbolClientes();
    
private:
    unsigned int idUltimo=0;
    VDinamico<Moto> motos;
    AVL<Cliente> clientes;
    double calculaDistancia(UTM& orig, UTM& destino);
};

#endif /* ECOCITYMOTO_H */

