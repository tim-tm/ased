#ifndef PLATFORM_H
#define PLATFORM_H

#define MPU6050_DEVICE_ADDRESS 0x68
#define MPU6050_PWR_MGMT_1     0x6B
#define MPU6050_SMPLRT_DIV     0x19
#define MPU6050_CONFIG         0x1A
#define MPU6050_GYRO_CONFIG    0x1B
#define MPU6050_INT_ENABLE     0x38
#define MPU6050_GYRO_XOUT_H    0x43
#define MPU6050_GYRO_YOUT_H    0x45
#define MPU6050_GYRO_ZOUT_H    0x47

typedef struct _platform_state_ {
    int data;
} platform_state;

platform_state *platform_mpu6050_init(void);
void platform_mpu6050_free(platform_state *state);

short platform_mpu6050_read_data_x(platform_state *state);
short platform_mpu6050_read_data_y(platform_state *state);
short platform_mpu6050_read_data_z(platform_state *state);

#endif // PLATFORM_H
