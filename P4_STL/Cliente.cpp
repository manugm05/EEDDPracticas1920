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
#include "EcoCityMoto.h"
#include "Moto.h"

Cliente::Cliente(string _dni, string _pass, string _nombre, string _direccion, double _latitud, double _longitud) :
dni(_dni), pass(_pass), nombre(_nombre), direccion(_direccion), posicion(_latitud, _longitud) {
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
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Cliente& obj) {
    os << obj.dni << ";" << obj.pass << ";" << obj.nombre << ";" << obj.direccion << ";" << obj.posicion.latitud << ";" << obj.posicion.longitud << endl;
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
    acceso->desbloqueaMoto(m, this);

}

/**
 * @brief Finaliza el trayecto de la moto, terminar de crear el itinerario y bloquea la moto
 */
void Cliente::terminarTrayecto() {
    UTM fin;
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
    moto->setPorcentajeBateria(bateriaf);
    moto->seDesactiva();
}

void Cliente::setAcceso(EcoCityMoto* acceso) {
    this->acceso = acceso;
}

void Cliente::anadeIti(Itinerario iti) {
    itirenario.push_back(iti);
}
