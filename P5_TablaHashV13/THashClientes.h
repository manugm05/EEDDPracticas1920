/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   THashClientes.h
 * Author: admin
 *
 * Created on 14 de noviembre de 2019, 15:57
 */

#ifndef THASHCLIENTES_H
#define THASHCLIENTES_H

#include "EntradaCliente.h"
#include <iostream>
#include <vector>

using namespace std;

class THashClientes {
public:
    THashClientes();
    THashClientes(unsigned int tamatabla);
    THashClientes(const THashClientes& orig);
    virtual ~THashClientes();
    unsigned int maxColisiones();
    float promedioColisiones();
    float factorCarga();
    unsigned int tamaTabla();
    bool insertar(unsigned long clave, string &dni, Cliente &cli);
    bool buscar(unsigned long clave, string &dni, Cliente* &cli);
    bool  borrar(unsigned long clave, string &dni);
    unsigned int numClientes();
    void redispersar(unsigned int tama);
    vector<string> getDNI();
    int getnClientes() const;
    int getNumColisiones() const;
    void setPrimeraCarga(bool primeraCarga);
    
    
private:
    int hash(unsigned long clave, int intento);
    unsigned long djb2(string str);
    int buscaPrimoMayor(int num);
    int buscaPrimoMenor(int num);
    int tamTabla=0;
    int nClientes=0;
    int numColisiones=0;
    float totalColisiones=0;
    int maxCol=0;
    bool primeraCarga = true;
    vector<EntradaCliente> tabla;

};

#endif /* THASHCLIENTES_H */

