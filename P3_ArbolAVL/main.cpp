/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: lidia
 *
 */

#include <cstdlib>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <complex>
#include "Cliente.h"
#include "UTM.h"
#include "ListaDEnlazada.h"
#include "Iterator.h"
#include "AVL.h"
#include "EcoCityMoto.h"

using namespace std;

void leeClientes(string fileNameClientes, VDinamico<Cliente>& v) {
    ifstream fe; //Flujo de entrada
    string linea; //Cada línea tiene un clienete
    int total = 0; //Contador de líneas o clientes

    //Variables auxiliares para almacenar los valores leídos
    string dni, nombre, pass, direccion, latitud, longitud;
    double dlat, dlon;



    //Asociamos el flujo al fichero 
    fe.open(fileNameClientes);

    if (fe.good()) {
        //Mientras no se haya llegado al final del fichero
        while (!fe.eof()) {
            getline(fe, linea); //Toma una línea del fichero
            stringstream ss; //Stream que trabaja sobre buffer interno         

            if (linea != "") {
                ++total;
            }

            if (total > 1) {
                //Inicializamos el contenido de ss
                ss << linea;

                //Leemos el NIF
                getline(ss, dni, ';'); //El carácter ; se lee y se elimina de ss

                //Leemos el pass
                getline(ss, pass, ';'); //El caráter ; se lee y se elimina de ss

                //Leemos el nombre
                getline(ss, nombre, ';'); //El caráter ; se lee y se elimina de ss

                //Leemos la dirección
                getline(ss, direccion, ';'); //El caráter ; se lee y se elimina de ss

                //Leemos la latitud y longitud
                getline(ss, latitud, ';'); //El caráter ; se lee y se elimina de ss
                getline(ss, longitud, ';'); //El caráter ; se lee y se elimina de ss

                dlat = stod(latitud);
                dlon = stod(longitud);

                //con todos los atributos leídos, se crea el cliente
                Cliente c(dni, pass, nombre, direccion, dlat, dlon);
                v.insertar(c);
                //                cout << "leido cliente " << total << "  ";
            }
        }

        cout << "Total de clientes en el fichero: " << total << endl;
        fe.close(); //Cerramos el flujo de entrada
    } else {
        cerr << "No se puede abrir el fichero" << endl;
    }
}

/**
 * @brief busqueda de elementos segun nombre.
 * @param nombre - string
 * @param v - VDinamico
 * @pre v debe estar instanciado en T=Cliente
 * @return Número de veces que se borra un cliente con el nombre dado
 */
int busquedaPorNombre(std::string nombre, VDinamico<Cliente>& v) {
    Cliente aux = Cliente(nombre);
    int pos = 0;
    int cont = 0;
    pos = v.busquedaBin(aux);
    while (pos != -1) {
        v.borrar(pos);
        cont++;
        pos = v.busquedaBin(aux);
    }
    return cont;
}

/**
 * @brief Cálculo de la distancia máxima entre dos clientes de un vector.
 * @param v - Objeto de tipo VDinamico<Cliente>
 * @return distanciaMax - Dato de doble precisión que almacena la distancia pedida.
 */
double distanciaMaxima(VDinamico<Cliente>& v) {
    double distanciaMax = 0;
    for (int i = 0; i < v.getTam(); i++) {
        for (int j = i + 1; j < v.getTam(); j++) {
            double distancia = v[i].distancia(v[j]);
            if (distanciaMax <= distancia) distanciaMax = distancia;
        }
    }
    return distanciaMax;
}

/**
 * @brief Búsqueda de latitudes y longitudes minimas y maximas.
 * @param v - Parámetro de entrada tipo VDinamico<Cliente>.
 * @param min - Parámetro de salida tipo UTM. Se guardan las coordenadas mínimas.
 * @param max - Parámetro de salida tipo UTM. Se guardan las coordenadas máximas.
 * @throw std::invalid_argument si el vector esta vacio.
 */
void limites(VDinamico<Cliente>& v, UTM& min, UTM& max) {
    if (v.getTam() == 0) throw std::invalid_argument("[!] Main.cpp limites(): El vector no tiene elementos.");
    double latmax, longmax, latmin, longmin;
    latmax = 0;
    longmax = 0;
    latmin = v[0].getPosicion().latitud;
    longmin = v[0].getPosicion().longitud;
    for (int i = 0; i < v.getTam(); i++) {
        if (latmax < v[i].getPosicion().latitud)
            latmax = v[i].getPosicion().latitud;
        if (longmax < v[i].getPosicion().longitud)
            longmax = v[i].getPosicion().longitud;
        if (latmin > v[i].getPosicion().latitud)
            latmin = v[i].getPosicion().latitud;
        if (longmin > v[i].getPosicion().longitud)
            longmin = v[i].getPosicion().longitud;
    }
    max = UTM(latmax, longmax);
    min = UTM(latmin, longmin);

}

