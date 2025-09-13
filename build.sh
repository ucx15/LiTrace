#!/bin/bash
#setup
# Project Dir
#    └---> Obj
#    └---> Src
#    		└--> Include


# -------- BUILD SETTINGS --------
buildAll=true  #for Rebuilding whole project

# Compiler and Linker flags
compilerFlags=("-std=c++20" "-Wall" "-Wextra" "-Werror" "-Wpedantic" "-Wno-missing-field-initializers" "-Wshadow" "-O2")
linkerFlags="-s"

out_file="LiTrace"

srcFiles=("main" "engine" "camera" "color" "light" "material" "ray" "scene" "sphere" "surface" "utils" "vec")

include_dir="Src/Include/"
lib_inc_path="Lib/"

# Create Obj directory if it doesn't exist
mkdir -p Obj
mkdir -p Build

# -------- BUILD SCRIPT --------
# removing previous build
if [ -f "Build/$out_file" ]; then
	rm "Build/$out_file"
fi

# Compiling
isCompiled=true
for srcFile in "${srcFiles[@]}"; do
	isModified=true

	if [ -f "Obj/${srcFile}.o" ]; then
		# Check if source file is newer than object file
		if [ "Src/${srcFile}.cpp" -ot "Obj/${srcFile}.o" ]; then
			isModified=false
		fi
	fi

	# compile if source file is modified
	if [ "$isModified" = true ] || [ "$buildAll" = true ]; then
		sourceModifiedDate=$(stat -c %Y "Src/${srcFile}.cpp" 2>/dev/null || echo "unknown")
		echo "${srcFile}.cpp    $(date -d @$sourceModifiedDate 2>/dev/null || echo "unknown date")"

		# removing previous object file
		rm -f "Obj/${srcFile}.o"

		g++ "${compilerFlags[@]}" -I "$include_dir" -I "$lib_inc_path" -o "Obj/${srcFile}.o" -c "Src/${srcFile}.cpp"

		if [ $? -ne 0 ]; then
			echo "failed to compile!" >&2
			isCompiled=false
		fi
	fi
done

if [ "$isCompiled" = false ]; then
	echo "Compilation failed!"
	exit 1
fi

# Linking
g++ $linkerFlags Obj/*.o -o "Build/$out_file"

if [ $? -ne 0 ]; then
	echo "Linking failed!"
	exit 1
fi

# Running current build
./Build/"$out_file"
