from platformio import util
from SCons.Script import AlwaysBuild
from decoder import ExceptionDataParser
from decoder import AddressResolver
from decoder import print_result
Import("env")

config = util.load_project_config()

def decode():
    args = parse_args()

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

    parser.parse_file(file, args.stack_only)
    resolver.fill(parser)

    print_result(parser, resolver, args.full, args.stack_only)

AlwaysBuild(env.Alias("decode", "", decode))