int main(int argc, char** argv) {
    try {
        srand(time(0));
        EcoCityMoto ecocity;
        Cliente* usuario = nullptr;
        Moto* motocercana = nullptr;
        int op = -1;
        bool datos = false; //¿Se han cargado los datos?
        bool viajando = false; //¿Se esta usando una moto?
        bool usuarioactivo = false; //¿Se ha cargado un usuario mediante la opcion 2?
        string dni;
        do {
            cout << "\n\nEcoCityMoto\n";
            cout << "Menu de opciones\n";
            cout << "=================\n";
            cout << "1) Cargar motos y clientes\n";
            cout << "2) Bucar y cargar cliente por DNI.\n";
            cout << "3) Localizar moto cercana del cliente buscado, desbloquearla y crear itirenario\n";
            cout << "4) Terminar Trayecto\n";
            cout << "5) Desactivar usuairo\n";
            cout << "6) Mostrar arbol en inorden (no recomendado si hay muchos datos)\n";
            cout << "7) Exportar clientes a CSV de Ecocity en inorden.\n";
            cout << "8) Mostrar altura del arbol de Clientes\n\n";
            cout << "Seleccione una opcion (otra para salir) > ";
            cin >> op;
            switch (op) {
                case 1:
                    if (datos) cout << "\n[!] Ya estan cargados!\n";
                    else {
                        ecocity.anadirClientes("clientes_v2.csv");
                        ecocity.anadirMotos("motos.txt");
                        datos = true;
                    }
                    break;
                case 2:
                    if (!datos) cout << "\n[!] Necesita cargar los datos antes\n";
                    else if (usuarioactivo) cout << "\n[!]Ya has activado un usuario\n";
                    else {
                        cout << "Introduzca el DNI que desea buscar: ";
                        cin >> dni;
                        usuario = ecocity.buscarCliente(dni);
                        if (usuario) {
                            cout << "\nAsignando EcoCityMoto a cliente...\n";
                            usuario->setAcceso(&ecocity);
                            cout << "Hecho! El cliente " << *usuario << "tiene ahora acceso.";
                            usuarioactivo = true;
                        }
                    }
                    break;
                case 3:
                    if (!datos)cout << "\n[!] Necesita cargar los datos antes\n";
                    else if (!usuarioactivo) cout << "\n[!] Necesita actvar un usuario\n";
                    else if (viajando) cout << "\nYa estas usando una moto!\n";
                    else {
                        motocercana = usuario->buscarMotoCercana();
                        cout << "La moto mas cercana al cliente " << *usuario << " y disponible es: " << *motocercana;
                        char c = 'q';
                        while (c != 's' || c != 'n') {
                            cout << "\nDesea comenzar a usar la moto? (s/n)";
                            cin >> c;
                            {
                                if (c == 's') {
                                    usuario->desbloquearMoto(*motocercana);
                                    cout << "Moto desbloqueada y modo viaje activado. Buen viaje!\n";
                                    viajando = true;
                                    break;
                                } else if (c == 'n') break;
                            }
                        }
                    }
                    break;
                case 4:
                    if (!viajando) cout << "\n[!] Debes estar en modo viaje!!!\n";
                    else {
                        usuario->terminarTrayecto();
                        viajando = false;
                        cout << "Trayecto terminado. Modo viaje desactivado. Informacion de tus trayectos realizados: \n" << *usuario;
                    }
                    break;
                case 5:
                    if (!usuarioactivo) cout << "\n[!] No hay ningun usuario activo!!!\n";
                    else {
                        usuarioactivo = false;
                        usuario = 0;
                        cout << "Usuario desactivado\n";
                    }
                    break;
                case 6:
                    if (!datos) cout << "\n[!] Necesita cargar los datos antes\n";
                    else ecocity.muestraArbolClientes();
                    break;
                case 7:
                    if (!datos) cout << "\n[!] Necesita cargar los datos antes\n";
                    else ecocity.exportaClientesCSV();
                    break;
                case 8:
                    if (!datos) cout << "\n[!] Necesita cargar los datos antes\n";
                    else cout << "\nAltura del arbol de clientes: " << ecocity.alturaArbolClientes();
                    break;
            }
        } while (op < 9 && op > 0);
    } catch (std::out_of_range& err) {
        cerr << err.what();
    } catch (std::invalid_argument& err) {
        cerr << err.what();
    } catch (std::domain_error& err) {
        cerr << err.what();
    } catch (std::exception& err) {
        cerr << "Ha habido un error inseperado.";
    }

    return 0;
}

