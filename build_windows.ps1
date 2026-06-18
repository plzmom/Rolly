Param(
    [string]$BuildDir = "build",
    [string]$Config = "Release",
    [string]$Arch = "x64",
    [string]$VcpkgToolchain = ""
)

if ($VcpkgToolchain -ne "") {
    $toolchainArg = "-DCMAKE_TOOLCHAIN_FILE=`"$VcpkgToolchain`""
} else {
    $toolchainArg = ""
}

Write-Output "BuildDir=$BuildDir Config=$Config Arch=$Arch"
New-Item -ItemType Directory -Path $BuildDir -Force | Out-Null
Push-Location $BuildDir

$cmakeCmd = "cmake .. -A $Arch -DCMAKE_BUILD_TYPE=$Config $toolchainArg"
Write-Output "Running: $cmakeCmd"
Invoke-Expression $cmakeCmd

Write-Output "Building..."
cmake --build . --config $Config

Pop-Location
Write-Output "Build finished. Executable likely at: $BuildDir\$Config\app.exe or $BuildDir\app.exe"
