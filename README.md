# Sistema de Estoque com Árvore-B

## Descrição ##

Este projeto consiste em um sistema de gerenciamento de estoque de uma empresa de eletrônicos, desenvolvido em linguagem C, utilizando arquivos e estrutura de dados Árvore-B como índice primário.

Os produtos são armazenados em um arquivo de dados `dados.txt`, enquanto os índices primários são organizados através de uma Árvore-B, permitindo acesso rápido aos registros utilizando a chave primária do produto.

---

# Objetivo do Projeto

O objetivo principal do projeto é demonstrar o funcionamento de:

- Organização de dados em arquivos;
- Índice primário;
- Estrutura de dados Árvore-B;
- Operações de busca eficientes;
- Inserção e remoção balanceadas;
- Manipulação de arquivos em linguagem C.

---

# Empresa Escolhida

A empresa escolhida foi uma loja de eletrônicos.

O sistema realiza o gerenciamento de produtos como:

- Smartphones;
- Notebooks;
- Monitores;
- Fones Bluetooth;
- Periféricos.

Cada produto possui:

- Código;
- Nome;
- Categoria;
- Marca;
- Preço;
- Quantidade.

---

# Estrutura do Projeto

```
ARVORE_B/
│
├── arquivo/
│   ├── arquivo.c
│   └── arquivo.h
│
├── arvore_b/
│   ├── arvore_b.c
│   └── arvore_b.h
│
├── build/
│
├── dados/
│   ├── dados.txt
│   └── indice.txt
│
├── produto/
│   ├── produto.c
│   └── produto.h
│
└── main.c
```

# Estrutura dos Dados

## Arquivo de Dados ##

O arquivo `dados.txt` armazena os registros completos dos produtos.

Exemplo:

```
1001|Notebook|Computador|Dell|4200.00|5
1002|Smartphone|Celular|Samsung|2500.00|10
1003|Mouse|Periferico|Logitech|120.00|20
```

Os campos possuem tamanhos fixos e variáveis:

| Campo      | Tipo     |
| ---------- | -------- |
| Código     | Fixo     |
| Nome       | Variável |
| Categoria  | Variável |
| Marca      | Variável |
| Preço      | Fixo     |
| Quantidade | Fixo     |

O sistema utiliza campos de tamanho fixo e variável. Os campos numéricos, como código, preço e quantidade, possuem tamanho previsível e são tratados como campos fixos. Já os campos textuais, como nome, categoria e marca, possuem tamanhos variáveis dependendo do conteúdo armazenado. Essa abordagem permite representar diferentes tipos de dados presentes no sistema de estoque.

## Índice Primário ##

O arquivo indice.txt armazena: codigo -> posição do registro no arquivo

Exemplo:

```
1001|0
1002|58
1003|120
```

A posição representa o byte inicial do registro dentro do arquivo de dados.

## Árvore-B ##

A Árvore-B é utilizada como estrutura principal de gerenciamento dos índices do sistema.

Ela é responsável por:

Inserção das chaves;
Busca das chaves;
Remoção das chaves;
Balanceamento da árvore;
Organização ordenada dos registros.

Foi utilizada uma Árvore-B com grau mínimo: t = 2

Características:

Mínimo de 1 chave por nó;
Máximo de 3 chaves por nó;
Máximo de 4 filhos por nó.

# Funcionamento Geral do Sistema

## Inicialização ##

Ao iniciar o sistema:

O arquivo dados.txt é lido;
Os índices são carregados;
A Árvore-B é reconstruída em memória;
O sistema fica pronto para operações.
Operações do Sistema
Inserção

Fluxo:

```
Usuário informa os dados do produto
↓
Produto é salvo no arquivo dados.txt
↓
Posição do registro é obtida com ftell()
↓
Código e posição são inseridos na Árvore-B
↓
Índice é atualizado
```

## Busca ##

Fluxo:

```
Usuário informa o código do produto
↓
Árvore-B localiza a chave
↓
Posição do registro é encontrada
↓
Sistema acessa diretamente o arquivo
↓
Produto é exibido
```

A busca ocorre através da chave primária: codigo do produto

## Alteração ##

Fluxo:

```
Usuário informa o código
↓
Árvore-B localiza o registro
↓
Arquivo é atualizado
↓
Índice é reconstruído
```

## Remoção ##

A remoção é gerenciada diretamente pela Árvore-B.

Fluxo:

```
Usuário informa o código
↓
Árvore-B remove a chave
↓
Árvore realiza balanceamento
↓
Registro é removido fisicamente do arquivo
↓
Índice é reconstruído
```

Durante a remoção podem ocorrer:

Redistribuição;
Concatenação (merge);
Rebalanceamento.

## Listagem Ordenada ##

Fluxo:

```
Percurso em ordem da Árvore-B
↓
Registros são acessados no arquivo
↓
Produtos são exibidos ordenados pelo código
```

# Estruturas Utilizadas

## Produto ##

```
typedef struct {
    int codigo;
    char nome[50];
    char categoria[30];
    char marca[30];
    float preco;
    int quantidade;
} Produto;
```

## Índice Primário ##

```
typedef struct {
    int codigo;
    long posicao;
} ChaveIndice;
```

## Nó da Árvore-B ##

```
#define T 2

typedef struct ArvoreBNode {
    int n;
    int folha;

    ChaveIndice chaves[2 * T - 1];

    struct ArvoreBNode *filhos[2 * T];

} ArvoreBNode;
```

## Bibliotecas Utilizadas ##

```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
```

# Compilação

- Linux/Mac: gcc main.c produto/produto.c arvore_b/arvore_b.c arquivo/arquivo.c -o build/programa

- Windows: gcc main.c produto/produto.c arvore_b/arvore_b.c arquivo/arquivo.c -o build/programa.exe

# Execução

- Linux/Mac: ./build/programa

- Windows: build\programa.exe
