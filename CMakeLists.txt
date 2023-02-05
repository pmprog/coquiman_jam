cmake_minimum_required(VERSION 3.1)

set(PROJECT_TITLE coquiman CACHE STRING "Project's name")

project(${PROJECT_TITLE} C)


list(APPEND CMAKE_PREFIX_PATH "." ".." "../..")

find_path(KIT_ROOT_DIR kit/include/kit.h)

# Add game source code folders here
aux_source_directory(./src SRC_LIST)
aux_source_directory(./resources SRC_LIST)
aux_source_directory(${KIT_ROOT_DIR}/kit/src.a5 SRC_LIST)
aux_source_directory(${KIT_ROOT_DIR}/kit/src.common SRC_LIST)

add_definitions(-DTARGET_ALLEGRO)

find_path(ALLEGRO_INCLUDE_DIR allegro5/allegro.h)
find_library(ALLEGRO_LIB liballegro.so allegro.lib PATHS thirdparty/allegro/build/native/v142/x64/lib)
find_library(ALLEGRO_COLOUR_LIB liballegro_color.so allegro_color.lib PATHS thirdparty/allegro/build/native/v142/x64/lib)
find_library(ALLEGRO_PRIMITIVES_LIB liballegro_primitives.so allegro_primitives.lib PATHS thirdparty/allegro/build/native/v142/x64/lib)
find_library(ALLEGRO_AUDIO_LIB liballegro_audio.so allegro_audio.lib PATHS thirdparty/allegro/build/native/v142/x64/lib)
find_library(ALLEGRO_ACODEC_LIB liballegro_acodec.so allegro_acodec.lib PATHS thirdparty/allegro/build/native/v142/x64/lib)
find_library(ALLEGRO_IMAGE_LIB liballegro_image.so allegro_image.lib PATHS thirdparty/allegro/build/native/v142/x64/lib)
find_library(ALLEGRO_FONT_LIB liballegro_font.so allegro_font.lib PATHS thirdparty/allegro/build/native/v142/x64/lib)
find_library(ALLEGRO_TTF_LIB liballegro_ttf.so allegro_ttf.lib PATHS thirdparty/allegro/build/native/v142/x64/lib)

find_library(ENET_LIB libenet.so libenet.a enet.lib PATHS thirdparty/enet/build)
if(MSVC)
        set(EXTRALIB1 winmm.lib)
        set(EXTRALIB2 ws2_32.lib)
else()
        set(EXTRALIB1 m)
endif()

# Build the executable
add_executable(${PROJECT_TITLE} ${SRC_LIST})

target_include_directories(${PROJECT_TITLE} PRIVATE ${KIT_ROOT_DIR}/kit/include ${ALLEGRO_INCLUDE_DIR} thirdparty/enet/include)

target_link_libraries(${PROJECT_TITLE}
        ${ENET_LIB}
        ${ALLEGRO_LIB}
        ${ALLEGRO_COLOUR_LIB}
        ${ALLEGRO_PRIMITIVES_LIB}
        ${ALLEGRO_AUDIO_LIB}
        ${ALLEGRO_ACODEC_LIB}
        ${ALLEGRO_IMAGE_LIB}
        ${ALLEGRO_FONT_LIB}
        ${ALLEGRO_TTF_LIB}
        ${EXTRALIB1}
        ${EXTRALIB2}
        )
