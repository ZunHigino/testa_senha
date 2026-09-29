import itertools
import string #ferramentas relacionadas a textos - A

# Função que determina as configurações (tipos de caracteres e tamanho) e a senha teste - H
def tipoSenha():

    # Lista que salvará as configurações e será retornada no final da função - H
    ajustes = []

    # Faz o usuário escolher quais caracteres serão testados - H
    tipoCaracteres = int(input('''Selecione o tipo de senha: 
1 - Letras e Símbolos + Números
2 - Apenas Letras e Símbolos
3 - Apenas Números

Tipo: '''))

    # Caracteres que farão parte da senha (letras maiusculas, minusculas e numeros) - A/H
    match tipoCaracteres:
        case 1:
            caracteres = string.ascii_letters + string.digits
        case 2:
            caracteres = string.ascii_letters
        case 3:
            caracteres = string.digits

    # Adiciona a configuração na lista - H
    ajustes.append(caracteres)

    #limite de caracteres que o programa vai testar - A
    tamanho_maximo = int(input("Insira tamanho da senha: "))
    ajustes.append(tamanho_maximo)

    # recebe a senha - A 
    senha = input("Digite a senha de teste: ")
    ajustes.append(senha)

    # A função salvará todas as configurações - H
    return ajustes

# Variável que recebe a função - H
configs = tipoSenha()

# configs[0] = caracteres
# configs[1] = tamanho_maximo
# configs[2] = senha

# contador de tentativas - A
tentativas = 0

# loop que testa todas as combinacoes possiveis baseado nos caracteres definidos e no tamanho maximo - A 
for tamanho in range(1, configs[1] + 1):

    # itertools.product() gera todas as combinacoes possiveis baseadas nos caracteres  
    # fornecidos e no tamanho especificado - A
    
# repeat=tamanho determina quantos caracteres tera cada combinacao - A
    for combinacao in itertools.product(configs[0], repeat=tamanho):

        # converte a tupla de caracteres em uma string - A
        tentativa = ''.join(combinacao)

        # conta mais uma tentativa - A
        tentativas += 1

        # mostra na tela a combinação que está sendo testada. - A
        print(f"Tentando: {tentativa}")

        # verifica se a combinação atual e igual a senha - A
        if tentativa == configs[2]:
            print("\nSenha encontrada!")
            print(f"Senha: {tentativa}")
            print(f"Total de tentativas: {tentativas}")

            # encerra o programa porque a senha foi encontrada - A
            exit()

# caso todas as combinações sejam testadas sem encontrar a senha - A
print("\nSenha não encontrada.")