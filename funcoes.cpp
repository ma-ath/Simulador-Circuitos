#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include "Eigen/Eigen"
#include "tipos.hpp"
#include "funcoes.hpp"
#include "const.hpp"

using namespace std;
using namespace Eigen;

_long heaviside(_long t)
{
  if (t >= 0)
    return 1;
  else
    return 0;
}

size_t split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}/*https://stackoverflow.com/questions/13035674/how-to-read-line-by-line-or-a-whole-text-file-at-once*/

int numerarNoDoNetlist(string nomeDoNo,vector <string> (&listaDeNos))    //numera os nos do netlist (nos do netlist podem ser nomes, e repetidos)
{
  unsigned int i=0;
  for(;i<listaDeNos.size();i++)               //atualiza a Lista De Nos
    if (listaDeNos[i] == nomeDoNo)         //encontrei, retorna valor
      return i;
  listaDeNos.push_back(nomeDoNo);             //nao encontrei, entao é novo. retorna novo valor
  return i;
}

void testarListaDeParametros(char tipo,vector<string> parametros) //ainda nao faz nada
{
  switch (tipo) {
    case 'R':
    case 'C':
    case 'L':

    if (parametros.size() < 4)
    {
      cout << "Não há parametros suficientes em " << parametros.at(0) << endl;
      cout << "Abortando simulacao" << endl;
      exit(1);
    }
    break;

    case 'V':
    case 'I':
      if (parametros.at(3) == "DC"){
        if (parametros.size() < 5)
        {
          cout << "Não há parametros suficientes em " << parametros.at(0) << endl;
          cout << "Abortando simulacao" << endl;
          exit(1);
        }
      break;
      }

      if (parametros.at(3) == "SIN"){
        if (parametros.size() < 11)
        {
          cout << "Não há parametros suficientes em " << parametros.at(0) << endl;
          cout << "Abortando simulacao" << endl;
          exit(1);
        }
      break;
      }

      if (parametros.at(3) == "PULSE"){
        if (parametros.size() < 12)
        {
          cout << "Não há parametros suficientes em " << parametros.at(0) << endl;
          cout << "Abortando simulacao" << endl;
          exit(1);
        }
      break;
      }

      cout << "Componente desconhecido: " << parametros.at(0) << ":" << parametros.at(3) << endl;
      cout << "Abortando simulacao" << endl;
      exit(1);
    break;

    case 'G':
    case 'E':
    case 'F':
    case 'H':
    case 'K':
    if (parametros.size() < 6)
    {
      cout << "Não há parametros suficientes em " << parametros.at(0) << endl;
      cout << "Abortando simulacao" << endl;
      exit(1);
    }
    break;

    case 'O':
    if (parametros.size() < 5)
    {
      cout << "Não há parametros suficientes em " << parametros.at(0) << endl;
      cout << "Abortando simulacao" << endl;
      exit(1);
    }
    break;

    case '$':
    if (parametros.size() < 8)
    {
      cout << "Não há parametros suficientes em " << parametros.at(0) << endl;
      cout << "Abortando simulacao" << endl;
      exit(1);
    }
    break;

    case 'N':
    if (parametros.size() < 11)
    {
      cout << "Não há parametros suficientes em " << parametros.at(0) << endl;
      cout << "Abortando simulacao" << endl;
      exit(1);
    }
    break;

    case '.':
    if (parametros.size() < 5)
    {
      cout << "Não há parametros suficientes em " << parametros.at(0) << endl;
      cout << "Abortando simulacao" << endl;
      exit(1);
    }
    break;

    default:
    cout << "Componente desconhecido: " << parametros.at(0) << endl;
    cout << "Abortando simulacao" << endl;
    exit(1);
    break;
  }
}

