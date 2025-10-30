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
	v[pos2] = v[pos1]
}

void Median3 (int v[], int i, int j) {
	int k = (i + j) / 2;
	if (v[k] > v[j]) {
		swap(v, k, j);
	} else if (v[k] > v[i]) {
		swap(v, k, i]);
	} else {
		swap(v, i, j);
	}
}

void OrdenarAux (int v[], int left, int right, int THRESHOLD) {
	int pivote, i, j;
	if (left + THRESHOLD <= right) {
		Median3(v, left, right);
		pivote = v[left];
		i = left;
		j = right;
		while (j <= i) {
			for (i; v[i] >= pivote; i++);
			for (j; v[j] <= pivote; j--);
			swap(v, i, j);
		}
		swap(v, i, j);
		swap(v, left, j);
		OrdenarAux(v, left, j - 1);
		OrdenarAux(v, j + 1, right);
	}
}

void InsertionSort(int v[], int n) {
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

void OrdenacionRapida(int v[], int n, int THRESHOLD) {
	OrdenarAux(v, 0, n - 1);
	if (THRESHOLD > 1) {
		InsertionSort(v, n);
	}
}

/*void contarTiempoShellCiura(int n, int k, int m, int exp, double confianza){
	int conf, *v, *incr, mm;
	double ta, tb, t, t1, t2;
	printf("Algoritmo Shell: Ciura\n");
	printf("%10s%18s%18s%18s%18s\n", 
		"n", "t (n)", "t (n) / n", "t (n) / n^1.11", "t (n) / n*log^2(n)");
	for (int i = 0; i < m; i++) {
		conf = 0;
		v = malloc(n * sizeof(int));
		if (!v){
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		incr = ciura(n, &mm);
		aleatorio(v, n);
		ta = microsegundos();
		ord_shell(v, n, incr, mm);
		tb = microsegundos();
		t = tb - ta;
		if (t < confianza){
			ta = microsegundos();
			for (int i = 0; i < k; i++){
				aleatorio(v, n);
				ord_shell(v, n, incr, mm);
			}
			tb = microsegundos();
			t1 = tb - ta;
			ta = microsegundos();
			for (int j = 0; j < k; j++){
				aleatorio(v, n);
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
		free(incr);
		n *=exp;
	}
}*/

void test_th1(){
	int THRESHOLD = 1, n = 20;
	int v[n];
	aleatorio(v, n);
	for(int i = 0; i < n; i++){
		printf("%d  ", v[i]);
	}
	OrdenacionRapida(v, n, THRESHOLD);
	for(int i = 0; i < n; i++){
		printf("%d  ", v[i]);
	}
	inicializar_ascendente(v, n);
	for(int i = 0; i < n; i++){
		printf("%d  ", v[i]);
	}
	OrdenacionRapida(v, n, THRESHOLD);
	for(int i = 0; i < n; i++){
		printf("%d  ", v[i]);
	}
	inicializar_descendente(v, n);
	for(int i = 0; i < n; i++){
		printf("%d  ", v[i]);
	}
	OrdenacionRapida(v, n, THRESHOLD);
	for(int i = 0; i < n; i++){
		printf("%d  ", v[i]);
	}
}

int main(void){
	//int k = 1000, n = 500, m = 11, exp = 2;
	//double confianza = 500.00;

	test_th1();
	return 0;
}