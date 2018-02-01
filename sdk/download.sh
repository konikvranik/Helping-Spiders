#!/bin/sh

HOST="x86_64-pc-linux-gnu"
[ -z "$HOST" ] && HOST="i686-mingw32"
[ -z "$ARDUINO_CDT" ] && ARDUINO_CDT="${HOME}/.arduinocdt"
TMP="$ARDUINO_CDT/downloads"
mkdir -p "$TMP"

download_newer() {
	#curl -sLf -z "$2" -o "$2" "$1"
	curl -sLf -C - -o "$2" "$1"
}

unpack() {
	bsdtar -xf "$1" -C "$2" --strip-components=1 #-s'|[^/]*/||'
}

download_platform_esp8266() {
	download_newer http://arduino.esp8266.com/versions/2.3.0/package_esp8266com_index.json $TMP/package_esp8266com_index.json
	jq -carj ".packages[] | .name as \$name | .tools as \$tools | .platforms[] | select( ( .name | contains( \"$2\" ) ) and ( .version | contains( \"$3\" ) ) ) | (\"PLATFORM='\",\$name,\"' NAME='\",(.name | gsub(\" \";\"_\")),\"' VERSION='\",.version,\"' URL='\",.url,\"' ARCHIVE='\",.archiveFileName,\"' TOOLS='\",( .toolsDependencies[] | . as \$dep | (\$tools[] | select( .name == \$dep.name and .version == \$dep.version ) | .systems[] | select( .host == \"$1\") | . + { name: \$dep.name, version: \$dep.version } ) ),\"'\n\")" $TMP/package_esp8266com_index.json | while read a; do
		eval "$a"
		ARCHIVE="$TMP/$ARCHIVE"
		download_newer "$URL" "$ARCHIVE"
		TARGET="${ARDUINO_CDT}/packages/$PLATFORM"
		PLATFORM_DIR="$TARGET/hardware/$NAME/$VERSION"
		mkdir -p "$PLATFORM_DIR"
		unpack "$ARCHIVE" "$PLATFORM_DIR"
		echo "installed platform $NAME ($VERSION)"
		echo "$TOOLS" | jq -carj "(\"NAME='\",(.name | gsub(\" \";\"_\")),\"' VERSION='\",.version,\"' ARCHIVE='\",.archiveFileName,\"' URL='\",.url,\"'\n\")" | while read b; do
			eval "$b"
			ARCHIVE="$TMP/$ARCHIVE"
			download_newer "$URL" "$ARCHIVE"
			TOOLDIR="$TARGET/tools/$NAME/$VERSION"
			mkdir -p "$TOOLDIR"
			unpack "$ARCHIVE" "$TOOLDIR"
			echo "installed tool $NAME ($VERSION)"
		done
	done
}

search_platform() {
	download_newer http://arduino.esp8266.com/versions/2.3.0/package_esp8266com_index.json $TMP/package_esp8266com_index.json
	jq ".packages[] | .platforms[] | select( ( .name | contains( \"$1\" ) ) and ( .version | contains( \"$2\" ) ) )" $TMP/package_esp8266com_index.json
}

download_platform() {
	download_platform_esp8266 $@
}

search_library() {
	download_newer http://downloads.arduino.cc/libraries/library_index.json $TMP/library_index.json
	#jq -carj ".libraries[]" $TMP/library_index.json
	jq  ".libraries[] | select( ( .name | contains(\"$1\") ) and ( .version | contains(\"$2\") ) ) " $TMP/library_index.json
}

download_library() {
	download_newer http://downloads.arduino.cc/libraries/library_index.json $TMP/library_index.json
	jq -carj ".libraries[] | select( ( .name | contains(\"$1\") ) and ( .version | contains(\"$2\") ) ) | (\"URL='\",.url,\"' NAME='\",(.name | gsub(\" \";\"_\")),\"' VERSION='\",.version,\"' ARCHIVE='\",.archiveFileName,\"'\n\")" $TMP/library_index.json | while read a; do
		eval "$a"
		ARCHIVE=$TMP/$ARCHIVE
		TARGET=${ARDUINO_CDT}/libraries/$NAME/$VERSION
		download_newer "$URL" "$ARCHIVE"
		mkdir -p $TARGET
		unpack "$ARCHIVE" "$TARGET"
		echo "installed library $NAME ($VERSION)" 
	done
}

help() {
		echo "download [-p|-h] {search|install} NAME VERSION"
}

if [ -z "$*" ]; then
	help
	exit 2
fi
type=library
command=$1
shift
if [ "$command" = "-p" ]; then
	type=platform
	command=$1
	shift
fi
if [ "$command" = "-h" ]; then
	type=help
fi
case $type in
	library)
		case $command in
			search) search_library $@;;
			install) download_library $@;;
			*) help;;
		esac
	;;
	platform)
		case $command in
			search) search_platform $@;;
			install) download_platform $HOST $@;;
			*) help;;
		esac
	;;
	*)
		help
	;;
esac