vector<elemento> lerNetlist(char* nomeArquivo, int (&numeroDeVariaveis),vector <string> (&listaDeNos))
{
  ifstream arquivo(nomeArquivo);
  if (arquivo.fail())
  {
    cout << "Não foi possível abrir o arquivo." << endl;
    exit(1);
  }

  string linha;
  vector<elemento> netlist;   /*netlist a ser preenchido*/
  elemento componente;  // qual o componente
  listaDeNos.push_back("0"); //lista q associa nome dos nos com no do netlist. o nó de terra é adicionado automaticamente

  getline(arquivo,linha); //apenas pula primeira linha, reservada para o comentario
  while (getline(arquivo,linha))
  {
      char tipo = toupper(linha[0]);  //primeiro se pega qual o tipo do componente
      vector <string> parametros;
      split(linha.c_str(),parametros,' ');  //depois se pegam todos os parametros

      switch (tipo) { //se preenche o netlist de acordo com os parametros pegos
        case 'R':
        case 'L':
        case 'C':

        testarListaDeParametros(tipo,parametros);   //testa se o usuario colocou os parametros corretamente

        componente.nome = parametros.at(0);                             //nome do componente
        componente.a = numerarNoDoNetlist(parametros.at(1),listaDeNos); //nó1
        componente.b = numerarNoDoNetlist(parametros.at(2),listaDeNos); //nó2
        componente.valor = atof(parametros.at(3).c_str());              //valor do componente

        componente.tipo = tipo;
        netlist.push_back(componente);                                  //adiciona o componente à netlist
        break;

        case 'V':
        case 'I':

        testarListaDeParametros(tipo,parametros);

        componente.nome = parametros.at(0);                             //nome do componente
        componente.a = numerarNoDoNetlist(parametros.at(1),listaDeNos); //nó1
        componente.b = numerarNoDoNetlist(parametros.at(2),listaDeNos); //nó2

          if (parametros.at(3) == "DC"){                           //Dependendo do tipo de fonte, os parametros pegos são diferentes
              componente.tipoDeFonte = DC;
              componente.valor = atof(parametros.at(4).c_str());        //valor da fonte DC
            }

          if (parametros.at(3) == "SIN"){
              componente.tipoDeFonte = SIN;
              componente.offset = atof(parametros.at(4).c_str());        //valor do offset
              componente.amplitude = atof(parametros.at(5).c_str());     //valor da amplitude
              componente.frequencia = atof(parametros.at(6).c_str());    //valor da frequencia
              componente.atraso = atof(parametros.at(7).c_str());        //valor do atraso
              componente.amortecimento = atof(parametros.at(8).c_str()); //valor do amortecimento
              componente.defasagem = atof(parametros.at(9).c_str());     //valor da defasagem (graus)
              componente.ciclos = atof(parametros.at(10).c_str());       //numero de ciclos
            }

          if (parametros.at(3) == "PULSE"){
              componente.tipoDeFonte = PULSE;
              componente.amplitude1 = atof(parametros.at(4).c_str());    //valor da amplitude1
              componente.amplitude2 = atof(parametros.at(5).c_str());    //valor da amplitude2
              componente.atraso = atof(parametros.at(6).c_str());        //valor do atraso
              componente.tempoSubida = atof(parametros.at(7).c_str());   //valor do tempo de subida
              componente.tempoDescida = atof(parametros.at(8).c_str());  //valor do tempo de descida
              componente.tempoLigada = atof(parametros.at(9).c_str());   //valor do tempo ligado
              componente.periodo = atof(parametros.at(10).c_str());      //valor do periodo
              componente.ciclos = atof(parametros.at(11).c_str());       //numero de ciclos
            }

        componente.tipo = tipo;
        netlist.push_back(componente);                                  //adiciona o componente à netlist

        break;

        case 'G':
        case 'E':
        case 'F':
        case 'H':
        case 'K':

        testarListaDeParametros(tipo,parametros);

        componente.nome = parametros.at(0);                             //nome do componente
        componente.a = numerarNoDoNetlist(parametros.at(1),listaDeNos); //nó1
        componente.b = numerarNoDoNetlist(parametros.at(2),listaDeNos); //nó2
        componente.c = numerarNoDoNetlist(parametros.at(3),listaDeNos); //nó3
        componente.d = numerarNoDoNetlist(parametros.at(4),listaDeNos); //nó4
        componente.valor = atof(parametros.at(5).c_str());              //valor

        componente.tipo = tipo;
        netlist.push_back(componente);                                  //adiciona o componente à netlist
        break;

        case 'O':

        testarListaDeParametros(tipo,parametros);

        componente.nome = parametros.at(0);                             //nome do componente
        componente.a = numerarNoDoNetlist(parametros.at(1),listaDeNos); //nó1
        componente.b = numerarNoDoNetlist(parametros.at(2),listaDeNos); //nó2
        componente.c = numerarNoDoNetlist(parametros.at(3),listaDeNos); //nó3
        componente.d = numerarNoDoNetlist(parametros.at(4),listaDeNos); //nó4

        componente.tipo = tipo;
        netlist.push_back(componente);                                  //adiciona o componente à netlist
        break;

        case '$':

        testarListaDeParametros(tipo,parametros);

        componente.nome = parametros.at(0);                             //nome do componente
        componente.a = numerarNoDoNetlist(parametros.at(1),listaDeNos); //nó1
        componente.b = numerarNoDoNetlist(parametros.at(2),listaDeNos); //nó2
        componente.c = numerarNoDoNetlist(parametros.at(3),listaDeNos); //nó3
        componente.d = numerarNoDoNetlist(parametros.at(4),listaDeNos); //nó4
        componente.gon = atof(parametros.at(5).c_str());                //gon
        componente.goff = atof(parametros.at(6).c_str());               //goff
        componente.valor = atof(parametros.at(7).c_str());              //vref

        componente.tipo = tipo;
        netlist.push_back(componente);                                  //adiciona o componente à netlist
        break;

        case 'N':

        testarListaDeParametros(tipo,parametros);

        componente.nome = parametros.at(0);                             //nome do componente
        componente.a = numerarNoDoNetlist(parametros.at(1),listaDeNos); //nó1
        componente.b = numerarNoDoNetlist(parametros.at(2),listaDeNos); //nó2
        componente.pv1 = atof(parametros.at(3).c_str());                //pv1
        componente.pj1 = atof(parametros.at(4).c_str());                //pj1
        componente.pv2 = atof(parametros.at(5).c_str());                //pv2
        componente.pj2 = atof(parametros.at(6).c_str());                //pj2
        componente.pv3 = atof(parametros.at(7).c_str());                //pv3
        componente.pj3 = atof(parametros.at(8).c_str());                //pj3
        componente.pv4 = atof(parametros.at(9).c_str());                //pv4
        componente.pj4 = atof(parametros.at(10).c_str());               //pj4

        componente.tipo = tipo;
        netlist.push_back(componente);                                  //adiciona o componente à netlist
        break;

        case '.':
        testarListaDeParametros(tipo,parametros);
        //Programa só faz simulacao do tipo TRAN TRAPEZIO
        componente.nome = parametros.at(0);                             //nome do componente
        componente.tempoFinal = atof(parametros.at(1).c_str());         //tempo final
        componente.passo = atof(parametros.at(2).c_str());              //passo
        componente.metodoIntegracao = parametros.at(3);                  //metodo de integracao
        componente.passoPorPt = atof(parametros.at(4).c_str());         //passo por pontos

        componente.tipoDeAnalise = componente.nome;                     //tipo de analise
        componente.tipo = tipo;                                         //tipo de componente
        netlist.push_back(componente);                                  //adiciona o componente à netlist
        break;

        case '*': //comentario, nao faz nada
        break;
        default:
          testarListaDeParametros(tipo,parametros);   //testa se o usuario colocou os parametros corretamente
        break;
      }
  }

  numeroDeVariaveis = listaDeNos.size();

  /*Apos os componentes estarem lidos, é necessário atualizar quais componentes necessitam de mais de uma variavel para ser resolvido (correntes auxiliares)*/
  for(unsigned i=0;i<netlist.size();i++)
  {
    char tipo = netlist[i].tipo;
    if (tipo=='V' || tipo=='E' || tipo=='F' || tipo=='O' || tipo=='K')
    {
      numeroDeVariaveis++;
      netlist[i].x=numeroDeVariaveis-1;

      stringstream jx;
      jx << "j" << netlist[i].nome;        //Adiciona as correntes à lista de nós, necessario para escrever em arquivo corretamente
      listaDeNos.push_back(jx.str());
    }
    /*o tipo H, q eh uma tensao ligada a corrente, precisam de 2 variaveis
    de corrente para construir a estampa*/
    else if (tipo=='H')
    {
      numeroDeVariaveis+=2;
      netlist.at(i).x=numeroDeVariaveis-2;
      netlist.at(i).y=numeroDeVariaveis-1;

      stringstream jx;
      jx << "jH" << (numeroDeVariaveis-2);        //Adiciona as correntes à lista de nós
      listaDeNos.push_back(jx.str());             //Não sei uma forma melhor de nomear isso
      jx << "jH" << (numeroDeVariaveis-1);
      listaDeNos.push_back(jx.str());
    }
  }

  numeroDeVariaveis-=1;         //Um nó sempre é o TERRA, então não conta como variavel
  return netlist;
}

