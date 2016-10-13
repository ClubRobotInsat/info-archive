
SET(CMAKE_SYSTEM_NAME Linux)

SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
#SET(CMAKE_C_COMPILER /opt/cros/x-tools/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-gcc )
SET(CMAKE_C_COMPILER /usr/bin/arm-linux-gnueabihf-gcc-5)

#SET(CMAKE_CXX_COMPILER /opt/cros/x-tools/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-g++ )
SET(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabihf-g++-5)

# where is the target environment
#SET(CMAKE_FIND_ROOT_PATH /opt/cros/x-tools/arm-unknown-linux-gnueabi)
SET(CMAKE_FIND_ROOT_PATH /usr/arm-linux-gnueabihf)


# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
