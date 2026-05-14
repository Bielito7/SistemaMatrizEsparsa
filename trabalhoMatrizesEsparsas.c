#include <stdio.h>
#include <stdlib.h>

typedef struct nodo {
    float dado;
    int lin, col;
    struct nodo *prox;
} matriz;

typedef struct {
    matriz *dados; 
    int linhas;
    int colunas;
} MatrizEsparsa;

typedef struct dimensao {
    int linhas;
    int colunas;
} dimensao;


matriz *cria_nodo(){
    matriz *novo = (matriz *) malloc(sizeof(matriz));
    if(!novo) {
        printf("Erro na alocacao");
        exit(0);
    }
    return novo; 
} 

void liberar_lista(matriz *lista) {
    matriz *aux;
    while(lista != NULL) {
        aux = lista;
        lista = lista->prox;
        free(aux);
    }
}

float buscar_elemento(matriz *lista, int lin, int col) {        
    matriz *aux = lista;
    while(aux != NULL) {
        if(aux->lin == lin && aux->col == col) {
            return aux->dado;
        }
        aux = aux->prox;
    }
    return 0;
}

void insere_fim(matriz **lista, int lin, int col, float dado){
    // Verificar se a posição ja existe para atualizar o valor
    matriz *aux = *lista;
    while (aux != NULL) {
        if (aux->lin == lin && aux->col == col) {
            aux->dado = dado; // Atualiza o valor W
            return;
        }
        aux = aux->prox;
    }

    matriz *novo = cria_nodo();
    novo->lin = lin;
    novo->col = col;
    novo->dado = dado;
    novo->prox = NULL;
    
    if (*lista == NULL){
        *lista = novo;
    } else {
        matriz *aux = *lista;
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = novo;
    }
}

void imprimir_matriz(matriz *lista, int qntLinhas, int qntColunas){
    if (qntLinhas == 0) {
        printf("Matriz vazia ou tamanho nao definido.\n");
        return;
    }
    printf("\n--- MATRIZ (%dx%d) ---\n", qntLinhas, qntColunas);
    for(int lin = 1; lin <= qntLinhas; lin ++){
        for (int col = 1; col <= qntColunas; col++){
            float val = buscar_elemento(lista, lin, col);
            if (val == 0) printf("\t%8.2f", 0);
            else printf("\t%8.2f", val);
        }
        printf("\n");
    }
}

void somaMatrizes(MatrizEsparsa *A, MatrizEsparsa *B, MatrizEsparsa *R){
    liberar_lista(R->dados);
    R->dados = NULL;

    R->linhas = A->linhas;
    R->colunas = A->colunas;

    for (int i = 1; i <= A->linhas; i++) {
        for (int j = 1; j <= A->colunas; j++) {
            float valA = buscar_elemento(A->dados, i, j);
            float valB = buscar_elemento(B->dados, i, j);
            float soma = valA + valB;

            if (soma != 0) {
                insere_fim(&(R->dados), i, j, soma);
            }
        }
    }
    printf("\nSoma concluida com sucesso! Resultado salvo na Matriz 0\n");
}

void subtraiMatrizes(MatrizEsparsa *A, MatrizEsparsa *B, MatrizEsparsa *R) {
	liberar_lista(R->dados);
	R->dados = NULL;
	
	R->linhas = A->linhas;
	R->colunas = A->colunas;
	
	for(int i = 1; i <= A->linhas; i++){
		for(int j = 1; j <= A->colunas; j++) {
			float valA = buscar_elemento(A->dados,i,j);
			float valB = buscar_elemento(B->dados,i,j);
			float subtracao = valA - valB;
			
			if(subtracao != 0) {
				insere_fim(&(R->dados),i,j,subtracao);
			}
		}
	}
	printf("\nSubtracao concluida com sucesso! Resultado salvo na matriz 0.\n");
}

