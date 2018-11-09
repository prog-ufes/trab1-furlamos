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
	float dist = 0;
	for (int i = 0; i < n; i++) {
		dist += pow (abs (p[i] - q[i]), r);
	}
	dist = pow(dist, 1/r);
	return dist;
}

// DISTANCIA DE CHEBYSHEV
float distCheb (float *p, float *q, int n) {
	float dist = 0;
	for (int i = 0; i < n; i++) {
		if (abs(p[i] - q[i]) > dist) {
			dist = abs (p[i] - q[i]);
		}
	}
	return dist;
}

// LEITURA DO ARQUIVO config.txt
int leituraConfig (char *treino, char *teste, char *saida) {
	FILE *config;
	treino = (char*) malloc (N * sizeof (char));
	teste = (char*) malloc (N * sizeof (char));
	saida = (char*) malloc (N * sizeof (char));

	config = fopen ("config.txt", "r");
	if(config == NULL) {
      perror ("Error opening file");
      fclose (config); //tem que dar free nos ponteiros
      return(1);
   	}
  else {
		fgets (treino, N, config);
		fgets (teste, N, config);
		fgets (saida, N, config);

		if (!(treino) || !(teste) || !(saida)) {
			fclose (config);
			return(1);
		}
  }
   	fclose (config);

	return 0;
}

int main () {
	return 0;
}