void montarEstampasInvariantes(vector <elemento> (&netlist),MatrixXd (&A),VectorXd (&b))
{
  MatrixXd Matriz(A.rows()+1,A.cols()+1);   //Matriz aumentada, contendo nó de terra. Primeira linha e coluna sempre é descartada
  VectorXd vetor(b.size()+1);               //sempre é esperado q o circuito tenha ao menos 1 nó de terra, caso contrário não é possível resolver por esse método

  Matriz =  MatrixXd::Zero(A.rows()+1,A.cols()+1);
  vetor = ArrayXd::Zero(b.size()+1);

  for(unsigned i=0;i<netlist.size();i++)
  {
    if (netlist.at(i).tipo == 'R') {
      _long g = 1/netlist.at(i).valor;
      Matriz(netlist.at(i).a,netlist.at(i).a)+=g;
      Matriz(netlist.at(i).b,netlist.at(i).b)+=g;
      Matriz(netlist.at(i).a,netlist.at(i).b)-=g;
      Matriz(netlist.at(i).b,netlist.at(i).a)-=g;

      netlist.erase(netlist.begin() + i); //Apos se montar uma estampa invariavel, o elemento não será mais acessado e pode-se retira-lo do netlist
      i--;  //Como eu deletei uma posição, não é necessario incrementar o valor de i para ir para proximo elemento do netlist
    }
    else if (netlist.at(i).tipo =='G') {
      _long g = 1/netlist.at(i).valor;
      Matriz(netlist.at(i).a,netlist.at(i).c)+=g;
      Matriz(netlist.at(i).b,netlist.at(i).d)+=g;
      Matriz(netlist.at(i).a,netlist.at(i).d)-=g;
      Matriz(netlist.at(i).b,netlist.at(i).c)-=g;

      netlist.erase(netlist.begin() + i); //Apos se montar uma estampa invariavel, o elemento não será mais acessado e pode-se retira-lo do netlist
      i--;
    }
    else if (netlist.at(i).tipo == 'I'){
      if (netlist.at(i).tipoDeFonte == DC)
      /*
      Fonte DC - Nao muda com o tempoAtual
      */
      {
          _long g = netlist.at(i).valor;
          vetor(netlist.at(i).a)-=g;
          vetor(netlist.at(i).b)+=g;

          netlist.erase(netlist.begin() + i);
          i--;
      }
    }
    else if (netlist.at(i).tipo == 'V'){
      if (netlist.at(i).tipoDeFonte == DC )//Fonte DC - Nao muda com o tempoAtual
      {
        _long g = netlist.at(i).valor;
        Matriz(netlist.at(i).a,netlist.at(i).x)+=1;
        Matriz(netlist.at(i).b,netlist.at(i).x)-=1;
        Matriz(netlist.at(i).x,netlist.at(i).a)-=1;
        Matriz(netlist.at(i).x,netlist.at(i).b)+=1;
        vetor(netlist.at(i).x)-=g;

        netlist.erase(netlist.begin() + i);
        i--;
      }
    }
    else if (netlist.at(i).tipo == 'E') {
      _long g = netlist.at(i).valor;
      Matriz(netlist.at(i).a,netlist.at(i).x)+=1;
      Matriz(netlist.at(i).b,netlist.at(i).x)-=1;
      Matriz(netlist.at(i).x,netlist.at(i).a)-=1;
      Matriz(netlist.at(i).x,netlist.at(i).b)+=1;
      Matriz(netlist.at(i).x,netlist.at(i).c)+=g;
      Matriz(netlist.at(i).x,netlist.at(i).d)-=g;

      netlist.erase(netlist.begin() + i);
      i--;
    }
    else if (netlist.at(i).tipo == 'F') {
      _long g = netlist.at(i).valor;
      Matriz(netlist.at(i).a,netlist.at(i).x)+=g;
      Matriz(netlist.at(i).b,netlist.at(i).x)-=g;
      Matriz(netlist.at(i).c,netlist.at(i).x)+=1;
      Matriz(netlist.at(i).d,netlist.at(i).x)-=1;
      Matriz(netlist.at(i).x,netlist.at(i).c)-=1;
      Matriz(netlist.at(i).x,netlist.at(i).d)+=1;

      netlist.erase(netlist.begin() + i);
      i--;
    }
    else if (netlist.at(i).tipo == 'H') {
      _long g = netlist.at(i).valor;
      Matriz(netlist.at(i).a,netlist.at(i).y)+=1;
      Matriz(netlist.at(i).b,netlist.at(i).y)-=1;
      Matriz(netlist.at(i).c,netlist.at(i).x)+=1;
      Matriz(netlist.at(i).d,netlist.at(i).x)-=1;
      Matriz(netlist.at(i).y,netlist.at(i).a)-=1;
      Matriz(netlist.at(i).y,netlist.at(i).b)+=1;
      Matriz(netlist.at(i).x,netlist.at(i).c)-=1;
      Matriz(netlist.at(i).x,netlist.at(i).d)+=1;
      Matriz(netlist.at(i).y,netlist.at(i).x)+=g;

      netlist.erase(netlist.begin() + i);
      i--;
    }
    else if (netlist.at(i).tipo == 'O') {
      Matriz(netlist.at(i).a,netlist.at(i).x)+=1;
      Matriz(netlist.at(i).b,netlist.at(i).x)-=1;
      Matriz(netlist.at(i).x,netlist.at(i).c)+=1;
      Matriz(netlist.at(i).x,netlist.at(i).d)-=1;

      netlist.erase(netlist.begin() + i);
      i--;
    }
    else if (netlist.at(i).tipo == 'K') {     /*Transformador Ideal*/
      _long g = netlist.at(i).valor;
      Matriz(netlist.at(i).a,netlist.at(i).x)-=g;
      Matriz(netlist.at(i).b,netlist.at(i).x)+=g;
      Matriz(netlist.at(i).c,netlist.at(i).x)+=1;
      Matriz(netlist.at(i).d,netlist.at(i).x)-=1;

      Matriz(netlist.at(i).x,netlist.at(i).a)+=g;
      Matriz(netlist.at(i).x,netlist.at(i).b)-=g;
      Matriz(netlist.at(i).x,netlist.at(i).c)-=1;
      Matriz(netlist.at(i).x,netlist.at(i).d)+=1;

      netlist.erase(netlist.begin() + i);
      i--;
    }
  }

  A += Matriz.bottomRightCorner(A.rows(),A.cols());    //Retorna a matriz sem o nós de terra, a ser resolvido
  b += vetor.segment(1,b.size());              //Retorna vetor b sem incluir no de terra
}

