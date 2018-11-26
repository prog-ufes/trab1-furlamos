// TRABALHO DE PROGRAMACAO II: Classificação de dados utilizando um KNN
// LEANDRO FURLAM TURI
// 22 de novembro de 2018

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct rotulo {
	float dist;
	float rotClass;
};

int leituraPath (FILE *config, char **path);																			// LEITURA DOS CAMINHOS PARA OS ARQUIVOS DE SAIDA, TREINO E TESTE
int leitura (float ***matBase, char *path, int *qntL, int *qntCol, float *rotMax);										// LEITURA DOS COMPONENTES DE TREINO E TESTE
float distancias (float *p, float *q, int n, float r, char tipo);														// CALCULO DAS DISTANCIAS
float classificador (int k, char tipo, float r, float *p, float **matTreino, int qntL, int qntCol, float rotMax);		// CLASSIFICADOR KNN
void bubbleSort (struct rotulo *distancia, int n);																		// ORDENACAO DA ESTRUTURA CRIADA
void confusao (int **matConfusa, float rotClass, float rotReal, float rotMax);											// MATRIZ DE CONFUSAO
float acuracia (int **matConfusa, int qntRot);																			// CALCULO DA ACURACIA

int main () {
    int **matConfusa,
    	nPath, qntL, qntLTeste, qntCol, k, retorno;
	char *pathTreino, *pathTeste, *pathSaida, *novoPathSaida,
		 tipo;
    float **matTreino, **matTeste,
    	  *p,
          rotMax, r, rotReal, acc;
	FILE *config, *saida;

	config = fopen ("config.txt", "r");

	if (config == NULL) {
    	printf ("Erro ao abrir o arquivo!\n");
      	return (1);
   	}

  	nPath = (leituraPath (config, &pathTreino));
	if (nPath == 0) return 1;
	nPath = (leituraPath (config, &pathTeste));
	if (nPath == 0) return 1;
	nPath = (leituraPath (config, &pathSaida));
	if (nPath == 0) return 1;
	retorno = 0;
	rotMax = 0;
    if(! (leitura (&matTreino, pathTreino, &qntL, &qntCol, &rotMax))) {
        if(! (leitura (&matTeste, pathTeste, &qntLTeste, &qntCol, &rotMax))) {

        	float rotClass[qntLTeste];
            matConfusa = (int**) malloc (((int) rotMax) * sizeof (int*));
            for (int i = 0; i < (int) rotMax; i ++) matConfusa[i] = (int*) malloc (((int) rotMax) * sizeof (int));

            fscanf (config, "%d, %c, %f", &k, &tipo, &r);																// LEITURA DOS PARAMETROS DO CLASSIFICADOR
            for (int s = 1; ((! feof (config)) && (retorno != 1)); s ++) {
                for (int i = 0; i < ((int) rotMax); i ++) {
                    for (int j = 0; j < ((int) rotMax); j ++) matConfusa[i][j] = 0;										// ZERAR OS ELEMENTOS DA MATRIZ DE CONFUSAO
                }

            	for (int i = 0; i < qntLTeste; i ++) {
            		p = matTeste[i];																					// OBTENCAO DE UM VETOR DE TESTE
            		rotReal = matTeste[i][qntCol - 1];
            		rotClass[i] = classificador (k, tipo, r, p, matTreino, qntL, qntCol, rotMax);						// CLASSIFICACAO (COM CALCULO DA DISTANCIA DEVIDA)
            		confusao (matConfusa, rotClass[i], rotReal, rotMax);												// PREENCHIMENTO DA MATRIZ DE CONFUSAO
            	}
            	acc = acuracia (matConfusa, ((int) rotMax));															// CALCULO DA ACURACIA

				novoPathSaida = (char*) malloc ((nPath + 16 + s) * sizeof (char));
				sprintf(novoPathSaida, "%spredicao_%d.txt", pathSaida, s);
            	saida = fopen (novoPathSaida, "w"); 																	// INICIO DA ESCRITA NO ARQUIVO DE SAIDA
            	if (saida == NULL) {
            		printf ("Erro ao abrir o arquivo de saida!\n");														// CASO O CAMINHO NAO EXISTA
            		free (novoPathSaida);
      				retorno = 1;
            	}
            	else {
	            	fprintf (saida, "%.2f\n\n", acc);
	            	for (int i = 0; i < (int) rotMax; i ++) {
	            		for (int j = 0; j < (int) rotMax; j ++) {
	            			if (j != 0) fprintf (saida, " ");
	            			fprintf (saida, "%d", matConfusa[i][j]);
	            		}
	            		fprintf (saida, "\n");
	            	}
	            	fprintf (saida, "\n");
					for (int i = 0; i < qntLTeste; i ++) {
						if (i != 0) fprintf (saida, "\n");
						fprintf (saida, "%d", (int) (rotClass[i] - 1));
					}
					fclose (saida);																						// TERMINO DA ESCRITA NO ARQUIVO DE SAIDA
					free (novoPathSaida);

	                fscanf (config, "%d, %c, %f", &k, &tipo, &r);
            	}
            }

			for (int i = 0; i < qntLTeste; i++) free (matTeste[i]);
	        free (matTeste);

	    	for (int i = 0; i < (int) rotMax; i ++) free (matConfusa[i]);
	    	free (matConfusa);
        }
		for (int i = 0; i < qntL; i++) free (matTreino[i]);
	    free (matTreino);
    }
    fclose (config);
	free (pathTreino);
	free (pathTeste);
	free (pathSaida);

    return (retorno);
}

