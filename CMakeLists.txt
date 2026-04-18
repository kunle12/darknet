cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0146 OLD)
project(darknet_objects_detection LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

#thread
FIND_PACKAGE(Threads)
list(APPEND LIBRARIES ${CMAKE_THREAD_LIBS_INIT})

#OpenMP
FIND_PACKAGE(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# CUDA
if (NOT CPUOnly)
FIND_PACKAGE(CUDA)
endif()

if (CUDA_FOUND)
    message(STATUS "CUDA Version: " ${CUDA_VERSION_STRING})
    message(STATUS "CUDA Libararies: " ${CUDA_LIBRARIES})
    include_directories(SYSTEM ${CUDA_INCLUDE_DIRS})
    add_library(cudnn SHARED IMPORTED)
    set_target_properties(cudnn PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libcudnn.so)

    list(APPEND LIBRARIES ${CUDA_LIBRARIES} ${CUDA_CUBLAS_LIBRARIES} ${CUDA_curand_LIBRARY} ${CUDA_cusparse_LIBRARY})
    list(APPEND CUDA_NVCC_FLAGS "-std=c++17;-O2;-Xcompiler \"-fPIC\" ")
    list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_61,code=compute_61 ")
    list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_70,code=compute_70 ")
    list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_75,code=compute_75 ")
    list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_80,code=compute_80 ")
    list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_86,code=compute_86 ")
    set(CUDA_PROPAGATE_HOST_FLAGS OFF)
    add_definitions(-DGPU -DCUDNN)
    cuda_include_directories(src)
else()
    list(APPEND LIBRARIES "m")
endif()

#BOOST
FIND_PACKAGE(OpenCV REQUIRED)

add_definitions(-DOPENCV)

include_directories(SYSTEM include ${OpenCV_INCLUDE_DIR})

include_directories(src)
set(SRC_DIR src)

FILE(GLOB SRC_FILES ${SRC_DIR}/*.c ${SRC_DIR}/*.cpp)
FILE(GLOB CU_SRC_FILES ${SRC_DIR}/*.cu)
LIST(REMOVE_ITEM SRC_FILES "${PROJECT_SOURCE_DIR}/src/demo.c")

set_source_files_properties(${SRC_FILES} PROPERTIES LANGUAGE CXX)

if (CUDA_FOUND)
message(STATUS "CUDA FOUND")
cuda_add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${CU_SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES} ${OpenCV_LIBS} cudnn)
else ()
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES} ${OpenCV_LIBS})
endif()

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include/darknet
            FILES_MATCHING PATTERN "*.h")
