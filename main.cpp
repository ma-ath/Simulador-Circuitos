#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <thread>
#include <ctime>
#include "Eigen/Eigen"
#include "tipos.hpp"
#include "funcoes.hpp"
#include "const.hpp"

using namespace std;
using namespace Eigen;

int main(int argc,char** argv)
{
  setNbThreads(6);    //Faz operações utilizarem multithread - Bom para resolver matrizes. Se funciona msm nem sei mas ta ai né pq não tentar quem não arrisca não petisca já dizia minha vózinha
  clrscr();
  cout <<"\n\t    /_\\ |  \\ /  -----  S I E"<<endl;
  cout <<"\t___/   \\|__ |     |     P C "<<endl;
  cout << endl << "Programa de Analise Nodal Modificada - AlyTSPICE 2.0" << endl;
  cout << "Analise no dominio do tempo com metodo de integracao dos trapezios" << endl;
  cout << "\t\tAlyssa de Oliveira Takazume - alyssa@poli.ufrj.br\n\t\tMatheus Silva de Lima - mathlima@poli.ufrj.br" << endl;
  cout << "\t\t __/\\  /\\  /\\  /\\__\n";
  cout << "\t\t     \\/  \\/  \\/    \n\n";

  int numeroDeVariaveis;      //Numero total de variaveis a serem calculadas
  Configuracao config;        //configuracao da simulacao (ex: TRAN 10 10 TRAP 10 etc...)
  vector<string> resultado;   //Vetor de strings, aonde é salvo os resultados da simulacao
  vector <string> listaDeNos; //Lista com todos os nós do netlist
  _long t = 0;                //tempo atual de simulacao

  cout << "Lendo Netlist: " << argv[1] << endl;
  vector<elemento> netlist = lerNetlist(argv[1],numeroDeVariaveis,listaDeNos);     //Lê netlist
  lerConfiguracao(netlist,config);                                      //inicializa configuracao de simulacao

  cout << "Numero de Componentes: " << netlist.size()-1 << "\nNúmero de variaveis: " << numeroDeVariaveis << endl;

  MatrixXd A(numeroDeVariaveis,numeroDeVariaveis);                      //matriz a ser resolvida
  MatrixXd A_invariante(numeroDeVariaveis,numeroDeVariaveis);           //matriz A contendo apenas elementos nao variantes no tempo
  MatrixXd nr_A(numeroDeVariaveis,numeroDeVariaveis);                   //matrix auxiliar para método de Newton Raphson

  VectorXd b(numeroDeVariaveis);                                        //vetor de Ax=b
  VectorXd b_invariante(numeroDeVariaveis);                             //parte do vetor b invariante no tempo
  VectorXd nr_b(numeroDeVariaveis);                                     //vetor auxiliar para método de Newton Raphson

  VectorXd x(numeroDeVariaveis);                                        //vetor a ser resolvido
  VectorXd nr_x_anterior(numeroDeVariaveis);                            //vetor auxiliar para método de Newton Raphson, guarda solucao anterior

  vector <int> nr_convergencia(numeroDeVariaveis,0);                    //vetor que indica quais variaveis estão convergindo ou não. Funciona como um bool (mantive int por facilidade)

  A_invariante = A_invariante*0;      //zera a matriz do sistema
  b_invariante = b_invariante*0;      //zera b
  x = x*0;                            //zera vetor a ser resolvido
  nr_x_anterior= nr_x_anterior*0;     //zera vetor de Newton Raphson

  montarEstampasInvariantes(netlist,A_invariante,b_invariante);         //Estampas invariantes só são montadas uma única vez

  bool circuitoLinear = checarLinearidade(netlist);   //Checa se circuito é linear ou não. Caso não, não é necessário fazer análise de Newton Raphson

  clock_t tbegin = clock();    //usado para medir tempo de simulacao
  /*Ponto de Operacao*/
  A = A_invariante;
  b = b_invariante;

  montarEstampasVariantesLinearesPontoOperacao(netlist,A,b,config);   //Monta estampas de ponto de operação em A e b

  if(circuitoLinear)
  {
    x = A.partialPivLu().solve(b);       //Resolve usando partialPivLu. Requer que a matriz seja invertível. Para melhor acurácia, usar método baseado em Decomposição QR
  }
  else
  {                                     //Newton Raphson Comum
    nr_x_anterior = nr_x_anterior*0;                                                //Aproximação Inicial de Newton Raphson, para ponto de operaçãonce
                                                                                    //Para análise no tempo, é necessário recuperar ultima solução do sistema
    unsigned i = 0;
    for(;i < NEWTON_RAPHSON_MAX_NUMERO_INTERACOES;i++)
    {
      nr_A = A;
      nr_b = b;

      montarEstampasNaoLineares(netlist,nr_A,nr_b,x);

      x = nr_A.partialPivLu().solve(nr_b);

      if (newtonRaphsonChecarConvergencia(x,nr_x_anterior,nr_convergencia))
      {
        newtonRaphsonAtualizarSolucao(x,nr_x_anterior,nr_convergencia);
        break;
      };

      newtonRaphsonAtualizarSolucao(x,nr_x_anterior,nr_convergencia);
    }
    if (i >= NEWTON_RAPHSON_MAX_NUMERO_INTERACOES)
    {
      cout << "Não foi possível encontrar uma solução apenas usando Newton Raphson. Iniciando gminstepping:" << endl;
      cout << "Condutancia máxima: " << NEWTON_RAPHSON_GMIN_CONDUTANCIA_MAX << endl;
      cout << "Condutancia mínima: " << NEWTON_RAPHSON_GMIN_CONDUTANCIA_MIN << endl;
      cout << "Fator de Divisão: " << NEWTON_RAPHSON_GMIN_PASSO << endl;

      _long ygmin = NEWTON_RAPHSON_GMIN_CONDUTANCIA_MAX;
      _long fatorDivisao = NEWTON_RAPHSON_GMIN_PASSO;

      unsigned iteracoesGmin = 0;   //Numero deiteracoes do gminstepping

      while (ygmin >= NEWTON_RAPHSON_GMIN_CONDUTANCIA_MIN)
      {
        for (i = 0; i < NEWTON_RAPHSON_MAX_NUMERO_INTERACOES;i++)
        {
          nr_A = A;                                           //Zera
          nr_b = b;
          montarEstampasNaoLineares(netlist,nr_A,nr_b,x);     //Monta
          x = nr_A.partialPivLu().solve(nr_b);                //Resolve

          if (newtonRaphsonChecarConvergencia(x,nr_x_anterior,nr_convergencia))
          {
            newtonRaphsonAtualizarSolucao(x,nr_x_anterior,nr_convergencia);
            break; //se convergir, sai do loop do NR, para dividir o fator e repetir
          };

          newtonRaphsonAtualizarSolucao(x,nr_x_anterior,nr_convergencia);
        }
        if (i > NEWTON_RAPHSON_GMIN_MAX_NUMERO_INTERACOES )   //Checa convergencia do Newton Raphson
        {
          if (iteracoesGmin >= NEWTON_RAPHSON_GMIN_MAX_NUMERO_INTERACOES)
          {
            cout<<"Gmin Stepping não convergiu."<<endl;
            cout<<"Não foi possível encontrar uma solução"<<endl;
            exit(1);
          }
          else
          {
            iteracoesGmin++;
            fatorDivisao = sqrtl(fatorDivisao);
            ygmin=ygmin*fatorDivisao;
            newtonRaphsonInicializaVetorFaltaConvergir(nr_convergencia);
            newtonRaphsonAtualizarSolucao(x,nr_x_anterior,nr_convergencia);
            nr_x_anterior = nr_x_anterior*0;
          }
        }
        else
        {
          fatorDivisao = NEWTON_RAPHSON_GMIN_PASSO;
          ygmin=ygmin*fatorDivisao;
          newtonRaphsonAtualizarSolucao(x,nr_x_anterior,nr_convergencia);
          newtonRaphsonInicializaVetorFaltaConvergir(nr_convergencia);
          nr_x_anterior = nr_x_anterior*0;
        }
      }
    }
  }

  atualizarMemorias(netlist,config,x);  //Salva estado atual do circuito (aonde interessa, por exemplo, tensoes em capacitores)
  salvarResultadoEmMemoria(x,resultado,t);

  /*Analise no Tempo*/
  while(t < config.getTempoFinal()){    //Loop de tempo
    A = A_invariante;
    b = b_invariante;

    printProgresso(PROGRESSO_PORCENTAGEM,PROGRESSO_SIMBOLO,config,t);

    montarEstampasVariantesLineares(netlist,A,b,config,t);    //Monta apenas o que varia com o tempo

    if(circuitoLinear)
    {
      x = A.partialPivLu().solve(b);
    }
    else
    {
      nr_x_anterior = x;                                                //Aproximação Inicial de Newton Raphson, para ponto de operaçãonce
                                                                        //Para análise no tempo, é necessário recuperar ultima solução do sistema
      newtonRaphsonInicializaVetorFaltaConvergir(nr_convergencia);
      unsigned i = 0;
      for(;i < NEWTON_RAPHSON_MAX_NUMERO_INTERACOES;i++)
      {
        nr_A = A;
        nr_b = b;

        montarEstampasNaoLineares(netlist,nr_A,nr_b,x);

        x = nr_A.partialPivLu().solve(nr_b);

        if (newtonRaphsonChecarConvergencia(x,nr_x_anterior,nr_convergencia))
        {
          break;
        };
        newtonRaphsonAtualizarSolucao(x,nr_x_anterior,nr_convergencia);
      }
      if (i >= NEWTON_RAPHSON_MAX_NUMERO_INTERACOES)
      {
        _long ygmin = NEWTON_RAPHSON_GMIN_CONDUTANCIA_MAX;
        _long fatorDivisao = NEWTON_RAPHSON_GMIN_PASSO;

        unsigned iteracoesGmin = 0;   //Numero deiteracoes do gminstepping

        while (ygmin >= NEWTON_RAPHSON_GMIN_CONDUTANCIA_MIN)
        {
          for (i = 0; i < NEWTON_RAPHSON_MAX_NUMERO_INTERACOES;i++)
          {
            nr_A = A;                                           //Zera
            nr_b = b;
            montarEstampasNaoLineares(netlist,nr_A,nr_b,x);     //Monta
            x = nr_A.partialPivLu().solve(nr_b);                //Resolve

            if (newtonRaphsonChecarConvergencia(x,nr_x_anterior,nr_convergencia))
            {
              newtonRaphsonAtualizarSolucao(x,nr_x_anterior,nr_convergencia);
              break; //se convergir, sai do loop do NR, para dividir o fator e repetir
            };

            newtonRaphsonAtualizarSolucao(x,nr_x_anterior,nr_convergencia);
          }
          if (i > NEWTON_RAPHSON_GMIN_MAX_NUMERO_INTERACOES )   //Checa convergencia do Newton Raphson
          {
            if (iteracoesGmin >= NEWTON_RAPHSON_GMIN_MAX_NUMERO_INTERACOES)
            {
              cout<<"Gmin Stepping não convergiu."<<endl;
              cout<<"Não foi possível encontrar uma solução"<<endl;
              exit(1);
            }
            else
            {
              iteracoesGmin++;
              fatorDivisao = sqrtl(fatorDivisao);
              ygmin=ygmin*fatorDivisao;
              newtonRaphsonInicializaVetorFaltaConvergir(nr_convergencia);
              newtonRaphsonAtualizarSolucao(x,nr_x_anterior,nr_convergencia);
              nr_x_anterior = x;
            }
          }
          else
          {
            fatorDivisao = NEWTON_RAPHSON_GMIN_PASSO;
            ygmin=ygmin*fatorDivisao;
            newtonRaphsonAtualizarSolucao(x,nr_x_anterior,nr_convergencia);
            newtonRaphsonInicializaVetorFaltaConvergir(nr_convergencia);
            nr_x_anterior = x;
          }
        }
      }
    }

    atualizarMemorias(netlist,config,x);  //Salva estado atual do circuito (aonde interessa, por exemplo, tensoes em capacitores)
    salvarResultadoEmMemoria(x,resultado,t);

    t+=config.getPasso();
  }
  clock_t tend = clock();

  cout << "Circuito Simulado!" << endl;
  double elapsed_secs = double(tend - tbegin) / CLOCKS_PER_SEC;
  double elapsed_steps = int(config.getTempoFinal()/config.getPasso());
  cout << elapsed_steps <<" passos concluídos em " << elapsed_secs << " segundos" << endl;

  salvarResultadoEmArquivo(argv[1],EXTENSAO,listaDeNos,resultado);
  return 0;
}
