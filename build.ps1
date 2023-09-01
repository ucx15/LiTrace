#setup
# Project Dir
#    └---> Obj
#    └---> Src
#    		└--> Include


# -------- BUILD SETTINGS --------
$Rebuild_All   = $false  #for Rebuilding whole project
$specific_builds = ""


$C_FLAGS = "-Wall", "-Wextra", "-pedantic", "-std=c++20", "-s", "-O3", "-ffast-math"


$OutFile_Name = "LiTrace.exe"
$main_files = "Src/main.cpp", "src/engine.cpp"
$include_dir = "Src/Include/"



# -------- BUILD SCRIPT --------
# removing previous build
if (Test-Path ./$OutFile_Name) {
	Remove-Item $OutFile_Name
}


# Compiling

[string[]]$src_files = (Get-ChildItem Src/ -File).BaseName

if ($Rebuild_All) {
	Write-Output "Building: ALL"
	foreach ($file in $src_files) {

		if ("Src/${file}.cpp" -notin $main_files) {
			Write-Output "    ${file}.cpp"

			if (Test-Path "Obj/${file}.o") {
				Remove-Item Obj/${file}.o
			}

			g++ $C_FLAGS  -I $include_dir -o Obj/${file}.o -c Src/${file}.cpp
		}
	}
}

elseif ($specific_builds) {
	Write-Output "Building: SPECIFIC"
	foreach ($file in $specific_builds) {
		Write-Output "    ${file}.cpp"

		if (Test-Path "Obj/${file}.o") {
			Remove-Item Obj/${file}.o
		}

		g++ $C_FLAGS  -I $include_dir -o Obj/${file}.o -c Src/${file}.cpp

	}
}
else {
	Write-Output "Building: NONE"
}

# Linking
$obj_files = Get-ChildItem -Path Obj/

Write-Output "Linking"
g++ $main_files $obj_files $C_FLAGS -o $OutFile_Name -I $include_dir 


# Running current build

if (Test-Path ./$OutFile_Name) {
	Write-Output "Build Successfully"
	Write-Output ""
	& ./$OutFile_Name
}
else {
	Write-Output "ERROR in Building!"
}
