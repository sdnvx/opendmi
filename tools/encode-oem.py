import sys
import re

from functools import reduce
from itertools import zip_longest
from more_itertools import split_before, split_at

file_path = "./decoded/IdeaPadFlex-5-14ITL05-82HS.txt"

def main(argv):
    pattern = re.compile(r"^Handle")

    with open(file_path, "r") as file:
        lines = file.readlines()

    lines = map(lambda line: line.strip(), lines)
    lines = filter(lambda line: len(line) > 0, lines)

    sections = split_before(lines, lambda line: pattern.match(line))
    sections = filter(
        lambda section: pattern.match(section[0]) and "OEM-specific Type" in section[1],
        sections
    )
    sections = map(
        lambda section: dict(
            zip_longest(
                ("header", "data", "strings"),
                split_at(section, lambda line: "Header and Data:" in line or "Strings:" in line),
                fillvalue=[]
            )
        ),
        sections
    )

    def prepare_header(data):
        rv = list([item.strip() for item in data[0].split(",")])
        return {
            "handle": int(re.search(r'0x[0-9a-fA-F]+', rv[0]).group(0), 16),
            "type":   int(re.search(r'\d+', rv[1]).group(0)),
            "length": int(re.search(r'\d+', rv[2]).group(0)),
        }

    sections = map(
        lambda section: section.update({
            "header":  prepare_header(section["header"]),
            "data":    bytearray.fromhex("".join(section["data"])),
        }) or section,
        sections
    )


    print(list(sections))

if __name__ == "__main__":
    main(sys.argv[1:])
