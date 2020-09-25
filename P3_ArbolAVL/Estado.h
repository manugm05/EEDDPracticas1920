/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Estado.h
 * Author: Manuel
 *
 * Created on 30 de octubre de 2019, 10:03
 */

#ifndef ESTADO_H
#define ESTADO_H

#ifdef __cplusplus
extern "C" {
#endif


enum Estado{
    bloqueada = 0,
    activa = 1,
    sinbateria = 2,
    rota = 3
};


#ifdef __cplusplus
}
#endif

#endif /* ESTADO_H */

