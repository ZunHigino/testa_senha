
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <cctype>
#include <algorithm>
#include "avaliar_senha.h"

using namespace std;

// Verifica se existem tres ou mais digitos iguais seguidos.
// Exemplos: 111, 2222, 999.
bool numerosRepetidos(const string& senha) {
    int repeticoes = 1;

    for (size_t i = 1; i < senha.size(); i++) {
        if (isdigit(static_cast<unsigned char>(senha[i])) &&
            senha[i] == senha[i - 1]) {
            repeticoes++;

            if (repeticoes >= 3) {
                return true;
            }
        } else {
            repeticoes = 1;
        }
    }

    return false;
}

// Procura sequencias numericas de 6 ou 8 digitos,
// que podem representar datas pessoais.
bool dataProvavel(const string& senha) {
    int sequencia = 0;

    for (char c : senha) {
        if (isdigit(static_cast<unsigned char>(c))) {
            sequencia++;

            if (sequencia >= 6) {
                return true;
            }
        } else {
            sequencia = 0;
        }
    }

    return false;
}

// Heuristica simples para alguns nomes e palavras comuns.
// A lista pode ser ampliada conforme o projeto.
bool nomeOuPalavraComum(const string& senha) {
    string texto = senha;

    transform(texto.begin(), texto.end(), texto.begin(),
        [](unsigned char c) {
            return static_cast<char>(tolower(c));
        });

    vector<string> palavras = {
        "maria", "joao", "pedro", "lucas",
        "gabriel", "julia", "ana", "admin",
        "password", "senha"
    };

    for (const string& palavra : palavras) {
        if (texto.find(palavra) != string::npos) {
            return true;
        }
    }

    return false;
}

string classificarSenha(int pontos) {
    switch (pontos) {
        case 0:
            return "Extremamente fraca";
        case 1:
            return "Fraca";
        case 2:
        case 3:
            return "Moderada";
        case 4:
            return "Forte";
        case 5:
            return "Poderosa";
        default:
            return "Nao classificada";
    }
}

int avaliarSenha(const string& senha) {
    int pontos = 0;

    bool temLetra = false;
    bool temNumero = false;
    bool temSimbolo = false;
    bool temMaiuscula = false;
    bool temMinuscula = false;

    // Identifica as categorias de caracteres.
    for (char c : senha) {
        unsigned char caractere = static_cast<unsigned char>(c);

        if (isalpha(caractere)) {
            temLetra = true;
        } if (isupper(caractere)) {
            temMaiuscula = true;
        } if (islower(caractere)) {
            temMinuscula = true;
        } else if (isdigit(caractere)) {
            temNumero = true;
        } else {
            temSimbolo = true;
        }
    }

    // 1. Comprimento da senha.
    if (senha.size() >= 12) {
        pontos += 2;
        cout << "+2: senha com 12 ou mais caracteres.\n";
    } else if (senha.size() > 8) {
        pontos += 1;
        cout << "+1: senha com mais de 8 caracteres.\n";
    } else {
        pontos -= 1;
        cout << "-1: senha com 8 caracteres ou menos.\n";
    }

    // 2. Variedade de categorias.
    int categorias = temLetra + temNumero + temSimbolo;

    if (categorias == 3) {
        pontos += 2;
        cout << "+2: possui letras, numeros e simbolos.\n";
    } else if (categorias == 2) {
        pontos += 1;
        cout << "+1: combina duas categorias.\n";
    } else {
        cout << "+0: utiliza apenas uma categoria.\n";
    }

    // 3. Caracteres unicos.
    set<char> caracteresUnicos(senha.begin(), senha.end());

    if (caracteresUnicos.size() == senha.size()) {
        pontos += 1;
        cout << "+1: nao possui caracteres repetidos.\n";
    }

    bool caracteresRepetidos = false;
    int repeticoes = 1;

    for (size_t i = 1; i < senha.size(); i++) {
        if (senha[i] == senha[i - 1]) {
            repeticoes++;

            if (repeticoes > 2) {
                caracteresRepetidos = true;
                break;
            }
        } else {
            repeticoes = 1;
        }
    }

    if (caracteresRepetidos) {
        pontos -= 1;
        cout << "-1: possui caracteres repetidos mais de duas vezes seguidas.\n";
    }

    // 4. Letras maiusculas e minusculas.
    if (temMaiuscula && temMinuscula) {
        pontos += 1;
        cout << "+1: combina letras maiusculas e minusculas.\n";
    } else {
        cout << "+0: nao combina letras maiusculas e minusculas.\n";
    }

    // 5. Sequencias de digitos repetidos.
    if (numerosRepetidos(senha)) {
        pontos -= 1;
        cout << "-1: possui uma sequencia de numeros repetidos.\n";
    }

    // 6. Possiveis datas pessoais.
    if (dataProvavel(senha)) {
        pontos -= 1;
        cout << "-1: possui uma sequencia numerica que pode ser uma data.\n";
    }

    // 7. Nomes e palavras comuns.
    if (nomeOuPalavraComum(senha)) {
        pontos -= 1;
        cout << "-1: contem um nome ou uma palavra comum da lista.\n";
    }

    // Limita a pontuacao entre 0 e 5.
    pontos = max(0, min(5, pontos));

    cout << "\nPontuacao final: " << pontos << "/5\n";
    cout << "Classificacao: " << classificarSenha(pontos) << "\n";

    return pontos;
}