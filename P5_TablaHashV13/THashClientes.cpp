/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   THashClientes.cpp
 * Author: admin
 * 
 * Created on 14 de noviembre de 2019, 15:57
 */

#include "THashClientes.h"
#include "EntradaCliente.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

/**
 * @brief Constructor por defecto.
 */
THashClientes::THashClientes() {
}

/**
 * @brief Constructor parametrizado
 * @param tamatabla - Tamaño de la tabla
 */
THashClientes::THashClientes(unsigned int tamatabla) : tamTabla(tamatabla) {
    vector<EntradaCliente> tablaaux(tamatabla);
    tabla = tablaaux;
}

/**
 * @brief Constructor Copia
 * @param orig - THashCliente a copiar
 */
THashClientes::THashClientes(const THashClientes& orig) : tamTabla(orig.tamTabla) {
    tabla = orig.tabla;
}

/**
 * @brief Destructor
 */
THashClientes::~THashClientes() {
}

/**
 * @brief Funcion de dispersion que calcula la posicion que debe ocupar el dato en el vector
 * @param clave - Valor que se sustituye en la funcion
 * @param intento - Numero que determina las colisiones que han ocurrido al ejecutar la funcion
 * @return int - Posicion que debe ocupar el dato en el vector
 */
int THashClientes::hash(unsigned long clave, int intento) {
    unsigned int salto = intento * 11 * (1 + clave % 7001);
    return (clave % tamTabla + salto) % tamTabla;
}

/**
 * @brief Calcula el factor de carga de la tabla hash segun fcarga = tamaño_vector / tamaño_tabla
 * @return float - El valor del factor de carga (debe estar comprendido entre 0.5 y 0.7)
 */
float THashClientes::factorCarga() {
    float nCli = nClientes, tamT = tamTabla;
    return nCli / tamT;
}

/**
 * @brief Calcula el número máximo de colisiones al hacer una inserción en la tabla hash
 * @return unsigned int - Numero maximo de colisiones
 */
unsigned int THashClientes::maxColisiones() {
    return maxCol;
}

/**
 * @brief Calcula el promedio de colisiones por operacion de insercion
 * @return unsigned int - Numero promedio de colisiones
 */
float THashClientes::promedioColisiones() {
    float ncli = nClientes;
    return totalColisiones / ncli;

}

/**
 * @brief Tamaño de la tabla hash
 * @return int - Tamaño que posee la tabla hash
 */
unsigned int THashClientes::tamaTabla() {
    return tamTabla;
}

/**
 * @brief Transforma una cadena (clave) en un dato tipo long
 * @param str - String que se pasa por parametro (clave DNI)
 * @return hash - Dato unsigned long
 */
