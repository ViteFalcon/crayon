cmake_minimum_required(VERSION 3.23)
project(crayon CXX C)

set(CMAKE_CXX_STANDARD 23)
add_executable(${PROJECT_NAME})

if (UNIX)
    target_compile_options(${PROJECT_NAME} PUBLIC "-fcoroutines")
    find_package(Threads REQUIRED)
elseif (WIN32)
    target_compile_definitions(${PROJECT_NAME} PUBLIC _WIN32_WINNT=0x0601 SOL_ALL_SAFETIES_ON=1)
endif ()
target_compile_features(${PROJECT_NAME} PUBLIC c_std_99)
target_compile_definitions(${PROJECT_NAME} PUBLIC ASIO_HAS_BOOST_DATE_TIME)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/game/precompiled/kaitai_struct.h)

find_package(asio CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS date_time iostreams program_options)
find_package(flecs CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_path(HFSM2_INCLUDE_DIRS "hfsm2/machine.hpp")
find_package(raylib CONFIG REQUIRED)
find_package(serdepp CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

add_subdirectory(dependencies)

set(KS_GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(MAKE_DIRECTORY "${KS_GEN_DIR}")
if (WIN32)
	set(KS_CMD kaitai-struct-compiler.bat)
else()
	set(KS_CMD kaitai-struct-compiler)
endif()

set(KS_SRCS
    "${KS_GEN_DIR}/server_packets.cpp"
    "${KS_GEN_DIR}/server_packets.h"
)

add_custom_target("${PROJECT_NAME}-ks" DEPENDS "${KS_GEN_DIR}/server_packets.cpp" "${KS_GEN_DIR}/server_packets.h")

add_custom_command(
	OUTPUT
        ${KS_SRCS}
	COMMAND
        ${KS_CMD}
		-t cpp_stl
		--cpp-standard 11
		-d "${KS_GEN_DIR}"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/game/network/server_packets.ksy"
    VERBATIM
	DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/game/network/server_packets.ksy"
)

# Get Sources
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.c**)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS src/*.h**)
target_sources(${PROJECT_NAME}
    PRIVATE
    ${KS_SRCS}
    ${HEADERS}
    ${SOURCES}
)

set(LIBS
    ${Boost_LIBRARIES}
    Boost::date_time
    Boost::iostreams
    flecs::flecs_static
    fmt::fmt
    raylib
    serdepp::serdepp
    sol2
    spdlog::spdlog
    kaitai_struct_cpp_stl_runtime
    yaml-cpp
)
if (UNIX)
    list(APPEND LIBS Threads::Threads pthread)
elseif (WIN32)
    # Required for RayLib's HighRes timer
    list(APPEND LIBS winmm.lib)
endif ()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CRAYON_BUILD_TYPE "debug")
else()
    set(CRAYON_BUILD_TYPE "release")
endif()
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CRAYON_ARCH "x86_64")
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(CRAYON_ARCH "x86_32")
else ()
    set(CRAYON_ARCH "unknown_arch")
endif ()
set(CRAYON_BIN_DIR "bin")
set(CRAYON_BIN_ARCH_SUFFIX ".${CRAYON_ARCH}")

SET(CRAYON_DEPENDENCIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies")
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${HFSM2_INCLUDE_DIRS}"
    "${CRAYON_DEPENDENCIES_DIR}/kaitai_struct"
    "${KS_GEN_DIR}"
    "${CRAYON_DEPENDENCIES_DIR}/raygui/src"
)
target_link_libraries(${PROJECT_NAME} PUBLIC -static ${LIBS})
