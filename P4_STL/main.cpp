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
#include "EcoCityMoto.h"

using namespace std;

int main(int argc, char** argv) {
    srand(time(0));
    EcoCityMoto ecocity;
    Cliente* usuario = nullptr;
    Moto* motocercana = nullptr;
    int op = -1;
    bool viajando = false;
    bool usuarioactivo = false;
    string dni;
    do {
        cout << "\n┌───────────────────┐\n";
        cout << "├─────  E C O C I T Y ──────┤\n";
        cout << "└───────────────────┘\n\n";
        cout << "╔═══════════════════╗\n";
        cout << "║           Menu de opciones           ║\n";
        cout << "╚═══════════════════╝\n\n";
        cout << "1) Agregar cliente a la aplicación\n";
        cout << "2) Bucar cliente por DNI.\n";
        cout << "3) Localizar moto cercana del cliente buscado, desbloquearla y crear itirenario\n";
        cout << "4) Terminar Trayecto\n";
        cout << "5) Desactivar usuario (Hacer logoff)\n";
        cout << "6) Mostrar arbol (no recomendado si hay muchos datos)\n";
        cout << "7) Mostrar motos sin bateria\n";
        cout << "8) Borrar usuario\n";
        cout << "\n[*] Seleccione una opcion (otra para salir) > ";
        cin >> op;
        cout << endl;
        switch (op) {
            case 1:
            {
                bool noesta = false;
                string dni2, pass, nombre, dire;
                double lat, longi;
                cout << "Introduce el DNI del nuevo cliente: ";
                cin >> dni2;
                cout << "Introduce la contrasenia del nuevo cliente: ";
                cin >> pass;
                cout << "Introduce el nombre del nuevo cliente: ";
                std::getline(std::cin>>ws, nombre);
                cout << "Introduce la direccion del nuevo cliente: ";
                std::getline(std::cin>>ws, dire);
                cout << "Introduce la latitud en la que se encuentra: ";
                cin >> lat;
                cout << "Introduce la longitud en la que se encuentra: ";
                cin >> longi;
                Cliente* cli = new Cliente(dni2, pass, nombre, dire, lat, longi);
                noesta = ecocity.nuevoCliente(*cli);
                if (noesta) cout << "\n[!] Cliente aniadido correctamente!\n\n";
                else cout << "\n[!] Ya existe un cliente con el DNI " << dni2 << "\n\n";
            }
                break;
            case 2:
                if (usuarioactivo) cout << "\n[!]Ya has activado un usuario\n";
                else {
                    cout << "Introduzca el DNI que desea buscar: ";
                    cin >> dni;
                    usuario = ecocity.buscarCliente(dni);
                    if (usuario) {
                        cout << "== BUSQUEDA SATISFACTORIA ==\n";
                        cout << "Usuario encontrado: " << *usuario;
                        char sn = '?';
                        while (sn != 's' && sn != 'n') {
                            cout << "Quieres asiganar a este usuario EcoCityMoto?\n";
                            cout << "Respuesta (s/n): ";
                            cin >> sn;
                        }
                        if (sn == 's') {
                            cout << "Asignando EcoCityMoto a cliente...\n";
                            usuario->setAcceso(&ecocity);
                            cout << "Hecho! El cliente " << *usuario << "tiene ahora acceso.";
                            usuarioactivo = true;
                        }
                    } else cout << "\n[!] No se ha encontrado al usuarui con DNI " << dni << endl;
                }
                break;
            case 3:
                if (!usuarioactivo) cout << "\n[!] Necesita actvar un usuario\n";
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
                    cout << "Moto usuada: " << *motocercana << endl;
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
            {
                ecocity.muestraArbolClientes();
                break;
            }
            case 7:
            {
                cout << "Motos sin bateria: \n";
                vector<Moto> vecM;
                vecM = ecocity.localizasSinBateria(15);
                bool nobat = false;
                if (motocercana) {
                    string ID = motocercana->getId();
                    for (int i = 0; i < vecM.size(); i++) {
                        if (vecM[i].getId() == ID) {
                            cout << vecM[i] << " - Moto utilizada por el cliente" << endl;
                            nobat = true;
                        } else cout << vecM[i] << endl;
                    }
                    if (nobat) cout << "La moto usada por el cliente no tiene bateria: " << *motocercana << endl;
                } else {
                    for (int i = 0; i < vecM.size(); i++) {
                        cout << vecM[i] << endl;
                    }
                }
                cout << "Hay un total de " << vecM.size() << " motos sin bateria.\n";
            }
                break;
            case 8:
            {
                string dni3;
                char sn;
                Cliente* compruebaborrado;
                bool satisfactorio = false;
                cout << "Introduzca el DNI del usuario a eliminar: ";
                cin >> dni3;
                satisfactorio = ecocity.eliminarCliente(dni3);
                compruebaborrado = ecocity.buscarCliente(dni3);
                if (satisfactorio && !compruebaborrado) cout << "\n[!] Cliente borrado correctamente\n";
                else cout << "\n[!] Ha habido un error en el borrado, el cliente con DNI " << dni3 << " no existe.\n";
            }
                break;
        }
    } while (op > 0 && op < 9);
    return 0;
}

