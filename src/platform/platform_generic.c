#include "platform.h"
#include <stdio.h>

platform_state *platform_mpu6050_init(void) {
    printf("platform_mpu6050_init is not implemented for platform: generic!\n");
    return NULL;
}

void platform_mpu6050_free(platform_state *state) {
    (void)(state);
    printf("platform_mpu6050_free is not implemented for platform: generic!\n");
}

short platform_mpu6050_read_data_x(platform_state *state) {
    (void)(state);
    printf("platform_mpu6050_read_data_x is not implemented for platform: generic!\n");
    return 0;
}

short platform_mpu6050_read_data_y(platform_state *state) {
    (void)(state);
    printf("platform_mpu6050_read_data_x is not implemented for platform: generic!\n");
    return 0;
}

short platform_mpu6050_read_data_z(platform_state *state) {
    (void)(state);
    printf("platform_mpu6050_read_data_x is not implemented for platform: generic!\n");
    return 0;
}
