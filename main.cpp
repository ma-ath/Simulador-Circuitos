#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <thread>

#include "Eigen/Eigen"
#include "tipos.hpp"
#include "funcoes.hpp"
#include "const.hpp"

using namespace std;
using namespace Eigen;

//thread t1(simulacao);
//t1.join();
// void simulacaoTransiente()
// {
//
// }

int main(int argc,char** argv)
{
  int numeroDeVariaveis;      //Numero total de variaveis a serem calculadas
  Configuracao config;        //configuracao da simulacao (ex: TRAN 10 10 TRAP 10 etc...)
  vector<string> resultado;   //Vetor de strings, aonde é salvo os resultados da simulacao
  vector <string> listaDeNos; //Lista com todos os nós do netlist
  _long t = 0;              //tempo atual de simulacao

  vector<elemento> netlist = lerNetlist(argv[1],numeroDeVariaveis,listaDeNos);     //Lê netlist
  lerConfiguracao(netlist,config);                                      //inicializa configuracao de simulacao

  MatrixXd A(numeroDeVariaveis,numeroDeVariaveis);                      //matriz a ser resolvida
  MatrixXd A_invariante(numeroDeVariaveis,numeroDeVariaveis);           //matriz A contendo apenas elementos nao variantes no tempo
  VectorXd b(numeroDeVariaveis);                                        //vetor de Ax=b
  VectorXd b_invariante(numeroDeVariaveis);                             //parte do vetor b invariante no tempo
  VectorXd x(numeroDeVariaveis);                                        //vetor a ser resolvido

  A_invariante = A_invariante*0;      //zera a matriz do sistema
  b_invariante = b_invariante*0;

  x = x*0;                            //zera vetor a ser resolvido

  montarEstampasInvariantes(netlist,A_invariante,b_invariante);         //Estampas invariantes só são montadas uma única vez

  /*Ponto de Operacao*/
  A = A_invariante;
  b = b_invariante;

  montarEstampasVariantesLinearesPontoOperacao(netlist,A,b,config); //Monta estampas de ponto de operação

  x = A.householderQr().solve(b);     //Resolve usando householder qr

  atualizarMemorias(netlist,config,x);  //Salva estado atual do circuito (aonde interessa, por exemplo, tensoes em capacitores)

  salvarResultadoEmMemoria(x,resultado,t);

  /*Analise no Tempo*/
  while(t < config.getTempoFinal()){    //Loop de tempo
    A = A_invariante;
    b = b_invariante;

    montarEstampasVariantesLineares(netlist,A,b,config,t);    //Monta apenas o que varia com o tempo

    x = A.householderQr().solve(b);     //Resolve Ax=b usando householder qr

    atualizarMemorias(netlist,config,x);  //Salva estado atual do circuito (aonde interessa, por exemplo, tensoes em capacitores)

    salvarResultadoEmMemoria(x,resultado,t);

    t+=config.getPasso();
  }

  salvarResultadoEmArquivo(argv[1],EXTENSAO,listaDeNos,resultado);
  return 0;
}
