/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Cliente.cpp
 * Author: admin
 * 
 * Created on 26 de septiembre de 2019, 17:08
 */

#include "Cliente.h"
#include "Itinerario.h"
#include <sstream>
#include <iostream>
#include <cmath>
#include <fstream>
#include "Moto.h"
#include "EcoCityMoto.h"

Cliente::Cliente(string _dni, string _pass, string _nombre, string _direccion, double _latitud, double _longitud) :
dni(_dni), pass(_pass), nombre(_nombre), direccion(_direccion), posicion(_latitud, _longitud), display(""), puntos(10) {
}

Cliente::Cliente() {

}

Cliente::~Cliente() {

}

string Cliente::getNombre() const {
    return nombre;
}

Cliente& Cliente::operator=(const Cliente& right) {
    // Check for self-assignment!
    if (this == &right) return *this;
    direccion = right.direccion;
    dni = right.dni;
    nombre = right.nombre;
    pass = right.pass;
    posicion = right.posicion;
    itirenario = right.itirenario;
    display = right.display;
    puntos = right.puntos;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Cliente& obj) {
    os << obj.dni << ";" << obj.pass << ";" << obj.nombre << ";" << obj.direccion << ";" << obj.posicion.latitud << ";" << obj.posicion.longitud << ";" << obj.puntos << endl;
    list<Itinerario>::const_iterator i = obj.itirenario.begin(); // RESUELTO
    while (i != obj.itirenario.end()) {
        os << ">" << *i << endl;
        i++;
    }
    return os;
}

/**
 * @brief Operador < enfocado a DNI.
 * @param right
 * @return true - si alfabeticamente el nombre del objeto que llama al operador es menor y viceversa
 */
bool Cliente::operator<(const Cliente& right) const {
    if (dni < right.dni) return true;
    return false;
}

/**
 * @brief Operador > enfocado a DNI
 * @param right
 * @return true - si alfabeticamente el nombre del objeto que llama al operador es mayor y viceversa
 */
bool Cliente::operator>(const Cliente& right) const {
    return !(*this < right);
}

/**
 * @breif Operador==. Enfocado a primer nombre.
 * @param right - Objeto de clase cliente..
 * @return True si el primer nombre coincide , false si no.
 */
bool Cliente::operator==(const Cliente& right) const {
    stringstream ss;
    std::string corte;
    ss << right.dni;
    getline(ss, corte, ' ');
    if (corte == dni) return true;
    return false;
}

/**
 * @brief Calculo de la distancia entre 2 clientes mediante el vector formado por la cordenadas
 * @param cli - Objeto tipo cliente
 * @return double - Módulo del vector (cordX,cordY).
 */
double Cliente::distancia(const Cliente& cli) {
    double cordX = posicion.latitud - cli.posicion.latitud;
    double cordY = posicion.longitud - cli.posicion.longitud;
    double distancia = sqrt(pow(cordX, 2) + pow(cordY, 2));
    return distancia;
}

Cliente::Cliente(std::string nombre) : nombre(nombre) {
}

UTM Cliente::getPosicion() const {
    return posicion;
}

/**
 * @bried Crear un numero de itirenarios para un cliente
 * @param num - Entero. Numero de iterenarios a crear
 * @param idUltimo - Entero. ID del último itirenario creado
 * @param min - Coordenadas mínimas para la generación de itirenarios
 * @param max - Coordenadas máximas para la generación de itirenarios
 */
