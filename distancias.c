#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 260

int leitura (int k, char tipo, float r, char *path, float ***matBase, int *qntL, int *qntCol);
float *rotulacao (float **matBase, int qntL, int qntCol, int *qntRot);
float distancias (float *p, float *q, int n, float r, char tipo);
int busca (float *vet, int n, float k);
void bubbleSort (float *vet, int *pos, int n);
int maior (int *vet, int n);
float classificador (int k, char tipo, float r, float *p, float **matBase, int qntCol, int qntL, float *rotulos, int qntRot);
void confusao (int **matConfusa, float rotClass, float rotReal, float *rotulos, int qntRot);
float acuracia (int **matConfusa, int qntRot);

int main () {
	int **matConfusa,
	    qntL = 0, qntLTeste = 0, qntCol = 0, qntRot = 0,
		k, flagRot = 0, acc;
	float ***matTreino, ***matTeste,
		  *rotulos, *p,
		  r, rotReal, rotClass;
	char *pathTreino, *pathTeste, *pathSaida,
		 tipo;
	FILE *config;

	config = fopen ("config.txt", "r");

	if(config == NULL) {
    	printf ("Erro ao abrir o arquivo!\n");
      	fclose (config);
      	exit(1);
   	}

  	else {
  		pathTreino = (char*) malloc (N * sizeof (char));
		pathTeste = (char*) malloc (N * sizeof (char));
		pathSaida = (char*) malloc (N * sizeof (char));

		fgets (pathTreino, N, config);
		fgets (pathTeste, N, config);
		fgets (pathSaida, N, config);

		if ((*pathTreino == '\n') || (*pathTeste == '\n') || (*pathSaida == '\n')) {
			printf ("Arquivo incorreto!\n");
			exit(1);
		}

		for (int s = 1; ! feof (config); s++) { //ESTA FAZENDO O FINAL DUAS VEZES
			fscanf (config, "%d, %c, %f", &k, &tipo, &r);

			matTreino = (float***) malloc (sizeof (float**));
			if (! (leitura (k, tipo, r, pathTreino, matTreino, &qntL, &qntCol))) {
				if (flagRot == 0) {
					rotulos = rotulacao (*matTreino, qntL, qntCol, &qntRot);
					flagRot = 1;
				}

				matConfusa = (int**) malloc (qntRot * sizeof (int*));
				for (int i = 0; i < qntRot; i ++) matConfusa[i] = (int*) malloc (qntRot * sizeof (int));

				for (int i = 0; i < qntL; i ++) {
					p = matTreino[0][i];
					rotReal = matTreino[0][i][qntCol - 1];
					rotClass = classificador (k, tipo, r, p, *matTreino, qntCol, qntL, rotulos, qntRot);
					confusao (matConfusa, rotClass, rotReal, rotulos, qntRot);
				}

				matTeste = (float***) malloc (sizeof (float**));
				if (! (leitura (k, tipo, r, pathTeste, matTeste, &qntLTeste, &qntCol))) {
					for (int i = 0; i < qntLTeste; i ++) {
						p = matTeste[0][i];
						rotReal = matTeste[0][i][qntCol - 1];
						rotClass = classificador (k, tipo, r, p, *matTreino, qntCol, qntL, rotulos, qntRot);
						confusao (matConfusa, rotClass, rotReal, rotulos, qntRot);
					}
				}
			}
			acc = acuracia (matConfusa, qntRot);
			
		}
	}
	fclose (config);
	return 0;
}

int leitura (int k, char tipo, float r, char *path, float ***matBase, int *qntL, int *qntCol) {
	int qntB = 0;
    char linha;
    float *base;
	FILE *treiste;

	treiste = fopen ("iris_treino.csv", "r");

	if (treiste == NULL) {
	    printf ("Erro ao abrir o arquivo!\n");
	    fclose (treiste);
	    return (1);
    }
  	base = (float*) malloc (sizeof (float));
    for (qntB = 0; ! feof (treiste); qntB++) {
    	base = realloc (base, (qntB + 1) * sizeof (float));
		fscanf (treiste, "%f%c", &base[qntB], &linha);
        if (linha == '\n') (*qntL) ++;
    }
	fclose (treiste);

	(*qntL) --;
    (*qntCol) = (qntB - 1) / (*qntL);
	matBase[0] = (float**) malloc (*qntL * (sizeof (float*)));

	for (int k = 0, i = -1, j = 0; k < (qntB - 1); k ++, j ++) {
		if (j % *qntCol == 0) {
			i ++;
			j = 0;
			matBase[0][i] = (float*) malloc (*qntCol * (sizeof (float)));
		}
		matBase[0][i][j] = base[k];
	}
  	free (base);
  	return 0;
}

float *rotulacao (float **matBase, int qntL, int qntCol, int *qntRot) {
	float *rotulos;
	rotulos = (float*) malloc (sizeof (float));
	rotulos[0] = matBase[0][qntCol - 1];
	(*qntRot) = 1;

	for (int i = 1; i < qntL; i ++) {
		if (busca (rotulos, *qntRot, matBase[i][qntCol - 1])) {
			(*qntRot) ++;
			rotulos = realloc (rotulos, *qntRot * sizeof (float));
			rotulos[(*qntRot) - 1] = matBase[i][qntCol - 1];
		}
	}
	return rotulos;
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

float classificador (int k, char tipo, float r, float *p, float **matBase, int qntCol, int qntL, float *rotulos, int qntRot) {
	int *pos, *contaRot,
	    posClass;
	float *q, *dist,
		  rotuloClass;

	pos = (int*) malloc (sizeof (int));
	dist = (float*) malloc (sizeof (float));

	for (int i = 0; i < qntL; i ++) {
		q = matBase[i];
		pos = realloc (pos, (i + 1) * sizeof (int));
		pos[i] = i;
		dist = realloc (dist, (i + 1) * sizeof (float));
		dist[i] = distancias (p, q, (qntCol - 1), r, tipo);
	}
	bubbleSort (dist, pos, qntL);
	contaRot = (int*) malloc (qntRot * sizeof (int));
	for (int i = 0; i < qntRot; i ++) contaRot[i] = 0;
	for (int i = 0; i < k; i ++) {
		for (int j = 0; j < qntRot; j ++) {
			if (matBase[pos[i]][qntCol - 1] == rotulos[j]) contaRot[j] ++;
		}
	}
	posClass = maior (contaRot, qntRot);
	rotuloClass = rotulos[posClass];
	return rotuloClass;
}

void confusao (int **matConfusa, float rotClass, float rotReal, float *rotulos, int qntRot) {
	int m, n;
	for (int i = 0; i < qntRot; i ++) {
		if (rotReal == rotulos[i]) m = i;
		if (rotClass == rotulos[i]) n = i;
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
	acc = acertos / total;
	return acc;
}

int busca (float *vet, int n, float k) {
	for (int i = 0; i < n; i ++) {
		if (vet[i] == k) {
			return 0;
		}
	}
	return 1;
}

void bubbleSort (float *vet, int *pos, int n) {
	int aux;
	for (int i = 0; i < n - 1; i ++) {
		for (int j = i + 1; j < n; j ++) {
			if (vet[j] < vet[i]) {
				aux = pos[i];
                pos[i] = pos[j];
                pos[j] = aux;
			}
		}
	}
}

int maior (int *vet, int n) {
	int posMaior, maior = 0;
	for (int i = 0; i < n; i ++) {
		if (vet[i] > maior) {
			maior = vet[i];
			posMaior = i;
		}
	}
	return posMaior;
}