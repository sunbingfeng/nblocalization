/**
 * Author: Bill(cocobill1987ATgmail.com)
 * Description: Demo
 * License: see the LICENSE.txt file
 */

#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <netinet/in.h> /* For htonl and ntohl */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <vector>

#include <input/nbdataset.hpp>
#include <input/nbrealtime.hpp>

using namespace std;

static bool isExit = false;
static void intSigintHandler(int sig) {
  isExit = true;
  std::cout << "user exit!" << std::endl;
}

typedef enum SOURCE_TYPE {
  SOURCE_TYPE_OFFLINE,
  SOURCE_TYPE_ONLINE,
} E_SOURCE_TYPE;

int main(int argc, char **argv) {
  if (argc == 2 && strstr(argv[1], "--help")) {
    std::cout << "Usage: " << argv[0] << " <optional>[path]" << std::endl;
    std::cout << "=============" << std::endl;
    std::cout << "datasource type:" << std::endl
              << ">[path] provided, from offline dataset" << std::endl
              << ">otherwise, from realtime capture" << std::endl;
    std::cout << "=============" << std::endl;

    return 0;
  }

  signal(SIGINT, intSigintHandler);
  E_SOURCE_TYPE type;
  std::shared_ptr<nblocalization::input::NBDatasource> psource;

  if (argc == 2) {
    type = SOURCE_TYPE_OFFLINE;
    psource = std::make_shared<nblocalization::input::NBDataset>(
        std::string(argv[1]));
  } else {
    type = SOURCE_TYPE_ONLINE;
    psource = std::make_shared<nblocalization::input::NBRealtime>();
  }

  static_pointer_cast<
      nbobservable::Observable<nblocalization::input::t_imu_callback>>(psource)
      ->registerObserver([](auto imus) {
        std::cout << imus.size() << " imus received!" << std::endl;
      });
  static_pointer_cast<
      nbobservable::Observable<nblocalization::input::t_stereo_callback>>(
      psource)
      ->registerObserver([](auto left, auto right) {
        std::cout << "[" << left.stamp << "] stereo images received!" << std::endl;
      });

  psource->start();
  while (!isExit) {
    // TODO
    usleep(10 * 1000);
  }

  psource->stop();

  return 0;
}
