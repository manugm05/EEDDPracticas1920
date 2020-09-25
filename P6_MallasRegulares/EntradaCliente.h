/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EntradaCliente.h
 * Author: admin
 *
 * Created on 14 de noviembre de 2019, 16:28
 */

#ifndef ENTRADACLIENTE_H
#define ENTRADACLIENTE_H

#include "Cliente.h"


enum Estatus{
    libre = 0,
    ocupado = 1,
    borrado = 2
};


class EntradaCliente {
public:
    EntradaCliente();
    EntradaCliente(long clave, Cliente dato);
    EntradaCliente(const EntradaCliente& orig);
    virtual ~EntradaCliente();
    long clave;
    Cliente dato;
    Estatus estatus=libre;   
};

#endif /* ENTRADACLIENTE_H */

