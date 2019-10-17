PROJECT(input)
set(CMAKE_CXX_STANDARD 14)

# Set the minimum version of cmake required to build this project
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

find_package(Boost REQUIRED)
find_package(OPENCV REQUIRED)
find_package(Eigen3 REQUIRED)

add_executable(demo main.cpp)
target_include_directories(demo PUBLIC 
                                include 
                                ${CMAKE_SOURCE_DIR}/../thirdparty/nbobservable/include
                                ${Boost_INCLUDE_DIRS}
                                ${OpenCV_INCLUDE_DIRS}
                                ${EIGEN3_INCLUDE_DIR}
                            )
target_link_libraries(demo ${Boost_LIBRARIES} ${OpenCV_LIBS})