void Cliente::crearItinerarios(unsigned int num, int idUltimo, UTM min, UTM max) {
    Fecha fecha;
    UTM inicio, fin;
    for (int i = 0; i < num; i++) {
        Itinerario* iti = new Itinerario();
        int dia = 1 + rand() % 29;
        int mes = 1 + rand() % 11;
        int anio = 2019;
        int hora = 1 + rand() % 22;
        int minut = 1 + rand() % 59;

        if (mes == 2 && dia > 28 && anio % 4 == 0) dia = 28;
        if (dia < 1) {
            if (dia == 0) dia = 1;
            else dia *= -1;
        }

        fecha.asignarDia(dia, mes, anio);
        fecha.asignarHora(hora, minut);
        iti->SetFecha(fecha);
        iti->SetId(idUltimo);
        iti->SetMinutos(rand() % 61);

        // Coordenadas aleatorias = (minimolat + rand()%Δlatitudes, minimolong + rand()%Δlongitudes)
        // Calculamos un numero aleatorio con rand() entre la distancia del maximo al minimo de las longitudes/latitudes, y le sumamos el minimo
        // para dejarlo en el intervalo [minimo,maximo].
        inicio = UTM(min.latitud + fmod(rand(), max.latitud - min.latitud), min.longitud + fmod(rand(), max.longitud - min.longitud));
        fin = UTM(min.latitud + fmod(rand(), max.latitud - min.latitud), min.longitud + fmod(rand(), max.longitud - min.longitud));

        iti->SetInicio(inicio);
        iti->SetFin(fin);

        itirenario.push_back(*iti);
        idUltimo++;
    }
}

/**
 * @brief Exportar los itirenarios de un cliente a CSV
 * @pre El cliente debe tener al menos 1 itirenario en su ListaDEnlazada
 * @return Crea un fichero CSV con los datos de todos los itirenarios en la carpeta de proyecto.
 */
void Cliente::iterenariosCSV() {
    //    if (itirenario.getTama() == 0) throw std::domain_error("[!] Cliente::itirenarioCSV(): No hay itirenarios para este cliente");
    std::ofstream arch;
    string nombrearch = "lista_Iti_" + nombre + dni + ".csv";
    arch.open(nombrearch);
    arch << *this << "\n";
    arch.close();
}

/**
 * @brief Llama a la funcion localizaMotoCercana para buscar una moto cercana del cliente
 * @return Puntero de Moto
 */
Moto* Cliente::buscarMotoCercana() {
    return acceso->localizaMotoCercana(posicion);
}

/**
 * @brief Cambia el estado de Moto para desbloquearla y crea un itinerario
 * @param m - Moto que queremos desbloquear
 */
void Cliente::desbloquearMoto(Moto &m) {
    Fecha fecha;
    unsigned int dia = 1 + rand() % 29;
    unsigned int mes = 1 + rand() % 11;
    unsigned int anio = 2019;

    if (mes == 2 && dia > 28 && anio % 4 == 0) dia = 28;
    fecha.asignarDia(dia, mes, anio);

    int ultID = acceso->getIdUltimo();
    UTM moto = m.getPosicion();
    Itinerario *it = new Itinerario();
    it->SetFecha(fecha);
    it->SetInicio(moto);
    it->SetId(ultID);
    it->SetVehiculo(&m);
    itirenario.push_back(*it);
    display = "== ECOCITYMOTO: Notificacion ==\nEstado de la moto:\nID: " + m.getId() + "\nEstatus: " + to_string(m.getEstatus()) + "\nBateria: " + to_string(m.getPorcentajeBateria()) + "%\nCoordenadas:" + " ( " + to_string(m.getPosicion().latitud) + "," + to_string(m.getPosicion().longitud) + " )";
    mostrarMensaje(display);
    acceso->desbloqueaMoto(m, this);

}

/**
 * @brief Finaliza el trayecto de la moto, terminar de crear el itinerario y bloquea la moto
 */
