import os
from os.path import join

from decoder import ExceptionDataParser, parse_args, AddressResolver, print_result, PLATFORMS

Import("env")


def decode(*args, **kwargs):
    stack_only = False
    full = True

    file = open(".pioenvs/stack.dump", "r")

    join("$BUILD_DIR", "firmware")

    addr2line = os.path.join(os.path.abspath(os.path.expanduser("~/.platformio/packages/toolchain-xtensa32")),
                             "bin/xtensa-" + PLATFORMS["ESP8266"] + "-elf-addr2line")
    if not os.path.exists(addr2line):
        print("ERROR: addr2line not found (" + addr2line + ")")

    elf_file = os.path.abspath(os.path.expanduser(join("$BUILD_DIR", "firmware.elf")))
    if not os.path.exists(elf_file):
        print("ERROR: elf file not found (" + elf_file + ")")

    parser = ExceptionDataParser()
    resolver = AddressResolver(addr2line, elf_file)

    parser.parse_file(file, stack_only)
    resolver.fill(parser)

    print_result(parser, resolver, full, stack_only)


env.AlwaysBuild(env.Alias("decode", None, decode))
