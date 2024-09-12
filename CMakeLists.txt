cmake_minimum_required(VERSION 3.14)
project(blinky LANGUAGES C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(NRF5_TARGET "nrf51822_xxaa" CACHE STRING "")
set(NRF5_BOARD "BOARD_CUSTOM" CACHE STRING "")
set(NRF5_CONFIG_DIR "${CMAKE_SOURCE_DIR}/config" CACHE STRING "")
set(NRF5_SDK_PATH "${CMAKE_SOURCE_DIR}/../nrf51SDK/nRF5_SDK_12.3.0_d7731ad" CACHE STRING "")
set(NRF5_SOFTDEVICE "s130" CACHE STRING "")
set(NRF5_LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/config/s130_gcc_nrf51.ld)
add_compile_options(-DUSE_APP_CONFIG)
include(cmake/nrf51-cmake/nrf5.cmake)

add_executable(main 
    src/main.c)

nrf5_setup_exe(main)
target_link_libraries(main PRIVATE
    nrf5_softdevice_s130
    nrf5_nrf51822_base
    nrf5_driver_clock
    nrf5_driver_delay
    nrf5_driver_gpiote
    nrf5_library_bsp
    # nrf5_library_bsp_btn_ble
    # nrf5_library_timer
    # nrf5_library_timersh
    nrf5_library_log
    # nrf5_library_fds
    # nrf5_library_fstorage
    nrf5_library_hardfault
    # nrf5_library_sensorsim
    # nrf5_ble_common
    nrf5_ble_ble_advertising
    # nrf5_ble_peer_manager
    # nrf5_ble_service_hids
    # nrf5_ble_service_bas
    # nrf5_ble_service_dis
)

nrf5_create_genexdebug(main)
# nrf5_create_genexdebug(nrf5_nrf51822_base)

# get_target_property(inc test INCLUDE_DIRECTORIES)
# message(STATUS "inc: ${inc}")
