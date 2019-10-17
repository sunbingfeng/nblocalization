/**
 * Author: Bill(cocobill1987ATgmail.com)
 * Description: common type defintions
 * License: see the LICENSE.txt file
 */
#ifndef NBLOCALIZATION_INPUT_COMMON_H_
#define NBLOCALIZATION_INPUT_COMMON_H_

#include <string>

#include <Eigen/Geometry>
#include <opencv2/core/core.hpp>

namespace nblocalization {
namespace input {
struct IMUData {
  // monotonic time from system startup, in the unit of second
  double stamp;

  Eigen::Vector3d angular_velocity;
  Eigen::Vector3d linear_acceleration;
};

struct CameraData {
  // monotonic time from system startup, in the unit of second
  double stamp;

  // name correlated to which camera, such as left/right, etc.
  std::string name;
  cv::Mat image;

  CameraData() {}

  CameraData(double st, std::string nm, cv::Mat img) : stamp(st), name(nm) {
    image = img.clone();
  }

  CameraData(const CameraData &other) {
    stamp = other.stamp;
    name = other.name;

    image = other.image.clone();
  }

  CameraData& operator=(const CameraData &other) {
    stamp = other.stamp;
    name = other.name;

    image = other.image.clone();

    return *this;
  }
};

} // namespace input
} // namespace nblocalization

#endif // NBLOCALIZATION_INPUT_COMMON_H_