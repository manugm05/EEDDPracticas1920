/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Iterator.h
 * Author: admin
 *
 * Created on 10 de octubre de 2019, 15:45
 */

#ifndef ITERATOR_H
#define ITERATOR_H
#include "Nodo.h"
#include "ListaDEnlazada.h"
template <typename T>
class ListaDEnlazada;

template <class T>
class Iterator{
    private:
        friend class ListaDEnlazada<T>;
        Nodo<T> *nodo;
        
    public:
        Iterator(Nodo<T>* nodo);
        void anterior();
        void siguiente();
        bool hayAnterior();
        bool haySiguiente();
        bool fin();
        T& dato();
};
 
/**
 * @brief Constructor parametrizado
 * @param nodo - Direccion de memoria del nodo apuntado por iterador.
 */
template<class T>
Iterator<T>::Iterator(Nodo<T>* nodo): nodo(nodo){
}

/**
 * @brief Desplazarse un nodo hacia atrás
 */
template<class T>
void Iterator<T>::anterior(){
    nodo = nodo->ant;
}

/**
 * @brief Desplazarse un nodo hacia delante
 */
template<class T>
void Iterator<T>::siguiente(){
    nodo = nodo->sig;
}

/**
 * @brief Comprobar si el nodo posee anterior
 * @return bool - true si lo posee o false si no
 */
template<class T>
bool Iterator<T>::hayAnterior(){
    return (nodo->ant != 0);
}

/**
 * @brief Comprobar si el nodo posee siguiente
 * @return bool - true si lo posee o false si no
 */
template<class T>
bool Iterator<T>::haySiguiente(){
    return (nodo->sig != 0);
}

/**
 * @brief Obtener dato del nodo apuntado
 * @return dato de tipo T del nodo apuntado
 */
template<class T>
T& Iterator<T>::dato(){
    return nodo->dato;
}

/**
 * @brief Comprobar si se ha llegado al final según iterador
 * @return bool - true si se ha llegado, false si no
 */
template <class T>
bool Iterator<T>::fin(){
    return (nodo == 0);

}
#endif /* ITERATOR_H */