void montarEstampasVariantesLineares(vector <elemento> (&netlist),MatrixXd (&A),VectorXd (&b),Configuracao (&config),_long t)
{

  MatrixXd Matriz(A.rows()+1,A.cols()+1);
  VectorXd vetor(b.size()+1);

  Matriz =  MatrixXd::Zero(A.rows()+1,A.cols()+1);
  vetor = ArrayXd::Zero(b.size()+1);

  for(unsigned i=0;i<netlist.size();i++)
  {
    if (netlist.at(i).tipo == 'C') {
      if (config.getMetodoDeIntegracao() == TRAP)   //MetodoDosTrapezios
      {
        _long g=((2*netlist[i].valor)/(config.getPasso()));    //Valor da condutancia para o MetodoDosTrapezios
        Matriz(netlist[i].a,netlist[i].a)+=g;                   //Estampas para o resistor
        Matriz(netlist[i].b,netlist[i].b)+=g;
        Matriz(netlist[i].a,netlist[i].b)-=g;
        Matriz(netlist[i].b,netlist[i].a)-=g;

        vetor(netlist[i].a)+=(g*netlist[i].vt0+netlist[i].jt0); //Estampas para a fonte de corrente
        vetor(netlist[i].b)-=(g*netlist[i].vt0+netlist[i].jt0);
      }
    }
    else if (netlist.at(i).tipo == 'L') {
      if (config.getMetodoDeIntegracao() == TRAP)   //MetodoDosTrapezios
      {
        _long g=((config.getPasso())/(2*netlist[i].valor));     //Valor da condutancia para o MetodoDosTrapezios
        Matriz(netlist[i].a,netlist[i].a)+=g;                    //Estampas para o resistor
        Matriz(netlist[i].b,netlist[i].b)+=g;
        Matriz(netlist[i].a,netlist[i].b)-=g;
        Matriz(netlist[i].b,netlist[i].a)-=g;

        vetor(netlist[i].a)-=(g*netlist[i].vt0+netlist[i].jt0);           //Estampas para a fonte de corrente
        vetor(netlist[i].b)+=(g*netlist[i].vt0+netlist[i].jt0);
      }
    }
    else if (netlist.at(i).tipo=='I'){
      if (netlist[i].tipoDeFonte == SIN)  //caso seno
          /*
          Fonte SIN - Muda de acordo com:
          off+Vp*exp(-dec*(tempoAtual-atraso))*sin(2*pi*f*(tempoAtual-atraso)+(pi/180)*fase)
          */
          {
            _long g=
              (
                netlist[i].offset +
                (
                  netlist[i].amplitude*exp(-1*netlist[i].amortecimento*(t-netlist[i].atraso)) *
                  sin(2*PI*netlist[i].frequencia*(t-netlist[i].atraso) + (PI/180)*netlist[i].defasagem)
                )*(heaviside(t-netlist[i].atraso) -
                    heaviside((t-netlist[i].atraso)-(netlist[i].ciclos/netlist[i].frequencia)))
              );

                vetor(netlist[i].a)-=g;
                vetor(netlist[i].b)+=g;
          }
      else if (netlist[i].tipoDeFonte == PULSE) //caso pulse
      {
        if (netlist[i].tempoSubida == 0)
        {
          netlist[i].tempoSubida = config.getPasso();
        }
        if (netlist[i].tempoDescida == 0)
        {
          netlist[i].tempoDescida = config.getPasso();
        }

        _long pulseRealTime;//,pulseOffTime;

        pulseRealTime = t-netlist[i].atraso;
        pulseRealTime = fmod(pulseRealTime,netlist[i].periodo);
        //pulseOffTime=netlist[i].periodo - (netlist[i].tempoSubida+netlist[i].tempoDescida+netlist[i].tempoLigada);
        if (t<netlist[i].atraso)
        {
            vetor(netlist[i].a)-=netlist[i].amplitude1;
            vetor(netlist[i].b)+=netlist[i].amplitude1;
        }
        else if (t > (netlist[i].atraso +(netlist[i].ciclos*netlist[i].periodo) ) )
        {
          vetor(netlist[i].a)-=netlist[i].amplitude1;
          vetor(netlist[i].b)+=netlist[i].amplitude1;
        }
        else
        {
          if (pulseRealTime<netlist[i].tempoSubida )
          { /* subindo*/
            vetor(netlist[i].a)-=((((netlist[i].amplitude2-netlist[i].amplitude1)/netlist[i].tempoSubida)*pulseRealTime)+ netlist[i].amplitude1);
            vetor(netlist[i].b)+=((((netlist[i].amplitude2-netlist[i].amplitude1)/netlist[i].tempoSubida)*pulseRealTime)+ netlist[i].amplitude1);
          }
          else if (pulseRealTime < (netlist[i].tempoSubida+netlist[i].tempoLigada))
          {
            vetor(netlist[i].a)-=netlist[i].amplitude2;
            vetor(netlist[i].b)+=netlist[i].amplitude2;
          }
          else if (pulseRealTime < (netlist[i].tempoSubida+netlist[i].tempoLigada+netlist[i].tempoDescida))
          {
            vetor(netlist[i].a)-= (netlist[i].amplitude2-
              (((netlist[i].amplitude2-netlist[i].amplitude1)/netlist[i].tempoDescida)*
              (pulseRealTime-(netlist[i].tempoSubida+netlist[i].tempoLigada))));

            vetor(netlist[i].b)+= (netlist[i].amplitude2-
                (((netlist[i].amplitude2-netlist[i].amplitude1)/netlist[i].tempoDescida)*
                (pulseRealTime-(netlist[i].tempoSubida+netlist[i].tempoLigada))));
          }
          else
          {
            vetor(netlist[i].a)-=netlist[i].amplitude1;
            vetor(netlist[i].b)+=netlist[i].amplitude1;
          }
        }
      }
    }
    else if (netlist.at(i).tipo=='V'){
      if (netlist[i].tipoDeFonte == SIN)  //caso seno
          {
            _long g=
              (
                netlist[i].offset +
                (
                  netlist[i].amplitude*exp(-1*netlist[i].amortecimento*(t-netlist[i].atraso)) *
                  sin(2*PI*netlist[i].frequencia*(t-netlist[i].atraso) + (PI/180)*netlist[i].defasagem)
                )*( heaviside(t-netlist[i].atraso) -
                    heaviside((t-netlist[i].atraso)-(netlist[i].ciclos/netlist[i].frequencia)))
              );

                Matriz(netlist[i].a,netlist[i].x)+=1;
                Matriz(netlist[i].b,netlist[i].x)-=1;
                Matriz(netlist[i].x,netlist[i].a)-=1;
                Matriz(netlist[i].x,netlist[i].b)+=1;
                vetor(netlist[i].x)-=g;
          }
      else if (netlist[i].tipoDeFonte == PULSE) //caso pulse
          {
            Matriz(netlist[i].a,netlist[i].x)+=1;
            Matriz(netlist[i].b,netlist[i].x)-=1;
            Matriz(netlist[i].x,netlist[i].a)-=1;
            Matriz(netlist[i].x,netlist[i].b)+=1;

            if (netlist[i].tempoSubida == 0)
            {
              netlist[i].tempoSubida = config.getPasso();
            }
            if (netlist[i].tempoDescida == 0)
            {
              netlist[i].tempoDescida = config.getPasso();
            }

            _long pulseRealTime;//,pulseOffTime;

            pulseRealTime = t-netlist[i].atraso;
            pulseRealTime = fmod(pulseRealTime,netlist[i].periodo);
            //pulseOffTime=netlist[i].periodo - (netlist[i].tempoSubida+netlist[i].tempoDescida+netlist[i].tempoLigada);
            if (t<netlist[i].atraso)
            {
                vetor(netlist[i].x)-=netlist[i].amplitude1;
            }
            else if (t > (netlist[i].atraso +(netlist[i].ciclos*netlist[i].periodo) ) )
            {
              vetor(netlist[i].x)-=netlist[i].amplitude1;
            }
            else
            {
              if (pulseRealTime<netlist[i].tempoSubida )
              { /* subindo*/
                vetor(netlist[i].x)-=((((netlist[i].amplitude2-netlist[i].amplitude1)/netlist[i].tempoSubida)*pulseRealTime)+ netlist[i].amplitude1);
              }
              else if (pulseRealTime < (netlist[i].tempoSubida+netlist[i].tempoLigada))
              {
                vetor(netlist[i].x)-=netlist[i].amplitude2;
              }
              else if (pulseRealTime < (netlist[i].tempoSubida+netlist[i].tempoLigada+netlist[i].tempoDescida))
              {
                vetor(netlist[i].x)-= (netlist[i].amplitude2-
                  (((netlist[i].amplitude2-netlist[i].amplitude1)/netlist[i].tempoDescida)*
                  (pulseRealTime-(netlist[i].tempoSubida+netlist[i].tempoLigada))));
              }
              else
              {
                vetor(netlist[i].x)-=netlist[i].amplitude1;
              }
            }
          }
        }
  }

  A += Matriz.bottomRightCorner(A.rows(),A.cols());    //Retorna a matriz sem o nós de terra, a ser resolvido
  b += vetor.segment(1,b.size());                      //Retorna vetor b sem incluir no de terra

}

