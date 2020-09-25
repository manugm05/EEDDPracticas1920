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

#ifndef NODO_H
#define NODO_H

template <class T>
class Nodo {
public:
    Nodo(T &aDato, Nodo *aAnt = 0, Nodo *aSig = 0);
    virtual ~Nodo();
    T GetDato() const;
    T dato;
    Nodo *ant, *sig;
    
};


/**
 * @brief Constructor parametrizado
 * @param aDato - Dato a introducir en el nodo
 * @param aAnt - Dirección de memoria del nodo anterior a este
 * @param aSig - Dirección de memoria del nodo siguiente a este
 */
template <class T>
Nodo<T>::Nodo(T& aDato, Nodo* aAnt, Nodo* aSig) : dato(aDato), ant(aAnt), sig(aSig) {
}

/**
 * @brief Obtener dato mediante getter
 * @return dato - tipo T
 */
template <class T>
T Nodo<T>::GetDato() const {
    return dato;
}

/**
 * @brief Destructor de nodo
 */
template <class T>
Nodo<T>::~Nodo() {
}

#endif /* NODO_H */
