import itertools
import string #ferramentas relacionadas a textos

# caracteres que fazem parte da senha (letras maiusculas, minusculas e numeros)
caracteres = string.ascii_letters + string.digits

# recebe a senha
senha = input("Digite a senha de teste: ")

#limite de caracteres que o programa vai testar
tamanho_maximo = 5

# contador de tentativas
tentativas = 0

# loop que testa todas as combinacoes possiveis baseado nos caracteres definidos e no tamanho maximo
for tamanho in range(1, tamanho_maximo + 1):

    # itertools.product() gera todas as combinacoes possiveis baseadas nos caracteres 
    # fornecidos e no tamanho especificado
    
# repeat=tamanho determina quantos caracteres tera cada combinacao
    for combinacao in itertools.product(caracteres, repeat=tamanho):

        # converte a tupla de caracteres em uma string
        tentativa = ''.join(combinacao)

        # conta mais uma tentativa
        tentativas += 1

        # mostra na tela a combinação que está sendo testada.
        print(f"Tentando: {tentativa}")

        # verifica se a combinação atual e igual a senha
        if tentativa == senha:
            print("\nSenha encontrada!")
            print(f"Senha: {tentativa}")
            print(f"Total de tentativas: {tentativas}")

            # encerra o programa porque a senha foi encontrada
            exit()

# caso todas as combinações sejam testadas sem encontrar a senha
print("\nSenha não encontrada.")