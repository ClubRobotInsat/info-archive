set(SOURCE
	src/enable.c
	src/fullscreen.c
	src/glext.c
	src/image.c
	src/init.c
	src/input.c
	src/joystick.c
	src/stream.c
	src/tga.c
	src/thread.c
	src/time.c
	src/window.c)

if(APPLE)
	include_directories(src/cocoa)
	set(SOURCE ${SOURCE}
		src/cocoa/cocoa_enable.m
		src/cocoa/cocoa_fullscreen.m
		src/cocoa/cocoa_glext.m
		src/cocoa/cocoa_init.m
		src/cocoa/cocoa_joystick.m
		src/cocoa/cocoa_thread.c
		src/cocoa/cocoa_time.m
		src/cocoa/cocoa_window.m)
elseif(UNIX)
	include_directories(src/x11)
	link_directories(/usr/X11R6/lib)
	set(SOURCE ${SOURCE}
		src/x11/x11_enable.c
		src/x11/x11_fullscreen.c
		src/x11/x11_glext.c
		src/x11/x11_init.c
		src/x11/x11_joystick.c
		src/x11/x11_keysym2unicode.c
		src/x11/x11_thread.c
		src/x11/x11_time.c
		src/x11/x11_window.c)
else()
	include_directories(src/win32)
	set(SOURCE ${SOURCE}
		src/win32/win32_dllmain.c
		src/win32/win32_enable.c
		src/win32/win32_fullscreen.c
		src/win32/win32_glext.c
		src/win32/win32_init.c
		src/win32/win32_joystick.c
		src/win32/win32_thread.c
		src/win32/win32_time.c
		src/win32/win32_window.c)
endif()


include_directories(src)

add_library(glfw_${SIMU_OU_ROBOT} STATIC ${SOURCE})

if(APPLE)
	target_compile_options(glfw_${SIMU_OU_ROBOT} PUBLIC -Wno-deprecated -Wno-deprecated-declarations)

	find_library(COCOA Cocoa)
	find_library(OPENGL OpenGL)
	find_library(GLUT GLUT)
	find_library(IOKIT IOKit)
target_link_libraries(glfw_${SIMU_OU_ROBOT} m pthread ${COCOA} ${OPENGL} ${GLUT} ${IOKIT})
elseif(UNIX)
	target_link_libraries(glfw_${SIMU_OU_ROBOT} GL GLU m X11 pthread Xrandr)
else()
	target_link_libraries(glfw_${SIMU_OU_ROBOT} opengl32 glu32)
endif()
