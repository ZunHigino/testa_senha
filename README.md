# testa_senha

## O que faz

O código visa a verificação da integridade de senhas. Você pode colocar senhas fictícias e vê o código tentar decifra-la a partir do método de força bruta, avaliando sua senha e sugerindo uma senha melhor ao final da verificação.

## Linguagem e bibliotecas

**C++**
  - algorithm
  - array
  - cctype
  - chrono
  - iostream
  - random
  - set
  - sstream
  - string
  - vector

## Pre-requesitos

- Compilador de C++

## Instalação e execução

Baixe o arquivo .zip do projeto, extraia-o e execute o executável com o compilador de c++ instalado.

## Como usar

Ao clicar no executável, uma janela CMD abrirá com o menu principal do programa. Para testar uma senha insira "1", para fazer ajustes no teste (mudar tamanho da senha e tipos de caracteres que serão usados) insira "2" e insira "3" para fechar a execução.

Ao fim da verificação, o programa dará uma nota de 0 a 5 para sua senha, caíndo em uma das cinco categorias: Extremamente fraca, Fraca, Moderada, Forte e Poderosa. Após isso, o programa lhe dará uma sugestão de senha e testa-la automaticamente.

No fim da sugestão, o código lhe dará as opções de recomeçar o sistema ou fecha-lo.
