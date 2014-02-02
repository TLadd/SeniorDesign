cmake_minimum_required(VERSION 2.8)
set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.53.0 COMPONENTS serialization thread system)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})

  find_package( OpenCV )
  include_directories( ${OpenCV_INCLUDE_DIRS} )


  
  add_executable(RandomFor RandomFor.cpp FeatureProjector.cpp Forest.cpp HistogramHelper.cpp HistogramMatrix.cpp ImageReader.cpp NodeFactory.cpp SerializeHelper.cpp TerminalNode.cpp TreeNode.cpp TripletWrapper.cpp FeatureProjector.h Forest.h HistogramHelper.h HistogramMatrix.h ITreeNode.h ImageReader.h NodeFactory.h SerializeHelper.h TerminalNode.h TreeNode.h TripletWrapper.h)
  target_link_libraries(RandomFor ${Boost_LIBRARIES})
  target_link_libraries(RandomFor ${OpenCV_LIBS} )
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
