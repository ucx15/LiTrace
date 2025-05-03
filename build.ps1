#setup
# Project Dir
#    └---> Obj
#    └---> Src
#    		└--> Include


# -------- BUILD SETTINGS --------
$buildAll   = $false  #for Rebuilding whole project

# Compiler and Linker flags
$compilerFlags = "-std=c++20", "-Wall", "-Wextra", "-Werror", "-Wpedantic","-Wno-missing-field-initializers", "-Wshadow", "-O2"
$linkerFlags = "-s"


$out_file = "LiTrace.exe"

$srcFiles = "main", "engine", "camera", "color", "light", "material", "ray", "scene", "sphere", "surface", "utils", "vec"

$include_dir = "Src/Include/"
$lib_inc_path = "Lib/"



# -------- BUILD SCRIPT --------
# removing previous build
if (Test-Path ./$out_file) {
	Remove-Item $out_file
}


# Compiling
$isCompiled = $true
foreach ($srcFile in $srcFiles) {

	$isModified = $true
	$sourceModifiedDate = (Get-Item "Src/${srcFile}.cpp").LastWriteTime
	if ( Test-Path "Obj/${srcFile}.o" ) {
		$targetModifiedDate = (Get-Item "Obj/${srcFile}.o").LastWriteTime
		$isModified = ($sourceModifiedDate -gt $targetModifiedDate)
	}

	# compile if source file is modified
	if (($isModified -eq $true) -or ($buildAll -eq $true)) {
		Write-Output "${srcFile}.cpp    ${sourceModifiedDate}"

		# removing previous object file
		Remove-Item Obj/${srcFile}.o  -Force -ErrorAction SilentlyContinue

		g++ $compilerFlags  -I $include_dir -I $lib_inc_path -o Obj/${srcFile}.o -c Src/${srcFile}.cpp


		if ($LASTEXITCODE -ne 0) {
			Write-Error "failed to compile!"
			$isCompiled = $false
		}
	}
}

if ($isCompiled -eq $false) {
	Write-Host "Compilation failed!"
	exit 1
}

# Linking
g++ $linkerFlags Obj/*.o -o $out_file

if ($LASTEXITCODE -ne 0) {
	Write-Host "Linking failed!"
	exit 1
}


# Running current build
& ./$out_file
