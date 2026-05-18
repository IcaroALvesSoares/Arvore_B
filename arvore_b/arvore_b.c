#include <stdio.h>
#include <stdlib.h>
#include "arvore_b.h"

ArvoreBNode* criarNo(int folha) {
    ArvoreBNode *no = (ArvoreBNode*) malloc(sizeof(ArvoreBNode));

    no->folha = folha;
    no->n = 0;

    for (int i = 0; i < 2 * T; i++) {
        no->filhos[i] = NULL;
    }

    return no;
}

ChaveIndice* buscarArvore(ArvoreBNode *raiz, int codigo) {
    if (raiz == NULL) return NULL;

    int i = 0;

    while (i < raiz->n && codigo > raiz->chaves[i].codigo) {
        i++;
    }

    if (i < raiz->n && codigo == raiz->chaves[i].codigo) {
        return &raiz->chaves[i];
    }

    if (raiz->folha) {
        return NULL;
    }

    return buscarArvore(raiz->filhos[i], codigo);
}

static void dividirFilho(ArvoreBNode *pai, int i, ArvoreBNode *filhoCheio) {
    ArvoreBNode *novo = criarNo(filhoCheio->folha);
    novo->n = T - 1;

    for (int j = 0; j < T - 1; j++) {
        novo->chaves[j] = filhoCheio->chaves[j + T];
    }

    if (!filhoCheio->folha) {
        for (int j = 0; j < T; j++) {
            novo->filhos[j] = filhoCheio->filhos[j + T];
        }
    }

    filhoCheio->n = T - 1;

    for (int j = pai->n; j >= i + 1; j--) {
        pai->filhos[j + 1] = pai->filhos[j];
    }

    pai->filhos[i + 1] = novo;

    for (int j = pai->n - 1; j >= i; j--) {
        pai->chaves[j + 1] = pai->chaves[j];
    }

    pai->chaves[i] = filhoCheio->chaves[T - 1];
    pai->n++;
}

static void inserirNaoCheio(ArvoreBNode *no, ChaveIndice chave) {
    int i = no->n - 1;

    if (no->folha) {
        while (i >= 0 && chave.codigo < no->chaves[i].codigo) {
            no->chaves[i + 1] = no->chaves[i];
            i--;
        }

        no->chaves[i + 1] = chave;
        no->n++;
    } else {
        while (i >= 0 && chave.codigo < no->chaves[i].codigo) {
            i--;
        }

        i++;

        if (no->filhos[i]->n == 2 * T - 1) {
            dividirFilho(no, i, no->filhos[i]);

            if (chave.codigo > no->chaves[i].codigo) {
                i++;
            }
        }

        inserirNaoCheio(no->filhos[i], chave);
    }
}

void inserirArvore(ArvoreBNode **raiz, ChaveIndice chave) {
    if (*raiz == NULL) {
        *raiz = criarNo(1);
        (*raiz)->chaves[0] = chave;
        (*raiz)->n = 1;
        return;
    }

    if ((*raiz)->n == 2 * T - 1) {
        ArvoreBNode *novaRaiz = criarNo(0);
        novaRaiz->filhos[0] = *raiz;

        dividirFilho(novaRaiz, 0, *raiz);

        int i = 0;
        if (chave.codigo > novaRaiz->chaves[0].codigo) {
            i++;
        }

        inserirNaoCheio(novaRaiz->filhos[i], chave);
        *raiz = novaRaiz;
    } else {
        inserirNaoCheio(*raiz, chave);
    }
}

static int encontrarChave(ArvoreBNode *no, int codigo) {
    int idx = 0;

    while (idx < no->n && no->chaves[idx].codigo < codigo) {
        idx++;
    }

    return idx;
}

static ChaveIndice obterPredecessor(ArvoreBNode *no, int idx) {
    ArvoreBNode *atual = no->filhos[idx];

    while (!atual->folha) {
        atual = atual->filhos[atual->n];
    }

    return atual->chaves[atual->n - 1];
}

static ChaveIndice obterSucessor(ArvoreBNode *no, int idx) {
    ArvoreBNode *atual = no->filhos[idx + 1];

    while (!atual->folha) {
        atual = atual->filhos[0];
    }

    return atual->chaves[0];
}

static void removerDeFolha(ArvoreBNode *no, int idx) {
    for (int i = idx + 1; i < no->n; i++) {
        no->chaves[i - 1] = no->chaves[i];
    }

    no->n--;
}

static void merge(ArvoreBNode *no, int idx);

static void removerDeNo(ArvoreBNode *no, int codigo);

static void removerDeInterno(ArvoreBNode *no, int idx) {
    ChaveIndice chave = no->chaves[idx];

    if (no->filhos[idx]->n >= T) {
        ChaveIndice pred = obterPredecessor(no, idx);
        no->chaves[idx] = pred;
        removerDeNo(no->filhos[idx], pred.codigo);
    } else if (no->filhos[idx + 1]->n >= T) {
        ChaveIndice succ = obterSucessor(no, idx);
        no->chaves[idx] = succ;
        removerDeNo(no->filhos[idx + 1], succ.codigo);
    } else {
        merge(no, idx);
        removerDeNo(no->filhos[idx], chave.codigo);
    }
}

