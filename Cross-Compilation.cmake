
SET(CMAKE_SYSTEM_NAME Linux)

SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
if(EXISTS "/usr/bin/arm-linux-gnueabihf-gcc-8")
    set(CMAKE_C_COMPILER /usr/bin/arm-linux-gnueabihf-gcc-8)
elseif(EXISTS "/usr/bin/arm-linux-gnueabihf-gcc-7")
    set(CMAKE_C_COMPILER /usr/bin/arm-linux-gnueabihf-gcc-7)
else()
    message(FATAL_ERROR "No supported version of ARM GCC cross compiler found.")
endif()

if(EXISTS "/usr/bin/arm-linux-gnueabihf-g++-8")
    set(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabihf-g++-8)
elseif(EXISTS "/usr/bin/arm-linux-gnueabihf-g++-7")
    set(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabihf-g++-7)
else()
    message(FATAL_ERROR "No supported version of ARM G++ cross compiler found.")
endif()

# where is the target environment
#SET(CMAKE_FIND_ROOT_PATH /opt/cros/x-tools/arm-unknown-linux-gnueabi)
SET(CMAKE_FIND_ROOT_PATH /usr/arm-linux-gnueabihf)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
