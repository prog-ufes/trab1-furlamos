#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 260

float distancias (float *p, float *q, int n, float r, char tipo);
int busca (float *vet, int n, float k);
void bubbleSort (float *vet, int *pos, int n);
int leitura (int k, char tipo, float r, char *path, int tipoLeitura);
float classificador (int k, char tipo, float r, float *p, float **matBase, int qntCol, int qntL, float *rotulos, int qntRot, int tipoClass);
void confusao (int **matConfusa, float rotClass, float rotReal, float *rotulos, int qntRot);
float acuracia (int **matConfusa, int qntRot);


// IMPLEMENTACAO DO CALCULO DAS DISTANCIAS
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

// LEITURA DO treino/teste
int leitura (int k, char tipo, float r, char *path, int tipoLeitura) { //1 para treino, 2 para teste
	int **matConfusa,
	    qntB = 0, qntCol = 0, qntL = 0, qntRot = 0;
    char linha;
    float **matBase,
		  *rotulos, *base,
		  rotReal, rotClass;
	FILE *treiste;

	treiste = fopen ("iris_teste.csv", "r");

	if (treiste == NULL) {
	    printf ("Erro ao abrir o arquivo!\n");
	    fclose (treiste);
	    return (1);
    }

  	base = (float*) malloc (sizeof (float));
    for (qntB = 0; ! feof (treiste); qntB++) {
    	base = realloc (base, (qntB + 1) * sizeof (float));
		fscanf (treiste, "%f%c", &base[qntB], &linha);
        if (linha == '\n') qntL ++;
    }
	fclose (treiste);

	qntL --;
    qntCol = (qntB - 1) / qntL;
	matBase = (float**) malloc (qntL * (sizeof (float*)));

	for (int k = 0, i = -1, j = 0; k < (qntB - 1); k ++, j ++) {
		if (j % qntCol == 0) {
			i ++;
			j = 0;
			matBase[i] = (float*) malloc (qntCol * (sizeof (float)));

			if ((tipoLeitura == 1) && (k != 0)) {
				if (qntRot == 0) {
					rotulos = (float*) malloc (sizeof (float));
					rotulos[0] = base[k - 1];
					qntRot = 1;
				}
				if (busca (rotulos, qntRot, base[k - 1])) {
					qntRot ++;
					rotulos = realloc (rotulos, qntRot * sizeof (float));
					rotulos[qntRot - 1] = base[k - 1];
				}
			}
		}

		matBase[i][j] = base[k];
	}
  	free (base);

	matConfusa = (int**) malloc (qntRot * (sizeof (int*)));
	for (int i = 0; i < qntRot; i ++) {
		matConfusa[i] = (int*) malloc (qntRot * (sizeof (int)));
		for (int j = 0; j < qntRot; j ++) matConfusa[i][j] = 0;
	}

	for (int i = 0; i < qntL; i ++) {
		rotReal = matBase[i][qntCol - 1];
		rotClass = classificador (k, tipo, r, matBase[i], matBase, qntCol, qntL, rotulos, qntRot, 1);
		confusao (matConfusa, rotClass, rotReal, rotulos, qntRot);
	}

	for (int i = 0; i < qntL; i ++) free (matBase[i]);
	free (matBase);
	for (int i = 0; i < qntRot; i ++) free (matConfusa[i]);
	free (matConfusa);
	free (rotulos);
    return 0;
}



float classificador (int k, char tipo, float r, float *p, float **matBase, int qntCol, int qntL, float *rotulos, int qntRot, int tipoClass) { //1 para treino, 2 tste
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
	if (tipoClass) k ++;
	for (int i = 0; i < k; i ++) {
		if (tipoClass) i ++;
		for (int j = 0; j < qntRot; j ++) {
			if (matBase[pos[i]][qntCol - 1] == rotulos[j]) contaRot[j] ++;
		}
	}
	posClass = maior (contaRot, qntRot);
	rotuloClass = rotulos[posClass];
	return rotuloClass;
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

void confusao (int **matConfusa, float rotClass, float rotReal, float *rotulos, int qntRot) {
	int m, n;
	for (int i = 0; i < qntRot; i ++) {
		if (rotReal == rotulos[i]) m = i;
		if (rotClass == rotulos[i]) n = i;
	}
	matConfusa[m][n] ++;
}

// LEITURA DO ARQUIVO config.txt
int main () {
	int k;
	float r;
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

		while (! feof (config)) { //ESTA FAZENDO O FINAL DUAS VEZES
			fscanf (config, "%d, %c, %f", &k, &tipo, &r);

			if (leitura (k, tipo, r, pathTreino, 1)) {

			}
		}
	}
	fclose (config);
	return 0;
}