void montarEstampasVariantesLinearesPontoOperacao(vector <elemento> (&netlist),MatrixXd (&A),VectorXd (&b),Configuracao (&config))
{

  MatrixXd Matriz(A.rows()+1,A.cols()+1);
  VectorXd vetor(b.size()+1);

  Matriz =  MatrixXd::Zero(A.rows()+1,A.cols()+1);
  vetor = ArrayXd::Zero(b.size()+1);

  for(unsigned i=0;i<netlist.size();i++)
  {
    if (netlist.at(i).tipo == 'C') {
      Matriz(netlist[i].a,netlist[i].a)+=GCAPACITORABERTO;
      Matriz(netlist[i].b,netlist[i].b)+=GCAPACITORABERTO;
      Matriz(netlist[i].a,netlist[i].b)-=GCAPACITORABERTO;
      Matriz(netlist[i].b,netlist[i].a)-=GCAPACITORABERTO;
    }
    else if (netlist.at(i).tipo == 'L') {
      Matriz(netlist[i].a,netlist[i].a)+=GINDUTORCURTO;
      Matriz(netlist[i].b,netlist[i].b)+=GINDUTORCURTO;
      Matriz(netlist[i].a,netlist[i].b)-=GINDUTORCURTO;
      Matriz(netlist[i].b,netlist[i].a)-=GINDUTORCURTO;
    }
    else if (netlist.at(i).tipo=='I'){
      if (netlist[i].tipoDeFonte == SIN)  //caso seno
        {
          _long g = netlist.at(i).offset;

          vetor(netlist.at(i).a)-=g;
          vetor(netlist.at(i).b)+=g;
        }
      else if (netlist[i].tipoDeFonte == PULSE) //caso pulse
        {
          //Eu discordo do Moreirão aqui. Nessa parte o Moreirão colocaria o valor da fonte pulso no instante zero (e para fazer com que os gráficos fiquem iguais aos dele)
          //é necessário fazer isso). Entretanto, eu considero a fonte pulso como um sinal de entrada, e portanto não acho correto considera-la no ponto de operaçãonce
          //Mais correto ainda fosse provavelmente utilizar o valor médio da fonte pulso, levando em conta as amplitudes1 e 2, e o duty cicle. Eu não me importei de programa-la.
        }
    }
    else if (netlist.at(i).tipo=='V'){
      if (netlist[i].tipoDeFonte == SIN)  //caso seno
          {
            _long g = netlist.at(i).offset;

            Matriz(netlist.at(i).a,netlist.at(i).x)+=1;
            Matriz(netlist.at(i).b,netlist.at(i).x)-=1;
            Matriz(netlist.at(i).x,netlist.at(i).a)-=1;
            Matriz(netlist.at(i).x,netlist.at(i).b)+=1;
            vetor(netlist.at(i).x)-=g;
          }
      else if (netlist[i].tipoDeFonte == PULSE) //caso pulse
          {
            //Ler comentario acima
          }
      }
  }

  A += Matriz.bottomRightCorner(A.rows(),A.cols());    //Retorna a matriz sem o nós de terra, a ser resolvido
  b += vetor.segment(1,b.size());                      //Retorna vetor b sem incluir no de terra

}

