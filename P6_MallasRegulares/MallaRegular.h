/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MallaRegular.h
 * Author: Manuel
 *
 * Created on 30 de noviembre de 2019, 17:50
 */

#ifndef MALLAREGULAR_H
#define MALLAREGULAR_H


#include "PuntoRecarga.h"
#include <vector>
#include <cmath>
#include "CasillaMalla.h"

template<class T>
class MallaRegular {
public:
    MallaRegular();
    MallaRegular(float aXMin, float aYMin, float aXMax, float aYMax, int nDivX, int nDivY);
    MallaRegular(const MallaRegular& orig);
    MallaRegular<T>& operator=(const MallaRegular<T>& right);
    T* buscaCercano(float x, float y);
    bool insertar(T& dato);
    bool compruebaCoords(float x, float y);
    bool fueraAmbito(float x, float y);
    unsigned int maxElementosPorCelda();
    float mediaElementosPorCelda();
    unsigned int gettamMalla();
    virtual ~MallaRegular();
private:
    CasillaMalla<T>* getCasilla(float x, float y);
    float calculaDistancia(float x1, float y1, float x2, float y2);
    float aXMin = 0, aYMin = 0, aXMax = 100.0, aYMax = 100.0;
    float tamaCasillaX = 10, tamaCasillaY = 10;
    int totalEle = 0;
    int maxPorCelda = 5;
    vector<vector<CasillaMalla<T>>> malla; //simil a CasillaMalla<T> malla[][]
};

template<typename T>
MallaRegular<T>::MallaRegular() {
}

template<typename T>
MallaRegular<T>::MallaRegular(float aXMin, float aYMin, float aXMax, float aYMax, int nDivX, int nDivY) : aXMin(aXMin), aXMax(aXMax),
aYMin(aYMin), aYMax(aYMax) {
    tamaCasillaX = (aXMax - aXMin) / nDivX;
    tamaCasillaY = (aYMax - aYMin) / nDivY;
    vector<vector<CasillaMalla < T>>> aux(nDivX);
    for (int i = 0; i < nDivX; i++) {
        aux[i].resize(nDivY);
    }
    malla = aux;
}

template <typename T>
MallaRegular<T>& MallaRegular<T>::operator=(const MallaRegular<T>& right) {
    if (this != &right) {
        aXMax = right.aXMax;
        aXMin = right.aXMin;
        aYMax = right.aYMax;
        aYMin = right.aYMin;
        maxPorCelda = right.maxPorCelda;
        tamaCasillaX = right.tamaCasillaX;
        tamaCasillaY = right.tamaCasillaY;
        totalEle = right.totalEle;
        malla = right.malla;
    } else return *this;
}

/**
 * @brief Calcula la distancia entre dos puntos
 * @param x1 - Coordenada X del primer elemento
 * @param y1 - Coordenada Y del primer elemento
 * @param x2 - Coordenada X del segundo elemento
 * @param y2 - Coordenada Y del segundo elemento
 * @return float - Distancia entre los dos puntos
 */
template<typename T>
float MallaRegular<T>::calculaDistancia(float x1, float y1, float x2, float y2) {
    float cordX = x2 - x1;
    float cordY = y2 - y1;
    float distancia = sqrt(pow(cordX, 2) + pow(cordY, 2));
    return distancia;
}

//Hay que pulirlo

/**
 * @brief Busca el elemento mas cercano dadas las coordenadas de otro elemento
 * @param x - Coordenada X del elemento pasado como parametro
 * @param y - Coordenada Y del elemento pasado como parametro
 * @return T - Elemento mas cercano dadas las coordenadas
 */
template<typename T>
T* MallaRegular<T>::buscaCercano(float x, float y) {
    CasillaMalla<T>* inicio = getCasilla(x, y);
    T* resultado = 0;
    if (inicio->puntos.size() != 0) {
        float distancia = calculaDistancia(x, y, inicio->puntos[0].getX(), inicio->puntos[0].getY());
        resultado = &(inicio->puntos[0]);
        for (int i = 1; i < inicio->puntos.size(); i++) {
            float aux = calculaDistancia(x, y, inicio->puntos[i].getX(), inicio->puntos[i].getY());
            if (aux < distancia) {
                distancia = aux;
                resultado = &(inicio->puntos[i]);
            }

        }
        return resultado;

    } else {
        bool dentroIni = true;
        bool dentroFin = true;
        float distancia;
        float iniX = x, iniY = y, finX = x, finY = y;
        int imin, imax, jmin, jmax;
        while (dentroIni || dentroFin) {
            iniX -= tamaCasillaX;
            iniY -= tamaCasillaY;
            finX += tamaCasillaX;
            finY += tamaCasillaY;
            if (!compruebaCoords(iniX, iniY)) {
                dentroIni = false;
                iniX = x;
                iniY = y;
            }
            if (!compruebaCoords(finX, finY)) {
                dentroFin = false;
                finX = x;
                finY = y;
            }
            imin = (iniX - aXMin) / tamaCasillaX;
            jmin = (iniY - aYMin) / tamaCasillaY;
            imax = (finX - aXMin) / tamaCasillaX;
            jmax = (finY - aYMin) / tamaCasillaY;
            for (int i = imin; i <= imax; i++) {
                for (int j = jmin; j <= jmax; j++) {
                    if (malla[i][j].puntos.size() != 0) {
                        distancia = calculaDistancia(x, y, malla[i][j].puntos[0].getX(), malla[i][j].puntos[0].getY());
                        resultado = &(malla[i][j].puntos[0]);
                        for (int i = 1; i < malla[i][j].puntos.size(); i++) {
                            float aux = calculaDistancia(x, y, malla[i][j].puntos[i].getX(), malla[i][j].puntos[i].getY());
                            if (aux < distancia) {
                                distancia = aux;
                                resultado = &(malla[i][j].puntos[i]);
                            }

                        }


                    }
                }


            }
            if (resultado != 0) return resultado;
        }
    }
    return 0;
}

