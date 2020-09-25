/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AVL.h
 * Author: admin
 *
 * Created on 17 de octubre de 2019, 15:58
 */

#ifndef AVL_H
#define AVL_H
#include <stdexcept>
#include "NodoAVL.h"
#include "Nodo.h"
#include "VDinamico.h"

template <class T>
class AVL {
public:
    AVL();
    AVL(const AVL<T>& orig);
    virtual ~AVL();
    AVL<T>& operator=(const AVL<T>& right);
    bool inserta(T& dato);
    bool busca(T& dato, T& resultado);
    bool buscaIt(T& dato, T& resultado);
    void recorreInorden();
    unsigned int numElementos();
    unsigned int altura();
    void muestra();
    void exportaCSV();

private:
    NodoAVL<T>* groot;
    void rotIzqda(NodoAVL<T>* &p);
    void rotDecha(NodoAVL<T>* &p);
    NodoAVL<T>* insertaDato(T& dato, NodoAVL<T>* &nodo);
    void inorden(NodoAVL<T>* p, int nivel);
    NodoAVL<T>* buscaDato(T &dato, NodoAVL<T>* nodo);
    void cuantosElementos(NodoAVL<T>* nodo, int &maxElementos);
    void muestra2(NodoAVL<T>* p, int l);
    int calculaAltura(NodoAVL<T>* nodo);
    void borrado(NodoAVL<T>* nodo);
    NodoAVL<T>* copia(NodoAVL<T>* &nodo, NodoAVL<T>* orig);
    void guardaInorden(VDinamico<T>* v, NodoAVL<T>* p);
};

/**
 * @brief Constructor por defecto
 */
template <class T>
AVL<T>::AVL() : groot(0) {
}

/**
 * @brief Metodo de copia recursivo
 * @param nodo - NodoAVL<T>* nodo en el cual se guarda la copia
 * @param orig - NodoAVL<T>* nodo que se quiere copiar
 * @return NodoAVL<T>* para recursividad
 */
template <class T>
NodoAVL<T>* AVL<T>::copia(NodoAVL<T>* &nodo, NodoAVL<T>* orig) {
    if (orig) {
        nodo = new NodoAVL<T>(orig->dato);
        nodo->bal = orig->bal;
        if (orig->izq) copia(nodo->izq, orig->izq);
        if (orig->der) copia(nodo->der, orig->der);
    }
}


/**
 * @brief Constructor de copia usando funcion recursiva
 * @param orig - Arbol a copiar
 */
template <class T>
AVL<T>::AVL(const AVL<T>& orig) {
    copia(groot, orig.groot);

}

/**
 * @breif Operador=
 * @param right - Arbol AVL a copiar
 * @return *this - Copia del arbol original
 */

template <class T>
AVL<T>& AVL<T>::operator=(const AVL<T>& right) {
    if (this != &right) {
        borrado(groot);
        copia(groot, right.groot);
    }
    return *this;
}

/**
 * @brief Metodo publico para mostrar el arbol usando recursividad
 */

template<class T>
void AVL<T>::muestra() {
    muestra2(groot, 0);
}

/**
 * @brief Metodo recursivo para mostrar en pantalla.
 * @param p - Nodo que se quiere mostar
 * @param l - Contador de niveles (para simular las ramas)
 */
template<class T>
void AVL<T>::muestra2(NodoAVL<T>* p, int l) {
    int i;
    if (p != NULL) {
        muestra2(p->der, l + 1);
        for (i = 0; i < l; i++)
            cout << "   ";
        cout << p->dato << endl;
        muestra2(p->izq, l + 1);
    }
}

/**
 * @brief Rotacion Izquierda del arbol
 * @param p - Nodo por el que rotar
 */
template<class T>
void AVL<T>::rotIzqda(NodoAVL<T>* &p) {
    NodoAVL<T> *q = p, *r;
    p = r = q->der;
    q->der = r->izq;
    r->izq = q;
    q->bal++;
    if (r->bal < 0) q->bal += -r->bal;
    r->bal++;
    if (q->bal > 0) r->bal += q->bal;
}

