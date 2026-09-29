import itertools
import string #ferramentas relacionadas a textos


def tipoSenha():
    ajustes = []

    tipoCaracteres = int(input('''Selecione o tipo de senha: 
1 - Letras e Símbolos + Números
2 - Apenas Letras e Símbolos
3 - Apenas Números

Tipo: '''))

    # caracteres que fazem parte da senha (letras maiusculas, minusculas e numeros)
    match tipoCaracteres:
        case 1:
            caracteres = string.ascii_letters + string.digits
        case 2:
            caracteres = string.ascii_letters
        case 3:
            caracteres = string.digits

    ajustes.append(tipoCaracteres)

    #limite de caracteres que o programa vai testar
    tamanho_maximo = int(input("Insira tamanho da senha: "))
    ajustes.append(tamanho_maximo)

    # recebe a senha
    senha = input("Digite a senha de teste: ")
    ajustes.append(senha)

    return ajustes


configs = tipoSenha()

# contador de tentativas
tentativas = 0

# loop que testa todas as combinacoes possiveis baseado nos caracteres definidos e no tamanho maximo
for tamanho in range(1, configs[1] + 1):

    # itertools.product() gera todas as combinacoes possiveis baseadas nos caracteres 
    # fornecidos e no tamanho especificado
    
# repeat=tamanho determina quantos caracteres tera cada combinacao
    for combinacao in itertools.product(configs[0], repeat=tamanho):

        # converte a tupla de caracteres em uma string
        tentativa = ''.join(combinacao)

        # conta mais uma tentativa
        tentativas += 1

        # mostra na tela a combinação que está sendo testada.
        print(f"Tentando: {tentativa}")

        # verifica se a combinação atual e igual a senha
        if tentativa == configs[2]:
            print("\nSenha encontrada!")
            print(f"Senha: {tentativa}")
            print(f"Total de tentativas: {tentativas}")

            # encerra o programa porque a senha foi encontrada
            exit()

# caso todas as combinações sejam testadas sem encontrar a senha
print("\nSenha não encontrada.")