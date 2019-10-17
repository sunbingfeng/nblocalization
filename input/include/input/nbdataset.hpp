/**
 * Author: Bill(cocobill1987ATgmail.com)
 * Description: Class related to offline dataset
 * License: see the LICENSE.txt file
 */
#ifndef NBLOCALIZATION_DATASET_H_
#define NBLOCALIZATION_DATASET_H_

#include <condition_variable>
#include <mutex>
#include <thread>

#include <input/nbdatasource.hpp>

namespace nblocalization {
namespace input {

class NBDataset : public NBDatasource {
public:
  NBDataset(const std::string &path) : m_path(path){
    std::cout << "NBDataset construct with path: " << m_path << std::endl;
  };
  ~NBDataset(){};

  virtual bool start() override {
    mp_thread_readimu = std::thread(&NBDataset::readImuLoop, this);
    mp_thread_readimg = std::thread(&NBDataset::readImgLoop, this);

    return true;
  };

  virtual bool isEnded() override { return is_ended_imu && is_ended_img; };

  void pause() override {
    std::unique_lock<std::mutex> lk(m_mutex_state);
    m_c_loop_continue = 0;
  }

  void step() override {
    std::unique_lock<std::mutex> lk(m_mutex_state);
    m_c_loop_continue = 1;
    m_cv_state.notify_one();
  }

  void backToRunning() override {
    std::unique_lock<std::mutex> lk(m_mutex_state);
    m_c_loop_continue = std::numeric_limits<long int>::max();
    m_cv_state.notify_one();
  }

protected:
  bool is_ended_imu = true;
  bool is_ended_img = true;

  std::mutex m_mutex_state;
  std::condition_variable m_cv_state;
  long int m_c_loop_continue = 0;

  virtual void readImuLoop() override {
    while (!is_exit_requested && !is_ended_imu) {
      // Step 1, read imu data from offline dataset
      std::vector<IMUData> imus;
      // TODO: parse and insert to imus vector

      // Step 2, notify observers.
      nbobservable::Observable<t_imu_callback>::notifyObservers(imus);
    }
  }

  virtual void readImgLoop() override {
    while (!is_exit_requested && !is_ended_img) {
      // Step 1, check debug state
      {
        std::unique_lock<std::mutex> lk(m_mutex_state);
        m_cv_state.wait(lk, [&] { return m_c_loop_continue > 0; });
        m_c_loop_continue--;
      }
      // Step 2, load stereo images from offline dataset
      CameraData left, right;
      // TODO: parse and initialize left/right mat data

      // Step 3, notify observers
      nbobservable::Observable<t_stereo_callback>::notifyObservers(left, right);
    }
  }

private:
  std::string m_path;
};
} // namespace input
} // namespace nblocalization
#endif // NBLOCALIZATION_DATASET_H_