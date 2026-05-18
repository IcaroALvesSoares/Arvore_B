#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arquivo.h"

#define CAMINHO_DADOS "dados/dados.txt"
#define CAMINHO_INDICE "dados/indice.txt"

long salvarProdutoArquivo(Produto p) {
    FILE *arquivo = fopen(CAMINHO_DADOS, "a+");

    if (arquivo == NULL) {
        printf("Erro ao abrir dados.txt\n");
        return -1;
    }

    fseek(arquivo, 0, SEEK_END);
    long posicao = ftell(arquivo);

    fprintf(
        arquivo,
        "%d|%s|%s|%s|%.2f|%d\n",
        p.codigo,
        p.nome,
        p.categoria,
        p.marca,
        p.preco,
        p.quantidade
    );

    fclose(arquivo);
    return posicao;
}

int lerProdutoPorPosicao(long posicao, Produto *p) {
    FILE *arquivo = fopen(CAMINHO_DADOS, "r");

    if (arquivo == NULL) {
        return 0;
    }

    fseek(arquivo, posicao, SEEK_SET);

    char linha[300];

    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        fclose(arquivo);
        return 0;
    }

    sscanf(
        linha,
        "%d|%49[^|]|%29[^|]|%29[^|]|%f|%d",
        &p->codigo,
        p->nome,
        p->categoria,
        p->marca,
        &p->preco,
        &p->quantidade
    );

    fclose(arquivo);
    return 1;
}

void carregarIndiceDoArquivo(ArvoreBNode **raiz) {
    FILE *arquivo = fopen(CAMINHO_DADOS, "r");

    if (arquivo == NULL) {
        arquivo = fopen(CAMINHO_DADOS, "w");
        if (arquivo != NULL) fclose(arquivo);
        return;
    }

    Produto p;
    char linha[300];
    long posicao;

    while (1) {
        posicao = ftell(arquivo);

        if (fgets(linha, sizeof(linha), arquivo) == NULL) {
            break;
        }

        if (sscanf(
                linha,
                "%d|%49[^|]|%29[^|]|%29[^|]|%f|%d",
                &p.codigo,
                p.nome,
                p.categoria,
                p.marca,
                &p.preco,
                &p.quantidade
            ) == 6) {

            ChaveIndice chave;
            chave.codigo = p.codigo;
            chave.posicao = posicao;

            inserirArvore(raiz, chave);
        }
    }

    fclose(arquivo);
}

static void salvarChaveCallback(ChaveIndice chave, void *ctx) {
    FILE *arquivo = (FILE*) ctx;
    fprintf(arquivo, "%d|%ld\n", chave.codigo, chave.posicao);
}

void salvarIndiceArquivo(ArvoreBNode *raiz) {
    FILE *arquivo = fopen(CAMINHO_INDICE, "w");

    if (arquivo == NULL) {
        printf("Erro ao salvar indice.txt\n");
        return;
    }

    percorrerArvore(raiz, salvarChaveCallback, arquivo);

    fclose(arquivo);
}

int removerProdutoArquivo(int codigo) {
    FILE *original = fopen(CAMINHO_DADOS, "r");
    FILE *temp = fopen("dados/temp.txt", "w");

    if (original == NULL || temp == NULL) {
        printf("Erro ao abrir arquivos.\n");
        return 0;
    }

    Produto p;
    char linha[300];
    int removido = 0;

    while (fgets(linha, sizeof(linha), original) != NULL) {
        if (sscanf(
                linha,
                "%d|%49[^|]|%29[^|]|%29[^|]|%f|%d",
                &p.codigo,
                p.nome,
                p.categoria,
                p.marca,
                &p.preco,
                &p.quantidade
            ) == 6) {

            if (p.codigo == codigo) {
                removido = 1;
                continue;
            }
        }

        fputs(linha, temp);
    }

    fclose(original);
    fclose(temp);

    remove(CAMINHO_DADOS);
    rename("dados/temp.txt", CAMINHO_DADOS);

    return removido;
}

int atualizarProdutoArquivo(int codigo, Produto novoProduto) {
    FILE *original = fopen(CAMINHO_DADOS, "r");
    FILE *temp = fopen("dados/temp.txt", "w");

    if (original == NULL || temp == NULL) {
        printf("Erro ao abrir arquivos.\n");
        return 0;
    }

    Produto p;
    char linha[300];
    int atualizado = 0;

    while (fgets(linha, sizeof(linha), original) != NULL) {
        if (sscanf(
                linha,
                "%d|%49[^|]|%29[^|]|%29[^|]|%f|%d",
                &p.codigo,
                p.nome,
                p.categoria,
                p.marca,
                &p.preco,
                &p.quantidade
            ) == 6) {

            if (p.codigo == codigo) {
                fprintf(
                    temp,
                    "%d|%s|%s|%s|%.2f|%d\n",
                    novoProduto.codigo,
                    novoProduto.nome,
                    novoProduto.categoria,
                    novoProduto.marca,
                    novoProduto.preco,
                    novoProduto.quantidade
                );

                atualizado = 1;
                continue;
            }
        }

        fputs(linha, temp);
    }

    fclose(original);
    fclose(temp);

    remove(CAMINHO_DADOS);
    rename("dados/temp.txt", CAMINHO_DADOS);

    return atualizado;
}

static void listarProdutoCallback(ChaveIndice chave, void *ctx) {
    Produto p;

    if (lerProdutoPorPosicao(chave.posicao, &p)) {
        imprimirProduto(p);
        printf("-----------------------------\n");
    }
}

void listarProdutosOrdenados(ArvoreBNode *raiz) {
    if (raiz == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    percorrerArvore(raiz, listarProdutoCallback, NULL);
}