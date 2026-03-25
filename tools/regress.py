#
# OpenDMI: Cross-platform DMI/SMBIOS framework
# Copyright (c) 2025-2026, The OpenDMI contributors
#
# SPDX-License-Identifier: BSD-3-Clause
#
import os
import getopt
import subprocess
import sys
import yaml

from yaml.loader import SafeLoader
from jsonschema import validate

tools_dir  = os.path.dirname(os.path.realpath(__file__))
base_dir   = os.path.abspath(os.path.join(tools_dir, '..'))

build_dir  = os.path.join(base_dir, "build/opendmi")
schema_dir = os.path.join(base_dir, "opendmi/share/opendmi")
data_dir   = os.path.join(base_dir, "data")

schema = None

def data_files():
    for root, _, files in os.walk(data_dir):
        for file_name in files:
            file_path = os.path.join(root, file_name)

            file_base, file_ext = os.path.splitext(file_path)
            if file_ext != ".bin":
                continue

            yield file_path, file_base + ".yaml"

def load_yaml(path: str):
    try:
        with open(path) as file:
            return yaml.load(file, Loader=SafeLoader)
    except FileNotFoundError:
        print(f"ERROR: File not found: ${path}")
        return None

def load_dump(dump_path: str):
    try:
        process = subprocess.Popen(
            [f"{build_dir}/bin/opendmi", "--file", dump_path, "export", "--all", "--format=yaml"],
            stdout = subprocess.PIPE,
            stderr = subprocess.PIPE
        )
        stdout, stderr = process.communicate()

        return yaml.load(stdout, Loader=SafeLoader)
    except subprocess.CalledProcessError as e:
        print("Unable to read SMBIOS dump:")
        print(e.stderr)

def generate_spec(dump_path: str, spec_path: str):
    print(f"Generating: {dump_path} -> {spec_path}")

    data = load_dump(dump_path)
    validate(data, schema)

    try:
        with open(spec_path, "w+") as file:
            yaml.dump(data, file, sort_keys=False)
    except OSError as e:
        print("Unable to write specification:")
        print(e.stderr)

def check_spec(dump_path: str, spec_path: str):
    if not os.path.isfile(spec_path):
        print(f"Skipping: {dump_path} (no spec)")
        return

    print(f"Checking spec: {dump_path}")
    spec = load_yaml(spec_path)
    data = load_dump(dump_path)

def show_usage():
    pass

def main(argv) -> int:
    global schema

    mode = "test"

    try:
        opts, args = getopt.getopt(argv, "hb", ["help", "baseline"])
    except getopt.GetoptError as e:
        print(str(e))
        print('Use -h or --help for help')
        sys.exit(os.EX_USAGE)

    for opt, arg in opts:
        if opt in ("-h", "--help"):
            show_usage()
            sys.exit(0)
        elif opt in ("-b", "--baseline"):
            mode = "generate"

    yaml.SafeLoader.yaml_implicit_resolvers = {
        k: [r for r in v if r[0] != 'tag:yaml.org,2002:timestamp'] for
            k, v in yaml.SafeLoader.yaml_implicit_resolvers.items()
    }

    print("Loading schema...")
    schema = load_yaml(os.path.join(schema_dir, "opendmi.yml"))
    if schema == None:
        return os.EX_OSFILE

    for dump_path, spec_path in data_files():
        if mode == "generate":
            generate_spec(dump_path, spec_path)
        else:
            check_spec(dump_path, spec_path)

    return os.EX_OK

if __name__ == "__main__":
	main(sys.argv[1:])
