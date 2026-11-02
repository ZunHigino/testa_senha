#ifndef AVALIAR_SENHA_H
#define AVALIAR_SENHA_H

#include <string>
#include <cstddef>

enum class Categorias { LetrasSimbolos = 1, Numeros = 2, Ambos = 3 };

struct Configuracao {
    std::size_t tamanho = 8;
    Categorias categorias = Categorias::Ambos;
};

int avaliarSenha(const std::string& senha, bool exibir = true);
std::string sugerirSenha(const std::string& senha, int pontuacao,
                         const Configuracao& config);

#endif
