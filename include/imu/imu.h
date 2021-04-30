/*
* Brian R Taylor
* brian.taylor@bolderflight.com
* 
* Copyright (c) 2021 Bolder Flight Systems Inc
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the “Software”), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*/

#ifndef INCLUDE_IMU_IMU_H_
#define INCLUDE_IMU_IMU_H_

#include <concepts>
#include <optional>
#include "Eigen/Core"
#include "Eigen/Dense"
#include "core/core.h"

namespace bfs {

enum FrameRate : int8_t {
  RATE_200HZ = 0,
  RATE_100HZ = 1,
  RATE_50HZ = 2
};
struct ImuConfig {
  std::optional<TwoWire *> i2c;
  std::optional<SPIClass *> spi;
  int8_t dev;
  FrameRate frame_rate;
  Eigen::Vector3f accel_bias_mps2;
  Eigen::Vector3f mag_bias_ut;
  Eigen::Matrix3f accel_scale;
  Eigen::Matrix3f mag_scale;
  Eigen::Matrix3f rotation;
};
struct ImuData {
  bool new_imu_data;
  bool imu_healthy;
  bool new_mag_data;
  bool mag_healthy;
  Eigen::Vector3f accel_mps2;
  Eigen::Vector3f gyro_radps;
  Eigen::Vector3f mag_ut;
};

template<typename T>
concept Imu = requires(T imu, const ImuConfig &ref, ImuData * const ptr) {
  { imu.Init(ref) } -> std::same_as<bool>;
  { imu.Read(ptr) } -> std::same_as<bool>;
};  // NOLINT - gets confused with concepts and semicolon after braces

}  // namespace bfs

#endif  // INCLUDE_IMU_IMU_H_
