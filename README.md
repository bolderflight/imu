[![Pipeline](https://gitlab.com/bolderflight/software/imu/badges/main/pipeline.svg)](https://gitlab.com/bolderflight/software/imu/) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

![Bolder Flight Systems Logo](img/logo-words_75.png)

# Imu
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
cmake ..
make
```

This will build the library and an example called *example*, which has source code located in *examples/example.cc*.

## Namespace
This library is within the namespace *bfs*.

## Class / Methods

**struct ImuConfig** defines a structure used to configure the sensor. The data fields are:

| Name | Description |
| --- | --- |
| float accel_bias_mps2[3] | A vector of accelerometer biases, m/s/s |
| float accel_scale[3][3] | A matrix of accelerometer scale factors |
| float rotation[3][3] | Rotation matrix to align sensor data with vehicle frame |

The accel biases and scale factors should be determined offline and input here, they are relatively stable with respect to temperature. Gyro biases are estimated during init and a scale factor is not applied to the gyro data.

The bias and scale factor should be input such that:

```
y = c * x + b
```

Where *y* is the corrected sensor output, *c* is the scale factor matrix, *x* is the uncorrected sensor output, and *b* is the bias vector. An ideal sensor would have a bias vector of zeros and an identity scale factor matrix. 

The rotation matrix is used to align the sensor data with the vehicle frame. This is useful if the sensor cannot be installed aligned with the vehicle axes. The rotation should be used to align the sensor x-axis out the vehicle nose, the y-axis out the right, and the z-axis down.

**struct ImuData** defines a structure of data returned from the sensor. The data fields are:

| Name | Description |
| --- | --- |
| bool new_data | Whether new IMU data was read |
| bool imu_healthy | Whether the IMU is healthy |
| float die_temp_c | The IMU die temperature, C |
| float accel_mps2[3] | The 3-axis accel data, m/s/s |
| float gyro_radps[3] | The 3-axis gyro data, rad/s |

Health is determined by whether the sensor fails to read 5 times in a row at the expected sampling rate. The output data should be aligned in the direction of the vehicle, as given by the *rotation* configuration data member.

**Imu** Concepts are used to define what an *Imu* compliant object looks like and provide a means to templating against an *Imu* interface. The required methods are:

**bool Config(const ImuConfig &ref)** This method should receive an ImuConfig struct and setup the sensor driver configuration. Note that the configuration should be applied in the Init method, this simply checks the configuration for validity and sets up the sensor driver object. True is returned if the config is valid, otherwise false if returned.

**ImuData imu_data()** This method returns the ImuData from the last successful Read.
