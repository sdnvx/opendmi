# OpenDMI: Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.2] - Unreleased

- Added displaying structure types in handle arrays
- Unified date handling via `dmi_date_t` type:
  - Firmware strcture now uses `dmi_date_t` for release date
  - Portable battery now used `dmi_date_t` for manufacture date
  - Removed field `sbds_manufacture_date` from portable battery descriptor
- Added manpages for `dmi_bswap16()`, `dmi_bswap32()` and `dmi_bswap64()` functions

## [0.1.1] - Unreleased

- Fixed baseboard features set decoding.
- Fixed incorrect decoding of incomplete structures:
  - system information (`dmi_system_t`);
  - processor (`dmi_processor_t`);
  - memory device (`dmi_memory_device_t`);
  - system enclosure or chassis (`dmi_chassis_t`).
- Fixed incorrect address display formats:
  - firmware BIOS segment address;
  - memory array stating & ending addresses;
  - memory device stating & ending addresses;
  - memory error address;
  - management device address.
- Fixed formatting signed integers of size less than sizeof(int).

## [0.1] - January 02, 2026

- First public release.
- Full support of SMBIOS specification up to version 3.9.
- Basic implementation of `libopendmi` library with backends for `Linux`, `MacOS` and dump files.
- Basic implementation of `opendmi` command line tool with XML and YAML output formats and color output support.
