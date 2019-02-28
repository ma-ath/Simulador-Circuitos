#include <vector>
#include <string>
#include "Eigen/Eigen"
#include "tipos.hpp"

using namespace std;
using namespace Eigen;

#ifndef _FUNCOES_
#define _FUNCOES_

/*lerNetlist: entra com nome do arquivo de netlist, lê ele e retorna o netlist*/
/*em forma de vetor de elementos e numero de variaveis*/
vector<elemento> lerNetlist(char* nomeArquivo, int (&numeroDeVariaveis),vector <string> (&listaDeNos));

/*split: pega uma string e separa a cada 'ch' caracteres, salva resultado em um vetor de strings*/
size_t split(const std::string &txt, std::vector<std::string> &strs, char ch);

/*Funcao q transforma os nomes de nos do netlist em posicoes para matriz a resolver*/
int numerarNoDoNetlist(string nomeDoNo,vector <string> (&listaDeNos));

/*funcao q verifica se os parametros escritos no arquivo de netlist estao corretos*/
void testarListaDeParametros(char tipo,vector<string> parametros);

/*Monta as estampas que nao variam no tempo, ou seja:*/
/*Fontes dependentes e independentes DC, Resistores, Transformador ideal e OPAMP*/
void montarEstampasInvariantes(vector <elemento> (&netlist),MatrixXd (&Matriz),VectorXd (&vetor));

/*Estampas variantes sao as que variam no tempo. Elas dependem da configuracao, e por isso é necessario passar para a funcao*/
void montarEstampasVariantesLineares(vector <elemento> (&netlist),MatrixXd (&Matriz),VectorXd (&vetor),Configuracao (&config),_long t);

/*Estampas de elementos variantes Para o ponto de operação. Elas dependem da configuracao, e por isso é necessario passar para a funcao*/
void montarEstampasVariantesLinearesPontoOperacao(vector <elemento> (&netlist),MatrixXd (&Matriz),VectorXd (&vetor),Configuracao (&config));

/*Le as informacoes de configuracao do Netlist*/
void lerConfiguracao(vector <elemento> (&netlist),Configuracao (&config));

/*h(t)*/
_long heaviside(_long t);

/*atualiza as memorias em capacitores e indutores*/
void atualizarMemorias(vector<elemento> (&netlist),Configuracao (&config),VectorXd (&vetor));

/*Salva resultados de simulacao em um vetor de strings, para ao final da simulacao salvar em disco de uma só vez (evita acesso ao disco o tempo todo)*/
void salvarResultadoEmMemoria(VectorXd (&x),vector<string> (&resultado),_long t);

/*Salva resultados de simulacao do vetor no disco, em arquivo com "nome de arquivo"*/
void salvarResultadoEmArquivo(string nomeArquivo,const string extensao,vector <string> (&listaDeNos),vector<string> (&resultado));

#endif
