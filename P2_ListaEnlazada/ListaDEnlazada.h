/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ListaDEnlazada.h
 * Author: admin
 *
 * Created on 3 de octubre de 2019, 15:56
 */

#ifndef LISTADENLAZADA_H
#define LISTADENLAZADA_H
#include "Nodo.h"
#include "Iterator.h"

template <class T>
class ListaDEnlazada {
private:
    Nodo<T> *cola;
    Nodo<T> *cabecera;
    int tam = 0;

public:
    ListaDEnlazada();
    ListaDEnlazada(const ListaDEnlazada& orig);
    ListaDEnlazada<T>& operator=(const ListaDEnlazada<T>& right);
    T& inicio();
    T& fin();
    void insertaInicio(T& dato);
    void insertaFinal(T& dato);
    void insertar(Iterator<T>& i, T& dato);
    void borraIncio();
    void borraFinal();
    void borra(Iterator<T>& i);
    int getTam();
    ListaDEnlazada<T>& concatena(const ListaDEnlazada<T> &l);
    Iterator<T> iterador() const;
    virtual ~ListaDEnlazada();
};

/**
 * @brief Constructor por defecto
 */
template <class T>
ListaDEnlazada<T>::ListaDEnlazada() : cola(0), cabecera(0) {
}

/**
 * @brief Contructor de copia
 * @param orig - Lista enlazada a copiar
 */
template <class T>
ListaDEnlazada<T>::ListaDEnlazada(const ListaDEnlazada<T>& orig) {
    Nodo<T>* aux = orig.cabecera;
    while (aux != 0) {
        insertaFinal(aux->dato);
        aux = aux->sig;
    }
}

/**
 * @brief Operador=
 * @param right - Objeto de tipo ListaDEnlazada<T>
 * @return *this - Objeto que llama al operador para permitir la concadenación.
 */
template <class T>
ListaDEnlazada<T>& ListaDEnlazada<T>::operator=(const ListaDEnlazada<T>& right) {
    if (this != &right) {
        Iterator<T> it = right.iterador();

        while (cabecera != 0) {
            borraFinal();
        }

        while (!it.fin()) {
            insertaFinal(it.dato());
            it.siguiente();
        }

        tam = right.tam;
    }
    return *this;
}

/**
 * @brief Devolver el dato de cabecera
 * @return Dato de tipo T que contiene el nodo cabecera
 */
template <class T>
T& ListaDEnlazada<T>::inicio() {
    return cabecera->dato;
}

/**
 * @bried Devolver el dato de cola
 * @return Dato de tipo T que contiene el nodo cola
 */
template <class T>
T& ListaDEnlazada<T>::fin() {
    return cola->dato;
}

/**
 * @brief Obtener iterador
 * @return Iterador<T> que apunta a la cabecera de la lista
 */
template <class T>
Iterator<T> ListaDEnlazada<T>::iterador() const {
    return Iterator<T>(cabecera);
}

/**
 * @brief Insertar un dato en el inicio de la lista
 * @param dato - Dato de tipo T que se desea insertar
 */
template <class T>
void ListaDEnlazada<T>::insertaInicio(T& dato) {
    Nodo<T>* aux = new Nodo<T>(dato, 0, 0);
    if (cabecera == 0) {
        cabecera = aux;
        cola = cabecera;
    } else {
        aux->sig = cabecera;
        cabecera->ant = aux;
        cabecera = aux;
    }
    tam++;
}

/**
 * @brief Insertar un dato al final de la lista
 * @param dato - Dato de tipo T que se desea insertar
 */
template <class T>
void ListaDEnlazada<T>::insertaFinal(T& dato) {
    Nodo<T>* aux = new Nodo<T>(dato, 0, 0);
    if (cola == 0) {
        cabecera = aux;
        cola = cabecera;
    } else {
        aux->ant = cola;
        cola->sig = aux;
        cola = aux;
    }
    tam++;
}

/**
 * @brief Insercción genérica según iterador
 * @param i - Iterador<T>. Usado para indicar detras de que dato se desea insertar el deseado
 * @param dato - Dato de tipo T que se desea insertar
 */
template <class T>
void ListaDEnlazada<T>::insertar(Iterator<T>& i, T& dato) {
    Nodo<T>* aux = new Nodo<T>(dato, 0, 0);
    if (!i.hayAnterior()) insertaInicio(dato);
    else if (!i.haySiguiente()) insertaFinal(dato);
    else {
        i.nodo->ant->sig = aux;
        aux->sig = i.nodo;
        aux->ant = i.nodo->ant;
        i.nodo->ant = aux;
        tam++;
    }

}

/**
 * @brief Borrar primer elemento de la lista
 */
template <class T>
void ListaDEnlazada<T>::borraIncio() {
    if (cabecera != 0) {
        if (cabecera == cola) {
            delete cola;
            cabecera = cola = 0;
        } else {
            Nodo<T>* aux = cabecera;
            cabecera->sig->ant = 0;
            cabecera = cabecera->sig;
            delete aux;
        }
        tam--;
    } else cola = 0;
}

/**
 * @brief Borrar el último elemento de la lista
 */
template <class T>
void ListaDEnlazada<T>::borraFinal() {
    if (cola != 0) {
        if (cabecera == cola) {
            delete cola;
            cabecera = cola = 0;
        } else {
            Nodo<T>* aux = cola;
            cola->ant->sig = 0;
            cola = cola->ant;
            delete aux;
        }
        tam--;

    } else cabecera = 0;

}

/**
 * @brief Borrado genérico según iterador
 * @param i - Iterador<T>. Apunta a elemento que se desea eliminar.
 */
template <class T>
void ListaDEnlazada<T>::borra(Iterator<T>& i) {
    Nodo<T>* aux = i.nodo;
    aux->ant->sig = aux->sig;
    aux->sig->ant = aux->ant;
    delete aux;
    tam--;
}

/**
 * @brief Obtener tamaño de la lista
 * @return tam - entero que indica el tamaño.
 */
template <class T>
int ListaDEnlazada<T>::getTam() {
    return tam;
}

/**
 * @brief Concatenar dos listas
 * @param l - Lista que se desea concatenar
 * @return Nueva lista con todos los elementos de ambas listas. Si l está vacía, devuelve una copia de this.
 */
template <class T >
ListaDEnlazada<T>& ListaDEnlazada<T>::concatena(const ListaDEnlazada<T>& l) {
    if (l.cabecera != 0) {
        ListaDEnlazada<T>* aux = new ListaDEnlazada(*this);
        Iterator<T> it = l.iterador();
        while (!it.fin()) {
            aux->insertaFinal(it.dato());
            it.siguiente();
        }
        return *aux;
    } else return *this;
}

/**
 * @brief Destructor de ListaDEnlazada.
 */
template<class T>
ListaDEnlazada<T>::~ListaDEnlazada() {
    while (cabecera != 0) borraFinal();
}

#endif /* LISTADENLAZADA_H */