/**
 * @brief Verifica si dadas las coordenadas de un elemento, éste se encuentra fuera de la casilla que le corresponde
 * @param x - Coordenada X del elemento pasado por parametro
 * @param y - Coordenada Y del elemento pasado por parametro
 * @return bool - True si el elemento no esta en la casilla, false si ocurre lo contrario
 */
template<typename T>
bool MallaRegular<T>::fueraAmbito(float x, float y) {
    float iniX = x, iniY = y, finX = x, finY= y;
    int imin, imax, jmin, jmax;
    iniX -= tamaCasillaX;
    iniY -= tamaCasillaY;
    finX += tamaCasillaX;
    finY += tamaCasillaY;
    imin = (iniX - aXMin) / tamaCasillaX;
    jmin = (iniY - aYMin) / tamaCasillaY;
    imax = (finX - aXMin) / tamaCasillaX;
    jmax = (finY - aYMin) / tamaCasillaY;
    for (int i = imin; i <= imax; i++) {
        for (int j = jmin; j <= jmax; j++) {
            if (malla[i][j].puntos.size() != 0) return false;
        }
    }
    return true;
}

/**
 * @brief Comprueba si las coordenadas no estan fuera de la malla regular
 * @param x - Coordenada X pasada por parametro
 * @param y - Coordenada Y pasada por parametro
 * @return bool - True si esta dentro de la malla, false si esta fuera
 */
template<typename T>
bool MallaRegular<T>::compruebaCoords(float x, float y) {
    if (x <= aXMax && x >= aXMin && y <= aYMax && y >= aYMin) return true;
    return false;
}

/**
 * @brief Obtiene la casilla dadas unas coordenadas
 * @param x - Coordenada X
 * @param y - Coordenada Y
 * @return CasillaMalla<T> - Casilla que corresponde a las coordenadas pasadas como parametro
 */
template<typename T>
CasillaMalla<T>* MallaRegular<T>::getCasilla(float x, float y) {

    int i = (x - aXMin) / tamaCasillaX;
    int j = (y - aYMin) / tamaCasillaY;
    if (i > malla.size() - 1) i = malla.size() - 1;
    if (j > malla[i].size() - 1) j = malla[i].size() - 1;
    return &malla[i][j];
}

/**
 * @brief Metodo que inserta datos tipo T en la malla regular
 * @param dato - Tipo de dato a insertar
 * @return bool - True si el dato se ha insertado con exito, false si pasa lo contrario
 */
template<typename T>
bool MallaRegular<T>::insertar(T& dato) {
    float cordX = dato.getX();
    float cordY = dato.getY();
    if (!compruebaCoords(cordX, cordY)) throw std::domain_error("[!] MallaRegular<T>::insertar(): El objeto que se desea insertar no pertenece a la zona de la malla");
    CasillaMalla<T>* casilla = getCasilla(cordX, cordY);
    if (casilla->puntos.size() < maxPorCelda) {
        casilla->puntos.push_back(dato);
        totalEle++;
        return true;
    } else return false;

}

/**
 * @brief Devuelve el maximo de elementos que puede haber en una Casilla
 * @return unsigned int - Maximo de elementos por casilla
 */
template<typename T>
unsigned int MallaRegular<T>::maxElementosPorCelda() {
    return maxPorCelda;
}

/**
 * @brief Devuelve la media de elementos que hay por casilla de la malla regular
 * @return float - Media de elementos por casilla
 */
template<typename T>
float MallaRegular<T>::mediaElementosPorCelda() {
    float total = 0.0;
    float nCeldas = 0.0;
    for (int i = 0; i < malla.size(); i++) {
        for (int j = 0; j < malla[i].size(); j++) {
            total += malla[i][j].puntos.size();
            nCeldas += malla[i].size();
        }
    }

    float media = total / nCeldas;
    return media;
}

template<typename T>
unsigned int MallaRegular<T>::gettamMalla() {
    int cont = 0;
    for (int i = 0; i < malla.size(); i++) {
        for (int j = 0; j < malla[i].size(); j++) {
            cont++;
        }
    }

    return cont;
}

template<typename T>
MallaRegular<T>::~MallaRegular() {
}

#endif /* MALLAREGULAR_H */

