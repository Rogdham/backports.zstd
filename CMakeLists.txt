cmake_minimum_required(VERSION 3.15...3.26)
project(${SKBUILD_PROJECT_NAME} LANGUAGES C)

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error
                    LANGUAGES C CXX)

find_package(Python REQUIRED COMPONENTS 
             Development.Module)


# TODO(emmatyping): build as apple framework?
#set(ZSTD_FRAMEWORK OFF)
set(ZSTD_LEGACY_SUPPORT OFF)
# Multithreading support may get overridden per platform, e.g. Android
set(ZSTD_MULTITHREAD_SUPPORT_DEFAULT ON)
set(ZSTD_BUILD_PROGRAMS OFF)
set(ZSTD_BUILD_CONTRIB OFF)
set(ZSTD_BUILD_TESTS OFF)
set(ZSTD_ENABLE_CXX OFF)
set(ZSTD_BUILD_STATIC ON)
set(ZSTD_BUILD_SHARED OFF)

add_subdirectory(src/c/zstd/build/cmake
                 # The zstd cmake file recommends an out of tree build
                 src/c/zstd/build/cmake/builddir
                 # IMPORTANT: do not run default install targets so we don't
                 # install cmake/header files from libzstd
                 EXCLUDE_FROM_ALL)

python_add_library(_zstd MODULE
    WITH_SOABI

    src/c/compat/backports_zstd_compat_py310.c
    src/c/compat/backports_zstd_redef.c
    src/c/compression_zstd/buffer.h
    src/c/compression_zstd/_zstdmodule.h
    src/c/compression_zstd/compressor.c
    src/c/compression_zstd/zstddict.c
    src/c/compression_zstd/_zstdmodule.c
    src/c/compression_zstd/decompressor.c
)

if (ipo_supported)
  set_property(TARGET _zstd PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

target_include_directories(_zstd PRIVATE src/c/compat)
target_include_directories(_zstd PRIVATE src/c/compression_zstd/clinic)
target_include_directories(_zstd PRIVATE src/c/compression_zstd)

target_link_libraries(_zstd PRIVATE libzstd_static)

install(TARGETS _zstd DESTINATION backports/zstd/)