void montarEstampasNaoLineares(vector <elemento> (&netlist),MatrixXd (&A),VectorXd (&b),VectorXd (&x))
{

  MatrixXd Matriz(A.rows()+1,A.cols()+1);
  VectorXd vetor(b.size()+1);
  VectorXd estado(x.size()+1);

  Matriz =  MatrixXd::Zero(A.rows()+1,A.cols()+1);
  vetor = ArrayXd::Zero(b.size()+1);

  estado[0] = 0;
  estado.segment(1,x.size()) = x;


  for(unsigned i=0;i<netlist.size();i++)
  {
    if (netlist.at(i).tipo == '$') {    ////////////////////////////////
      if ((estado[netlist[i].c] - estado[netlist[i].d]) <= netlist[i].valor)  //param3 = vref =valor
      {
        Matriz(netlist[i].a,netlist[i].a)+=netlist[i].goff; //param2 = goff
        Matriz(netlist[i].b,netlist[i].b)+=netlist[i].goff;
        Matriz(netlist[i].a,netlist[i].b)-=netlist[i].goff;
        Matriz(netlist[i].b,netlist[i].a)-=netlist[i].goff;
      }
      else
      {
        Matriz(netlist[i].a,netlist[i].a)+=netlist[i].gon;  //param1 = gon
        Matriz(netlist[i].b,netlist[i].b)+=netlist[i].gon;
        Matriz(netlist[i].a,netlist[i].b)-=netlist[i].gon;
        Matriz(netlist[i].b,netlist[i].a)-=netlist[i].gon;
      }
    }   ///////////////////
    else if (netlist.at(i).tipo == 'N')
    {
      _long g;
      _long z;

      if ( (estado[netlist[i].a] - estado[netlist[i].b]) < netlist[i].pv2 )
      {
        if ((netlist[i].pv2-netlist[i].pv1) != 0)       //EVITA SISTEMAS SINGULARES
        {
          g=((netlist[i].pj2 - netlist[i].pj1) / (netlist[i].pv2-netlist[i].pv1));
          z=(netlist[i].pj2 - g*netlist[i].pv2);
        }
        else
        {
          if (netlist[i].pv2 > netlist[i].pv1)
            g=INFINITO;
          else
            g=-INFINITO;
          z=(netlist[i].pj2 - g*netlist[i].pv2);
        }
      }
      else if (((estado[netlist[i].a] - estado[netlist[i].b]) < netlist[i].pv3))
      {
        if ((netlist[i].pv3-netlist[i].pv2) != 0)     //EVITA SISTEMAS SINGULARES
        {
          g=(netlist[i].pj3 - netlist[i].pj2) / (netlist[i].pv3 - netlist[i].pv2);
          z=(netlist[i].pj3 - g*netlist[i].pv3);
        }
        else
        {
          if (netlist[i].pv3 > netlist[i].pv2)
            g=INFINITO;
          else
            g=-INFINITO;
          z=(netlist[i].pj3 - g*netlist[i].pv3);
        }
      }
      else
      {
        if ((netlist[i].pv4-netlist[i].pv3) != 0)     //EVITA SISTEMAS SINGULARES
        {
          g=(netlist[i].pj4 - netlist[i].pj3) / (netlist[i].pv4 - netlist[i].pv3);
          z=(netlist[i].pj4 - g*netlist[i].pv4);
        }
        else
        {
          if (netlist[i].pv4 > netlist[i].pv3)
            g=INFINITO;
          else
            g=-INFINITO;
          z=(netlist[i].pj4 - g*netlist[i].pv4);
        }
      }

          Matriz(netlist[i].a,netlist[i].a)+=g;
          Matriz(netlist[i].b,netlist[i].b)+=g;
          Matriz(netlist[i].a,netlist[i].b)-=g;
          Matriz(netlist[i].b,netlist[i].a)-=g;
          vetor(netlist[i].a)-=z;
          vetor(netlist[i].b)+=z;
    }
  }

  A += Matriz.bottomRightCorner(A.rows(),A.cols());    //Retorna a matriz sem o nós de terra, a ser resolvido
  b += vetor.segment(1,b.size());              //Retorna vetor b sem incluir no de terra
};

