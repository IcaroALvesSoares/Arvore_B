#ifndef ARVORE_B_H
#define ARVORE_B_H

#define T 2

/*
 * Estrutura que representa uma chave do índice primário.
 *
 * codigo:
 *  Chave primária do produto.
 *
 * posicao:
 *  Posição do registro no arquivo de dados.
 */
typedef struct {
    int codigo;
    long posicao;
} ChaveIndice;

/*
 * Estrutura de um nó da Árvore-B.
 *
 * n:
 *  Quantidade de chaves presentes no nó.
 *
 * folha:
 *  Indica se o nó é folha.
 *
 * chaves:
 *  Vetor contendo as chaves do nó.
 *
 * filhos:
 *  Vetor de ponteiros para os filhos.
 */
typedef struct ArvoreBNode {
    int n;
    int folha;

    ChaveIndice chaves[2 * T - 1];

    struct ArvoreBNode *filhos[2 * T];
} ArvoreBNode;

/*
 * Cria e inicializa um novo nó da Árvore-B.
 *
 * folha:
 *  1 -> nó folha
 *  0 -> nó interno
 *
 * Retorna:
 *  Ponteiro para o nó criado.
 */
ArvoreBNode* criarNo(int folha);

/*
 * Busca uma chave primária na Árvore-B.
 *
 * raiz:
 *  Ponteiro para a raiz da árvore.
 *
 * codigo:
 *  Código do produto procurado.
 *
 * Retorna:
 *  Ponteiro para a chave encontrada ou NULL.
 */
ChaveIndice* buscarArvore(ArvoreBNode *raiz, int codigo);

/*
 * Insere uma nova chave na Árvore-B.
 *
 * raiz:
 *  Ponteiro duplo para a raiz da árvore.
 *
 * chave:
 *  Estrutura contendo código e posição do registro.
 */
void inserirArvore(ArvoreBNode **raiz, ChaveIndice chave);

/*
 * Remove uma chave da Árvore-B.
 *
 * raiz:
 *  Ponteiro duplo para a raiz da árvore.
 *
 * codigo:
 *  Código do produto que será removido.
 */
void removerArvore(ArvoreBNode **raiz, int codigo);

/*
 * Percorre a árvore em ordem crescente.
 *
 * raiz:
 *  Ponteiro para a raiz da árvore.
 *
 * callback:
 *  Função executada para cada chave encontrada.
 *
 * ctx:
 *  Contexto auxiliar utilizado pelo callback.
 */
void percorrerArvore(
    ArvoreBNode *raiz,
    void (*callback)(ChaveIndice, void*),
    void *ctx
);

/*
 * Libera toda memória utilizada pela Árvore-B.
 *
 * raiz:
 *  Ponteiro para a raiz da árvore.
 */
void liberarArvore(ArvoreBNode *raiz);

#endif