int leituraPath (FILE *config, char **path) {																			// LEITURA DINAMICA DOS CAMINHOS DOS AQUIVOS
	int n = 0;
	*path = (char*) malloc (sizeof (char));
	fscanf (config, "%c", &(*path)[0]);
	if ((*path)[0] == '\n') {
		printf("Caminho desconhecido!\n");
		return 0;																										// RETORNA 0 EM CASO DE FALHAS, OU A QUANTIDADE DE CARACTERES, REPRESENTANDO SUCESSO
	}

	for (n = 1; (*path)[n - 1] != '\n'; n ++) {
		*path = realloc (*path, (n + 1) * sizeof (char));
		fscanf (config, "%c", &(*path)[n]);
	};
	(*path)[n - 1] = '\0';
	return n;
}

int leitura (float ***matBase, char *path, int *qntL, int *qntCol, float *rotMax) {										// LEITURA DINAMICA DOS COMPONENTES DE TREINO E TESTE
    int b;
    char l;
    float *base;
    FILE *treiste;

    treiste = fopen (path, "r");

	if (treiste == NULL) {
	    printf ("Erro ao abrir o arquivo!\n");
        return 1;																										// RETORNA 1 EM CASO DE FALHAS, OU 0 EM CASO DE SUCESSO
    }

    base = (float*) malloc (sizeof (float));
    (*qntL) = 0;
    for (b = 0; ! feof (treiste); b ++) {
        base = realloc (base, (b + 1) * sizeof (float));
    	fscanf (treiste, "%f%c", &base[b], &l);																			// ARMAZENAMENTO DOS ELEMENTOS EM UM VETOR
        if (l == '\n') (*qntL) ++;
    }
    fclose (treiste);

    (*qntL) --;
    (*qntCol) = (b - 1) / (*qntL);

    (*matBase) = (float**) malloc ((*qntL) * sizeof (float*));

    for (int k = 0, i = -1, j = 0; k < (b - 1); k ++, j ++) {															// CONVERTER O VETOR DE DADOS EM UMA MATRIZ
		if (j % (*qntCol) == 0) {
			i ++;
			j = 0;
            (*matBase)[i] = (float*) malloc ((*qntCol) * sizeof (float));
			if (k != 0) {
                if ((*rotMax) < base[k - 1]) (*rotMax) = base[k - 1];
            }
		}
		(*matBase)[i][j] = base[k];																						// MATRIZ CONTENDO A BASE DE DADOS
	}
    free (base);
    return 0;
}


