
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <chrono>
#include "avaliar_senha.h"

using namespace std;
using namespace chrono;

// Matriz bivetorial:
// coluna 0 = nome do modelo
// coluna 1 = candidato ficticio
using Modelo = array<string, 2>;

vector<Modelo> matrizSenhas = {
    {"Sequencia numerica", "1234"},
    {"Digitos repetidos", "000000"},
    {"Sequencia alfabetica", "abc"},
    {"Padrao alternado", "121"},
    {"Exemplo alfanumerico", "a1b"},
    {"Sequência fraca", "12345678"},
    {"Senha do notebook", "020699"},
    {"Senha: senha", "senha"},
    {"Repetição", "123123"},
    {"Senha: senha (ingles)", "password"},
    {"Sequência ao contraria", "654321"}
};

// Consulta a matriz de modelos conhecidos.
bool consultarMatriz(
    const string& senha,
    string& nomeModelo
) {
    for (const auto& modelo : matrizSenhas) {
        if (modelo[1] == senha) {
            nomeModelo = modelo[0];
            return true;
        }
    }

    return false;
}

// Busca didatica em um espaco pequeno e controlado.
bool gerarCombinacoes(
    const string& caracteres,
    const string& alvo,
    string& tentativa,
    int tamanhoMaximo,
    unsigned long long& tentativas
) {
    if (!tentativa.empty()) {
        tentativas++;

        if (tentativa == alvo) {
            return true;
        }
    }

    if (tentativa.size() >=
        static_cast<size_t>(tamanhoMaximo)) {
        return false;
    }

    for (char caractere : caracteres) {
        tentativa.push_back(caractere);

        if (gerarCombinacoes(
                caracteres,
                alvo,
                tentativa,
                tamanhoMaximo,
                tentativas)) {
            return true;
        }

        tentativa.pop_back();
    }

    return false;
}

int main() {
    // Dados ficticios para demonstracao academica.
    string senhaTeste;
    const string caracteres = "abcdefghijklmnopqrstuvwxyz0123456789.";
    const int tamanhoMaximo = 6;

    cout << "Digite uma senha para testar: ";
    getline(cin, senhaTeste);

    if (senhaTeste.empty()) {
        cout << "\nErro: a senha nao pode estar vazia.\n";
        return 1;
    }

    string nomeModelo;
    unsigned long long tentativas = 0;

    auto inicio = steady_clock::now();

    cout << "=== TESTE DE MODELOS ===\n";

    // Etapa 1: consultar a matriz.
    if (consultarMatriz(senhaTeste, nomeModelo)) {
        cout << "Modelo encontrado: "
             << nomeModelo << '\n';
        cout << "Candidato: " << senhaTeste << '\n';
        cout << "Busca exaustiva nao necessaria.\n";
    } else {
        cout << "Nenhum modelo correspondente.\n";
        cout << "Iniciando demonstracao exaustiva...\n";

        // Etapa 2: busca no espaco didatico.
        string tentativa;
        bool encontrada = gerarCombinacoes(
            caracteres,
            senhaTeste,
            tentativa,
            tamanhoMaximo,
            tentativas
        );

        if (encontrada) {
            cout << "Candidato encontrado: "
                 << tentativa << '\n';
        } else {
            cout << "Candidato nao encontrado.\n";
        }

        cout << "Tentativas realizadas: "
             << tentativas << '\n';
    }

    auto fim = steady_clock::now();

    double tempo = duration<double>(
        fim - inicio
    ).count();

    cout << "Tempo total: "
         << tempo << " segundos\n";

    int pontuacao = avaliarSenha(senhaTeste);
     
    return 0;
}

//g++ -std=c++17 main.cpp avaliar_senha.cpp -o TestaSenha.exe
//.\TestaSenha.exe