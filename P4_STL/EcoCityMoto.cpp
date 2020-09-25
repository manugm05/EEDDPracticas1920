/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EcoCityMoto.cpp
 * Author: Samuel
 * 
 * Created on 20 de octubre de 2019, 16:29
 */

#include <map>
#include <vector>
#include "fecha.h"
#include "EcoCityMoto.h"
#include "Moto.h"
#include "Cliente.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

EcoCityMoto::EcoCityMoto() : idUltimo(0) {
    anadirClientes("clientes_v2.csv");
    anadirMotos("motos.txt");

}

EcoCityMoto::EcoCityMoto(const EcoCityMoto& orig) {
}

/**
 * @brief Añade datos de motos desde un fichero a un vector dinamico
 * @param nombrefich - Fichero que contiene los datos de motos
 */
void EcoCityMoto::anadirMotos(string nombrefich) {
    ifstream fich;
    string linea;
    string matricula, estado, lat, longit;
    int estadoint;
    float bateria;
    int dato0;
    double latd, longd;
    int total = 0;
    fich.open(nombrefich);
    if (fich.good()) {
        while (!fich.eof()) {
            getline(fich, linea);
            stringstream ss;

            if (linea != "") total++;
            if (total > 1) {
                ss << linea;
                getline(ss, matricula, ';');
                getline(ss, estado, ';');
                getline(ss, lat, ';');
                getline(ss, longit, ';');
                latd = stod(lat);
                longd = stod(longit);
                estadoint = stoi(estado);
                dato0 = rand() % 101;
                bateria = 1 + rand() % 101 + (dato0) / 100.0;
                Moto* mot = new Moto(matricula, estadoint, latd, longd);
                motos.push_back(*mot);
            }

        }
    }
    fich.close();
    cout << "\n=== MOTOS AÑADIDAS CORRECTAMENTE ===\nTotal: " << motos.size() << endl;
}

/**
 * @brief Añade datos de cliente desde un fichero a un arbol AVL
 * @param nombrefich - Fichero que contiene los datos de clientes
 */
void EcoCityMoto::anadirClientes(string nombrefich) {
    ifstream fich;
    string linea;
    string basura, dni, nombre, pass, direccion, latitud, longitud;
    string id, fecha, dia, mes, anio, hora, minuto, minutos, ilat, ilong, flat, flong;
    int idn, min, idia, imes, ianio, ihora, imin;
    int cuentaiti = 0;
    double inilat, inilong, finallat, finallong;
    double latd, longd;
    int total = 0;
    fich.open(nombrefich);
    if (fich.good()) {
        while (!fich.eof()) {
            getline(fich, linea);
            stringstream ss;
            stringstream ss2;
            std::pair < std::map<string, Cliente>::iterator, bool> it;
            std::map<string, Cliente>::iterator i;
            if (linea != "" || total >= 0 && total <= 2) total++;
            if (total > 2 && linea[0] != '>') {
                ss << linea;
                getline(ss, dni, ';');
                getline(ss, nombre, ';');
                getline(ss, pass, ';');
                getline(ss, direccion, ';');
                getline(ss, latitud, ';');
                getline(ss, longitud, ';');
                latd = stod(latitud);
                longd = stod(longitud);
                Cliente nuevo(dni, nombre, pass, direccion, latd, longd);
                it = clientes.insert(std::pair<string, Cliente>(dni, nuevo));
                if (!it.second) cout << "Cliente con DNI " << dni << " ya estaba en el arbol. Inserción abortada.\n";
            }
            if (total > 2 && linea[0] == '>') {
                i = clientes.find(dni);
                ss << linea;
                getline(ss, basura, '>');
                getline(ss, id, ';');
                getline(ss, fecha, ';');
                getline(ss, minutos, ';');
                getline(ss, ilat, ';');
                getline(ss, ilong, ';');
                getline(ss, flat, ';');
                getline(ss, flong, ';');
                ss2 << fecha;
                getline(ss2, dia, '/');
                getline(ss2, mes, '/');
                getline(ss2, anio, ' ');
                getline(ss2, hora, ':');
                getline(ss2, minuto, ';');
                inilat = stod(ilat);
                inilong = stod(ilong);
                idia = stoi(dia);
                imes = stoi(mes);
                ianio = stoi(anio);
                ihora = stoi(hora);
                imin = stoi(minuto);
                Fecha fech(idia, imes, ianio, ihora, imin);
                UTM inicio(inilat, inilong);
                finallat = stod(flat);
                finallong = stod(flong);
                UTM fin(finallat, finallong);
                idn = stoi(id);
                min = stoi(minutos);
                Itinerario* iti = new Itinerario(idn, min, inicio, fin, fech);
                i->second.anadeIti(*iti);
                cuentaiti++;
            }

        }
    }

    fich.close();
    if (cuentaiti == 0) {
        cout << "== GENERANDO ITIRENARIOS ==\n";
        crearItinerarios();
    }
    cout << "\n=== CLIENTES AÑADIDOS CORRECTAMENTE ===\n";
}

