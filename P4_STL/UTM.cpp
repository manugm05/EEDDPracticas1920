/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UTM.cpp
 * Author: admin
 * 
 * Created on 26 de septiembre de 2019, 17:09
 */

#include "UTM.h"

UTM::UTM(double _lat, double _long) : latitud(_lat), longitud(_long) {
}

UTM::UTM() {

}

bool UTM::operator<(const UTM& right) const {
    if(latitud < right.latitud && longitud < right.longitud) return true;
    return false;
}

bool UTM::operator>(const UTM& right) const {
    return !(*this < right);
}

bool UTM::operator<=(const UTM& right) const {
    return !(*this > right);
}

bool UTM::operator>=(const UTM& right) const {
    return !(right > * this);
}

UTM::~UTM() {
}
