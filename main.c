#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include <stdbool.h>

/*
 * Ali Abu-afash Nayef
 * Martín Rodríguez Arévalo
 * Alejandro Patiño Jaramillo
*/

double microsegundos() { /* obtiene la hora del sistema en microsegundos */
	struct timeval t;
	if (gettimeofday(&t, NULL) < 0 )
		return 0.0;
	return (t.tv_usec + t.tv_sec * 1000000.0);
}

void inicializar_semilla() {
	srand(time(NULL));
/* se establece la semilla de una nueva serie de enteros pseudo-aleatorios */
}

void aleatorio(int v[], int n) {
	int i, m=2*n+1;
	for (i=0; i < n; i++)
		v[i] = (rand() % m) - n;
	/* se generan números pseudoaleatorio entre -n y +n */
}

void inicializar_ascendente(int v[], int n) {
	for (int i = 0; i < n; i++) {
		v[i] = i;
	}
}

void inicializar_descendente(int v[], int n) {
	for (int i = 0; i < n; i++) {
		v[i] = n - i;
	}
}

bool ordenado(int v[], int n){
	for (int i = 1; i < n; i++){
		if (v[i - 1] > v[i]) { return false; }
	}
	return true;
}

void swap(int v[], int pos1, int pos2) {
	int tmp = v[pos1];
	v[pos1] = v[pos2];
	v[pos2] = tmp;
}

void Mediana3 (int v[], int i, int j) {
	int k = (i + j) / 2;
	if (v[k] > v[j]) {
		swap(v, k, j);
	} else if (v[k] > v[i]) {
		swap(v, k, i);
	} else {
		swap(v, i, j);
	}
}

void ordenar_aux(int v[], int left, int right, int umbral) {
	int pivote, i, j;
	if (left + umbral <= right) {
		Mediana3(v, left, right);
		pivote = v[left];
		i = left;
		j = right;
		while (j >= i) {
			while (v[i] <= pivote) {
				i++;
			}
			while (v[j] >= pivote) {
				j--;
			}
			swap(v, i, j);
		}
		swap(v, i, j);
		swap(v, left, j);
		ordenar_aux(v, left, j - 1, umbral);
		ordenar_aux(v, j + 1, right, umbral);
	}
}

void ord_ins(int v[], int n) {
	int x, j;
	for (int i = 1; i < n; i++){
		x = v[i];
		j = i - 1;
		while (j >= 0 && v[j] >= x){
			v[j + 1] = v[j];
			j--;
		}
		v[j + 1] = x;
	}
}

void ord_rapida(int v[], int n, int umbral) {
	ordenar_aux(v, 0, n - 1, umbral);
	if (umbral > 1) {
		ord_ins(v, n);
	}
}

void contarTiempo(int k, double confianza, int inicializacion) {
	int conf, *v, n, umbral = 1;
	double ta, tb, t = 0.0, t1, t2;
	for (int j = 0; j < 3; j++) {
		switch(inicializacion) {
			case 0: 
				printf("Algoritmo desordenado con umbral %d\n", umbral); break;
			case 1: 
				printf("Algoritmo ascendente con umbral %d\n", umbral); break;
			case 2: 
				printf("Algoritmo descendente con umbral %d\n", umbral); break;
			default: break; }
		printf("%10s%18s%18s%18s%18s\n", 
			"n", "t (n)", "t (n) / n", "t (n) / n^1.11", "t (n) / n*log^2(n)");
		for (n = 500 ; n <= 256000 && t < 500000; n=n*2) {
			conf = 0;
			v = malloc(n * sizeof(int));
			if (!v) {
				perror("malloc");
				exit(EXIT_FAILURE); }
			switch(inicializacion) {
				case 0: aleatorio(v, n); break;
				case 1: inicializar_ascendente(v, n); break;
				case 2: inicializar_descendente(v, n); break;
				default: break; }
			ta = microsegundos();
			ord_rapida(v, n, umbral);
			tb = microsegundos();
			t = tb - ta;
			if (t < confianza){
				ta = microsegundos();
				for (int i = 0; i < k; i++){
					switch(inicializacion) {
						case 0: aleatorio(v, n); break;
						case 1: inicializar_ascendente(v, n); break;
						case 2: inicializar_descendente(v, n); break;
						default: break; }
					ord_rapida(v, n, umbral);
				}
				tb = microsegundos();
				t1 = tb - ta;
				ta = microsegundos();
				for (int j = 0; j < k; j++){
					switch(inicializacion) {
						case 0: aleatorio(v, n); break;
						case 1: inicializar_ascendente(v, n); break;
						case 2: inicializar_descendente(v, n); break;
						default: break; }
				}
				tb = microsegundos();
				t2 = tb - ta;
				t = (t1 - t2) / k;
				conf = 1;
			}
			if (conf == 1){
				printf("(*)");
				printf("%7d", n);

			} else {
				printf("%10d", n);
			}
			printf("%18lf", t);
			printf("%18lf", t / pow((double)n, 1));
			printf("%18.7lf", t / pow((double)n, 1.11));
			printf("%18lf", t / ((double)n * pow(log(n), 2)));
			printf("\n");
			free(v);
		}
		umbral *= 10;
	}
}

void test_th1(){
	int umbral = 1, n = 20;
	int v[n];
	aleatorio(v, n);
	for(int i = 0; i < n; i++){
		printf("%d  ", v[i]);
	}
	printf("\n");
	ord_rapida(v, n, umbral);
	for(int i = 0; i < n; i++){
		printf("%d  ", v[i]);
	}
	printf("\n\n");
	inicializar_ascendente(v, n);
	for(int i = 0; i < n; i++){
		printf("%d  ", v[i]);
	}
	printf("\n");
	ord_rapida(v, n, umbral);
	for(int i = 0; i < n; i++){
		printf("%d  ", v[i]);
	}
	printf("\n\n");
	inicializar_descendente(v, n);
	for(int i = 0; i < n; i++){
		printf("%d  ", v[i]);
	}
	printf("\n");
	ord_rapida(v, n, umbral);
	for(int i = 0; i < n; i++){
		printf("%d  ", v[i]);
	}
	printf("\n\n");
}

// n * log n

int main(void){
	int k = 1000;
	double confianza = 500.00;
	// inicializacion = 0 -- Desordenado
	// inicializacion = 1 -- Ascendente
	// inicializacion = 2 -- Descendente

	contarTiempo(k, confianza, 0);
	test_th1();
	return 0;
}