cmake_minimum_required(VERSION 3.16)
project(WarehouseApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(PkgConfig REQUIRED)


pkg_check_modules(PQXX REQUIRED libpqxx)

find_package(FLTK REQUIRED)


set(SOURCES
    src/main.cpp
    src/db/Connection.cpp
    src/repository/PgCategoryRepository.cpp
    src/repository/PgProductRepository.cpp
    src/repository/PgSupplierRepository.cpp
    src/service/CategoryService.cpp
    src/service/ProductService.cpp
    src/service/SupplierService.cpp
    src/ui/MainWindow.cpp
)

add_executable(warehouse ${SOURCES})

target_include_directories(warehouse PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${PQXX_INCLUDE_DIRS}
    ${FLTK_INCLUDE_DIRS}
)

target_link_libraries(warehouse PRIVATE
    ${PQXX_LIBRARIES}
    ${FLTK_LIBRARIES}
)


if(UNIX AND NOT APPLE)
    target_link_libraries(warehouse PRIVATE pthread)
endif()

message(STATUS "libpqxx: ${PQXX_LIBRARIES}")
message(STATUS "FLTK: ${FLTK_LIBRARIES}")