/**
 * @brief Metodo que busca un cliente en el VDinamico segun su DNI
 * @param dni - string que sirve para identificar cada cliente
 * @return Cliente* - es el cliente buscado
 */
Cliente* EcoCityMoto::buscarCliente(string& dni) {
    std::map<string, Cliente>::iterator i;
    i = clientes.find(dni);
    if (i != clientes.end()) {
        return &(i->second);
    }
    return nullptr;
}

/**
 * @brief Calcula la distancia entre dos coordenadas
 * @param orig - Coordenada uno
 * @param destino - Coordenada dos
 * @return double - Distancia comprendida entre las dos coordenadas
 */
double EcoCityMoto::calculaDistancia(UTM& orig, UTM& destino) {
    double cordX = destino.latitud - orig.latitud;
    double cordY = destino.longitud - orig.longitud;
    double distancia = sqrt(pow(cordX, 2) + pow(cordY, 2));
    return distancia;
}

/**
 * @brief Busca la moto mas cercana a la ubicacion que se le pasa como parametro
 * @param ubicacion - UTM posicion formada por coordenadas de longitud y latitud
 * @return Moto* - devuelve la moto mas cercana dada la ubicacion introducida
 */
Moto* EcoCityMoto::localizaMotoCercana(UTM& ubicacion) {
    UTM auxpos = motos[0].getPosicion();
    double min = calculaDistancia(ubicacion, auxpos);
    double auxmin;
    int pos = -1;
    for (int i = 1; i < motos.size(); i++) {
        auxpos = motos[i].getPosicion();
        auxmin = calculaDistancia(ubicacion, auxpos);
        if (auxmin < min && motos[i].getEstatus() == 0) {
            min = auxmin;
            pos = i;
        }
    }
    if (pos != -1) {
        cout << "\n== MOTO MAS CERCANA ENCONTRADA ==\n";
        cout << "Distancia desde tu ubicacion hasta la moto: " << auxmin;
        cout << "\nDatos de la moto: " << motos[pos] << endl;
        cout << "Posicion actual: " << "( " << motos[pos].getPosicion().latitud << ", " << motos[pos].getPosicion().longitud << " )" << endl;
        cout << "Porcentaje de bateria: " << motos[pos].getPorcentajeBateria() << " %" << endl;
        return &(motos[pos]);
    } else return nullptr;
}

/**
 * @brief Desbloquea la moto que ha solicitado un cliente
 * @param m - Moto cercana al cliente que ha pedido que sea desbloqueada
 * @param cli - Cliente que solicita el desbloqueo
 */
void EcoCityMoto::desbloqueaMoto(Moto& m, Cliente* cli) {
    idUltimo++;
    m.seActiva(cli);
}

unsigned int EcoCityMoto::getIdUltimo() const {
    return idUltimo;
}

/**
 * @brief Metodo que exporta los datos de clientes en un fichero
 */
void EcoCityMoto::exportaClientesCSV() {
    //    clientes.exportaCSV();
}

/**
 * @brief Localiza una moto asociada a un cliente
 * @param p - Cliente que tiene asociada una moto
 * @return Moto* - Moto que esta asociada a un cliente
 */
Moto* EcoCityMoto::localizaMoto(Cliente *p) {
    for (int i = 0; i < motos.size(); i++) {
        if (motos[i].getPcliente() == p) {
            return &motos[i];
        }
    }
}

