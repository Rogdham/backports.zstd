cmake_minimum_required(VERSION 3.15...3.26)
project(${SKBUILD_PROJECT_NAME} LANGUAGES C)

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported LANGUAGES C CXX)
if (ipo_supported)
  set_property(TARGET _zstd PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

find_package(Python REQUIRED COMPONENTS Development.Module)

set(ZSTD_LEGACY_SUPPORT OFF)
set(ZSTD_MULTITHREAD_SUPPORT_DEFAULT ON)
set(ZSTD_BUILD_PROGRAMS OFF)
set(ZSTD_BUILD_CONTRIB OFF)
set(ZSTD_BUILD_TESTS OFF)
set(ZSTD_ENABLE_CXX OFF)
set(ZSTD_BUILD_STATIC ON)
set(ZSTD_BUILD_SHARED OFF)

# zstd recommends an out of tree build
# https://github.com/facebook/zstd/blob/dev/build/cmake/README.md
add_subdirectory(src/c/zstd/build/cmake
                 src/c/zstd/build/cmake/builddir
                 # IMPORTANT: do not run default install targets so we don't
                 # install cmake/header files from libzstd
                 EXCLUDE_FROM_ALL)

file(GLOB CompatSources src/c/compat/*.[ch])
file(GLOB ModuleSources src/c/compression_zstd/*.[ch])

python_add_library(_zstd MODULE
    WITH_SOABI
    ${CompatSources}
    ${ModuleSources}
)

target_include_directories(_zstd PRIVATE
                           src/c/compat
                           src/c/compression_zstd
                           src/c/compression_zstd/clinic)

target_link_libraries(_zstd PRIVATE libzstd_static)

install(TARGETS _zstd DESTINATION backports/zstd)
