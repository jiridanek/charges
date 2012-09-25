cmake_minimum_required(VERSION 2.8)
project(charges++)

set(VERBOSE)

# IF(WIN32)
#     SET(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
# ELSE(WIN32)
#     SET(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
# ENDIF(WIN32)

add_executable(charges++ undefinedeemparameter.cpp invalidparamline.cpp eemparams.cpp eemsolver.cpp main.cpp)
include(FindPkgConfig)

# http://openbabel.org/api/2.3/cmake_project.shtml
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

find_package(OpenBabel2 REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

include_directories(${OPENBABEL2_INCLUDE_DIR} ${EIGEN3_INCLUDE_DIR} ${Boost_INCLUDE_DIR} )
link_directories(${OPENBABEL2_LIBRARY_DIR} ${QTTEST_LIBRARY_DIRS} ${BOOST_LIBRARYDIR})

target_link_libraries(charges++ ${OPENBABEL2_LIBRARIES} "-Wl,-Bstatic" ${Boost_LIBRARIES} "-Wl,-Bdynamic" )

#něco jako unit_testy
#find_package(Qt4)
if (${QT_FOUND})
  SET(QT_USE_QTTEST TRUE)
  include(${QT_USE_FILE})

  QT4_WRAP_CPP(test_eemparams_mmc test_eemparams.hpp)
  QT4_WRAP_CPP(test_convert_mmc test_convert.hpp)

  add_executable(test_eemparams ${test_eemparams_mmc} eemparams.cpp invalidparamline.cpp)
  add_executable(test_convert ${test_convert_mmc} convert.hpp)

  target_link_libraries(test_eemparams ${OPENBABEL2_LIBRARIES} ${QT_LIBRARIES})
  target_link_libraries(test_convert ${QT_LIBRARIES})

  ENABLE_TESTING()
  ADD_TEST(eemparams test_eemparams)
  ADD_TEST(convert test_convert)

  add_custom_target(runtests ALL DEPENDS test_eemparams test_convert)

  add_custom_command(TARGET runtests POST_BUILD
		    COMMAND ctest --output-on-failure)
endif (${QT_FOUND})