/**
 * @brief Muestra el arbol AVL de clientes
 */
void EcoCityMoto::muestraArbolClientes() {
    map<string, Cliente>::iterator i = clientes.begin();
    while (i != clientes.end()) {
        cout << "Clave: " << i->first << " | " << "Cliente: " << i->second << endl;
        i++;
    }
}

/**
 * @brief Devuelve la altura del arbol AVL de clientes
 * @return int - Altura del arbol AVL
 */
int EcoCityMoto::alturaArbolClientes() {
    //    return clientes.();
}

/**
 * @brief Crea los itinerarios de los clientes
 * @param num - Numero de itinerarios que se van a crear
 * @param min - UTM posicion minima de los itinerarios
 * @param max - UTM posicion maxima de los itinerarios
 */
void EcoCityMoto::crearItinerarios() {
    map<string, Cliente>::iterator i = clientes.begin();
    UTM jaen(37.3, 38.4);
    while (i != clientes.end()) {
        int numItis = 1 + rand() % 50;
        UTM poscli = i->second.getPosicion();
        i->second.crearItinerarios(numItis, idUltimo, poscli, jaen);
        i++;
        idUltimo += numItis;
    }
}

/**
 * @brief Localiza todas las motos de EcoCityMoto sin bateria
 * @return vector<Moto> - Vector que contiene las motos sin bateria
 */
vector<Moto> EcoCityMoto::localizasSinBateria(float porctBateria) { // NO SE SI ESTA FUNCION DEBE DEVOLVER EL VECTOR O HACERLE COUTS
    int total = 0;
    vector<Moto> vecMotos;
    for (int i = 0; i < motos.size(); i++) {
        if (motos[i].getPorcentajeBateria() < porctBateria) {
            vecMotos.push_back(motos[i]);
            total++;
        }
    }
    cout << "Total motos sin bateria: " << total << endl;
    return vecMotos;
}

/**
 * @brief Guarda los clientes junto con sus itinerarios en un archivo CSV
 * @param nombrefich - Archivo que contendrá los clientes y sus itinerarios
 */
void EcoCityMoto::guardarClientesItinerarios(string nombrefich) {
    std::ofstream fich;
    fich.open(nombrefich);
    fich << "Nombre;NIE\nID;Fecha;Minutos;CoordenadasInicio;CoordenadasFin\n";
    map<string, Cliente>::iterator i = clientes.begin();
    while (i != clientes.end()) {
        fich << i->second;
        i++;

    }
    fich.close();
}

/**
 * @brief Añade nuevo cliente al atributo clientes de EcoCityMoto
 * @param cli - Cliente que se desea añadir
 * @return bool - True si se ha conseguido añadir el cliente, False si ya pertenecía a la base de datos
 */
bool EcoCityMoto::nuevoCliente(Cliente cli) {
    std::map<string, Cliente>::iterator i;
    string dni = cli.getDni();
    i = clientes.find(dni);
    if (i == clientes.end()) {
        clientes.insert(std::pair<string, Cliente>(dni, cli));
        return true;
    } else return false;
}

/**
 * @brief Elimina un cliente de la base de datos segun su dni
 * @param dni - String para buscar y eliminar el cliente
 * @return bool - True si elimina el cliente, False si no lo elimmina
 */
bool EcoCityMoto::eliminarCliente(string dni) {
    std::map<string, Cliente>::iterator i;
    i = clientes.find(dni);
    if (i != clientes.end()) {
        clientes.erase(i);
        return true;
    } else return false;
}

/**
 * @brief Destructor de EcoCityMoto que escribe en disco.
 */
EcoCityMoto::~EcoCityMoto() {
    char sn = '?';
    while (sn != 's' && sn != 'n') {
        cout << "[!] Va a salir de la aplicacion. Desea guardar los datos en un fichero csv?\n";
        cout << "Respuesta (s/n): ";
        cin >> sn;
    }
    if (sn == 's') {
        cout << "[*] Nombre del archivo: clientes_v2.csv";
        guardarClientesItinerarios("clientes_v2.csv");
    }
}



