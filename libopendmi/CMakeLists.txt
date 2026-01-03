#
# OpenDMI: Cross-platform DMI/SMBIOS framework
# Copyright (c) 2025-2026, The OpenDMI contributors
#
# SPDX-License-Identifier: BSD-3-Clause
#
project(opendmi-libopendmi)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

configure_file(
    ${PROJECT_SOURCE_DIR}/src/config.h.in
    ${PROJECT_SOURCE_DIR}/src/config.h
    USE_SOURCE_PERMISSIONS)

add_library(opendmi-object OBJECT)
target_include_directories(opendmi-object PUBLIC include)
target_sources(opendmi-object
    PRIVATE
        src/name.c
        src/error.c
        src/log.c
        src/context.c
        src/entry.c
        src/entity.c
        src/filter.c
        src/attribute.c
        src/registry.c
        src/utils.c
        src/utils/datetime.c
        src/utils/endian.c
        src/utils/uuid.c
        src/utils/version.c
        src/entity/additional-info.c
        src/entity/baseboard.c
        src/entity/battery.c
        src/entity/bis-entry-point.c
        src/entity/cache.c
        src/entity/chassis.c
        src/entity/common.c
        src/entity/cooling-device.c
        src/entity/current-probe.c
        src/entity/firmware.c
        src/entity/firmware-inventory.c
        src/entity/firmware-language.c
        src/entity/group-assoc.c
        src/entity/hardware-security.c
        src/entity/ipmi-device.c
        src/entity/memory-array.c
        src/entity/memory-array-addr.c
        src/entity/memory-channel.c
        src/entity/memory-controller.c
        src/entity/memory-device.c
        src/entity/memory-device-addr.c
        src/entity/memory-error.c
        src/entity/memory-error-32.c
        src/entity/memory-error-64.c
        src/entity/memory-module.c
        src/entity/mgmt-controller.c
        src/entity/mgmt-device.c
        src/entity/mgmt-device-component.c
        src/entity/mgmt-device-threshold.c
        src/entity/oem-strings.c
        src/entity/onboard-device.c
        src/entity/onboard-device-ex.c
        src/entity/oob-remote-access.c
        src/entity/pointing-device.c
        src/entity/port-connector.c
        src/entity/power-controls.c
        src/entity/power-supply.c
        src/entity/probe.c
        src/entity/processor.c
        src/entity/processor-ex.c
        src/entity/slot.c
        src/entity/string-property.c
        src/entity/system.c
        src/entity/system-boot.c
        src/entity/system-config.c
        src/entity/system-event-log.c
        src/entity/system-reset.c
        src/entity/temperature-probe.c
        src/entity/tpm-device.c
        src/entity/voltage-probe.c
        src/extension/apple.c
        src/extension/dell.c
        src/extension/intel.c
        src/backend/dump.c
    PUBLIC INTERFACE
        FILE_SET HEADERS
            BASE_DIRS include
            FILES
                include/opendmi/attribute.h
                include/opendmi/backend.h
                include/opendmi/context.h
                include/opendmi/defs.h
                include/opendmi/entry.h
                include/opendmi/error.h
                include/opendmi/extension.h
                include/opendmi/filter.h
                include/opendmi/keyword.h
                include/opendmi/log.h
                include/opendmi/name.h
                include/opendmi/registry.h
                include/opendmi/entity.h
                include/opendmi/types.h
                include/opendmi/utils.h
                include/opendmi/value.h
                include/opendmi/utils/datetime.h
                include/opendmi/utils/endian.h
                include/opendmi/utils/uuid.h
                include/opendmi/utils/version.h
                include/opendmi/backend/dump.h
                include/opendmi/entity/additional-info.h
                include/opendmi/entity/baseboard.h
                include/opendmi/entity/battery.h
                include/opendmi/entity/bis-entry-point.h
                include/opendmi/entity/cache.h
                include/opendmi/entity/chassis.h
                include/opendmi/entity/common.h
                include/opendmi/entity/cooling-device.h
                include/opendmi/entity/current-probe.h
                include/opendmi/entity/device.h
                include/opendmi/entity/firmware-inventory.h
                include/opendmi/entity/firmware-language.h
                include/opendmi/entity/firmware.h
                include/opendmi/entity/group-assoc.h
                include/opendmi/entity/hardware-security.h
                include/opendmi/entity/ipmi-device.h
                include/opendmi/entity/memory-array-addr.h
                include/opendmi/entity/memory-array.h
                include/opendmi/entity/memory-channel.h
                include/opendmi/entity/memory-controller.h
                include/opendmi/entity/memory-device-addr.h
                include/opendmi/entity/memory-device.h
                include/opendmi/entity/memory-error.h
                include/opendmi/entity/memory-error-32.h
                include/opendmi/entity/memory-error-64.h
                include/opendmi/entity/memory-module.h
                include/opendmi/entity/mgmt-controller.h
                include/opendmi/entity/mgmt-device.h
                include/opendmi/entity/mgmt-device-component.h
                include/opendmi/entity/mgmt-device-threshold.h
                include/opendmi/entity/oem-strings.h
                include/opendmi/entity/onboard-device.h
                include/opendmi/entity/oob-remote-access.h
                include/opendmi/entity/pointing-device.h
                include/opendmi/entity/port-connector.h
                include/opendmi/entity/power-controls.h
                include/opendmi/entity/power-supply.h
                include/opendmi/entity/probe.h
                include/opendmi/entity/processor.h
                include/opendmi/entity/processor-ex.h
                include/opendmi/entity/slot.h
                include/opendmi/entity/string-property.h
                include/opendmi/entity/system.h
                include/opendmi/entity/system-boot.h
                include/opendmi/entity/system-config.h
                include/opendmi/entity/system-event-log.h
                include/opendmi/entity/system-reset.h
                include/opendmi/entity/temperature-probe.h
                include/opendmi/entity/tpm-device.h
                include/opendmi/entity/voltage-probe.h
                include/opendmi/extension/apple.h
                include/opendmi/extension/dell.h
                include/opendmi/extension/intel.h
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_definitions(opendmi-object PRIVATE __linux__)
    target_sources(opendmi-object
        PRIVATE
            src/backend/linux.c
        PUBLIC INTERFACE
            FILE_SET HEADERS
                BASE_DIRS include
                FILES include/opendmi/backend/linux.h
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_compile_definitions(opendmi-object PRIVATE __APPLE__)

    find_library(COREFOUNDATION_LIB CoreFoundation)
    find_library(IOKIT_LIB IOKit)
    list(APPEND LIBOPENDMI_LIBRARIES ${COREFOUNDATION_LIB} ${IOKIT_LIB})

    target_sources(opendmi-object
        PRIVATE
            src/backend/darwin.c
        PUBLIC INTERFACE
            FILE_SET HEADERS
                BASE_DIRS include
                FILES include/opendmi/backend/darwin.h
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    target_compile_definitions(opendmi-object PRIVATE __FreeBSD__)

    target_sources(opendmi-object
        PRIVATE
            src/backend/freebsd.c
        PUBLIC INTERFACE
            FILE_SET HEADERS
                BASE_DIRS include
                FILES include/opendmi/backend/freebsd.h
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(opendmi-object PRIVATE _WIN32)

    target_sources(opendmi-object
        PRIVATE
            src/backend/windows.c
        PUBLIC INTERFACE
            FILE_SET HEADERS
                BASE_DIRS include
                FILES include/opendmi/backend/windows.h
    )
endif()

if(NOT MSVC)
    target_compile_options(opendmi-object PRIVATE -fPIC)
endif()

if(CPPCHECK_EXECUTABLE)
    set(CPPCHECK_ARGS
        --library=posix
        --check-library
        --inline-suppr
        --enable=all
    )

    set_target_properties(opendmi-object PROPERTIES
        C_CPPCHECK ${CPPCHECK_EXECUTABLE} ${CPPCHECK_ARGS}
    )
endif()

add_library(opendmi-static STATIC $<TARGET_OBJECTS:opendmi-object>)
target_include_directories(opendmi-static INTERFACE include)
target_link_libraries(opendmi-static PUBLIC ${LIBOPENDMI_LIBRARIES})
set_target_properties(opendmi-static PROPERTIES OUTPUT_NAME opendmi)

add_library(opendmi-shared SHARED $<TARGET_OBJECTS:opendmi-object>)
target_include_directories(opendmi-shared INTERFACE include)
target_link_libraries(opendmi-shared PUBLIC ${LIBOPENDMI_LIBRARIES})
set_target_properties(opendmi-shared PROPERTIES OUTPUT_NAME opendmi)

install(TARGETS opendmi-object FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS opendmi-static ARCHIVE DESTINATION lib)
install(TARGETS opendmi-shared LIBRARY DESTINATION lib)

macro(opendmi_test name sources)
    add_executable(${name}-test ${sources})
    target_link_libraries(${name}-test opendmi-shared)
    add_test(NAME ${name} COMMAND ${name}-test)
    set_property(TEST ${name} PROPERTY SKIP_RETURN_CODE 69)
endmacro()

opendmi_test(bswap16 test/bswap16.c)
opendmi_test(bswap32 test/bswap32.c)
opendmi_test(bswap64 test/bswap64.c)

opendmi_test(hton16 test/hton16.c)
opendmi_test(hton32 test/hton32.c)
opendmi_test(hton64 test/hton64.c)

opendmi_test(ntoh16 test/ntoh16.c)
opendmi_test(ntoh32 test/ntoh32.c)
opendmi_test(ntoh64 test/ntoh64.c)

opendmi_test(ipow32 test/ipow32.c)
opendmi_test(ipow64 test/ipow64.c)

opendmi_test(word-decode test/word-decode.c)
opendmi_test(dword-decode test/dword-decode.c)
opendmi_test(qword-decode test/qword-decode.c)
opendmi_test(bcd-decode test/bcd-decode.c)
opendmi_test(uuid-decode test/uuid-decode.c)
opendmi_test(uuid-encode test/uuid-encode.c)

opendmi_test(attribute-format-handle test/attribute-format-handle.c)
opendmi_test(attribute-format-string test/attribute-format-string.c)
opendmi_test(attribute-format-bool test/attribute-format-bool.c)
opendmi_test(attribute-format-integer test/attribute-format-integer.c)
opendmi_test(attribute-format-decimal test/attribute-format-decimal.c)
opendmi_test(attribute-format-size test/attribute-format-size.c)
opendmi_test(attribute-format-address test/attribute-format-address.c)
opendmi_test(attribute-format-enum test/attribute-format-enum.c)
opendmi_test(attribute-format-set test/attribute-format-set.c)
opendmi_test(attribute-format-version test/attribute-format-version.c)
opendmi_test(attribute-format-uuid test/attribute-format-uuid.c)

opendmi_test(status-name test/status-name.c)
opendmi_test(error-correct-type-name test/error-correct-type-name.c)
opendmi_test(baseboard-type-name test/baseboard-type-name.c)
opendmi_test(chassis-type-name test/chassis-type-name.c)
opendmi_test(chassis-security-status-name test/chassis-security-status-name.c)
opendmi_test(cache-type-name test/cache-type-name.c)
opendmi_test(cache-mode-name test/cache-mode-name.c)
opendmi_test(cache-assoc-name test/cache-assoc-name.c)
opendmi_test(cache-location-name test/cache-location-name.c)
opendmi_test(cache-size test/cache-size.c)
opendmi_test(cache-size-ex test/cache-size-ex.c)
opendmi_test(onboard-device-type-name test/onboard-device-type-name.c)
opendmi_test(memory-array-location-name test/memory-array-location-name.c)
opendmi_test(memory-array-usage-name test/memory-array-usage-name.c)
opendmi_test(memory-interleave-name test/memory-interleave-name.c)
opendmi_test(memory-channel-type-name test/memory-channel-type-name.c)
opendmi_test(memory-device-size test/memory-device-size.c)
opendmi_test(memory-device-size-ex test/memory-device-size-ex.c)
opendmi_test(memory-error-type-name test/memory-error-type-name.c)
opendmi_test(memory-error-granularity-name test/memory-error-granularity-name.c)
opendmi_test(memory-error-operation-name test/memory-error-operation-name.c)
opendmi_test(probe-location-name test/probe-location-name.c)
opendmi_test(battery-chemistry-name test/battery-chemistry-name.c)
opendmi_test(slot-usage-name test/slot-usage-name.c)
opendmi_test(slot-width-name test/slot-width-name.c)
