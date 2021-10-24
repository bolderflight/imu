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

namespace bfs {

struct ImuConfig {
  float accel_bias_mps2[3];
  float accel_scale[3][3];
  float rotation[3][3];
};
struct ImuData {
  bool new_data;
  bool healthy;
  float die_temp_c;
  float accel_mps2[3];
  float gyro_radps[3];
};

template<typename T>
concept Imu = requires(T imu, const ImuConfig &ref) {
  { imu.Config(ref) } -> std::same_as<bool>;
  { imu.imu_data() } -> std::same_as<ImuData>;
};  // NOLINT - gets confused with concepts and semicolon after braces

}  // namespace bfs

#endif  // INCLUDE_IMU_IMU_H_
