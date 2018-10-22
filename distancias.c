#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// IMPLEMENTACAO DO CALCULO DAS DISTANCIAS

// DISTANCIA EUCLIDIANA
float distEucl (float *p, float *q, int n){
	float dist = 0;
	for(int i = 0; i < n; i++)
		dist += pow(p[i] - q[i], 2);
	dist = sqrt(dist);
	return dist;
}

// DISTANCIA DE MINKOWSKY
float distMink (float *p, float *q, int n, float r){
	float dist = 0;
	for(int i = 0; i < n; i++)
		dist += pow(abs(p[i] - q[i]), r);
	dist = pow(dist, 1/r);
	return dist;
}

// SIMILARIDADE DE COSSENOS
float simCos (float *p, float *q, int n){
	float dist = 0;
	for(int i = 0; i < n; i++)
		dist += p[i] * q[i];
	dist = dist / distEucl(p, q, n);
	return dist;
}

int main(){
	char dist;
	switch(dist){
		case 'E':
		break;

		case 'M':
		break;

		case 'C':
		break;

		default:
		break;
	}
	return 0;
}