void multiplicaMatrizes(MatrizEsparsa *A, MatrizEsparsa *B, MatrizEsparsa *R) {
    liberar_lista(R->dados);
    R->dados = NULL;

    R->linhas = A->linhas;
    R->colunas = B->colunas;

    for (int i = 1; i <= A->linhas; i++) {
        for (int j = 1; j <= B->colunas; j++) {
            
            float soma_produtos = 0;
            
            // k percorre a coluna de A e a linha de B simultaneamente
            for (int k = 1; k <= A->colunas; k++) {              // k percorre ao mesmo tempo:
                float valA = buscar_elemento(A->dados, i, k);   // - a coluna da linha i de A (A[i][k])
                float valB = buscar_elemento(B->dados, k, j);   // - a linha da coluna j de B (B[k][j])
                soma_produtos += valA * valB;                  // isso permite calcular o produto escalar entre linha de A e coluna de B
            }

            if (soma_produtos != 0) {
                insere_fim(&(R->dados), i, j, soma_produtos);
            }
        }
    }
    printf("\nMultiplicacao concluida! Resultado salvo na matriz 0\n");
}

void gerar_transposta(MatrizEsparsa *A, MatrizEsparsa *R){
    liberar_lista(R->dados);
    R->dados = NULL;

    R->colunas = A ->linhas;
    R->linhas = A -> colunas;

    matriz *aux = A-> dados;
    while (aux != NULL){
        insere_fim(&(R->dados), aux->col, aux->lin, aux->dado);
        aux = aux ->prox;
    }

    printf("\nMatriz transposta gerada! Resultado salvo em Matriz 0\n");
}

void imprime_lista(matriz *lista){
    if(lista == NULL){
        printf("Lista vazia!\n");
        return;
    }
    matriz *aux = lista;
    while(aux != NULL){
        printf("[l:%d|c:%d|%.2f]%s", aux->lin, aux->col, aux->dado, aux->prox ? " --> " : "\n");
        aux = aux->prox;
    }
}

void imprimir_diagonal(MatrizEsparsa *m) {
    if (m->linhas == 0 || m->colunas == 0) {
        printf("Matriz vazia ou dimensao nao definida.\n");
        return;
    }

    printf("\n---DIAGONAL PRINCIPAL---\n");

    int i = 1;
    while (i <= m->linhas && i <= m->colunas) {
        float val = buscar_elemento(m->dados, i, i);
        printf("%.2f ", val);
        i++; 
        printf("\n");
    }
    
}

dimensao solicita_tamanho_matriz(){
    dimensao dimensao_usuario;
    do {
        printf("Digite o numero de linhas: ");
        scanf("%d", &dimensao_usuario.linhas);
        if(dimensao_usuario.linhas <= 0) printf("Erro: deve ser > 0\n");
    } while(dimensao_usuario.linhas <= 0);
    
    do {
        printf("Digite o numero de colunas: ");
        scanf("%d", &dimensao_usuario.colunas);
        if(dimensao_usuario.colunas <= 0) printf("Erro: deve ser > 0\n");
    } while(dimensao_usuario.colunas <= 0);
    
    return dimensao_usuario;
}


