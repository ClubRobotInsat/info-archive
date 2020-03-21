set(MODULE_SOURCES
        Module.hpp
        ModuleManager.cpp
        Webcam.cpp
        IO.cpp
        Motors.cpp
        Navigation.cpp
        NavigationParameters.cpp
        Pumps.cpp
        Servos.cpp)

add_library(Modules STATIC ${MODULE_SOURCES})
target_link_libraries(Modules Outils_Robot)

if(RASPI)
    file(GLOB files "${PROJECT_SOURCE_DIR}/precompiled-libraries/opencv/*.so")
    foreach(file ${files})
        get_filename_component(ocv_lib ${file} NAME_WE)
       #message(STATUS "lib: ${ocv_lib}")
        #message(STATUS "file: ${file}")
        add_library(${ocv_lib} SHARED IMPORTED)
        set_target_properties(${ocv_lib} PROPERTIES IMPORTED_LOCATION "${file}")
        target_link_libraries(Modules ${ocv_lib})

    endforeach()

   # file(GLOB files "${PROJECT_SOURCE_DIR}/precompiled-libraries/opencv/*.a")
   # foreach(file ${files})
    #    message(STATUS "Static Lib: ${ocv_lib}")
    #    target_link_libraries(Modules ${file})
    #endforeach()


    message(STATUS "done")
else()
    find_package( OpenCV REQUIRED )
    target_link_libraries(Modules ${OpenCV_LIBS})
    message(STATUS "${OpenCV_LIBS}")
endif()

#find_package( OpenCV REQUIRED CONFIG
#        NO_DEFAULT_PATH
#        PATHS ../../../precompiled-libraries/build_hardfp/)