static void emprestarAnterior(ArvoreBNode *no, int idx) {
    ArvoreBNode *filho = no->filhos[idx];
    ArvoreBNode *irmao = no->filhos[idx - 1];

    for (int i = filho->n - 1; i >= 0; i--) {
        filho->chaves[i + 1] = filho->chaves[i];
    }

    if (!filho->folha) {
        for (int i = filho->n; i >= 0; i--) {
            filho->filhos[i + 1] = filho->filhos[i];
        }
    }

    filho->chaves[0] = no->chaves[idx - 1];

    if (!filho->folha) {
        filho->filhos[0] = irmao->filhos[irmao->n];
    }

    no->chaves[idx - 1] = irmao->chaves[irmao->n - 1];

    filho->n++;
    irmao->n--;
}

static void emprestarProximo(ArvoreBNode *no, int idx) {
    ArvoreBNode *filho = no->filhos[idx];
    ArvoreBNode *irmao = no->filhos[idx + 1];

    filho->chaves[filho->n] = no->chaves[idx];

    if (!filho->folha) {
        filho->filhos[filho->n + 1] = irmao->filhos[0];
    }

    no->chaves[idx] = irmao->chaves[0];

    for (int i = 1; i < irmao->n; i++) {
        irmao->chaves[i - 1] = irmao->chaves[i];
    }

    if (!irmao->folha) {
        for (int i = 1; i <= irmao->n; i++) {
            irmao->filhos[i - 1] = irmao->filhos[i];
        }
    }

    filho->n++;
    irmao->n--;
}

static void merge(ArvoreBNode *no, int idx) {
    ArvoreBNode *filho = no->filhos[idx];
    ArvoreBNode *irmao = no->filhos[idx + 1];

    filho->chaves[T - 1] = no->chaves[idx];

    for (int i = 0; i < irmao->n; i++) {
        filho->chaves[i + T] = irmao->chaves[i];
    }

    if (!filho->folha) {
        for (int i = 0; i <= irmao->n; i++) {
            filho->filhos[i + T] = irmao->filhos[i];
        }
    }

    for (int i = idx + 1; i < no->n; i++) {
        no->chaves[i - 1] = no->chaves[i];
    }

    for (int i = idx + 2; i <= no->n; i++) {
        no->filhos[i - 1] = no->filhos[i];
    }

    filho->n += irmao->n + 1;
    no->n--;

    free(irmao);
}

static void preencher(ArvoreBNode *no, int idx) {
    if (idx != 0 && no->filhos[idx - 1]->n >= T) {
        emprestarAnterior(no, idx);
    } else if (idx != no->n && no->filhos[idx + 1]->n >= T) {
        emprestarProximo(no, idx);
    } else {
        if (idx != no->n) {
            merge(no, idx);
        } else {
            merge(no, idx - 1);
        }
    }
}

static void removerDeNo(ArvoreBNode *no, int codigo) {
    int idx = encontrarChave(no, codigo);

    if (idx < no->n && no->chaves[idx].codigo == codigo) {
        if (no->folha) {
            removerDeFolha(no, idx);
        } else {
            removerDeInterno(no, idx);
        }
    } else {
        if (no->folha) {
            return;
        }

        int ultimaPosicao = (idx == no->n);

        if (no->filhos[idx]->n < T) {
            preencher(no, idx);
        }

        if (ultimaPosicao && idx > no->n) {
            removerDeNo(no->filhos[idx - 1], codigo);
        } else {
            removerDeNo(no->filhos[idx], codigo);
        }
    }
}

void removerArvore(ArvoreBNode **raiz, int codigo) {
    if (*raiz == NULL) {
        return;
    }

    removerDeNo(*raiz, codigo);

    if ((*raiz)->n == 0) {
        ArvoreBNode *temp = *raiz;

        if ((*raiz)->folha) {
            *raiz = NULL;
        } else {
            *raiz = (*raiz)->filhos[0];
        }

        free(temp);
    }
}

void percorrerArvore(ArvoreBNode *raiz, void (*callback)(ChaveIndice, void*), void *ctx) {
    if (raiz == NULL) return;

    int i;

    for (i = 0; i < raiz->n; i++) {
        if (!raiz->folha) {
            percorrerArvore(raiz->filhos[i], callback, ctx);
        }

        callback(raiz->chaves[i], ctx);
    }

    if (!raiz->folha) {
        percorrerArvore(raiz->filhos[i], callback, ctx);
    }
}

void liberarArvore(ArvoreBNode *raiz) {
    if (raiz == NULL) return;

    if (!raiz->folha) {
        for (int i = 0; i <= raiz->n; i++) {
            liberarArvore(raiz->filhos[i]);
        }
    }

    free(raiz);
}