/**
 * @brief Rotacion Derecha del arbol
 * @param p - Nodo por el que rotar
 */
template<class T>
void AVL<T>::rotDecha(NodoAVL<T>* &p) {
    NodoAVL<T> *q = p, *l;
    p = l = q->izq;
    q->izq = l->der;
    l->der = q;
    q->bal--;
    if (l->bal > 0) q->bal -= l->bal;
    l->bal--;
    if (q->bal < 0) l->bal -= -q->bal;
}

/**
 * @breif Insertar un dato (funcion recursiva privada)
 * @param dato - Dato T a insertar
 * @param nodo - Nodo en el que insertar (usado para recursividad)
 * @return nodo - para recursividad
 */
template <class T>
NodoAVL<T>* AVL<T>::insertaDato(T& dato, NodoAVL<T>* &nodo) {
    if (!nodo) {
        nodo = new NodoAVL<T>(dato);
    } else {
        if (dato < nodo->dato) {
            nodo->izq = insertaDato(dato, nodo->izq);
        } else if (dato > nodo->dato) {
            nodo->der = insertaDato(dato, nodo->der);
        } else return 0;
        nodo->bal = calculaAltura(nodo->izq) - calculaAltura(nodo->der);
        if (nodo->bal > 1) rotDecha(nodo);
        else if (nodo->bal < -1) rotIzqda(nodo);
    }
    return nodo;
}


/**
 * @breif Metodo inserta (publico) que usa el recursivo
 * @param dato - Dato a insertar
 * @pre El dato no deberia de estar ya contenido en el arbol para una inserccion satisfactoria (definicion de AVL)
 * @return bool - Si se ha insertado con exito o no
 */
template <class T>
bool AVL<T>::inserta(T& dato) {
    T* restultado;
    bool incluido = busca(dato, *restultado);
    if (!incluido) {
        insertaDato(dato, groot);
        return true;
    }

    return false;
}

/**
 * @brief Metodo privado recursivo para recorrer el arbol en inorden
 * @param p - Nodo a mostrar
 * @param nivel - Contador de nivel
 */
template <class T>
void AVL<T>::inorden(NodoAVL<T>* p, int nivel) {
    if (p) {
        inorden(p->izq, nivel + 1);
        if (!p->izq) {
            inorden(p->der, nivel + 1);
            cout << p->dato << endl;
        } else cout << p->dato << endl;
        inorden(p->der, nivel + 1);
    }
}

/**
 * @brief Metodo publico para recorrer en inorden a partir de privado
 */
template <class T>
void AVL<T>::recorreInorden() {
    inorden(groot, 0);
}

/**
 * @brief Contar el numero de elementos de un arbol AVL (metodo recursivo privado)
 * @param nodo - Nodo del arbol a contar
 * @param maxElementos - Contador de elementos
 */
template <class T>
void AVL<T>::cuantosElementos(NodoAVL<T>* nodo, int &maxElementos) {
    if (nodo) {

        if (nodo->izq) {
            maxElementos++;
            cuantosElementos(nodo->izq, maxElementos);
        }

        if (nodo->der) {
            maxElementos++;
            cuantosElementos(nodo->der, maxElementos);
        }

    }
}

/**
 * @brief Contar numero de elementos del arbol
 * @return max + 1 - Numero de elementos del arbol + 1 (que es la raiz)
 */
template <class T>
unsigned int AVL<T>::numElementos() {
    int max = 0;
    cuantosElementos(groot, max);
    return max + 1;
}

/**
 * @brief Metodo recursivo privado para la busqueda de un dato
 * @param dato - dato T a buscar
 * @param nodo - Nodo en el cual se busca si contiene o no el dato (usado para recursividad)
 * @return nodo - Si es el nodo el cual contiene el dato.
 */
template <class T>
NodoAVL<T>* AVL<T>::buscaDato(T& dato, NodoAVL<T>* nodo) {
    if (nodo) {
        if (nodo->dato == dato) {
            return nodo;
        } else {
            if (dato > nodo->dato) buscaDato(dato, nodo->der);
            else if (dato < nodo->dato) buscaDato(dato, nodo->izq);
        }
    } else return nullptr;
}

