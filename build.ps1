#setup
# Project Dir
#    └---> Obj
#    └---> Src
#    		└--> Include


# -------- BUILD SETTINGS --------
$build_all   = $true  #for Rebuilding whole project
$build_specific = ""

$C_FLAGS = "-Wall", "-Wextra", "-pedantic", "-std=c++20", "-s", "-O3", "-ffast-math"

$out_file = "LiTrace.exe"
$main_files = "Src/main.cpp", "src/engine.cpp"
$include_dir = "Src/Include/"
$lib_inc_path = "Lib/"



# -------- BUILD SCRIPT --------
# removing previous build
if (Test-Path ./$out_file) {
	Remove-Item $out_file
}


# Compiling
[string[]]$src_files = (Get-ChildItem Src/ -File).BaseName

if ($build_all) {
	Write-Output "Building: ALL"
	foreach ($file in $src_files) {

		if ("Src/${file}.cpp" -notin $main_files) {
			Write-Output "    ${file}.cpp"

			if (Test-Path "Obj/${file}.o") {
				Remove-Item Obj/${file}.o
			}

			g++ $C_FLAGS  -I $include_dir -I $lib_inc_path -o Obj/${file}.o -c Src/${file}.cpp
		}
	}
}

elseif ($build_specific) {
	Write-Output "Building: SPECIFIC"
	foreach ($file in $build_specific) {
		Write-Output "    ${file}.cpp"

		if (Test-Path "Obj/${file}.o") {
			Remove-Item Obj/${file}.o
		}

		g++ $C_FLAGS  -I $include_dir -I $lib_inc_path -o Obj/${file}.o -c Src/${file}.cpp

	}
}
else {
	Write-Output "Building: NONE"
}

# Linking
$obj_files = Get-ChildItem -Path Obj/

Write-Output "Linking"
g++ $main_files $obj_files $C_FLAGS -o $out_file -I $include_dir 


# Running current build

if (Test-Path ./$out_file) {
	Write-Output "Build Successfully"
	Write-Output ""
	& ./$out_file
}
else {
	Write-Output "ERROR in Building!"
}
