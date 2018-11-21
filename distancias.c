#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define N 260

int leitura (float ***matBase, char *path, int *qntL, int *qntCol, float *rotMax);
float distancias (float *p, float *q, int n, float r, char tipo);

int main () {
    int qntL, qntLTeste, qntCol, k;
	char pathTreino[N], pathTeste[N], pathSaida[N], novoPathSaida[N],
		 tipo;
    float **matTreino = NULL, **matTeste = NULL,
          rotMax = 0, r;
	FILE *config;

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

            int matConfusa[(int) rotMax][(int) rotMax];
            fscanf (config, "%d, %c, %f", &k, &tipo, &r);
            for (int s = 1; (! feof (config)); s++) {
                for (int i = 0; i < ((int) rotMax); i ++) {
                    for (int j = 0; j < ((int) rotMax); j ++) matConfusa[i][j] = 0;
                }

                fscanf (config, "%d, %c, %f", &k, &tipo, &r);
            }
            fclose (config);

        }
        for (int i = 0; i < qntLTeste; i++) free (matTeste[i]);
        free (matTeste);
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
