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

double microsegundos() {
	struct timeval t;
	if (gettimeofday(&t, NULL) < 0)
		return 0.0;
	return (t.tv_usec + t.tv_sec * 1000000.0);
}

void inicializar_semilla() {
	srand(time(NULL));
}

void aleatorio(int v[], int n) {
	int i, m = 2 * n + 1;
	for (i = 0; i < n; i++)
		v[i] = (rand() % m) - n;
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

bool ordenado(int v[], int n) {
	for (int i = 1; i < n; i++) {
		if (v[i - 1] > v[i]) {
			return false;
		}
	}
	return true;
}

void swap(int v[], int pos1, int pos2) {
	int tmp = v[pos1];
	v[pos1] = v[pos2];
	v[pos2] = tmp;
}

void Mediana3(int v[], int i, int j) {
	int k = (i + j) / 2;
	if (v[k] > v[j]) {
		swap(v, k, j);
	}
	if (v[k] > v[i]) {
		swap(v, k, i);
	}
	if (v[i] > v[j]) {
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
		do {
			i++;
			while (v[i] < pivote) {
				i++;
			}
			j--;
			while (v[j] > pivote) {
				j--;
			}
			swap(v, i, j);
		} while (i < j);
		swap(v, i, j);
		swap(v, left, j);
		ordenar_aux(v, left, j - 1, umbral);
		ordenar_aux(v, j + 1, right, umbral);
	}
}

void ord_ins(int v[], int n) {
	int x, j;
	for (int i = 1; i < n; i++) {
		x = v[i];
		j = i - 1;
		while (j >= 0 && v[j] > x) {
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

// Función auxiliar para copiar vectores
void copiar_vector(int origen[], int destino[], int n) {
	for (int i = 0; i < n; i++) {
		destino[i] = origen[i];
	}
}

// Función para inicializar según el tipo
void inicializar_vector(int v[], int n, int tipo_inicializacion) {
	switch(tipo_inicializacion) {
		case 0: aleatorio(v, n); break;
		case 1: inicializar_ascendente(v, n); break;
		case 2: inicializar_descendente(v, n); break;
	}
}

void contarTiempo(int k, double confianza, int inicializacion) {
	int *v, *v_aux, n;
	double ta, tb, t, t1, t2;
	int umbrales[] = {1, 10, 100};
	const char *nombres[] = {
		"Algoritmo desordenado",
		"Algoritmo ascendente",
		"Algoritmo descendente"
	};

	for (int idx_umbral = 0; idx_umbral < 3; idx_umbral++) {
		int umbral = umbrales[idx_umbral];

		printf("\n%s con umbral %d\n", nombres[inicializacion], umbral);
		printf("%10s%18s%18s%18s%18s\n",
			"n", "t (n)", "t (n) / n", "t (n) / n^1.11", "t (n) / n*log^2(n)");

		for (n = 500; n <= 256000; n *= 2) {
			v = malloc(n * sizeof(int));
			v_aux = malloc(n * sizeof(int));

			if (!v || !v_aux) {
				perror("malloc");
				exit(EXIT_FAILURE);
			}

			// Inicializar vector original
			inicializar_vector(v, n, inicializacion);

			// Medición simple
			copiar_vector(v, v_aux, n);
			ta = microsegundos();
			ord_rapida(v_aux, n, umbral);
			tb = microsegundos();
			t = tb - ta;

			// Verificar que está ordenado
			if (!ordenado(v_aux, n)) {
				printf("ERROR: Vector no ordenado correctamente para n=%d\n", n);
				free(v);
				free(v_aux);
				continue;
			}

			bool confianza_baja = false;

			// Si el tiempo es menor que el umbral de confianza, hacer múltiples mediciones
			if (t < confianza) {
				confianza_baja = true;

				// Medir k ejecuciones del algoritmo
				ta = microsegundos();
				for (int i = 0; i < k; i++) {
					inicializar_vector(v_aux, n, inicializacion);
					ord_rapida(v_aux, n, umbral);
				}
				tb = microsegundos();
				t1 = tb - ta;

				// Medir k inicializaciones (overhead)
				ta = microsegundos();
				for (int j = 0; j < k; j++) {
					inicializar_vector(v_aux, n, inicializacion);
				}
				tb = microsegundos();
				t2 = tb - ta;

				// Tiempo promedio neto
				t = (t1 - t2) / k;
			}

			// Imprimir resultados
			if (confianza_baja) {
				printf("(*)%7d", n);
			} else {
				printf("%10d", n);
			}

			printf("%18.3lf", t);
			printf("%18.8lf", t / n);
			printf("%18.8lf", t / pow(n, 1.11));
			printf("%18.8lf", t / (n * pow(log(n), 2)));
			printf("\n");

			free(v);
			free(v_aux);

			// Detener si el tiempo supera un límite razonable
			if (t > 10000000) { // 10 segundos
				printf("Tiempo excesivo, deteniendo mediciones para este umbral.\n");
				break;
			}
		}
	}
}

void test_th1() {
	int umbral = 1, n = 20;
	int v[n];

	printf("=== TEST DE VALIDACIÓN ===\n\n");

	// Test 1: Vector aleatorio
	printf("Vector aleatorio:\n");
	aleatorio(v, n);
	printf("Original: ");
	for(int i = 0; i < n; i++) {
		printf("%d ", v[i]);
	}
	printf("\n");
	ord_rapida(v, n, umbral);
	printf("Ordenado: ");
	for(int i = 0; i < n; i++) {
		printf("%d ", v[i]);
	}
	printf("\nVerificación: %s\n\n", ordenado(v, n) ? "CORRECTO" : "ERROR");

	// Test 2: Vector ascendente
	printf("Vector ascendente:\n");
	inicializar_ascendente(v, n);
	printf("Original: ");
	for(int i = 0; i < n; i++) {
		printf("%d ", v[i]);
	}
	printf("\n");
	ord_rapida(v, n, umbral);
	printf("Ordenado: ");
	for(int i = 0; i < n; i++) {
		printf("%d ", v[i]);
	}
	printf("\nVerificación: %s\n\n", ordenado(v, n) ? "CORRECTO" : "ERROR");

	// Test 3: Vector descendente
	printf("Vector descendente:\n");
	inicializar_descendente(v, n);
	printf("Original: ");
	for(int i = 0; i < n; i++) {
		printf("%d ", v[i]);
	}
	printf("\n");
	ord_rapida(v, n, umbral);
	printf("Ordenado: ");
	for(int i = 0; i < n; i++) {
		printf("%d ", v[i]);
	}
	printf("\nVerificación: %s\n\n", ordenado(v, n) ? "CORRECTO" : "ERROR");
}

int main(void) {
	inicializar_semilla();

	int k = 1000;
	double confianza = 500.0;

	// Ejecutar tests de validación
	test_th1();

	printf("\n=== MEDICIONES DE TIEMPO ===\n");

	// Mediciones para cada tipo de inicialización
	for (int tipo = 0; tipo < 3; tipo++) {
		contarTiempo(k, confianza, tipo);
	}

	return 0;
}