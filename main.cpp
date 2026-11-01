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
    {"Sequencia fraca", "12345678"},
    {"Senha do notebook", "020699"},
    {"Senha: senha", "senha"},
    {"Repeticao", "123123"},
    {"Senha: senha (ingles)", "password"},
    {"Sequencia ao contrario", "654321"}
};

// Consulta a matriz de modelos conhecidos.
bool consultarMatriz(const string& senha, string& nomeModelo) {
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
        ++tentativas;
        if (tentativa == alvo) {
            return true;
        }
    }

    if (tentativa.size() >= static_cast<size_t>(tamanhoMaximo)) {
        return false;
    }

    for (char caractere : caracteres) {
        tentativa.push_back(caractere);

        if (gerarCombinacoes(
                caracteres, alvo, tentativa,
                tamanhoMaximo, tentativas)) {
            return true;
        }

        tentativa.pop_back();
    }

    return false;
}

int main() {
    string senhaTeste;
    const string caracteres = "abcdefghijklmnopqrstuvwxyz0123456789.";
    const int tamanhoMaximo = 6;

    cout << "==============================\n";
    cout << "          TESTASENHA\n";
    cout << "==============================\n";
    cout << "Digite uma senha ficticia para testar: ";
    getline(cin, senhaTeste);

    if (senhaTeste.empty()) {
        cout << "\nErro: a senha nao pode estar vazia.\n";
        return 1;
    }

    string nomeModelo;
    unsigned long long tentativas = 0;

    auto inicio = steady_clock::now();

    cout << "\n=== TESTE DE MODELOS ===\n";

    if (consultarMatriz(senhaTeste, nomeModelo)) {
        cout << "Modelo encontrado: " << nomeModelo << '\n';
        cout << "Candidato: " << senhaTeste << '\n';
        cout << "Busca exaustiva nao necessaria.\n";
    } else if (senhaTeste.size() <= static_cast<size_t>(tamanhoMaximo)) {
        cout << "Nenhum modelo correspondente.\n";
        cout << "Iniciando demonstracao exaustiva didatica...\n";

        string tentativa;
        bool encontrada = gerarCombinacoes(
            caracteres, senhaTeste, tentativa,
            tamanhoMaximo, tentativas
        );

        if (encontrada) {
            cout << "Candidato encontrado: " << tentativa << '\n';
        } else {
            cout << "Candidato nao encontrado no conjunto testado.\n";
        }

        cout << "Tentativas realizadas: " << tentativas << '\n';
    } else {
        cout << "Nenhum modelo correspondente.\n";
        cout << "Busca didatica ignorada: a senha ultrapassa "
             << tamanhoMaximo << " caracteres.\n";
    }

    auto fim = steady_clock::now();
    double tempo = duration<double>(fim - inicio).count();

    cout << "Tempo da etapa de modelos: " << tempo << " segundos\n";

    cout << "\n=== AVALIACAO DA SENHA ORIGINAL ===\n";
    int pontuacao = avaliarSenha(senhaTeste);

    if (pontuacao == 5) {
        cout << "\nSua senha ja foi classificada como PODEROSA.\n";
        cout << "Nenhuma sugestao de alteracao e necessaria.\n";
        return 0;
    }

    cout << "\n=== SUGESTAO DE SENHA MAIS FORTE ===\n";
    string sugestao = sugerirSenha(senhaTeste, pontuacao);

    cout << "Sugestao inicial: " << sugestao << "\n";

    cout << "\n=== AVALIACAO DA SUGESTAO ===\n";
    int novaPontuacao = avaliarSenha(sugestao);

    // Se a sugestao baseada na senha original nao atingir 5,
    // tenta gerar senhas aleatorias ate encontrar uma aprovada.
    const int maxTentativasGeracao = 10;
    int tentativasGeracao = 0;

    while (novaPontuacao < 5 &&
           tentativasGeracao < maxTentativasGeracao) {
        ++tentativasGeracao;
        cout << "\nA sugestao ainda nao atingiu 5/5.\n";
        cout << "Gerando uma nova senha aleatoria...\n";

        sugestao = sugerirSenha("", 1);
        novaPontuacao = avaliarSenha(sugestao);
    }

    cout << "\n==============================\n";
    cout << "       RESULTADO DA SUGESTAO\n";
    cout << "==============================\n";
    cout << "Senha sugerida: " << sugestao << '\n';
    cout << "Pontuacao obtida: " << novaPontuacao << "/5\n";

    if (novaPontuacao == 5) {
        cout << "Classificacao: Poderosa\n";
    } else {
        cout << "Nao foi possivel confirmar uma sugestao 5/5 "
             << "nesta execucao.\n";
    }

    cout << "\nUse apenas senhas ficticias durante os testes.\n";
    return 0;
}

// Compilacao:
// g++ -std=c++17 main.cpp avaliar_senha.cpp -o TestaSenha.exe
// Execucao no PowerShell:
// .\\TestaSenha.exe
