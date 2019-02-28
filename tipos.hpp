#include <string>

using namespace std;

#ifndef _TIPOS_
#define _TIPOS_

typedef double _long;

typedef enum TipoDeFonte  /*tipo de indica qual a fonte*/
{
  DC,
  SIN,
  PULSE
} TipoDeFonte;

typedef enum TipoMetodoIntegracao /*tipos de metodo de integracao - Nesse programa só tem 1 (trapézio)*/
{
  TRAP
} TipoMetodoIntegracao;

typedef enum TipoAnalise /*tipos de metodo de analise - Nesse programa só tem 1 (transiente)*/
{
  TRAN
} TipoAnalise;

typedef struct elemento /* Elemento do netlist */
{
  string nome;
  char tipo;
  _long valor;
  int a,b,c,d,x,y;
  /*a partir daqui so vai usar se for uma fonte que n eh dc: I ou V que tem <parametro>*/
  TipoDeFonte tipoDeFonte; //se eh DC, SIN, ou PULSE
  _long amplitude, frequencia, atraso, amortecimento,
          defasagem, ciclos, tempoSubida, tempoDescida, tempoLigada, periodo, amplitude1, amplitude2, offset;

  /*Aqui sao os parametros de memoria para o capacitor e indutor no tempo*/
  _long vt0; /*Tensao anterior sobre o capacitor/indutor*/
  _long jt0; /*Corrente anterior sobre o capacitor/indutor*/
  _long gon ; //condutancia da chave se vcd>vref
  _long goff;//condutancia da chave se vcd<=vref

  _long pv1, pj1, pv2, pj2, pv3, pj3, pv4, pj4; //pv1 = ponto tensao 1, pj1= ponto corrente 1

  _long tempoFinal, passo, passoPorPt;
  string metodoIntegracao;
  string tipoDeAnalise;
} elemento;

class Configuracao
{
  public:
  Configuracao(){};
  _long getTempoFinal(){return tempoFinal;};
  _long getPasso(){return passo;};
  _long getPassoPorPt(){return passoPorPt;};
  TipoMetodoIntegracao getMetodoDeIntegracao(){return metodoDeIntegracao;};
  TipoAnalise getTipoAnalise(){return analise;};

  Configuracao& setTempoFinal(_long t){tempoFinal=t;return *this;};
  Configuracao& setPasso(_long t){passo=t;return *this;};
  Configuracao& setPassoPorPt(_long t){passoPorPt=t;return *this;};
  Configuracao& setMetodoDeIntegracao(string m)
  {
    if (m == "TRAP")
      metodoDeIntegracao = TRAP;
    return *this;
  };
  Configuracao& setTipoAnalise(string a)
  {
    if (a == ".TRAN")
      analise = TRAN;
    return *this;
  };

  private:
  _long tempoFinal;
  _long passo;
  _long passoPorPt;
  TipoMetodoIntegracao metodoDeIntegracao;
  TipoAnalise analise;
};


#endif