void lerConfiguracao(vector <elemento> (&netlist),Configuracao (&config))
{
  for(unsigned i=0;i<netlist.size();i++)
  {
    if (netlist[i].tipo == '.')
    {
      config.setTempoFinal(netlist[i].tempoFinal).setPasso(netlist[i].passo).setPassoPorPt(netlist[i].passoPorPt).setMetodoDeIntegracao(netlist[i].metodoIntegracao).setTipoAnalise(netlist[i].nome);
    }
  }
}

void atualizarMemorias(vector<elemento> (&netlist),Configuracao (&config),VectorXd (&x))
{
  VectorXd vetor(x.size()+1);               //Vetor de tensoes aumentado, dessa forma netlist[i].a=0 corresponde ao nó de terra (=0)
  vetor = ArrayXd::Zero(x.size()+1);        //Isso evita q seja necessario usar algum tipo de condicao especial para caso do nó de terra
  vetor.segment(1,x.size()) = x;

  //  Apos resolucao do sistema no dominio do tempo, eh preciso atualizaar as memorias
  // de corrente e tensao nos capacitores e indutores do sistema (feito nessa funcao);
  // Lembrar que: "Yn[i][nv+1]" é a solucao da tensao no nó i para o tempo atual (é mesmo?)
  // Olha pagina 104 pra entender as contas
  for (unsigned i=0; i<netlist.size(); i++)
    {
      if (netlist[i].tipo == 'C')
      {
        if (config.getMetodoDeIntegracao() == TRAP)   //MetodoDosTrapezios
          {
            _long g=((2*netlist[i].valor)/(config.getPasso())); //Valor da condutancia para o MetodoDosTrapezios
            netlist[i].jt0 = ((vetor[netlist[i].a]) - (vetor[netlist[i].b]))*g -1*(g*netlist[i].vt0+netlist[i].jt0); //jt0 = (ea-eb)/R - I ; R = 1/g, I = g*vt0+jt0
            netlist[i].vt0 = ((vetor[netlist[i].a]) - (vetor[netlist[i].b])); //vt0 = (ea - eb)
          }
      }
      else
      if (netlist[i].tipo == 'L')
      {
        if (config.getMetodoDeIntegracao() == TRAP)   //MetodoDosTrapezios
          {
            _long g=((config.getPasso())/(2*netlist[i].valor));                                                                              //Valor da condutancia para o MetodoDosTrapezios
            netlist[i].jt0 = ((vetor[netlist[i].a]) - (vetor[netlist[i].b]))*g + (g*netlist[i].vt0+netlist[i].jt0);  //jt0 = (ea-eb)/R + I ; R = 1/g, I = g*vt0+jt0
            netlist[i].vt0 = ((vetor[netlist[i].a]) - (vetor[netlist[i].b]));                                        //vt0 = (ea - eb)
          }
      }
    }
}

