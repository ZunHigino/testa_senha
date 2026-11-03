#include <iostream> //Exibe mensagens e recebe dados pelo terminal
#include <vector>   //Armazena vários conjuntos
#include <array>    //Organiza os dados em array
#include <string>   //Manipula strings
#include <chrono>   //Mede o tempo de execução
#include <cctype>   //Verifica os tipos de caracteres
#include "avaliar_senha.h"  // Acessa as funções e configurações do sistema

using namespace std;    //Evita escrever std:: em alguns elementos
using namespace chrono; 

using Modelo = array<string, 2>;        //Conjunto de duas strings
const vector<Modelo> matrizSenhas = {   //Armazena os conjuntos
    {"Sequencia numerica", "1234"},
    {"Digitos repetidos", "000000"},
    {"Sequencia alfabetica", "abc"},
    {"Padrao alternado", "121"},
    {"Exemplo alfanumerico", "a1b"},
    {"Sequencia fraca", "12345678"},
    {"Senha: senha", "senha"},
    {"Repeticao", "123123"},
    {"Senha: senha (ingles)", "password"},
    {"Sequencia ao contrario", "654321"}
};

bool consultarMatriz(const string& senha, string& nomeModelo) { //Consulta a tabela de modelos
    for (const auto& modelo : matrizSenhas) {
        if (modelo[1] == senha) {   //Compara a senha fornecida com todas as senhas do modelo
            nomeModelo = modelo[0]; //Guarda o nome do modelo em uma variável caso encontre
            return true;            //Encontrou
        }
    }
    return false;   //Não encontrou
}

// Busca exaustiva apenas para demonstração, num espaco pequeno.
bool gerarCombinacoes(const string& caracteres, const string& alvo, 
                     string& tentativa, int tamanhoMaximo,
                     unsigned long long& tentativas) {
    if (!tentativa.empty()) {
        ++tentativas;
        if (tentativa == alvo) return true;
    }
    if (tentativa.size() >= static_cast<size_t>(tamanhoMaximo)) return false;
    for (char caractere : caracteres) { 
        tentativa.push_back(caractere);     // Adiciona um caractere na tentativa
        if (gerarCombinacoes(caracteres, alvo, tentativa,
                            tamanhoMaximo, tentativas)) return true;   //Função para se encontrar o caractere certo
        tentativa.pop_back();   //Se não, a função tira o caractere e repete
    }
    return false;
}

// Entrada numérica; retorna falso no fim da entrada.
bool lerOpcao(const string& mensagem, int minimo, int maximo, int& resultado) {
    string entrada;         //Função de escolhas do menu em dígitos numéricos
    while (true) {
        cout << mensagem;
        if (!getline(cin, entrada)) return false;
        if (!entrada.empty() && entrada.size() <= 6) {
            bool apenasDigitos = true;
            for (unsigned char c : entrada) {
                if (!isdigit(c)) { apenasDigitos = false; break; }
            }
            if (apenasDigitos) {
                try {
                    int valor = stoi(entrada); //Transforma a string em int
                    if (valor >= minimo && valor <= maximo) {
                        resultado = valor;
                        return true;
                    }
                } catch (...) {}    // Try e Except do Python
            }
        }
        cout << "Opcao invalida. Digite um numero entre "
             << minimo << " e " << maximo << ".\n";
    }
}

string nomeCategorias(Categorias categoria) {   //Analisa a categoria recebida
    switch (categoria) {    //Switch Case nos Enums (veja avaliar_senha.cpp)
        case Categorias::LetrasSimbolos: return "Letras e simbolos";
        case Categorias::Numeros: return "Apenas numeros";
        case Categorias::Ambos: return "Letras, simbolos e numeros";
    }
    return "Desconhecida";
}

bool configurarSistema(Configuracao& config) {
    int tamanho = 0;
    int categoria = 0;
    cout << "\n-----Configuracoes-----\n";
    cout << "Tamanho atual: " << config.tamanho << " caracteres\n";
    cout << "Categorias atuais: " << nomeCategorias(config.categorias) << "\n\n";
    if (!lerOpcao("Novo tamanho de senha (4 a 128): ", 4, 128, tamanho)) return false;
    cout << "\n1 - Apenas letras e simbolos\n"
         << "2 - Apenas numeros\n"
         << "3 - Ambos (letras, simbolos e numeros)\n";
    if (!lerOpcao("Escolha as categorias: ", 1, 3, categoria)) return false;
    config.tamanho = static_cast<size_t>(tamanho);
    config.categorias = static_cast<Categorias>(categoria);
    cout << "\nConfiguracoes salvas para esta execucao.\n";
    cout << "Tamanho: " << config.tamanho << " | Categorias: "
         << nomeCategorias(config.categorias) << "\n";
    return true;
}

