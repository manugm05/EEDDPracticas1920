/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   VDinamico.h
 * Author: admin
 *
 * Created on 19 de septiembre de 2019, 16:02
 */

#ifndef VDINAMICO_H
#define VDINAMICO_H
#include <cmath>
#include <climits>
#include <stdexcept>
#include <algorithm>
#include "Cliente.h"

template<class T>
class VDinamico {
private:
    T* memo;
    int taml, tamf;
    bool ordenado = false;
    void aumenta();
    void reduce();

public:
    VDinamico();
    VDinamico(const VDinamico<T>& orig);
    VDinamico(unsigned int taml);
    VDinamico(const VDinamico<T>& orig, unsigned int inicio, unsigned int num);
    VDinamico<T>& insertar(const T& dato, unsigned int pos = UINT_MAX);
    void lee();
    VDinamico<T>& ordenar();
    int busquedaBin(T& dato);
    unsigned int getTam();
    VDinamico<T>& borrar(unsigned int pos = UINT_MAX);
    VDinamico<T>& operator=(const VDinamico<T>& der);
    T& operator[](int pos);

    virtual ~VDinamico();
};

/**
 * @brief Constructor por defecto.
 */
template<class T>
VDinamico<T>::VDinamico() : taml(0), tamf(1) {
    memo = new T[tamf];
}


/**
 * @brief Constructor de copia
 * @param orig - Objeto de tipo VDinamico<T>.
 */
template<class T>
VDinamico<T>::VDinamico(const VDinamico<T>& orig) : taml(orig.taml), tamf(orig.tamf), ordenado(orig.ordenado) {
    memo = new T[tamf];
    for (int i = 0; i < taml; i++) {
        memo[i] = orig.memo[i];
    }
}

/**
 * @brief Constructor parametrizado
 * @param taml - Entero (Int)
 */
template<class T>
VDinamico<T>::VDinamico(unsigned int taml) : taml(taml) {
    for (int i = 0; i <= taml; i++) {
        int potencia = pow(2, i);
        if (potencia > taml) {
            tamf = potencia;
            break;
        }
    }
}

/**
 * @brief Destructor de objetos de la clase.
 */
template<class T>
VDinamico<T>::~VDinamico() {
    delete[] memo;
}

/**
 * @brief Operador de asignacion.
 * @param der - Objeto de tipo VDinamico<T>
 * @return *this - Objeto que llama al operador para permitir la concadenación.
 */
template<class T>
VDinamico<T>& VDinamico<T>::operator=(const VDinamico<T>& der) {
    if (this != &der) {
        delete[] memo;
        tamf = der.tamf;
        taml = der.taml;
        ordenado = der.ordenado;
        memo = new T[tamf];
        for (int i = 0; i < taml; i++) {
            memo[i] = der.memo[i];
        }
    }
    return *this;
}

/**
 * @brief Operador [] (corchete).
 * @param pos - entero (Int).
 * @pre pos no debe ser mayor o igual que taml (tamaño lógico).
 * @return Elemento que ocupa la posición pos del vector.
 * @throw Si no se cumple la precondición, salta out_of_range.
 */
template<class T>
T& VDinamico<T>::operator[](int pos) {
    if (pos >= taml) throw std::out_of_range("[!] VDinamico<T>::operator[]: Posicion no valida");
    return memo[pos];
}

/**
 * @brief Constructor de copia parcial - Copia, a partir de una posicion inicial, n unidades en adelante.
 * @param orig - Objeto de tipo VDinamico<T> vector que se quiere copiar.
 * @param inicio - Posicion en la que se inicia la copia de los objetos de orig.
 * @param num - Número de posiciones que se deben copiar a partide de incio.
 * @pre inicio debe ser menor que taml de orig.
 * @pre num+inicio debe ser menor que taml de orig.
 * @throw si no se cumple alguna de las precondiciones, salta invalid_argument.
 */
template<class T>
VDinamico<T>::VDinamico(const VDinamico<T>& orig, unsigned int inicio, unsigned int num) : tamf(orig.tamf), taml(num), ordenado(orig.ordenado) {
    if(inicio >= orig.taml || inicio+num >= orig.taml) throw std::invalid_argument("[!] VDinamico(orig,inicio,num): Se supera el taml de orig. Imposible copiar.");
    memo = new T[tamf];
    for (int i = 0; i < num; i++) {
        memo[i] = orig.memo[i + inicio];
    }
}