void salvarResultadoEmMemoria(VectorXd (&x),vector<string> (&resultado),_long t)
{
  stringstream ss;
  ss << t << " " << x.transpose();
  resultado.push_back(ss.str());
};

void salvarResultadoEmArquivo(string nomeArquivo,const string extensao,vector <string> (&listaDeNos),vector<string> (&resultado))
{
  string str = nomeArquivo+extensao;
  FILE* arquivoSolucao = fopen(str.c_str(),"w");

  if (arquivoSolucao == NULL)
  {
    cout << "Não foi possível abrir o arquivo necessário para salvar simulacao"<<endl;
    exit(1);
  }

  /*Header*/
  fprintf (arquivoSolucao,"t ");
  for(unsigned int i = 1;i < listaDeNos.size();i++) //Não é necessário imprimir nó de terra, i inicia de 1
  {
    fprintf (arquivoSolucao,"%s ",listaDeNos[i].c_str());
  }
  fprintf (arquivoSolucao,"\n");

  /*Pontos*/
  for(unsigned int i = 0;i < resultado.size();i++)
  {
    fprintf (arquivoSolucao,"%s\n",resultado[i].c_str());
  }

  fclose(arquivoSolucao);
};

bool checarLinearidade(vector<elemento> (&netlist))
{
  unsigned int i=0;
  for(;i<netlist.size();i++)
    if ( (netlist[i].tipo == 'N') || (netlist[i].tipo == '$'))         //Não é linear
      return false;
  return true;  //é linear
};

bool newtonRaphsonChecarConvergencia(VectorXd (&x),VectorXd (&nr_x_anterior),vector <int> (&faltaConvergir))
{
  bool erroGrande = false;
  for (unsigned i=0; i< x.size(); i++)
  {
    if ( (fabs(x[i] - nr_x_anterior[i])) > NEWTON_RAPHSON_MAX_ERRO_PERMITIDO)
    {
      faltaConvergir[i]=1;  //Nó ainda não convergiu
      erroGrande=true;
    }
    else
    {
      faltaConvergir[i]=0;  //Nó convergiu
    }
  }
  return (!erroGrande);
}

void newtonRaphsonInicializaVetorFaltaConvergir(vector<int> (&v))
{
  for(unsigned i=0;i<v.size();i++)
    v[i] = 0;
};

void newtonRaphsonAtualizarSolucao(VectorXd (&x),VectorXd (&nr_x_anterior),vector <int> (&faltaConvergir))
{
  for (unsigned i=0;i < faltaConvergir.size();i++)
  {
    if (faltaConvergir[i] == 1)
    {
      nr_x_anterior[i] = x[i];
    }
  }
};

void printProgresso(int i, char simbolo,Configuracao (&config),_long (&t)){ //Printa o progresso do calculo, dependendo do passo atual{
  int porcentagem = 100/i;
  int progredi =
  static_cast <int> (floor((t/(config.getTempoFinal()/porcentagem))) - floor( (t-config.getPasso()) / (config.getTempoFinal()/porcentagem) ) );

  if(t == 0)
    {
      cout << "Progresso: " << flush;
    }

  if(progredi != 0)   //se eu tiver progredido
    {
      for(int i = 1;i <= progredi;i++)  //printo pauzinhos suficientes para quanto eu progredi
      cout << simbolo << flush;
    }

  if(t > config.getTempoFinal()-config.getPasso())
  {
      cout << endl;
  }
};

void clrscr()
{
  cout << "\033[2J\033[1;1H";
}

void newtonRaphsonMontarCondutanciasGmin(vector <elemento> (&netlist),MatrixXd (&A),VectorXd (&b),vector <int> (&faltaConvergir),_long condutanciaGmin)
{

  MatrixXd Matriz(A.rows()+1,A.cols()+1);
  VectorXd vetor(b.size()+1);

  Matriz =  MatrixXd::Zero(A.rows()+1,A.cols()+1);
  vetor = ArrayXd::Zero(b.size()+1);

  for(unsigned i=1;i<=netlist.size();i++)
  {
    if (faltaConvergir[i-1] == 1){                      //se esse no i nao convergiu..
      if ((netlist[i].a==(int)i)||(netlist[i].b==(int)i))
      {                                                 //e se o elemento encosta nesse no problematico..
        Matriz(netlist[i].a,netlist[i].a)+=condutanciaGmin;
        Matriz(netlist[i].b,netlist[i].b)+=condutanciaGmin;
        Matriz(netlist[i].a,netlist[i].b)-=condutanciaGmin;
        Matriz(netlist[i].b,netlist[i].a)-=condutanciaGmin;

        if (netlist[i].tipo =='N')
        {                   //e se for um resistor nao linear, coloco uma fonte de corrente
          vetor(netlist[i].a)-=(netlist[i].pv2 + netlist[i].pv3)*condutanciaGmin/(2);
          vetor(netlist[i].b)+=(netlist[i].pv2 + netlist[i].pv3)*condutanciaGmin/(2);
        }
      }
    }
  }
  A += Matriz.bottomRightCorner(A.rows(),A.cols());    //Retorna a matriz sem o nós de terra, a ser resolvido
  b += vetor.segment(1,b.size());                      //Retorna vetor b sem incluir no de terra
};
