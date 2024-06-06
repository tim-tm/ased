#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>

#include "platform/platform.h"
#include "database/database.h"

float max_gx = 0;
float max_gy = 0;
float max_gz = 0;
float min_gx = 0;
float min_gy = 0;
float min_gz = 0;
float avg_dx = 0;
float avg_dy = 0;
float avg_dz = 0;
int n_itr = 500;
// Muss noch getestet werden
float max_min_max_off = 0.8f;
float max_avg_diff_off = 1.8f;

platform_state *state;

void read_data(float *gx, float *gy, float *gz, float *last_gx, float *last_gy, float *last_gz);

int main(int argc, char **argv) {
	if (argc != 4) {
		printf("Nutze Standard: %i Iterationen, max_min_max_off=%f, max_avg_diff_off=%f\n", n_itr, max_min_max_off, max_avg_diff_off);
	} else {
		n_itr = atoi(argv[1]);
		if (n_itr <= 0) {
			printf("Anzahl von Iterationen muss größer als 0 sein.\n");
			return 1;
		}
		max_min_max_off = atof(argv[2]);
		if (max_min_max_off <= 0) {
			printf("max_min_max_off muss größer als 0 sein.\n");
			return 1;
		}
		max_avg_diff_off = atof(argv[3]);
		if (max_avg_diff_off <= 0) {
			printf("max_avg_diff_off muss größer als 0 sein.\n");
			return 1;
		}
		printf("Nutze %i Iterationen, max_min_max_off=%f, max_avg_diff_off=%f\n", n_itr, max_min_max_off, max_avg_diff_off);
	}

    state = platform_mpu6050_init();
    if (state == NULL) return 1;

	printf("Berechne Vergleichswerte anhand von %i Iterationen...\n", n_itr);
	float gx = 0, gy = 0, gz = 0;
	float last_gx = 0, last_gy = 0, last_gz = 0;
	
	for (int i = 0; i < n_itr; ++i) {
		read_data(&gx, &gy, &gz, &last_gx, &last_gy, &last_gz);

		if (gx > max_gx) {
			max_gx = gx;
		}
		if (gy > max_gy) {
			max_gy = gy;
		}
		if (gz > max_gz) {
			max_gz = gz;
		}
		if (gx < min_gx) {
			min_gx = gx;
		}
		if (gy < min_gy) {
			min_gy = gy;
		}
		if (gz < min_gz) {
			min_gz = gz;
		}
		avg_dx += fabs(last_gx-gx);
		avg_dy += fabs(last_gy-gy);
		avg_dz += fabs(last_gz-gz);
	}
	avg_dx /= n_itr;
	avg_dy /= n_itr;
	avg_dz /= n_itr;
	printf("max_gx = %.2f, max_gy = %.2f, max_gz = %.2f\nmin_gx = %.2f, min_gy = %.2f, min_gz = %.2f\navg_dx = %.2f, avg_dy = %.2f, avg_dz = %.2f\n", max_gx, max_gy, max_gz, min_gx, min_gy, min_gz, avg_dx, avg_dy, avg_dz);
	
	while (1) {
		read_data(&gx, &gy, &gz, &last_gx, &last_gy, &last_gz);
		
		struct timeval current_time;
		gettimeofday(&current_time, NULL);
		char time_str[128];
		strftime(time_str, 80, "%H:%M:%S %d-%m-%Y", localtime(&current_time.tv_sec));

		if (gx > (max_gx+max_min_max_off)) {
			printf("%s: Fehler entdeckt (max_x)\n", time_str);
		}
		if (gy > (max_gy+max_min_max_off)) {
			printf("%s: Fehler entdeckt (max_y)\n", time_str);
		}
		if (gz > (max_gz+max_min_max_off)) {
			printf("%s: Fehler entdeckt (max_z)\n", time_str);
		}
		if (gx < (min_gx-max_min_max_off)) {
			printf("%s: Fehler entdeckt (min_x)\n", time_str);
		}
		if (gy < (min_gy-max_min_max_off)) {
			printf("%s: Fehler entdeckt (min_y)\n", time_str);
		}
		if (gz < (min_gz-max_min_max_off)) {
			printf("%s: Fehler entdeckt (min_z)\n", time_str);
		}
		
		float dx = fabs(last_gx-gx);
		float dy = fabs(last_gy-gy);
		float dz = fabs(last_gz-gz);
		if (dx > (avg_dx+max_avg_diff_off)) {
			printf("%s: Fehler entdeckt (avg_diff_x)\n", time_str);
		}
		if (dy > (avg_dy+max_avg_diff_off)) {
			printf("%s: Fehler entdeckt (avg_diff_y)\n", time_str);
		}
		if (dz > (avg_dz+max_avg_diff_off)) {
			printf("%s: Fehler entdeckt (avg_diff_z)\n", time_str);
		}
	}
    platform_mpu6050_free(state);
	return 0;
}

void read_data(float *gx, float *gy, float *gz, float *last_gx, float *last_gy, float *last_gz) {
	if (gx == NULL || gy == NULL || gz == NULL || last_gx == NULL || last_gy == NULL || last_gz == NULL) return;

	*last_gx = *gx;
	*last_gy = *gy;
	*last_gz = *gz;

	*gx = platform_mpu6050_read_data_x(state);
	*gy = platform_mpu6050_read_data_y(state);
	*gz = platform_mpu6050_read_data_z(state);
}