/**
 * @brief Busqueda iterativa de un elemento sin usar recursividad
 * @param dato - Dato T a buscar en el arbol
 * @param resultado - Parametro de salido para recoger el dato buscado y comprobar si se ha encontrado o no
 *        explicitamente
 * @return bool - Si se ha encontrado con exito (true) o no (false)
 */
template <class T>
bool AVL<T>::buscaIt(T& dato, T& resultado) {
    NodoAVL<T>* aux = groot;
    NodoAVL<T>* sig = groot;
    while (sig) {
        resultado = aux->dato;
        if (aux->dato == dato) return true;
        else if (dato < aux->dato) sig = aux->izq;
        else if (dato > aux->dato) sig = aux->der;
        if (sig) aux = sig;
    }
    return false;
}
/**
 * @brief Funcion publica de Busqueda recursiva que usa la función privada
 * @param dato - Dato T a encontrar
 * @param resultado - Parametro de salido para recoger el dato buscado y comprobar si se ha encontrado o no
 *        explicitamente
 * @return bool - Si se ha encontrado con exito (true) o no (false)
 */
template <class T>
bool AVL<T>::busca(T& dato, T& resultado) {
    NodoAVL<T>* nodo = 0;
    nodo = buscaDato(dato, groot);
    if (nodo) {
        resultado = nodo->dato;
        return true;
    }
    return false;
}

/**
 * @brief Metodo privado recursivo para el calculo de la altura
 * @param nodo - NodoAVL<T>* usado para viajar por el arbol al comprobar su altura
 * @return altura - Altura del arbol, se considera que la raiz posee altura 1.
 *         (Si no se considera así, la altura del arbol seria altura-1).
 */
template <class T>
int AVL<T>::calculaAltura(NodoAVL<T>* nodo) {
    int altura = 0;
    if (nodo) {
        int altd = calculaAltura(nodo->der);
        int alti = calculaAltura(nodo->izq);
        altura = max(altd, alti) + 1;
    }
    return altura;
}

/**
 * @brief Metodo publico para el calculo de la altura, el cual usa el recursivo privado
 * @return maxa - Maximo entre las alturas de los dos subarboles.
 */
template <class T>
unsigned int AVL<T>::altura() {
    int maxa = max(calculaAltura(groot->der), calculaAltura(groot->izq));
    return maxa;
}

/**
 * @brief Borrado recursivo privado desde las hojas hacia arriba.
 * @param nodo - Nodo a borrar
 */
template <class T>
void AVL<T>::borrado(NodoAVL<T>* nodo) {
    if (nodo) {
        borrado(nodo->der);
        borrado(nodo->izq);
        delete nodo;
    }
}

/**
 * @bried Metodo recursivo privado que traslada todos los elementos del arbol a vector
 * @param v - Vector en el que se guardaran los elementos
 * @param p - Nodo a guardar
 */
template<class T>
void AVL<T>::guardaInorden(VDinamico<T>* v, NodoAVL<T>* p) {
    if (p) {
        guardaInorden(v, p->izq);
        if (!p->izq) {
            guardaInorden(v, p->der);
            v->insertar(p->dato);
        } else v->insertar(p->dato);
        guardaInorden(v, p->der);
    }

}

/**
 * @bried Exportar a CSV el contenido de un Arbol usando antes la funcion recursiva
 *        para guardar los elementos en un vector.
 */
template<class T>
void AVL<T>::exportaCSV() {
    if(numElementos() == 0) throw std::domain_error("[!] AVL<T>::exportaCSV: No hay elementos");
    VDinamico<T> aux;
    guardaInorden(&aux,groot);
    aux.exportaCSV("ArbolInorden.csv");
    cout << "Exportado correctamente.";
}


/**
 * @brief Destructor de ArbolAVL que usa la funcion de borrado recursiva
 */
template<class T>
AVL<T>::~AVL() {
    if(groot) borrado(groot);
}

#endif /* AVL_H */

