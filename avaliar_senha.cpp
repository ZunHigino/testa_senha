#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <cctype>
#include <algorithm>
#include <random>

#include "avaliar_senha.h"

using namespace std;

// Verifica se existem tres ou mais digitos iguais seguidos.
bool numerosRepetidos(const string& senha) {
    int repeticoes = 1;

    for (size_t i = 1; i < senha.size(); ++i) {
        unsigned char atual = static_cast<unsigned char>(senha[i]);

        if (isdigit(atual) && senha[i] == senha[i - 1]) {
            ++repeticoes;
            if (repeticoes >= 3) {
                return true;
            }
        } else {
            repeticoes = 1;
        }
    }
    return false;
}

// Procura sequencias de seis ou mais digitos consecutivos,
// que podem representar datas pessoais.
bool dataProvavel(const string& senha) {
    int sequencia = 0;

    for (unsigned char c : senha) {
        if (isdigit(c)) {
            ++sequencia;
            if (sequencia >= 6) {
                return true;
            }
        } else {
            sequencia = 0;
        }
    }
    return false;
}

// Heuristica simples para nomes e palavras comuns.
bool nomeOuPalavraComum(const string& senha) {
    string texto = senha;

    transform(texto.begin(), texto.end(), texto.begin(),
        [](unsigned char c) {
            return static_cast<char>(tolower(c));
        });

    const vector<string> palavras = {
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
        case 0: return "Extremamente fraca";
        case 1: return "Fraca";
        case 2:
        case 3: return "Moderada";
        case 4: return "Forte";
        case 5: return "Poderosa";
        default: return "Nao classificada";
    }
}

int avaliarSenha(const string& senha) {
    int pontos = 0;

    bool temLetra = false;
    bool temNumero = false;
    bool temSimbolo = false;
    bool temMaiuscula = false;
    bool temMinuscula = false;

    // Identifica corretamente as categorias de caracteres.
    for (unsigned char caractere : senha) {
        if (isalpha(caractere)) {
            temLetra = true;
        }
        if (isupper(caractere)) {
            temMaiuscula = true;
        }
        if (islower(caractere)) {
            temMinuscula = true;
        }
        if (isdigit(caractere)) {
            temNumero = true;
        }
        if (!isalnum(caractere)) {
            temSimbolo = true;
        }
    }

    // 1. Comprimento.
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
    int categorias = static_cast<int>(temLetra) +
                     static_cast<int>(temNumero) +
                     static_cast<int>(temSimbolo);

    if (categorias == 3) {
        pontos += 2;
        cout << "+2: possui letras, numeros e simbolos.\n";
    } else if (categorias == 2) {
        pontos += 1;
        cout << "+1: combina duas categorias.\n";
    } else {
        cout << "+0: utiliza apenas uma categoria.\n";
    }

    // 3. Caracteres unicos: bonus se nenhum caractere se repetir.
    set<char> caracteresUnicos(senha.begin(), senha.end());

    if (caracteresUnicos.size() == senha.size()) {
        pontos += 1;
        cout << "+1: nao possui caracteres repetidos.\n";
    }

    // Penalidade quando qualquer caractere aparece 3 ou mais
    // vezes consecutivas (letras, numeros ou simbolos).
    bool repeticaoConsecutiva = false;
    int repeticoes = 1;

    for (size_t i = 1; i < senha.size(); ++i) {
        if (senha[i] == senha[i - 1]) {
            ++repeticoes;
            if (repeticoes >= 3) {
                repeticaoConsecutiva = true;
                break;
            }
        } else {
            repeticoes = 1;
        }
    }

    if (repeticaoConsecutiva) {
        pontos -= 1;
        cout << "-1: possui caracteres repetidos tres ou mais vezes seguidas.\n";
    }

    // 4. Letras maiusculas e minusculas.
    if (temMaiuscula && temMinuscula) {
        pontos += 1;
        cout << "+1: combina letras maiusculas e minusculas.\n";
    } else {
        cout << "+0: nao combina letras maiusculas e minusculas.\n";
    }

    // 5. Sequencias de digitos iguais.
    // Mantido como criterio independente do projeto original.
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

    pontos = max(0, min(5, pontos));

    cout << "\nPontuacao final: " << pontos << "/5\n";
    cout << "Classificacao: " << classificarSenha(pontos) << "\n";

    return pontos;
}