unsigned long THashClientes::djb2(string str) {
    unsigned long hash = 5381;
    int c;
    for (int i = 0; i < str.length(); i++) {
        c = (int) str[i];
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

/**
 * @brief Devuelve el tamaño de clientes de la tabla
 * @return unsigned int - Tamaño del vector de clientes de la tabla hash
 */
unsigned int THashClientes::numClientes() {
    return nClientes;
}

/**
 * @brief Metodo que inserta datos de tipo Cliente en la tabla hash
 * @param clave - clave del objeto
 * @param dni - DNI del cliente
 * @param cli - Cliente a insertar
 * @return bool - 1 si la insercion es satisfactoria o 0.
 */
bool THashClientes::insertar(unsigned long clave, string& dni, Cliente& cli) {
    Cliente* busca = 0;
    if (buscar(0, dni, busca)) {
        return false;
    }

    int intento = 0;
    int pos = hash(djb2(dni), intento);
    EntradaCliente* aux = &tabla[pos];
    while (aux->estatus == ocupado) {
        intento++;
        pos = hash(djb2(dni), intento);
        aux = &tabla[pos];
    }
    if (intento > maxCol) maxCol = intento;
    totalColisiones += intento;
    numColisiones = intento;
    aux->dato = cli;
    aux->estatus = ocupado;
    aux->clave = djb2(dni);
    nClientes++;
    if (!primeraCarga) {
        if (factorCarga() > 0.85) {
            float ncli = nClientes, total = tamTabla;
            float factor = ncli / total;
            while (factor > 0.75) {
                total = buscaPrimoMayor(total);
            }
            cout << "Redispersando a " << total << " para disminuir el factor de carga.\n";
            redispersar(total);
        }
    }
    return true;
}

/**
 * @brief Busqueda en la tabla hash
 * @param clave - Clave del cliente a introducir
 * @param dni - string que contiene el Dni del cliente
 * @param cli - Cliente* usado como parametro de salida
 * @return bool - 1 si la busqueda es satisfactoria o 0.
 */
bool THashClientes::buscar(unsigned long clave, string& dni, Cliente* &cli) {
    int intento = 0;
    int pos = hash(djb2(dni), intento);
    EntradaCliente* aux = &tabla[pos];
    while (aux->estatus == ocupado || aux->estatus == borrado) {
        if (aux->estatus == ocupado) {
            if (dni == aux->dato.getDni()) {
                cli = &aux->dato;
                return true;
            }
        }
        intento++;
        pos = hash(djb2(dni), intento);
        aux = &tabla[pos];
    }
    return false;
}

/**
 * @brief Borrado en la tabla hash
 * @param clave - Clave del cliente a borrar
 * @param dni - DNI del cliente a borrar
 * @return bool - 1 si el borrado es satisfactorio o 0.
 */
bool THashClientes::borrar(unsigned long clave, string& dni) {
    Cliente* basura;
    if (!buscar(0, dni, basura)) {
        return false;
    }

    int intento = 0;
    int pos = hash(djb2(dni), intento);
    EntradaCliente* aux = &tabla[pos];
    while (aux->estatus == ocupado || aux->estatus == borrado) {
        if (aux->estatus == ocupado && dni == aux->dato.getDni()) {
            aux->estatus = borrado;
            aux->dato = Cliente();
            aux->clave = 0;
            nClientes--;
            if (!primeraCarga) {
                if (factorCarga() < 0.6) {
                    float ncli = nClientes, total = tamTabla;
                    float factor = ncli / total;
                    while (factor < 0.65) {
                        total = buscaPrimoMenor(total);
                        factor = ncli / total;
                    }
                    cout << "\n[!]REDISPERSANDO a un nuevo tamanio ya que el factor de carga ha bajado de 0.6 " << "(" << factorCarga() << "): " << total << " para aumentar el factor de carga.\n" << endl;
                    redispersar(total);
                }
            }
            return true;
        }
        intento++;
        pos = hash(djb2(dni), intento);
        aux = &tabla[pos];
    }
}

/**
 * @brief Redispersion de la tabla
 * @param tama - Nuevo tamaño a redispersar
 */
void THashClientes::redispersar(unsigned int tama) {
    vector<EntradaCliente> nuevo(tama);
    tamTabla = tama;
    for (int i = 0; i < tabla.size(); i++) {
        if (tabla[i].estatus == ocupado) {
            Cliente redisperso = tabla[i].dato;
            string dni = redisperso.getDni();
            
            int intento = 0;
            int pos = hash(djb2(dni), intento);
            
            EntradaCliente* aux = &nuevo[pos];
            
            while (aux->estatus == ocupado) {
                intento++;
                pos = hash(djb2(dni), intento);
                aux = &nuevo[pos];
            }
            aux->dato = redisperso;
            aux->clave = djb2(dni);
            aux->estatus = ocupado;
        }
    }
    tabla = nuevo;
}

int THashClientes::buscaPrimoMayor(int num) {
    if (num <= 2) return num;
    bool encontrado = false;
    int a = num + 1;
    while (!encontrado) {
        for (int i = a - 1; i > 2; i++) {
            if (a % i == 0) {
                a++;
                break;
            }
        }
        encontrado = true;
    }
    return a;
}

int THashClientes::buscaPrimoMenor(int num) {
    if (num <= 2) return num;
    bool encontrado = false;
    int a = num - 1;
    while (!encontrado) {
        for (int i = a - 1; i > 2; i--) {
            if (a % i == 0) {
                a--;
                break;
            }
        }
        encontrado = true;
    }
    return a;
}

/**
 * @brief Obtener todos los DNIs de la tabla
 * @return vector<string> con todos los DNIs de los elementos de la tabla
 */
vector<string> THashClientes::getDNI() {
    vector<string> aux;
    for (int i = 0; i < tabla.size(); i++) {
        if (tabla[i].estatus == ocupado) {
            string dni = tabla[i].dato.getDni();
            aux.push_back(dni);
        }
    }
    return aux;
}

int THashClientes::getnClientes() const {
    return nClientes;
}

int THashClientes::getNumColisiones() const {
    return numColisiones;
}

void THashClientes::setPrimeraCarga(bool primeraCarga) {
    this->primeraCarga = primeraCarga;
}