void Cliente::terminarTrayecto() {
    UTM fin;
    char resp = 'q';
    Moto* moto = itirenario.back().GetMoto();
    int dato0 = rand() % 101;
    int dato1 = rand() % 101;
    int bat = moto->getPorcentajeBateria();
    double dato2 = 37.0 + (dato0) / 100.0;
    double dato3 = 3.0 + (dato1) / 100.0;
    int minutos = 1 + rand() % bat;
    float bateriaf = moto->getPorcentajeBateria() - minutos;
    fin = UTM(dato2, dato3);
    itirenario.back().SetFin(fin);
    itirenario.back().SetMinutos(minutos);
    moto->setPosicion(fin);
    this->setPosicion(fin);
    moto->setPorcentajeBateria(bateriaf);
    if (moto->getPorcentajeBateria() < 15) {
        moto->darAviso();
        PuntoRecarga* pr = puntoRecargaCercano();
        while (resp != 's' && resp != 'n') {
            cout << "\n[!] Su moto está sin bateria. Debe de llevarla al punto de recarga mas cercano o si no perderá un punto.\nPunto más cercano: " << pr->getId() << " Posicion: " + to_string(pr->getX()) + ";" + to_string(pr->getY()) + "\n" << "¿Vas a llevar la moto? (s/n):";
            cin >> resp;
        }
        if (resp == 's') {
            display = "\n== ACTUALIZANDO GPS HACIA PUNTO DE RECARGA: ID " + pr->getId() + " Posicion: " + to_string(pr->getX()) + ";" + to_string(pr->getY()) + " ==\n";
            mostrarMensaje(display);
            double x = pr->getX(), y = pr->getY();
            UTM pos(x, y);
            display = "== MOTO CONECTADA AL PUNTO DE RECARGA ==\n\n== BATERIA ANTES DE RECARGAR MOTO: " + to_string(moto->getPorcentajeBateria()) + " ==\n";
            mostrarMensaje(display);
            recargarMoto(pr);
            display = "== BATERIA DESPUES DE RECARGAR MOTO: " + to_string(moto->getPorcentajeBateria()) + " ==\n";
            mostrarMensaje(display);
            setPosicion(pos);
            moto->terminarRecarga();
            display = "== MOTO DESCONECTADA DEL PUNTO DE RECARGA == \n";
            mostrarMensaje(display);
            incrementarPunto();
        } else {
            display = "\n== [!] DECREMENTANDO PUNTO ==\n";
            mostrarMensaje(display);
            decrementarPunto();
        }
    }
    cout << endl;
    moto->seDesactiva();
    display = "== ECOCITYMOTO: Notificacion ==\nEstado de la moto:\nID: " + moto->getId() + "\nEstatus: " + to_string(moto->getEstatus()) + "\nBateria: " + to_string(moto->getPorcentajeBateria()) + "%\nCoordenadas:" + " ( " + to_string(moto->getPosicion().latitud) + "," + to_string(moto->getPosicion().longitud) + " )";
    mostrarMensaje(display);
}

void Cliente::setAcceso(EcoCityMoto* acceso) {
    this->acceso = acceso;
}

void Cliente::anadeIti(Itinerario iti) {
    itirenario.push_back(iti);
}

string Cliente::getDni() const {
    return dni;
}

void Cliente::setPosicion(UTM posicion) {
    this->posicion = posicion;
}

void Cliente::setPuntos(int puntos) {
    this->puntos = puntos;
}

unsigned int Cliente::getPuntos() const {
    return puntos;
}

void Cliente::mostrarMensaje(string texto) {
    cout << texto << endl;
}

/**
 * @brief Aumenta la cantidad de puntos del cliente
 */
void Cliente::incrementarPunto() {
    if (puntos < 10) {
        puntos++;
        cout << "== RECARGA DE MOTO SATISFACTORIA ==\n";
    } else cout << "== MAXIMO DE PUNTOS ALCANZADO ==\n";

    display = "== PUNTOS DEL CLIENTE: " + to_string(puntos) + " ==";
    mostrarMensaje(display);
}

/**
 * @brief Decrementa la cantidad de puntos del cliente
 */
void Cliente::decrementarPunto() {
    this->puntos--;
    cout << "== RECARGA DE MOTO NO SATISFACTORIA ==" << endl;
    if (puntos == 0) {
        acceso = 0;
        cout << "== PUNTUACION MINIMA ALCANZADA == " << endl;
        cout << "== CLIENTE DADO DE BAJA EN EL SISTEMA ==" << endl;
    }
}

/**
 * @brief Busca el punto de recarga mas cercano al cliente
 * @return PuntoRecarga - Punto de recarga mas cercano al cliente
 */
PuntoRecarga* Cliente::puntoRecargaCercano() {
    PuntoRecarga* pr = acceso->puntoRecargaCercano(this);
    return pr;
}

/**
 * @brief Recarga la moto sin bateria dado un punto de recarga
 * @param pr - Punto de recarga que se pasa como parametro
 */
void Cliente::recargarMoto(PuntoRecarga* pr) {
    Moto *moto = acceso->localizaMoto(this);
    moto->recargar(pr);
}


