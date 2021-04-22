# imu
Defines a common interface for our Inertial Measurement Unit (IMU) sensors.
   * [License](LICENSE.md)
   * [Changelog](CHANGELOG.md)
   * [Contributing guide](CONTRIBUTING.md)

# Description

## Installation
CMake is used to build this library, which is exported as a library target called *imu*. The header is added as:

```
#include "imu/imu.h"
```

The library can be also be compiled stand-alone using the CMake idiom of creating a *build* directory and then, from within that directory issuing:

```
cmake .. -DMCU=MK66FX1M0
make
```

This will build the library. Notice that the *cmake* command includes a define specifying the microcontroller the code is being compiled for. This is required to correctly configure the code, CPU frequency, and compile/linker options. The available MCUs are:
   * MK20DX128
   * MK20DX256
   * MK64FX512
   * MK66FX1M0
   * MKL26Z64
   * IMXRT1062_T40
   * IMXRT1062_T41

These are known to work with the same packages used in Teensy products. Also switching packages is known to work well, as long as it's only a package change.

## Namespace
This library is within the namespace *bfs*.

## Class / Methods

**struct ImuConfig** defines a structure used to configure the sensor. The data fields are:

| Name | Description |
| --- | --- |
| enum Odr | The desired output data rate of the sensor |
| Eigen::Vector3f accel_bias_mps2 | A vector of accelerometer biases, m/s/s |
| Eigen::Vector3f mag_bias_ut | A vector of mag biases, uT |
| Eigen::Matrix3f accel_scale | A vector of accelerometer scale factors |
| Eigen::Matrix3f mag_scale | A vector of mag scale factors |
| Eigen::Matrix3f rotation | Rotation matrix to align sensor data with vehicle frame |

The output data rate enum defines three possible data rates:

| Enum | Description |
| --- | --- |
| ODR_200HZ | 200 Hz output data rate |
| ODR_100HZ | 100 Hz output data rate |
| ODR_50HZ | 50 Hz output data rate |

The accel and mag biases and scale factors should be determined offline and input here, they are relatively stable with respect to temperature. Gyro biases are estimated during init and a scale factor is not applied to the gyro data.

The bias and scale factor should be input such that:

```
y = c * x + b
```

Where *y* is the corrected sensor output, *c* is the scale factor matrix, *x* is the uncorrected sensor output, and *b* is the bias vector. An ideal sensor would have a bias vector of zeros and an identity scale factor matrix. 

The rotation matrix is used to align the sensor data with the vehicle frame. This is useful if the sensor cannot be installed aligned with the vehicle axes. The rotation should be used to align the sensor x-axis out the vehicle nose, the y-axis out the right, and the z-axis down.

**struct ImuData** defines a structure of data returned from the sensor. The data fields are:

| Name | Description |
| --- | --- |
| bool new_imu_data | Whether new accel and gyro data was read |
| bool imu_healthy | Whether the accel and gyro are healthy |
| bool new_mag_data | Whether new mag data was read |
| bool mag_healthy | Whether the mag is healthy |
| Eigen::Vector3f accel_mps2 | The 3-axis accel data, m/s/s |
| Eigen::Vector3f gyro_radps | The 3-axis gyro data, rad/s |
| Eigen::Vector3f mag_ut | The 3-axis mag data, uT |

Health is determined by whether the sensor fails to read 5 times in a row at the expected sampling rate.

**Imu** The *Imu* class defines a common interface to IMU sensors. It is templated with the object implementing this interface for the desired sensor. For example, the MPU-9250 implementation may be:

```C++
bfs::Imu<Mpu9250Imu> imu(&SPI, 10);
```

Similar to how a pure virtual class can be used to define an interface using dynamic polymorphism, this approach uses static polymorphism.

**Imu(TwoWire &ast;bus, const int8_t addr)** creates an Imu object that uses an I2C communication interface. A pointer to the I2C bus object is passed along with the I2C address of the sensor.

**Imu(SPIClass &ast;bus, const int8_t cs)** creates an Imu object that uses a SPI communication interface. A pointer to the SPI bus object is passed along with the chip select pin of the sensor.

**bool Init(const ImuConfig &ref)** initializes communication with the sensor and configures it. Returns true if communication is established and configuration was successful.

**bool Read(ImuData &ast; const ptr)** reads data from the sensor. Returns true if new data was received from either the accel, gyro, or mag.
