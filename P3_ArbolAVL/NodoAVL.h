/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Nodo.h
 * Author: admin
 *
 * Created on 3 de octubre de 2019, 15:58
 */

#ifndef NODOAVL_H
#define NODOAVL_H

template <class T>
class NodoAVL {
public:
    NodoAVL(T &aDato, NodoAVL *der = 0, NodoAVL *izq = 0);
    virtual ~NodoAVL();
    T GetDato() const;
    T dato;
    NodoAVL *der, *izq;
    int bal;
};

/**
 * @brief Constructor parametrizado
 * @param aDato - Dato a introducir en el nodo
 * @param der - Dirección de memoria del nodo de la derecha
 * @param izq - Dirección de memoria del nodo de la izquierda
 */
template <class T>
NodoAVL<T>::NodoAVL(T& aDato, NodoAVL* der, NodoAVL* izq) : dato(aDato), der(der), izq(izq) {
    int vder = 0, vizq = 0;
    if (der) vder++;
    if (izq) vizq++;
    bal = vizq - vder;
}

/**
 * @brief Obtener dato mediante getter
 * @return dato - tipo T
 */
template <class T>
T NodoAVL<T>::GetDato() const {
    return dato;
}

/**
 * @brief Destructor de nodo
 */
template <class T>
NodoAVL<T>::~NodoAVL() {
}

#endif /* NODO_H */