/**
 * @brief Inserción de un dato en una posición de un vector.
 * @param dato - Objeto de clase T.
 * @param pos - Posición en la que se desea insertar el objeto.
 * @note Si no se especifica pos, se tomará la última posición del vector.
 * @pre pos debe ser menor que taml.
 * @throw  Si no se cumple la precondición, salta excepción invalid_argument.
 * @return *this - Para poder encadenar los métodos.
 */
template<class T>
VDinamico<T>& VDinamico<T>::insertar(const T& dato, unsigned int pos) {
    if (taml == tamf) aumenta();
    if (pos != UINT_MAX) {
        if (pos < taml) {
            for (int i = taml - 1; i > pos; i--) {
                memo[i + 1] = memo[i];
            }
            memo[pos] = dato;
        } else throw std::invalid_argument("[!] VDinamico<T>::insertar(): Posicion no valida");
    } else memo[taml] = dato;
    ordenado = false;
    taml++;
    return *this;
}

/**
 * @brief Borrado de un elemento del vector.
 * @param pos - Entero (Int) posicion que ocupa el elemento que se desea borrar.
 * @pre pos debe ser menor que taml
 * @throw invalid_argument si pos no es válida
 */
template<class T>
VDinamico<T>& VDinamico<T>::borrar(unsigned int pos) {
    if (pos == UINT_MAX) {
        taml--;
    } else if (pos < taml) {
        taml--;
        for (int i = pos; i < taml; i++) {
            memo[i] = memo[i + 1];
        }
        ordenado = false;
    } else throw std::invalid_argument("[!] VDinamico::borrar(): Posicion incorrecta");
    reduce();
    return *this;
}

/**
 * @brief Aumentar tamaño del vector.
 */
template<class T>
void VDinamico<T>::aumenta() {
    T* memoaux;
    int i = 0;
    int pot = 0;
    while (tamf >= pot) {
        pot = pow(2, i);
        i++;
    }
    memoaux = new T[pot];

    for (int i = 0; i < taml; i++) {
        memoaux[i] = memo[i];
    }
    delete[] memo;
    memo = memoaux;
    tamf = pot;
}

/**
 * @brief Reducir tamaño del vector
 */
template<class T>
void VDinamico<T>::reduce() {
    if (taml * 3 == tamf) {
        tamf /= 2;
        T* aux = new T[tamf];
        for (int i = 0; i < taml; i++) {
            aux[i] = memo[i];
        }
        delete[] memo;
        memo = aux;
    }
}

/**
 * @brief Ordenar el vector de menor a mayor.
 * @note Se usa la función sort de la librería algorithm.
 * @pre El tipo T debe de tener definido el operador <.
 * @return *this - Encadenación de métodos.
 */
template<class T>
VDinamico<T>& VDinamico<T>::ordenar() {
    //[inicio,fin) osease dir memoria fin debe ser taml
    std::sort(memo, &memo[taml]);
    ordenado = true;
    return *this;
}

/**
 * @brief Muestra por pantalla los elementos del vector.
 * @pre El tipo T debe de tener definido el operador <<.
 */
template<class T>
void VDinamico<T>::lee() {
    for (int i = 0; i < taml; i++) {
        std::cout << "Elemento " << i << ":" << memo[i] << '\n';

    }
}



/**
 * @brief Algoritmo de búsqueda binaria.
 * @param dato - Objeto que se desea encontrar en el vector.
 * @return Posición en el vector del objeto o -1 si no se encuentra.
 */
template<class T>
int VDinamico<T>::busquedaBin(T& dato) {
    if(!ordenado) ordenar();
    int inf = 0;
    int sup = taml - 1;
    int corte = 0;
    while (inf <= sup) {
        corte = (inf + sup) / 2;
        if (dato == memo[corte]) return corte;
        if (dato < memo[corte]) sup = corte - 1;
        else inf = corte + 1;
    }
    return -1;
}

/**
 * @brief Obtener tamaño lógico del vector.
 * @return taml - tamaño lógico del vector.
 */
template<class T>
unsigned int VDinamico<T>::getTam() {
    return taml;
}
#endif /* VDINAMICO_H */

