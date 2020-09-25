/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UTM.h
 * Author: admin
 *
 * Created on 26 de septiembre de 2019, 17:09
 */

#ifndef UTM_H
#define UTM_H

struct UTM {
    double latitud = 0.0;
    double longitud = 0.0;
    UTM(double _lat, double _long);
    UTM();
    virtual ~UTM() {
    }

};

#endif /* UTM_H */

