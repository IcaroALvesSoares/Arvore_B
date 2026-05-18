#include <stdio.h>
#include <string.h>
#include "produto.h"

static void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void lerTexto(char *campo, int tamanho) {
    fgets(campo, tamanho, stdin);
    campo[strcspn(campo, "\n")] = '\0';
}

Produto lerProdutoDoTeclado() {
    Produto p;

    printf("Codigo: ");
    scanf("%d", &p.codigo);
    limparBuffer();

    printf("Nome: ");
    lerTexto(p.nome, 50);

    printf("Categoria: ");
    lerTexto(p.categoria, 30);

    printf("Marca: ");
    lerTexto(p.marca, 30);

    printf("Preco: ");
    scanf("%f", &p.preco);

    printf("Quantidade: ");
    scanf("%d", &p.quantidade);

    return p;
}

void imprimirProduto(Produto p) {
    printf("\nCodigo: %d\n", p.codigo);
    printf("Nome: %s\n", p.nome);
    printf("Categoria: %s\n", p.categoria);
    printf("Marca: %s\n", p.marca);
    printf("Preco: %.2f\n", p.preco);
    printf("Quantidade: %d\n", p.quantidade);
}