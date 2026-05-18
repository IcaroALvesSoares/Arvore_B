#include <stdio.h>
#include <stdlib.h>

#include "produto/produto.h"
#include "arvore_b/arvore_b.h"
#include "arquivo/arquivo.h"

/*
 * Reconstrói completamente o índice primário.
 *
 * A árvore atual é liberada da memória e
 * reconstruída utilizando os registros presentes
 * no arquivo de dados.
 */
void reconstruirIndice(ArvoreBNode **raiz) {
    liberarArvore(*raiz);
    *raiz = NULL;

    carregarIndiceDoArquivo(raiz);
    salvarIndiceArquivo(*raiz);
}

/*
 * Controla o fluxo de inserção de produtos.
 *
 * Etapas:
 *  1. Lê os dados do produto.
 *  2. Verifica duplicidade da chave primária.
 *  3. Salva no arquivo de dados.
 *  4. Insere a chave na Árvore-B.
 *  5. Atualiza o índice primário.
 */
void inserirProduto(ArvoreBNode **raiz) {
    int codigo = lerCodigoProduto();

    if (buscarArvore(*raiz, codigo) != NULL) {
        printf("Erro: ja existe um produto com esse codigo.\n");
        return;
    }

    Produto p = lerDadosProdutoComCodigo(codigo);

    long posicao = salvarProdutoArquivo(p);

    if (posicao == -1) {
        printf("Erro ao salvar produto.\n");
        return;
    }

    ChaveIndice chave;
    chave.codigo = p.codigo;
    chave.posicao = posicao;

    inserirArvore(raiz, chave);
    salvarIndiceArquivo(*raiz);

    printf("Produto inserido com sucesso.\n");
}

/*
 * Realiza busca de um produto utilizando
 * a Árvore-B como índice primário.
 *
 * Etapas:
 *  1. Busca a chave na árvore.
 *  2. Obtém a posição do registro.
 *  3. Acessa diretamente o arquivo.
 *  4. Exibe os dados do produto.
 */
void buscarProduto(ArvoreBNode *raiz) {
    int codigo;

    printf("Digite o codigo do produto: ");
    scanf("%d", &codigo);

    ChaveIndice *chave = buscarArvore(raiz, codigo);

    if (chave == NULL) {
        printf("Produto nao encontrado.\n");
        return;
    }

    Produto p;

    if (lerProdutoPorPosicao(chave->posicao, &p)) {
        imprimirProduto(p);
    } else {
        printf("Erro ao ler produto no arquivo.\n");
    }
}

/*
 * Controla a alteração de um produto.
 *
 * Etapas:
 *  1. Busca o produto pela chave primária.
 *  2. Atualiza os dados no arquivo.
 *  3. Reconstrói o índice primário.
 */
void modificarProduto(ArvoreBNode **raiz) {
    int codigo;

    printf("Digite o codigo do produto que deseja modificar: ");
    scanf("%d", &codigo);

    ChaveIndice *chave = buscarArvore(*raiz, codigo);

    if (chave == NULL) {
        printf("Produto nao encontrado.\n");
        return;
    }

    printf("\nDigite os novos dados do produto.\n");

    int novoCodigo = lerCodigoProduto();

    if (novoCodigo != codigo && buscarArvore(*raiz, novoCodigo) != NULL) {
        printf("Erro: o novo codigo ja existe.\n");
        return;
    }

    Produto novo = lerDadosProdutoComCodigo(novoCodigo);

    if (atualizarProdutoArquivo(codigo, novo)) {
        if (novo.codigo != codigo) {
            removerArvore(raiz, codigo);
        }

        reconstruirIndice(raiz);

        printf("Produto modificado com sucesso.\n");
    } else {
        printf("Erro ao modificar produto.\n");
    }
}

/*
 * Controla a remoção de um produto.
 *
 * Etapas:
 *  1. Remove a chave da Árvore-B.
 *  2. Reorganiza a árvore.
 *  3. Remove fisicamente do arquivo.
 *  4. Reconstrói o índice primário.
 */
void removerProduto(ArvoreBNode **raiz) {
    int codigo;

    printf("Digite o codigo do produto que deseja remover: ");
    scanf("%d", &codigo);

    ChaveIndice *chave = buscarArvore(*raiz, codigo);

    if (chave == NULL) {
        printf("Produto nao encontrado.\n");
        return;
    }

    removerArvore(raiz, codigo);

    if (removerProdutoArquivo(codigo)) {
        reconstruirIndice(raiz);
        printf("Produto removido com sucesso.\n");
    } else {
        printf("Erro ao remover produto do arquivo.\n");
    }
}

/*
 * Pausa a execução até o usuário pressionar ENTER.
 */
void pausarTela() {
    printf("\nPressione ENTER para continuar...");
    
    int c;

    // limpa buffer restante
    while ((c = getchar()) != '\n' && c != EOF);

    getchar();
}

/*
 * Exibe o menu principal do sistema.
 */
void menu() {
    printf("\n===== ESTOQUE DE ELETRONICOS =====\n");
    printf("1 - Inserir produto\n");
    printf("2 - Buscar produto\n");
    printf("3 - Modificar produto\n");
    printf("4 - Remover produto\n");
    printf("5 - Listar produtos ordenados\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
}

int main() {
    ArvoreBNode *raiz = NULL;
    int opcao;

    carregarIndiceDoArquivo(&raiz);
    salvarIndiceArquivo(raiz);

    do {
        menu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                inserirProduto(&raiz);
                pausarTela();
                break;

            case 2:
                buscarProduto(raiz);
                pausarTela();
                break;

            case 3:
                modificarProduto(&raiz);
                pausarTela();
                break;

            case 4:
                removerProduto(&raiz);
                pausarTela();
                break;

            case 5:
                listarProdutosOrdenados(raiz);
                pausarTela();
                break;

            case 0:
                printf("Encerrando...\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    salvarIndiceArquivo(raiz);
    liberarArvore(raiz);

    return 0;
}