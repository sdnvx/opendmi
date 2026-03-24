# OpenDMI

OpenDMI is a cross-platform, commercial-grade DMI/SMBIOS framework, focused on functionality and ease of use. It provides direct access to all of the DMI/SMBIOS data, command line tools, bindings for major programming languages and even more.

The project is under active development, see [ROADMAP](ROADMAP.md) and [CHANGELOG](CHANGELOG.md) for details.

## Highlights

* Platform-agnostic access to DMI/SMBIOS structures.
* Full SMBIOS support up to version 3.9.
* Bindings for Python, Go and Rust languages.
* Modular extensions for handling OEM-specific structures.
* JSON, XML and YAML output support for automation purposes.
* Works on Linux, MacOS, BSD and Windows platforms.
* Small footprint, no external dependencies.

## Components

* `opendmi` - Command line tool to query DMI/SMBIOS data
* `opendmi-dbus` - D-bus service providing access to DMI/SMBIOS data
* `libopendmi` - C library providing direct interface to DMI/SMBIOS
* `libopendmi-go` - Go bindings for `libopendmi`
* `libopendmi-python` - Python bindings for `libopendmi`
* `libopendmi-rust` - Rust bindings for `libopendmi`

## Building from sources

### Prerequisites

* GCC, CLang or MSVC compiler
* CMake 3.25 or newer
* AsciiDoctor (used to generate documentation and man pages)
* CppCheck (optional)

### Dependencies

#### opendmi

* `libxml2` — optional, only when XML support is enabled
* `libyaml` — optional, only when YAML support is enabled
* `libyajl` — optional, only when JSON support is enabled

#### libopendmi

* None

### Configuring

Use the following command to configure OpenDMI build:

```sh
$ ./build.sh configure
```

This produces a debug build by default. Optimization isn't enabled, and debug
assertions are included. Pass `--release` to configure a release build:

```sh
$ ./build.sh configure --release
```

For advanced configuration options, see usage:

```sh
$ ./build.sh --help
```

### Building

Use the following command to build OpenDMI:

```sh
$ ./build.sh build
```

### Testing

OpenDMI uses the CTest framework for testing. You can simply run it to ensure
that build was successful:

```sh
$ ./build.sh test
```

## Contributing

Please feel free to test it and create bug reports or feature requests
on GitHub. You can also help the project by sending SMBIOS dumps to the
authors. To get the dump you can use the following command:

```sh
$ dmidecode --dump-bin <filename>
```

See [CONTRIBUTING](CONTRIBUTING.md) for more details.

## Contributors

* [Dmitry Sednev](https://github.com/sdnvx)
* [Dmitry Kuzmenko](https://github.com/xerrial)
* [Ritesh Kudkelwar](https://github.com/ritesh006)
* [Jorn](https://github.com/jornissimo)

## Licensing

OpenDMI is licensed under BSD 3-clause license. See `LICENSE` for details.

## Links

[![Hypercommit](https://img.shields.io/badge/Hypercommit-DB2475)](https://hypercommit.com/opendmi)