int main(){
    MatrizEsparsa m[30]; 
    int qntMatrizes = 0; 

    m[0].dados = NULL;
    m[0].linhas = 0;
    m[0].colunas = 0;
    
    int resposta = 0;

    printf("\n=== SISTEMA DE MATRIZ ESPARSA ===\n");
    while(resposta != 13){
        printf("\n--- MENU ---\n");
        printf("0 - criar nova matriz\n");
        printf("1 - determinar tamanho da matriz\n");
        printf("2 - adicionar valores diferentes de 0\n");
        printf("3 - exibir lista de elementos\n");
        printf("4 - buscar dado\n");
        printf("5 - liberar matriz\n");
        printf("6 - exibir matriz\n");
        printf("7 - ver RESULTADO (Matriz 0)\n");
        printf("8 - SOMAR matrizes\n");
        printf("9 - SUBTRAIR matrizes\n");
        printf("10 - gerar a matriz transposta\n");
        printf("11 - MULTIPLICAR matrizes\n");
        printf("12 - exibir diagonal principal\n");
        printf("13 - sair\n");
        printf("Matrizes criadas: %d\n", qntMatrizes);
        printf("Opcao: ");
        scanf("%d", &resposta);
        
        MatrizEsparsa *m_ativa = NULL;
        if (resposta >= 1 && resposta <= 6 || resposta == 10 || resposta == 12) {
            if(qntMatrizes < 1){
                printf("Crie uma matriz primeiro (Opcao 0).\n");
                continue;
            }
            int escolha;
            do {
                printf("Qual matriz (1 a %d): ", qntMatrizes);
                scanf("%d", &escolha);
                if(escolha > qntMatrizes){
                    printf("Ainda nao existe a Matriz %d!\n", escolha);
                }
            } while(escolha < 1 || escolha > qntMatrizes);
            m_ativa = &m[escolha];
        }

        switch (resposta){
            case 0:
                if (qntMatrizes < 29) {
                    qntMatrizes++;
                    m[qntMatrizes].dados = NULL;
                    m[qntMatrizes].linhas = 0;
                    m[qntMatrizes].colunas = 0;
                    printf("Matriz %d criada!\n", qntMatrizes);
                } else {
                    printf("Limite atingido\n");}
                break;

            case 1:
                {
                    dimensao dim = solicita_tamanho_matriz();
                    m_ativa->linhas = dim.linhas;
                    m_ativa->colunas = dim.colunas;
                }
                break;

            case 2:
                if (m_ativa->linhas == 0){
                    printf("Defina o tamanho primeiro\n");
                } else {
                    float valor;
                    int l, c;
                    do{
                        printf("\nValor (0 para sair): ");
                        scanf("%f", &valor);
                    
                    if(valor !=0){
                            printf("Linha e Coluna: ");
                            scanf("%d %d", &l, &c);
                            if(l > 0 && l <= m_ativa->linhas && c > 0 && c <= m_ativa->colunas){
                                insere_fim(&(m_ativa->dados), l, c, valor);
                                printf("%.2f adicionado\n", valor);
                            }
                            else printf("Coordenadas invalidas\n");
                    }
                    } while(valor != 0);
                }
                break;

            case 3: imprime_lista(m_ativa->dados); break;

            case 4: 
            {
                int l, c;
                printf("Digite a linha e coluna para busca: ");
                scanf("%d %d", &l, &c);
                if(l > 0 && l <= m_ativa->linhas && c > 0 && c <= m_ativa->colunas){
                    float v = buscar_elemento(m_ativa->dados, l, c);
                    printf("Valor na posicao [%d][%d]: %.2f\n", l, c, v);

                } else printf("Posicao excede o limite da matriz\n");
                
            }
            break;

            case 5: 
            liberar_lista(m_ativa->dados);
            m_ativa->dados = NULL;
            m_ativa->linhas = 0;
            m_ativa->colunas = 0;
            printf("Matriz limpa e dimensoes resetadas.\n");
            break;
            
            case 6: imprimir_matriz(m_ativa->dados, m_ativa->linhas, m_ativa->colunas); break;

            case 7: imprimir_matriz(m[0].dados, m[0].linhas, m[0].colunas); break;

            case 8:
                if (qntMatrizes < 2) {
                    printf("\nErro: Voce precisa de pelo menos 2 matrizes criadas para realizar uma soma.\n");
                } else {
                    int m1, m2;
                    
                    do {
                        printf("Numero da matriz A (1 a %d): ", qntMatrizes);
                        scanf("%d", &m1);
                        if (m1 < 1 || m1 > qntMatrizes) {
                            printf("Matriz A invalida! Tente novamente.\n");
                        }
                    } while (m1 < 1 || m1 > qntMatrizes);

                    printf("\nMatrizes disponiveis para somar com a Matriz %d (%dx%d):\n", m1, m[m1].linhas, m[m1].colunas);
                    int encontrou = 0;
                    for (int i = 1; i <= qntMatrizes; i++) {
                        if (m[i].linhas == m[m1].linhas && m[i].colunas == m[m1].colunas) {
                            printf("-> Matriz %d\n", i);
                            encontrou = 1;
                        }
                    }

                    if (!encontrou) {
                        printf("Nenhuma matriz encontrada com as mesmas dimensoes.\n");
                        break;
                    }

                    printf("\nNumero da matriz B: ");
                    scanf("%d", &m2);

                    if (m2 > 0 && m2 <= qntMatrizes && m[m1].linhas == m[m2].linhas && m[m1].colunas == m[m2].colunas) {
                        somaMatrizes(&m[m1], &m[m2], &m[0]);
                        imprimir_matriz(m[0].dados, m[0].linhas, m[0].colunas);
                    } else {
                        printf("\nErro: Matriz B invalida ou dimensoes incompativeis!\n");
                    }
                }
                break;
                
            case 9:
            	if(qntMatrizes < 2) {
            		printf("\nVoce precisa de pelo menos 2 matrizes criadas para realizar a subtracao!\n");
				} else{
					int m1,m2;
				
				do {
				printf("Numero da matriz A (1 a %d)",qntMatrizes);
				scanf("%d", &m1);
				if(m1 < 1 || m1 > qntMatrizes){
					printf("Matriz Invalida!Tente novamente.\n");
				}
			} while(m1 < 1 || m1 > qntMatrizes);
			
			printf("\nMatrizes disponiveis para subtrair com a Matriz %d (%dx%d):\n", m1, m[m1].linhas, m[m1].colunas);
                    int encontrou = 0;
                    for (int i = 1; i <= qntMatrizes; i++) {
                        if (m[i].linhas == m[m1].linhas && m[i].colunas == m[m1].colunas) {
                            printf("-> Matriz %d\n", i);
                            encontrou = 1;
                        }
                    }

                    if (!encontrou) {
                        printf("Nenhuma matriz encontrada com as mesmas dimensoes.\n");
                        break;
                    }

                    printf("\nNumero da matriz B: ");
                    scanf("%d", &m2);	
            		
            		if(m2 > 0 && m2 <= qntMatrizes && m[m1].colunas == m[m2].colunas && m[m1].linhas == m[m2].linhas) {
            			subtraiMatrizes(&m[m1],&m[m2],&m[0]);
            			imprimir_matriz(m[0].dados,m[0].linhas,m[0].colunas);
					} else {
						printf("Matriz B invalida ou dimensoes incompativeis!\n");
					}
            	}
            break;

            case 10: {
                if (qntMatrizes < 1) {
                    printf("Crie pelo menos uma matriz primeiro.\n");
                } else {
                    gerar_transposta(m_ativa, &m[0]);
                    imprimir_matriz(m[0].dados, m[0].linhas, m[0].colunas);
                }
                break;
            }

            case 11:{  	
                if(qntMatrizes < 2) {
                    printf("\nVoce precisa de pelo menos 2 matrizes criadas para realizar a multiplicacao!\n");
                } else {
                    int m1, m2;
                    
                    do {
                        printf("Numero da matriz A (1 a %d): ", qntMatrizes);
                        scanf("%d", &m1);
                        if(m1 < 1 || m1 > qntMatrizes){
                            printf("Matriz A invalida! Tente novamente.\n");
                        }
                    } while(m1 < 1 || m1 > qntMatrizes);
                    
                    printf("\nMatrizes disponiveis para multiplicar com a Matriz %d (%dx%d):\n",
                        m1, m[m1].linhas, m[m1].colunas);

                    int encontrou = 0;
                    for (int i = 1; i <= qntMatrizes; i++) {
                        if (m[m1].colunas == m[i].linhas) {   // REGRA DA MULTIPLICAÇÃO
                            printf("-> Matriz %d\n", i);
                            encontrou = 1;
                        }
                    }

                    if (!encontrou) {
                        printf("Nenhuma matriz encontrada com dimensoes compativeis.\n");
                        break;
                    }

                    printf("\nNumero da matriz B: ");
                    scanf("%d", &m2);

                    if(m2 > 0 && m2 <= qntMatrizes && m[m1].colunas == m[m2].linhas) {
                        multiplicaMatrizes(&m[m1], &m[m2], &m[0]);
                        imprimir_matriz(m[0].dados, m[0].linhas, m[0].colunas);

                    } else {
                        printf("Matriz B invalida ou dimensoes incompativeis!\n");
                    }
                }
                break;
            }

            case 12: {
                imprimir_diagonal(m_ativa);
                break;
            }

            case 13: printf("Saindo...\n"); break;
        }
    }

    for(int i = 0; i <= qntMatrizes; i++) {
        liberar_lista(m[i].dados);
    }

    return 0;
}