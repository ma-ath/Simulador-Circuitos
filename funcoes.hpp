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

/*Estampas não lineares. Nas estampas não lineares, a estampa se altera de acordo com estado atual da solução, e por isso é necessário passar "x" para solução*/
void montarEstampasNaoLineares(vector <elemento> (&netlist),MatrixXd (&A),VectorXd (&b),VectorXd (&x));

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

/*Checa se netlist só contem componentes lineares. Caso sim, não é necessário fazer Newton Raphson*/
bool checarLinearidade(vector<elemento> (&netlist));

/*Função que checa se análise de Newton Raphson já convergiu*/
bool newtonRaphsonChecarConvergencia(VectorXd (&x),VectorXd (&nr_x_anterior),vector <int> (&faltaConvergir));

/*funcao que inicializa o vetor que indica quais variaveis faltam convergir no Newton Raphson*/
void newtonRaphsonInicializaVetorFaltaConvergir(vector<int> (&v));

/*Atualiza solução anterior do Newton Raphson*/
void newtonRaphsonAtualizarSolucao(VectorXd (&x),VectorXd (&nr_x_anterior),vector <int> (&faltaConvergir));

/*gmin-stepping: Montar condutancias em paralelo com elementos não lineares*/
void newtonRaphsonMontarCondutanciasGmin(vector <elemento> (&netlist),MatrixXd (&A),VectorXd (&b),vector <int> (&faltaConvergir),_long condutanciaGmin);

/*Printa o progresso do calculo, dependendo do passo atual*/
void printProgresso(int i, char simbolo,Configuracao (&config),_long (&t));

/*Limpa a tela*/
void clrscr();

#endif
