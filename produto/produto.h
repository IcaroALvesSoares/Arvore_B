#ifndef PRODUTO_H
#define PRODUTO_H

/*
 * Estrutura que representa um produto do estoque.
 *
 * codigo:
 *  Chave primária do produto.
 *
 * nome:
 *  Nome do produto.
 *
 * categoria:
 *  Categoria do produto.
 *
 * marca:
 *  Marca fabricante.
 *
 * preco:
 *  Preço do produto.
 *
 * quantidade:
 *  Quantidade disponível em estoque.
 */
typedef struct {
    int codigo;
    char nome[50];
    char categoria[30];
    char marca[30];
    float preco;
    int quantidade;
} Produto;

/*
 * Lê o codigo de um produto informado pelo usuário.
 *
 * Retorna:
 *  Codigo do produto.
 */
int lerCodigoProduto();

/*
 * Lê os dados de um produto informados pelo usuário.
 *
 * codigo:
 *  Codigo do produto.
 *
 * Retorna:
 *  Estrutura Produto preenchida.
 */
Produto lerDadosProdutoComCodigo(int codigo);

/*
 * Exibe os dados de um produto na tela.
 *
 * p:
 *  Produto que será exibido.
 */
void imprimirProduto(Produto p);

#endif