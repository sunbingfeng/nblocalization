/**
 * Author: Bill(cocobill1987ATgmail.com)
 * Description: Base data source class
 * License: see the LICENSE.txt file
 */
#ifndef NBLOCALIZATION_DATASOURCE_H_
#define NBLOCALIZATION_DATASOURCE_H_

#include <thread>

#include <input/common.hpp>
#include <nbobservable/nbobservable.hpp>

namespace nblocalization {
namespace input {

typedef void t_imu_callback(const std::vector<IMUData> &imus);
typedef void t_stereo_callback(const CameraData &left, const CameraData &right);

class NBDatasource
    : public nbobservable::Observable<t_imu_callback>,
      public nbobservable::Observable<t_stereo_callback> {
public:
  NBDatasource() { std::cout << "NBDatasource construct!" << std::endl; };
  ~NBDatasource(){};

  virtual bool start() = 0;

  virtual void stop() {
    is_exit_requested = true;

    if (mp_thread_readimu.joinable()) {
      mp_thread_readimu.join();
    }

    if (mp_thread_readimg.joinable()) {
      mp_thread_readimg.join();
    }

    nbobservable::Observable<void(
        const std::vector<IMUData> &imus)>::clearAllObservers();
    nbobservable::Observable<void(
        const CameraData &left, const CameraData &right)>::clearAllObservers();

    is_exit = true;
  }

  virtual bool isEnded() { return false; }

  //******* operations for debug use: pause or run by one-step
  virtual void pause() {}
  virtual void step() {}
  virtual void backToRunning() {}
  // debug operations *********************************//

protected:
  bool is_exit_requested = false;
  bool is_exit = false;

  std::thread mp_thread_readimu;
  std::thread mp_thread_readimg;

  virtual void readImuLoop() = 0;
  virtual void readImgLoop() = 0;
};
} // namespace input
} // namespace nblocalization
#endif // NBLOCALIZATION_DATASOURCE_H_
