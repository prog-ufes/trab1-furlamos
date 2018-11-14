#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 40






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


/*
float *tiposRotulos (float *base, int b, int qntCol, int *qntRot) {
	float *tipos;
	tipos = (float*) malloc (sizeof (float));
	for (int i = (qntCol - 1); i < b; i += qntCol) {
		printf("%d\n", qntRot[0]);
		if (base[i] != tipos[*qntRot]) {
			tipos = realloc (tipos, (*qntRot + 1) * sizeof (float));
			tipos[i] = base[i];
			*qntRot ++;
		}
	}
	return tipos;
}
*/

// LEITURA DO treino/teste
int leitura (char *path) {
	int qntB, qntCol, qntL,
		*qntRot;
    char linha;
    float **matBase,
		  *base, *rotulos;
	FILE *treiste;

	treiste = fopen ("iris_treino.csv", "r");

	if (treiste == NULL) {
	    printf ("Erro ao abrir o arquivo!\n");
	    fclose (treiste);
	    return (1);
    }

  	base = (float*) malloc (sizeof (float));
	qntL = 0;
    for (qntB = 0; ! feof (treiste); qntB++) {
    	base = realloc (base, (qntB + 1) * sizeof (float));
		fscanf (treiste, "%f%c", &base[qntB], &linha);
        if (linha == '\n') {
        	qntL ++;
        }
    }
	qntL --;
    qntCol = (qntB - 1) / qntL;
	matBase = (float**) malloc (qntL * (sizeof (float*)));
	*matBase = (float*) malloc (qntCol * (sizeof (float)));

	for (int k = 0, i = 0, j = 0; k < (qntB - 1); k ++, i ++) {
		if ((i % qntCol == 0) && (i != 0)) {
			j ++;
			i = 0;
			matBase[j] = (float*) malloc (qntCol * (sizeof (float)));
		}
		//matBase[i][j] = base[k];
	}
/*
	for (size_t i = 0; i < qntL; i++) {
		for (size_t j = 0; j < qntCol; j++) {
			printf(" %.2f", matBase[i][j]);
		}
		printf("\n");
	}
*/
	qntRot = (int*) malloc (sizeof (int));
	*qntRot = 0;
	/*rotulos = tiposRotulos (base, (b - 1), qntCol, qntRot);

	printf("%d\n", *qntRot);
	for (size_t i = 0; i < *qntRot; i++) {
		printf("%f\n", rotulos[i]);
	}*/

  	free (base);
    return 0;
}









// LEITURA DO ARQUIVO config.txt
int main () {
	int k;
	float **rotulos,
		  r;
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
			exit(1);
		}
	}
	int l = leitura (pathTreino);

	while (! feof (config)) { //ESTA FAZENDO O FINAL DUAS VEZES
		fscanf (config, "%d, %c, %f", &k, &tipo, &r);


		//FAZER A MATRIZ DE CONFUSÃO PARA OS TESTES


	}

	return 0;
}