void demonstrarModelos(const string& senha) {
    const string caracteres = "abcdefghijklmnopqrstuvwxyz0123456789.";
    const int tamanhoMaximo = 3; // Limite reduzido para nao travar o menu.
    string nomeModelo;
    unsigned long long tentativas = 0;
    auto inicio = steady_clock::now();

    cout << "\n-----Teste de Modelos-----\n";
    if (consultarMatriz(senha, nomeModelo)) {
        cout << "Modelo encontrado: " << nomeModelo << "\n";
        cout << "Busca exaustiva nao necessaria.\n";
    } else if (senha.size() <= static_cast<size_t>(tamanhoMaximo)) {
        string tentativa;
        bool encontrada = gerarCombinacoes(caracteres, senha, tentativa,
                                          tamanhoMaximo, tentativas);
        cout << (encontrada ? "Candidato encontrado.\n" :
                            "Candidato nao encontrado nos modelos.\n");
        cout << "Tentativas realizadas: " << tentativas << "\n";
    } else {
        cout << "Nao consta na matriz. Busca ignorada "
             << "(limite: " << tamanhoMaximo << " caracteres).\n";
    }
    cout << "Tempo: " << duration<double>(steady_clock::now() - inicio).count()
         << " segundos\n";
}

void executarAvaliacao(const Configuracao& config, const string& senha) {
    demonstrarModelos(senha);
    cout << "\n-----Avaliacao da Senha Original-----\n";
    int pontuacao = avaliarSenha(senha);
    cout << "\nConfiguracao da sugestao: " << config.tamanho << " caracteres; "
         << nomeCategorias(config.categorias) << ".\n";
    if (pontuacao == 5) {
        cout << "A senha foi classificada como PODEROSA; nenhuma sugestao necessaria.\n";
        return;
    }

    cout << "\n-----Sugestao de Senha-----\n";
    string sugestao = sugerirSenha(senha, pontuacao, config);
    cout << "Sugestao: " << sugestao << "\n";
    cout << "\n-----Avaliacao da Sugestao-----\n";
    int novaPontuacao = avaliarSenha(sugestao);
    if (novaPontuacao < 5) {
        cout << "Observacao: com " << config.tamanho << " caracteres e as categorias "
             << "escolhidas, pode ser impossivel atingir 5/5. "
             << "Aumente o tamanho ou permita mais categorias.\n";
    }
}

int main() {
    Configuracao config; // Padrao: 8 caracteres, letras + simbolos + numeros.
    cout << "------------------------------\n"
         << "          TestaSenha\n"
         << "------------------------------\n";
    cout << "Senhas testadas NAO serao salvas.\n";

    while (true) {
        cout << "\n-----Menu Principal-----\n"
             << "1 - Inserir senha teste\n"
             << "2 - Ajustar configuracoes\n"
             << "3 - Sair\n"
             << "Configuracao atual: " << config.tamanho << " caracteres, "
             << nomeCategorias(config.categorias) << "\n";
        int opcao = 0;
        if (!lerOpcao("Escolha uma opcao: ", 1, 3, opcao)) break;
        if (opcao == 3) break;
        if (opcao == 2) {
            if (!configurarSistema(config)) break;
            continue;
        }

        while (true) {
            string senha;
            cout << "\nDigite uma senha para testar: ";
            if (!getline(cin, senha)) return 0;
            if (senha.empty()) {
                cout << "Erro: a senha nao pode estar vazia.\n";
                continue;
            }
            executarAvaliacao(config, senha);
            int repetir = 0;
            cout << "\n1 - Sim, testar outra senha\n2 - Nao, encerrar programa\n";
            if (!lerOpcao("Deseja inserir outra senha teste? ", 1, 2, repetir)) return 0;
            if (repetir == 2) {
                cout << "Encerrando TestaSenha.\n";
                return 0;
            }
            // Retorna ao menu principal, em vez de pedir outra senha diretamente.
            break;
        }
    }
    cout << "Encerrando TestaSenha.\n";
    return 0;
}

// g++ -std=c++17 main.cpp avaliar_senha.cpp -o TestaSenha.exe -> cria um .exe
// .\TestaSenha.exe -> executa o executável
 