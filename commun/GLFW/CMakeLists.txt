
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

include_directories(src)
include_directories(src/x11)

add_library(glfw2 STATIC ${SOURCE})
target_link_libraries(glfw2 GL GLU X11)