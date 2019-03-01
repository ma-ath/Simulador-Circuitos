#include "tipos.hpp"

#ifndef _CONST_
#define _CONST_

const _long PI = 3.14159265359;
const _long GCAPACITORABERTO = 1e-9;    //Resistencia equivalente para calculo de ponto de operação
const _long GINDUTORCURTO = 1e9;        //Resistencia equivalente para calculo de ponto de operação
const string EXTENSAO = ".tab";
const _long INFINITO = 1e15;

const _long NEWTON_RAPHSON_MAX_NUMERO_INTERACOES = 10;
const _long NEWTON_RAPHSON_MAX_ERRO_PERMITIDO = 1e-2;
const _long NEWTON_RAPHSON_GMIN_CONDUTANCIA_MAX = 1.1;
const _long NEWTON_RAPHSON_GMIN_CONDUTANCIA_MIN = 1e-12;
const _long NEWTON_RAPHSON_GMIN_PASSO = 1e-2;
const _long NEWTON_RAPHSON_GMIN_MAX_NUMERO_INTERACOES = 50;

const int PROGRESSO_PORCENTAGEM = 2;
const char PROGRESSO_SIMBOLO = '*';

#define EIGEN_USE_BLAS
#define EIGEN_USE_LAPACKE

#endif