float distancias (float *p, float *q, int n, float r, char tipo) {														// CALCULO DAS DISTANCIAS
	float dist = 0, mod = 0;
	switch (tipo) {
			case 'E':																									// DISTANCIA Euclidiana
				for (int i = 0; i < n; i++) {
					dist += pow (p[i] - q[i], 2);
				}
				dist = sqrt (dist);
			break;

			case 'M':																									// DISTANCIA DE Minkowsky
				if (r <= 0) r = 2.0;																					// CASO O RAIO NAO SEJA INTEIRO, CALCULAREMOS A DISTANCIA EUCLIDIANA
				for (int i = 0; i < n; i++) {																			// FOI ESCOLHIDA A DISTANCIA EUCLIDIANA PELO FATO DE QUE PARA SER DISTANCIA DE Chebyshev, ERA NECESSARIO O RAIO SER INFINITO.
					mod = sqrt (pow ((p[i] - q[i]), 2));																// IMPLEMENTACAO DA FUNCAO VALOR ABSOLUTO
					dist += pow (mod, r);
				}
				dist = pow(dist, 1/r);
			break;

			case 'C':																									// DISTANCIA DE Chebyshev
				for (int i = 0; i < n; i++) {
					if ((pow ((p[i] - q[i]), 2)) > dist) {
						dist = pow ((p[i] - q[i]), 2);
					}
				}
				dist = sqrt (dist);
			break;

			default:
				printf("Distancia desconhecida!\n");
				exit (1);
		}
	return dist;
}

float classificador (int k, char tipo, float r, float *p, float **matTreino, int qntL, int qntCol, float rotMax) {		// CLASSIFICADOR KNN
	float *q,
		  rotClass = 0, rotAtual;
	struct rotulo *distancia, contaRot[(int) rotMax];
	distancia = (struct rotulo*) malloc (qntL * sizeof (struct rotulo));
	for (int i = 0; i < (int) rotMax; i ++) contaRot[i].dist = 0;

	for (int i = 0; i < qntL; i ++) {																					// PERCORRE TODA A BASE DE TREINO, COMPARANDO COM O VETOR P DADO
		q = matTreino[i];
		distancia[i].dist = distancias (p, q, (qntCol - 1), r, tipo);													// ESTRUTURA CONTENDO A DISTANCIA E O ROTULO
		distancia[i].rotClass = q[qntCol - 1];
	}
	bubbleSort (distancia, (qntL));

	for (int i = 0; i < k; i ++) { 																						// ORDENACAO DAS DISTANCIAS, POR ORDEM CRESCENTE
		rotAtual = 1.0;
		for (int j = 0; j < (int) rotMax; j ++) {																		// CONTAGEM DOS ROTULOS CORRESPONDENTES AS K MENORES DISTANCIAS
			if (distancia[i].rotClass == rotAtual) {
				contaRot[j].dist ++;																					// CONTAGEM ARMAZENADA NA MESMA ESTRUTURA
			}
			contaRot[j].rotClass = rotAtual;
			rotAtual ++;
		}
	}
	bubbleSort (contaRot, (int) rotMax);																				// ORDENACAO DA CONTAGEM DE ROTULOS
	rotClass = contaRot[((int) rotMax) - 1].rotClass;																	// OBTENCAO DO ROTULO
	for (int i = (((int) rotMax) - 1); i > 0; i --) {
		if (contaRot[i - 1].dist == contaRot[i].dist) rotClass = contaRot[i - 1].rotClass;								// CONDICIONAL PARA PEGAR O MENOR ROTULO
		else break;
	}
	free (distancia);
	return rotClass;
}


void bubbleSort (struct rotulo *distancia, int n) {																		// ORDENACAO DA ESTRUTURA distancia, PELO PARAMETRO dist
	struct rotulo aux;
	for (int i = 0; i < n; i ++) {
		for (int j = i + 1; j < n; j ++) {
			if (distancia[j].dist < distancia[i].dist) {
				aux = distancia[i];
                distancia[i] = distancia[j];
                distancia[j] = aux;
			}
		}
	}
}

void confusao (int **matConfusa, float rotClass, float rotReal, float rotMax) {											// 	IMPLEMENTACAO DA MATRIZ DE CONFUSAO
	int m, n;
	for (float j = 1.0; j <= rotMax; j ++) {																			// COMPARACAO DOS ROTULOS
		if (rotReal == j) {
			m = ((int) j) - 1;
		}
		if (rotClass == j) {
			n = ((int) j) - 1;
		}
	}
	matConfusa[m][n] ++;
}

float acuracia (int **matConfusa, int qntRot) {																			// CALCULO DA ACURACIA
	int acertos = 0, total = 0;
	float acc;
	for (int i = 0; i < qntRot; i ++) {
		acertos += matConfusa[i][i];
		for (int j = 0; j < qntRot; j ++) total += matConfusa[i][j];
	}
	acc = ((float) acertos / (float) total) * 100;
	return acc;
}
