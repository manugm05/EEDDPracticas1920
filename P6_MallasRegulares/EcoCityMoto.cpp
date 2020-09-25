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
#include "Cliente.h"
#include "Moto.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

EcoCityMoto::EcoCityMoto() : idUltimo(0) {
    int cont = 0;
    clientes = THashClientes(16661);
    anadirClientes("clientes_v2.csv");
    anadirMotos("motos.txt");
    clientes.setPrimeraCarga(false);
    MallaRegular<PuntoRecarga> aux(37, 3, 38, 4, 10, 10);
    puntosrecarga = aux;

    while (cont < 300) {
        string id = "PR_" + to_string(cont);
        bool correcto = false;
        while (!correcto) {
            int dato0 = rand() % 101;
            int dato1 = rand() % 101;
            float x = 37.0 + ((dato0) / 100.0);
            float y = 3.0 + ((dato1) / 100.0);
            UTM pos(x, y);
            PuntoRecarga* p = new PuntoRecarga(id, pos);
            correcto = puntosrecarga.insertar(*p);
            if (!correcto) {
                delete p;
                p = nullptr;
            }
        }
        cont++;
    }
    cout << "== CARGADOS PUNTOS DE RECARGA ==\n";
    cout << "MaxElemetos/Celda: " << puntosrecarga.maxElementosPorCelda() << endl;
    cout << "Tamaño de la malla: " << puntosrecarga.gettamMalla() << endl;
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
    string matricula, estado, lat, longit,bat;
    int estadoint;
    float bateria=-1;
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
                getline(ss, bat, '%');
                latd = stod(lat);
                longd = stod(longit);
                estadoint = stoi(estado);
                dato0 = rand() % 101;
                if(bat.length()!=0) bateria = stof(bat);
                Moto* mot = new Moto(matricula, estadoint, latd, longd,bateria);
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
    Cliente* nuevo = nullptr;
    ifstream fich;
    string linea;
    string basura, dni, nombre, pass, direccion, latitud, longitud, puntos;
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
            if (linea != "" || total >= 0 && total <= 2) total++;
            if (total > 1 && linea[0] != '>') {
                ss << linea;
                getline(ss, dni, ';');
                getline(ss, nombre, ';');
                getline(ss, pass, ';');
                getline(ss, direccion, ';');
                getline(ss, latitud, ';');
                getline(ss, longitud, ';');
                getline(ss, puntos, ';');
                latd = stod(latitud);
                longd = stod(longitud);
                nuevo = new Cliente(dni, nombre, pass, direccion, latd, longd);
                if(puntos.length() != 0){
                    int p = stoi(puntos);
                    nuevo->setPuntos(p);
                }
                else nuevo->setPuntos(1 + rand() % 11);
                
            }
            if (total > 1 && linea[0] == '>') {
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
                nuevo->anadeIti(*iti);
                cuentaiti++;
            }
            if (nuevo != nullptr)
                clientes.insertar(0, dni, *nuevo);

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
    Cliente* cli = 0;
    bool encontrado = clientes.buscar(0, dni, cli);
    if (encontrado) return cli;
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
void EcoCityMoto::muestraTHashClientes() {
    vector<string> dnis = getDNI();
    for (int i = 0; i < dnis.size(); i++) {
        Cliente* cli = 0;
        clientes.buscar(0, dnis[i], cli);
        cout << "Cliente " << i << ": " << *cli << endl;
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
    vector<string> dnis = getDNI();
    UTM jaen(37.3, 38.4);
    for (int i = 0; i < dnis.size(); i++) {
        Cliente* cli = 0;
        clientes.buscar(0, dnis[i], cli);
        int numItis = 1 + rand() % 50;
        UTM poscli = cli->getPosicion();
        cli->crearItinerarios(numItis, idUltimo, poscli, jaen);
        idUltimo += numItis;
    }

}

/**
 * @brief Localiza todas las motos de EcoCityMoto sin bateria
 * @return vector<Moto> - Vector que contiene las motos sin bateria
 */
vector<Moto> EcoCityMoto::localizasSinBateria(float porctBateria) {
    vector<Moto> vecMotos;
    for (int i = 0; i < motos.size(); i++) {
        float x = motos[i].getPosicion().latitud, y = motos[i].getPosicion().longitud;
        bool fueraAmbito = puntosrecarga.fueraAmbito(x, y);
        if (motos[i].getPorcentajeBateria() < porctBateria && !fueraAmbito) {
            vecMotos.push_back(motos[i]);
        }
    }
    return vecMotos;
}

/**
 * @brief Guarda los clientes junto con sus itinerarios en un archivo CSV
 * @param nombrefich - Archivo que contendrá los clientes y sus itinerarios
 */
void EcoCityMoto::guardarClientesItinerarios(string nombrefich) {
    vector<string> dnis = getDNI();
    std::ofstream fich;
    fich.open(nombrefich);
    fich << "DatosCliente--;ID;Fecha;Minutos;CoordenadasInicio;CoordenadasFin\n";
    for (int i = 0; i < dnis.size(); i++) {
        Cliente* cli = 0;
        clientes.buscar(0, dnis[i], cli);
        fich << *cli;
    }

    fich.close();
}

void EcoCityMoto::guardarMotos(string nombrefich) {
    std::ofstream fich;
    fich.open(nombrefich);
    fich << "matricula;estado;lat;long;bateria\n";
    for (int i = 0; i < motos.size(); i++) {
        fich << motos[i];
    }
    fich.close();
}

/**
 * @brief Añade nuevo cliente al atributo clientes de EcoCityMoto
 * @param cli - Cliente que se desea añadir
 * @return bool - True si se ha conseguido añadir el cliente, False si ya pertenecía a la base de datos
 */
bool EcoCityMoto::nuevoCliente(Cliente& cli) {
    string dni = cli.getDni();
    if (!buscarCliente(dni)) {
        clientes.insertar(0, dni, cli);
        return true;
    } else return false;
}

/**
 * @brief Elimina un cliente de la base de datos segun su dni
 * @param dni - String para buscar y eliminar el cliente
 * @return bool - True si elimina el cliente, False si no lo elimmina
 */
bool EcoCityMoto::eliminarCliente(string dni) {
    if (buscarCliente(dni)) {
        clientes.borrar(0, dni);
        return true;
    } else return false;
}

/**
 * @brief Muestra la información relevante de la tabla Hash
 */
void EcoCityMoto::infoTHash() {
    cout << "==ESTADO DE LA TABLA HASH==\n";
    cout << "Factor de carga: " << clientes.factorCarga() << endl;
    cout << "Tamanio tabla: " << clientes.tamaTabla() << endl;
    cout << "Elementos en la tabla: " << clientes.getnClientes() << endl;
    cout << "Max Colisiones: " << clientes.maxColisiones() << endl;
    cout << "Promedio colisiones: " << clientes.promedioColisiones() << endl;
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
        cout << "[*] Nombre del archivo: clientes_v2.csv\n";
        guardarClientesItinerarios("clientes_v2.csv");
        cout << "[*] Nombre del archivo: motos.txt";
        guardarMotos("motos.txt");
    }
}

vector<string> EcoCityMoto::getDNI() {
    return clientes.getDNI();
}

void EcoCityMoto::numColisiones() {
    cout << "Número de colisiones al insertar: " << clientes.getNumColisiones() << endl;
}

/**
 * @brief Borrar los nprimeros primeros clientes del sistema
 * @param nprimeros - Numero de clientes a eliminar
 * @throw domain_error - Si se supera el numero de clientes en la tabla.
 */
void EcoCityMoto::borradoMasivo(int nprimeros) {
    if (nprimeros > clientes.getnClientes()) throw std::domain_error("[!] El numero introducido de clientes supera al de la tabla.");
    vector<string> dnis = getDNI();
    for (int i = 0; i < nprimeros; i++) {
        Cliente* eli = buscarCliente(dnis[i]);
        cout << "Eliminando cliente " << eli->getNombre() << " con DNI " << dnis[i] << endl;
        eliminarCliente(dnis[i]);

    }
}

float EcoCityMoto::mostrarFactorCarga() {
    return clientes.factorCarga();
}

void EcoCityMoto::redispersarHash(unsigned int tama) {
    clientes.redispersar(tama);
}

/**
 * @brief Busca el punto de recarga mas cercano a un cliente
 * @param cli - Cliente que se pasa como parametro
 * @return PuntoRecarga - Punto de recarga mas cercano a un cliente
 */
PuntoRecarga* EcoCityMoto::puntoRecargaCercano(Cliente *cli) {
    UTM pos = cli->getPosicion();
    PuntoRecarga* pr = puntosrecarga.buscaCercano(pos.latitud, pos.longitud);
    return pr;
}

