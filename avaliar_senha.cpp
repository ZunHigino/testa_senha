#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <cctype>
#include <algorithm>
#include <random>
#include <sstream>

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

int avaliarSenha(const string& senha, bool exibir) {
    ostringstream saida;
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
        saida << "+2: senha com 12 ou mais caracteres.\n";
    } else if (senha.size() > 8) {
        pontos += 1;
        saida << "+1: senha com mais de 8 caracteres.\n";
    } else {
        pontos -= 1;
        saida << "-1: senha com 8 caracteres ou menos.\n";
    }

    // 2. Variedade de categorias.
    int categorias = static_cast<int>(temLetra) +
                     static_cast<int>(temNumero) +
                     static_cast<int>(temSimbolo);

    if (categorias == 3) {
        pontos += 2;
        saida << "+2: possui letras, numeros e simbolos.\n";
    } else if (categorias == 2) {
        pontos += 1;
        saida << "+1: combina duas categorias.\n";
    } else {
        saida << "+0: utiliza apenas uma categoria.\n";
    }

    // 3. Caracteres unicos: bonus se nenhum caractere se repetir.
    set<char> caracteresUnicos(senha.begin(), senha.end());

    if (caracteresUnicos.size() == senha.size()) {
        pontos += 1;
        saida << "+1: nao possui caracteres repetidos.\n";
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
        saida << "-1: possui caracteres repetidos tres ou mais vezes seguidas.\n";
    }

    // 4. Letras maiusculas e minusculas.
    if (temMaiuscula && temMinuscula) {
        pontos += 1;
        saida << "+1: combina letras maiusculas e minusculas.\n";
    } else {
        saida << "+0: nao combina letras maiusculas e minusculas.\n";
    }

    // 5. Sequencias de digitos iguais.
    // Mantido como criterio independente do projeto original.
    if (numerosRepetidos(senha)) {
        pontos -= 1;
        saida << "-1: possui uma sequencia de numeros repetidos.\n";
    }

    // 6. Possiveis datas pessoais.
    if (dataProvavel(senha)) {
        pontos -= 1;
        saida << "-1: possui uma sequencia numerica que pode ser uma data.\n";
    }

    // 7. Nomes e palavras comuns.
    if (nomeOuPalavraComum(senha)) {
        pontos -= 1;
        saida << "-1: contem um nome ou uma palavra comum da lista.\n";
    }

    pontos = max(0, min(5, pontos));

    saida << "\nPontuacao final: " << pontos << "/5\n";
    saida << "Classificacao: " << classificarSenha(pontos) << "\n";

    if (exibir) cout << saida.str();
    return pontos;
}

// A geracao utiliza mt19937 apenas para demonstracao academica.
// Nao use as sugestoes como credenciais reais sem um gerador seguro.
static string gerarCandidata(const string& original, bool preservar,
                            const Configuracao& config, mt19937& gerador) {
    const string minusculas = "abcdefghijklmnopqrstuvwxyz";
    const string maiusculas = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const string numeros = "0123456789";
    const string simbolos = "!@#$%&*?-_+";
    const bool permitirLetras = config.categorias != Categorias::Numeros;
    const bool permitirNumeros = config.categorias != Categorias::LetrasSimbolos;
    const bool permitirSimbolos = config.categorias != Categorias::Numeros;
    string disponiveis = (permitirLetras ? minusculas + maiusculas : "") +
                        (permitirNumeros ? numeros : "") +
                        (permitirSimbolos ? simbolos : "");
    string resultado;
    auto adicionarSePossivel = [&](char c) {
        if (resultado.size() < config.tamanho &&
            disponiveis.find(c) != string::npos &&
            resultado.find(c) == string::npos) {
            resultado.push_back(c);
        }
    };

    // Preserva a ordem e os caracteres permitidos da senha original,
    // quando a classificacao for moderada ou forte.
    if (preservar) {
        for (char c : original) adicionarSePossivel(c);
    }

    auto incluirCategoria = [&](const string& grupo) {
        bool presente = false;
        for (char c : resultado) {
            if (grupo.find(c) != string::npos) { presente = true; break; }
        }
        if (!presente) {
            // Libera uma posicao para a categoria ausente, se necessario.
            if (resultado.size() >= config.tamanho && !resultado.empty()) {
                resultado.pop_back();
            }
            string escolhas = grupo;
            shuffle(escolhas.begin(), escolhas.end(), gerador);
            for (char c : escolhas) {
                if (resultado.find(c) == string::npos) {
                    adicionarSePossivel(c);
                    break;
                }
            }
        }
    };
    if (permitirLetras) {
        incluirCategoria(minusculas);
        incluirCategoria(maiusculas);
    }
    if (permitirNumeros) incluirCategoria(numeros);
    if (permitirSimbolos) incluirCategoria(simbolos);

    string restantes = disponiveis;
    shuffle(restantes.begin(), restantes.end(), gerador);
    for (char c : restantes) {
        if (resultado.size() >= config.tamanho) break;
        adicionarSePossivel(c);
    }
    // No modo numerico e tamanho >10, repeticoes sao inevitaveis.
    // Evita, sempre que possivel, 3 caracteres iguais consecutivos.
    uniform_int_distribution<size_t> escolher(0, disponiveis.size() - 1);
    while (resultado.size() < config.tamanho) {
        char c = disponiveis[escolher(gerador)];
        size_t n = resultado.size();
        if (n >= 2 && resultado[n-1] == c && resultado[n-2] == c) continue;
        resultado.push_back(c);
    }
    // Senhas totalmente novas nao precisam preservar nenhuma ordem.
    if (!preservar) shuffle(resultado.begin(), resultado.end(), gerador);
    return resultado;
}

string sugerirSenha(const string& senha, int pontuacao,
                    const Configuracao& config) {
    if (pontuacao >= 5) return senha;
    random_device rd;
    mt19937 gerador(rd());
    bool preservar = pontuacao >= 2;
    string melhor;
    int melhorNota = -1;
    for (int i = 0; i < 80; ++i) {
        string candidata = gerarCandidata(senha, preservar && i < 40,
                                         config, gerador);
        int nota = avaliarSenha(candidata, false);
        if (nota > melhorNota) {
            melhor = candidata;
            melhorNota = nota;
        }
        if (melhorNota == 5) break;
    }
    return melhor;
}
