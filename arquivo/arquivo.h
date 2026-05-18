#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "../produto/produto.h"
#include "../arvore_b/arvore_b.h"

/*
 * Salva um produto no arquivo de dados.
 *
 * p:
 *  Produto que será armazenado.
 *
 * Retorna:
 *  Posição em bytes onde o produto foi salvo.
 */
long salvarProdutoArquivo(Produto p);

/*
 * Lê um produto diretamente do arquivo usando
 * sua posição em bytes.
 *
 * posicao:
 *  Localização do registro no arquivo.
 *
 * p:
 *  Ponteiro onde os dados serão armazenados.
 *
 * Retorna:
 *  1 -> sucesso
 *  0 -> erro
 */
int lerProdutoPorPosicao(long posicao, Produto *p);

/*
 * Carrega os registros do arquivo de dados
 * e reconstrói a Árvore-B em memória.
 *
 * raiz:
 *  Ponteiro duplo para a raiz da árvore.
 */
void carregarIndiceDoArquivo(ArvoreBNode **raiz);

/*
 * Salva o índice primário no arquivo indice.txt.
 *
 * raiz:
 *  Ponteiro para a raiz da árvore.
 */
void salvarIndiceArquivo(ArvoreBNode *raiz);

/*
 * Remove fisicamente um produto do arquivo de dados.
 *
 * codigo:
 *  Código do produto removido.
 *
 * Retorna:
 *  1 -> sucesso
 *  0 -> erro
 */
int removerProdutoArquivo(int codigo);

/*
 * Atualiza os dados de um produto no arquivo.
 *
 * codigo:
 *  Código do produto original.
 *
 * novoProduto:
 *  Estrutura contendo os novos dados.
 *
 * Retorna:
 *  1 -> sucesso
 *  0 -> erro
 */
int atualizarProdutoArquivo(int codigo, Produto novoProduto);

/*
 * Lista todos os produtos em ordem crescente
 * da chave primária utilizando percurso
 * em ordem da Árvore-B.
 *
 * raiz:
 *  Ponteiro para a raiz da árvore.
 */
void listarProdutosOrdenados(ArvoreBNode *raiz);

#endif