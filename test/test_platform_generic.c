#define LOGGER_IMPLEMENTATION
#include "logger.h"

#include "platform/platform.h"

int main(void) {
#ifdef PLATFORM_GENERIC
    LOGGER_ASSERT_MSG(platform_mpu6050_init() == NULL, "Expected platform_mpu6050_init to return NULL on a generic platform!");
    LOGGER_ASSERT_MSG(platform_mpu6050_read_data_x(NULL) == 0, "Expected platform_mpu6050_read_data_x to return 0 on a generic platform!");
    LOGGER_ASSERT_MSG(platform_mpu6050_read_data_y(NULL) == 0, "Expected platform_mpu6050_read_data_y to return 0 on a generic platform!");
    LOGGER_ASSERT_MSG(platform_mpu6050_read_data_z(NULL) == 0, "Expected platform_mpu6050_read_data_z to return 0 on a generic platform!");
#endif /* ifdef PLATFORM_GENERIC */
}
