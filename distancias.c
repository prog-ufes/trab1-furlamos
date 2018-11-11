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






// LEITURA DO treino/teste
int leitura (char *path, float **rotulos) {
	int f, colunas, l;
    char linha;
    float *features;
	FILE *treiste;

	  	treiste = fopen ("iris_teste.csv", "r");

		if (treiste == NULL) {
	    printf ("Erro ao abrir o arquivo!\n");
	    fclose (treiste);
	    return (1);
    }

  	else {
  	  	features = (float*) malloc (sizeof (float));
      	for (f = 0; ! feof (treiste); f++) {
      		features = realloc (features, (f + 1) * sizeof (float));
			fscanf (treiste, "%f%c", (features + f), &linha);
        	if (linha == '\n') {
        		l ++;
        	}
      	}
      	colunas = (f - 1) / (l - 1); // CASO ENCNTRE O ERRO NO FEOF, TEM QUE FICAR F-1 AQUI
      	features = realloc (features, f * sizeof (float)); // tambme tirar uisso
  	}
  	free (features);
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
	int l = leitura (pathTreino, rotulos);

	while (! feof (config)) { //ESTA FAZENDO O FINAL DUAS VEZES
		fscanf (config, "%d, %c", &k, &tipo);
		if (tipo == 'M') {
			fscanf (config, ", %f", &r);
		}


		//FAZER A MATRIZ DE CONFUSÃO PARA OS TESTES


	}

	return 0;
}
