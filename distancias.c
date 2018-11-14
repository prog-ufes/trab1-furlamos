#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 260






// IMPLEMENTACAO DO CALCULO DAS DISTANCIAS

// DISTANCIA EUCLIDIANA
float distEucl (float *p, float *q, int n) {
	float dist = 0;
	for (int i = 0; i < n; i++) {
		dist += pow (p[i] - q[i], 2);
	}
	dist = sqrt (dist);
	return dist;
}

// DISTANCIA DE MINKOWSKY
float distMink (float *p, float *q, int n, float r) {
	float dist = 0, mod = 0;
	for (int i = 0; i < n; i++) {
		mod = sqrt (pow ((p[i] - q[i]), 2));
		dist += pow (mod, r);
	}
	dist = pow(dist, 1/r);
	return dist;
}

// DISTANCIA DE CHEBYSHEV
float distCheb (float *p, float *q, int n) {
	float dist = 0;
	for (int i = 0; i < n; i++) {
		if ((pow ((p[i] - q[i]), 2)) > dist) {
			dist = pow ((p[i] - q[i]), 2);
		}
	}
	dist = sqrt (dist);
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



// LEITURA DO treino/teste
int leitura (char *path, int tipoLeitura) { //1 para treino, 2 para teste
	int qntB = 0, qntCol = 0, qntL = 0, qntRot = 0;
    char linha;
    float **matBase,
		  *rotulos, *base;
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
        if (linha == '\n') qntL ++;
    }
	fclose (treiste);

	qntL --;
    qntCol = (qntB - 1) / qntL;
	matBase = (float**) malloc (qntL * (sizeof (float*)));
	*matBase = (float*) malloc (qntCol * (sizeof (float)));

	for (int k = 0, i = 0, j = 0; k < (qntB - 1); k ++, j ++) {
		if ((j % qntCol == 0) && (j != 0)) {
			i ++;
			j = 0;
			matBase[i] = (float*) malloc (qntCol * (sizeof (float)));
			matBase[i][j] = base[k];

			if (tipoLeitura == 1) {
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
				// começa a matriz de confusão
			}
		}
	}
  	free (base);

	// aqui vai a matriz de confusão e o teste

	for (int i = 0; i < qntL; i ++) {
		free (matBase[i]);
	}
	free (matBase);
	free (rotulos);
    return 0;
}


int **matConfusa (float **matBase, int qntCol, int qntL, float *rotulos, int qntRot, int inicial) { //lembrar de enviar qntCol - 1, pra tirar o rotulo
	int **confusa;
	confusa = (int**) malloc (qntRot * sizeof (int*));
	for (int i = 0; i < qntRot; i++) {
		confusa[i] = (int*) malloc (qntRot * sizeof (int));
	}

	for (int i = 0; i < qntL; i++) {
		for (int j = 0; j < qntCol; j++) {
			/* code */
		}
	}
	return confusa;
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
			fclose (config);
			free (pathTreino);
			free (pathTeste);
			free (pathSaida);
			exit(1);
		}

		while (! feof (config)) { //ESTA FAZENDO O FINAL DUAS VEZES
			fscanf (config, "%d, %c, %f", &k, &tipo, &r);

			if (leitura (pathTreino, 1)) {
				printf ("Arquivo incorreto!\n");
				fclose (config);
				free (pathTreino);
				free (pathTeste);
				free (pathSaida);
				exit(1);
			}
			//FAZER A MATRIZ DE CONFUSÃO PARA OS TESTES
		}
	}
	fclose (config);
	free (pathTreino);
	free (pathTeste);
	free (pathSaida);
	return 0;
}
