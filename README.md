# imu
Defines a common interface for our Inertial Measurement Unit (IMU) sensors.
   * [License](LICENSE.md)
   * [Changelog](CHANGELOG.md)
   * [Contributing guide](CONTRIBUTING.md)

# Description
This library defines what an *Imu* interface should look like, enabling higher level code to abstract out sensor specifics and design against this interface.

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

This will build the library and an example called *example*, which has source code located in *examples/example.cc*. Notice that the *cmake* command includes a define specifying the microcontroller the code is being compiled for. This is required to correctly configure the code, CPU frequency, and compile/linker options. The available MCUs are:
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

**enum FrameRate** specifies the frame rate for the IMU, which also specifies the flight computer frame rate.

| Enum | Value (int16_t) | Description |
| --- | --- | -- |
| FRAME_RATE_200HZ | 200 | 200 Hz |
| FRAME_RATE_100HZ | 100 | 100 Hz |
| FRAME_RATE_50HZ | 50 | 50 Hz |

**struct ImuConfig** defines a structure used to configure the sensor. The data fields are:

| Name | Description |
| --- | --- |
| int8_t dev | The I2C address or SPI pin |
| int16_t frame_rate | The frame rate the sensor should use |
| std::variant<TwoWire &ast;, SPIClass &ast;> bus | A pointer to the interface used to the communicate with the sensor |
| float accel_bias_mps2[3] | A vector of accelerometer biases, m/s/s |
| float mag_bias_ut[3] | A vector of mag biases, uT |
| float accel_scale[3][3] | A vector of accelerometer scale factors |
| float mag_scale[3][3] | A vector of mag scale factors |
| float rotation[3][3] | Rotation matrix to align sensor data with vehicle frame |

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
| bool new_mag_data | Whether new mag data was read |
| bool imu_healthy | Whether the accel and gyro are healthy |
| bool mag_healthy | Whether the mag is healthy |
| float die_temp_c | The IMU die temperature, C |
| float accel_mps2[3] | The 3-axis accel data, m/s/s |
| float gyro_radps[3] | The 3-axis gyro data, rad/s |
| float mag_ut[3] | The 3-axis mag data, uT |

Health is determined by whether the sensor fails to read 5 times in a row at the expected sampling rate. The output data should be aligned in the direction of the vehicle, as given by the *rotation* configuration data member.

**Imu** Concepts are used to define what an *Imu* compliant object looks like and provide a means to templating against an *Imu* interface. The two required methods are:

**bool Init(const ImuConfig &ref)** This method should receive an *ImuConfig* struct and should establish communication with the IMU sensor, configure the sensor, and zero gyro biases. True is returned on successfully initializing the sensor.

**bool Read(ImuData &ast; const ptr)** This method should get new data from the sensor and return it using a pointer to the *ImuData* struct. True is returned if new IMU data is received.
