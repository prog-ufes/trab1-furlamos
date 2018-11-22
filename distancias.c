#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define N 260

struct rotulo {
	float dist;
	float rotClass;
};

int leitura (float ***matBase, char *path, int *qntL, int *qntCol, float *rotMax);
float distancias (float *p, float *q, int n, float r, char tipo);
float classificador (int k, char tipo, float r, float *p, float **matTreino, int qntL, int qntCol, float rotMax);
void bubbleSort (struct rotulo *distancia, int n);
void confusao (int **matConfusa, float rotClass, float rotReal, float rotMax);
float acuracia (int **matConfusa, int qntRot);

int main () {
    int **matConfusa,
    	qntL, qntLTeste, qntCol, k;
	char pathTreino[N], pathTeste[N], pathSaida[N], novoPathSaida[N],
		 tipo;
    float **matTreino = NULL, **matTeste = NULL,
    	  *p,
          rotMax = 0, r, rotReal, acc;
	FILE *config, *saida;

	config = fopen ("config.txt", "r");

	if (config == NULL) {
    	printf ("Erro ao abrir o arquivo!\n");
      	fclose (config);
      	return (1);
   	}

  	else {
        fscanf (config, "%s\n", pathTreino);
        fscanf (config, "%s\n", pathTeste);
        fscanf (config, "%s\n", pathSaida);
    }

    if(! (leitura (&matTreino, pathTreino, &qntL, &qntCol, &rotMax))) {
        if(! (leitura (&matTeste, pathTeste, &qntLTeste, &qntCol, &rotMax))) {

        	float rotClass[qntLTeste];
            matConfusa = (int**) malloc (((int) rotMax) * sizeof (int*));
            for (int i = 0; i < (int) rotMax; i ++) matConfusa[i] = (int*) malloc (((int) rotMax) * sizeof (int));

            fscanf (config, "%d, %c, %f", &k, &tipo, &r);
            for (int s = 1; (! feof (config)); s++) {
                for (int i = 0; i < ((int) rotMax); i ++) {
                    for (int j = 0; j < ((int) rotMax); j ++) matConfusa[i][j] = 0;
                }

            	for (int i = 0; i < qntLTeste; i ++) {
            		p = matTeste[i];
            		rotReal = matTeste[i][qntCol - 1];
            		rotClass[i] = classificador (k, tipo, r, p, matTreino, qntL, qntCol, rotMax);
            		confusao (matConfusa, rotClass[i], rotReal, rotMax);
            	}
            	acc = acuracia (matConfusa, ((int) rotMax));


            	saida = fopen ("pathSaida", "w");
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
					fprintf (saida, "%.2f", rotClass[i]);
				}
				fclose (saida);

                fscanf (config, "%d, %c, %f", &k, &tipo, &r);
            }
            fclose (config);

        }
        for (int i = 0; i < qntLTeste; i++) free (matTeste[i]);
        free (matTeste);

    	for (int i = 0; i < (int) rotMax; i ++) free (matConfusa[i]);
    	free (matConfusa);
    }
    for (int i = 0; i < qntL; i++) free (matTreino[i]);
    free (matTreino);
    return 0;
}


int leitura (float ***matBase, char *path, int *qntL, int *qntCol, float *rotMax) {
    int b;
    char l;
    float *base;
    FILE *treiste;

    treiste = fopen (path, "r");

	if (treiste == NULL) {
	    printf ("Erro ao abrir o arquivo!\n");
	    fclose (treiste);
        return 1;
    }

    base = (float*) malloc (sizeof (float));
    (*qntL) = 0;
    for (b = 0; ! feof (treiste); b ++) {
        base = realloc (base, (b + 1) * sizeof (float));
    	fscanf (treiste, "%f%c", &base[b], &l);
        if (l == '\n') (*qntL) ++;
    }
    fclose (treiste);

    (*qntL) --;
    (*qntCol) = (b - 1) / (*qntL);

    (*matBase) = (float**) malloc ((*qntL) * sizeof (float*));

    for (int k = 0, i = -1, j = 0; k < (b - 1); k ++, j ++) {
		if (j % (*qntCol) == 0) {
			i ++;
			j = 0;
            (*matBase)[i] = (float*) malloc ((*qntCol) * sizeof (float));
			if (k != 0) {
                if ((*rotMax) < base[k - 1]) (*rotMax) = base[k - 1];
            }
		}
		(*matBase)[i][j] = base[k];
	}
    free (base);
    return 0;
}


float distancias (float *p, float *q, int n, float r, char tipo) {
	float dist = 0, mod = 0;
	switch (tipo) {
			case 'E':
				for (int i = 0; i < n; i++) {
					dist += pow (p[i] - q[i], 2);
				}
				dist = sqrt (dist);
			break;

			case 'M':
				for (int i = 0; i < n; i++) {
					mod = sqrt (pow ((p[i] - q[i]), 2));
					dist += pow (mod, r);
				}
				dist = pow(dist, 1/r);
			break;

			case 'C':
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

float classificador (int k, char tipo, float r, float *p, float **matTreino, int qntL, int qntCol, float rotMax) {
	float *q,
		  rotClass = 0, rotAtual;
	struct rotulo *distancia, contaRot[(int) rotMax];
	distancia = (struct rotulo*) malloc (qntL * sizeof (struct rotulo));
	for (int i = 0; i < (int) rotMax; i ++) contaRot[i].dist = 0;

	for (int i = 0; i < qntL; i ++) {
		q = matTreino[i];
		distancia[i].dist = distancias (p, q, (qntCol - 1), r, tipo);
		distancia[i].rotClass = q[qntCol - 1];
	}
	bubbleSort (distancia, (qntL));
	for (int i = 0; i < k; i ++) {
		rotAtual = 1.0;
		for (int j = 0; j < (int) rotMax; j ++) {
			if (distancia[i].rotClass == rotAtual) {
				contaRot[j].dist ++;
				contaRot[j].rotClass = rotAtual;
			}
			rotAtual ++;
		}
	}

	bubbleSort (contaRot, (int) rotMax);
	rotClass = contaRot[((int) rotMax) - 1].rotClass;
	free (distancia);
	return rotClass;
}


void bubbleSort (struct rotulo *distancia, int n) {
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

void confusao (int **matConfusa, float rotClass, float rotReal, float rotMax) {
	int m, n;
	for (float j = 1.0; j <= rotMax; j ++) {
		if (rotReal == j) {
			m = ((int) j) - 1;
		}
		if (rotClass == j) {
			n = ((int) j) - 1;
		}
	}
	matConfusa[m][n] ++;
}

float acuracia (int **matConfusa, int qntRot) {
	int acertos = 0, total = 0;
	float acc;
	for (int i = 0; i < qntRot; i ++) {
		acertos += matConfusa[i][i];
		for (int j = 0; j < qntRot; j ++) total += matConfusa[i][j];
	}
	acc = (float) acertos / (float) total;
	return acc;
}