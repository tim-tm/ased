#include "platform.h"

#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

static short platform_mpu6050_read_raw_data(platform_state *state, int addr) {
	if (state == NULL) return 0;

    short high_byte,low_byte,value;
	high_byte = wiringPiI2CReadReg8(state->data, addr);
	low_byte = wiringPiI2CReadReg8(state->data, addr+1);
	value = (high_byte << 8) | low_byte;
	return value;
}

platform_state *platform_mpu6050_init(void) {
    platform_state *state = malloc(sizeof(platform_state));
    if (state == NULL) return NULL;

	state->data = wiringPiI2CSetup(MPU6050_DEVICE_ADDRESS);
	wiringPiI2CWriteReg8(state->data, MPU6050_SMPLRT_DIV, 0x07);	/* Write to sample rate register */
	wiringPiI2CWriteReg8(state->data, MPU6050_PWR_MGMT_1, 0x01);	/* Write to power management register */
	wiringPiI2CWriteReg8(state->data, MPU6050_CONFIG, 0);		/* Write to Configuration register */
	wiringPiI2CWriteReg8(state->data, MPU6050_GYRO_CONFIG, 24);	/* Write to Gyro Configuration register */
	wiringPiI2CWriteReg8(state->data, MPU6050_INT_ENABLE, 0x01);	/*Write to interrupt enable register */
    
    return state;
}

void platform_mpu6050_free(platform_state *state) {
	if (state != NULL) {
		free(state);
	}
}

short platform_mpu6050_read_data_x(platform_state *state) {
	short data = platform_mpu6050_read_raw_data(state, MPU6050_GYRO_XOUT_H);
	data /= 131;
	return data;
}

short platform_mpu6050_read_data_y(platform_state *state) {
	short data = platform_mpu6050_read_raw_data(state, MPU6050_GYRO_YOUT_H);
	data /= 131;
	return data;
}

short platform_mpu6050_read_data_z(platform_state *state) {
	short data = platform_mpu6050_read_raw_data(state, MPU6050_GYRO_ZOUT_H);
	data /= 131;
	return data;
}