// Gera uma senha aleatoria para a demonstracao academica.
// Observacao: mt19937 nao e um gerador criptografico.
static string gerarSenhaAleatoria() {
    const string minusculas = "abcdefghijklmnopqrstuvwxyz";
    const string maiusculas = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const string numeros = "0123456789";
    const string simbolos = "!@#$%&*?-_+";
    const string todos = minusculas + maiusculas + numeros + simbolos;

    random_device rd;
    mt19937 gerador(rd());

    auto sortear = [&](const string& grupo) -> char {
        uniform_int_distribution<size_t> distribuicao(0, grupo.size() - 1);
        return grupo[distribuicao(gerador)];
    };

    string senha;
    senha += sortear(minusculas);
    senha += sortear(maiusculas);
    senha += sortear(numeros);
    senha += sortear(simbolos);

    string restantes;
    for (char c : todos) {
        if (senha.find(c) == string::npos) {
            restantes += c;
        }
    }

    shuffle(restantes.begin(), restantes.end(), gerador);
    while (senha.size() < 16 && !restantes.empty()) {
        senha += restantes.back();
        restantes.pop_back();
    }

    shuffle(senha.begin(), senha.end(), gerador);
    return senha;
}

string sugerirSenha(const string& senha, int pontuacao) {
    // Para senhas fracas, recomenda uma senha nova.
    if (pontuacao <= 1) {
        return gerarSenhaAleatoria();
    }

    // Se ja for poderosa, mantem a senha.
    if (pontuacao >= 5) {
        return senha;
    }

    // Para moderada ou forte, tenta preservar os caracteres originais,
    // retirando repeticoes para poder obter o bonus de caracteres unicos.
    string novaSenha;
    set<char> utilizados;

    for (char c : senha) {
        if (utilizados.insert(c).second) {
            novaSenha += c;
        }
    }

    const string minusculas = "abcdefghijklmnopqrstuvwxyz";
    const string maiusculas = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const string numeros = "0123456789";
    const string simbolos = "!@#$%&*?-_+";

    auto adicionarUm = [&](const string& grupo) {
        for (char c : grupo) {
            if (utilizados.insert(c).second) {
                novaSenha += c;
                return;
            }
        }
    };

    bool temMinuscula = false;
    bool temMaiuscula = false;
    bool temNumero = false;
    bool temSimbolo = false;

    for (unsigned char c : novaSenha) {
        if (islower(c)) temMinuscula = true;
        if (isupper(c)) temMaiuscula = true;
        if (isdigit(c)) temNumero = true;
        if (!isalnum(c)) temSimbolo = true;
    }

    if (!temMinuscula) adicionarUm(minusculas);
    if (!temMaiuscula) adicionarUm(maiusculas);
    if (!temNumero) adicionarUm(numeros);
    if (!temSimbolo) adicionarUm(simbolos);

    // Garante comprimento minimo de 12 caracteres.
    const string todos = minusculas + maiusculas + numeros + simbolos;
    random_device rd;
    mt19937 gerador(rd());
    string restantes;

    for (char c : todos) {
        if (utilizados.find(c) == utilizados.end()) {
            restantes += c;
        }
    }

    shuffle(restantes.begin(), restantes.end(), gerador);

    while (novaSenha.size() < 12 && !restantes.empty()) {
        char c = restantes.back();
        restantes.pop_back();
        if (utilizados.insert(c).second) {
            novaSenha += c;
        }
    }

    return novaSenha;
}
