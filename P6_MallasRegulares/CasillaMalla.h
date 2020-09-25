/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CasillaMalla.h
 * Author: Manuel
 *
 * Created on 30 de noviembre de 2019, 17:56
 */

#ifndef CASILLAMALLA_H
#define CASILLAMALLA_H
#include <vector>

template <typename T>
class MallaRegular;

using namespace std;

#include "CasillaMalla.h"
template <class T>
class CasillaMalla {
public:
    CasillaMalla();
    friend class MallaRegular<T>;
    void insertar(const T& dato);
    T* buscar(const T& dato);
    bool borrar(const T& dato);
    CasillaMalla<T>& operator=(const CasillaMalla<T>& right);

private:
    vector<T> puntos; //vector porque max 5 puntos.
};

template<typename T>
CasillaMalla<T>::CasillaMalla() {
}

template<typename T>
CasillaMalla<T>& CasillaMalla<T>::operator =(const CasillaMalla<T>& right){
    puntos = right.puntos;
}

template<typename T>
void CasillaMalla<T>::insertar(const T& dato) {
    puntos.push_back(dato);
}

template<typename T>
T* CasillaMalla<T>::buscar(const T& dato) {
    for (int i = 0; i < puntos.size(); i++) {
        if (dato == puntos[i]) return &puntos[i];
    }
    return 0;
}

template<typename T>
bool CasillaMalla<T>::borrar(const T& dato) {
    T* aux = buscar(dato);
    if (dato) {
        typename vector<T>::iterator i = puntos.begin();
        while (i != puntos.end()) {
            if (*i == dato) break;
            i++;
        }
        puntos.erase(i);
        return true;
    }
    return false;
}
#endif /* CASILLAMALLA_H */

