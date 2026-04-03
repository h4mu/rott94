<#
.SYNOPSIS
    Builds ROTT for Windows with flexible configuration options.
#>

[CmdletBinding()]
param(
    [ValidateSet('Shareware', 'SuperROTT', 'SiteLicense', 'Full')]
    [string]$BuildType = 'Full',

    [ValidateSet('Debug', 'Release')]
    [string]$Configuration = 'Release',

    [ValidateSet('Win32', 'x64')]
    [string]$Platform = 'Win32',

    [string]$SourceDir = $null,

    [string]$BuildDir = 'build',

    [string]$OutputDir = 'Windows\out',

    [string]$Suffix = $null,

    [switch]$CleanBuild   # Add this switch to force full clean
)

$ErrorActionPreference = 'Stop'

# Paths
$ScriptDir   = $PSScriptRoot
$ProjectRoot = Split-Path -Parent $ScriptDir
if (-not $SourceDir) {
    $SourceDir = $ProjectRoot
}

function Resolve-PathRelativeTo {
    param(
        [string]$Path,
        [string]$BasePath
    )

    if ([System.IO.Path]::IsPathRooted($Path)) {
        return [System.IO.Path]::GetFullPath($Path)
    }

    return [System.IO.Path]::GetFullPath((Join-Path $BasePath $Path))
}

function Test-SamePath {
    param(
        [string]$Left,
        [string]$Right
    )

    return ([System.IO.Path]::GetFullPath($Left).TrimEnd('\') -ieq [System.IO.Path]::GetFullPath($Right).TrimEnd('\'))
}

function Get-CMakeVersion {
    $versionLine = (& cmake --version | Select-Object -First 1)
    if ($versionLine -notmatch 'cmake version ([0-9]+\.[0-9]+\.[0-9]+)') {
        throw "Unable to determine CMake version from: $versionLine"
    }

    return [version]$matches[1]
}

function Initialize-VisualStudioEnvironment {
    param(
        [string]$TargetPlatform
    )

    if (Get-Command cl -ErrorAction SilentlyContinue) {
        return
    }

    $vsWhereCandidates = @(
        'C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe',
        'C:\Program Files\Microsoft Visual Studio\Installer\vswhere.exe'
    )

    $vsWhere = $vsWhereCandidates | Where-Object { Test-Path $_ } | Select-Object -First 1
    if (-not $vsWhere) {
        throw 'Visual Studio tools are not on PATH and vswhere.exe could not be found.'
    }

    $vsInstallRoot = & $vsWhere -latest -products * -requires Microsoft.Component.MSBuild -property installationPath
    if (-not $vsInstallRoot) {
        throw 'Unable to locate a Visual Studio installation with MSBuild support.'
    }

    $vsDevCmd = Join-Path $vsInstallRoot 'Common7\Tools\VsDevCmd.bat'
    if (-not (Test-Path $vsDevCmd)) {
        throw "VsDevCmd.bat not found at: $vsDevCmd"
    }

    $arch = switch ($TargetPlatform) {
        'x64' { 'x64' }
        default { 'x86' }
    }
    $hostArch = if ([IntPtr]::Size -eq 8) { 'x64' } else { 'x86' }

    Write-Host "Initializing Visual Studio environment from: $vsDevCmd" -ForegroundColor Yellow
    $envLines = & cmd /c "call `"$vsDevCmd`" -arch=$arch -host_arch=$hostArch -no_logo >nul && set"
    foreach ($line in $envLines) {
        if ($line -match '^([^=]+)=(.*)$') {
            Set-Item -Path "Env:$($matches[1])" -Value $matches[2]
        }
    }
}

$SourceDirResolved = Resolve-PathRelativeTo -Path $SourceDir -BasePath $ProjectRoot
$BuildDirResolved  = Resolve-PathRelativeTo -Path $BuildDir  -BasePath $SourceDirResolved
$OutputDirResolved = Resolve-PathRelativeTo -Path $OutputDir -BasePath $ProjectRoot

Write-Host "=== ROTT Windows Build ===" -ForegroundColor Cyan
Write-Host "Project Root  : $ProjectRoot"
Write-Host "SourceDir     : $SourceDirResolved"
Write-Host "BuildDir      : $BuildDirResolved"
Write-Host "OutputDir     : $OutputDirResolved"

$CMakeVersion = Get-CMakeVersion
Write-Host "CMake Version : $CMakeVersion"

Initialize-VisualStudioEnvironment -TargetPlatform $Platform

if (-not (Test-Path (Join-Path $SourceDirResolved "CMakeLists.txt"))) {
    throw "CMakeLists.txt not found at: $SourceDirResolved"
}

# Clean the build tree if requested
if ($CleanBuild) {
    if (Test-SamePath -Left $BuildDirResolved -Right $SourceDirResolved) {
        throw "Refusing to clean BuildDir because it matches SourceDir: $BuildDirResolved"
    }
    if (Test-SamePath -Left $OutputDirResolved -Right $SourceDirResolved) {
        throw "Refusing to clean OutputDir because it matches SourceDir: $OutputDirResolved"
    }

    Write-Host "Performing clean build..." -ForegroundColor Yellow
    Remove-Item (Join-Path $SourceDirResolved "CMakeCache.txt") -Force -ErrorAction SilentlyContinue
    Remove-Item (Join-Path $SourceDirResolved "CMakeFiles") -Recurse -Force -ErrorAction SilentlyContinue
    if (Test-Path $BuildDirResolved) {
        Remove-Item $BuildDirResolved -Recurse -Force
    }
    if (Test-Path $OutputDirResolved) {
        Remove-Item $OutputDirResolved -Recurse -Force
    }
} elseif (Test-Path (Join-Path $BuildDirResolved "CMakeCache.txt")) {
    Write-Host "Cleaning stale CMake cache..." -ForegroundColor Yellow
    Remove-Item (Join-Path $BuildDirResolved "CMakeCache.txt") -Force -ErrorAction SilentlyContinue
    Remove-Item (Join-Path $BuildDirResolved "CMakeFiles") -Recurse -Force -ErrorAction SilentlyContinue
}

# Create directories
New-Item -ItemType Directory -Force -Path $BuildDirResolved | Out-Null
New-Item -ItemType Directory -Force -Path $OutputDirResolved | Out-Null

# Configure
Write-Host "Running CMake configure..." -ForegroundColor Green

$configureArgs = @(
    "-A", $Platform,
    "-DCMAKE_BUILD_TYPE=$Configuration"
)

switch ($BuildType) {
    'Shareware' {
        $configureArgs += @("-DROTT_SHAREWARE=ON", "-DROTT_SUPERROTT=OFF", "-DROTT_SITELICENSE=OFF")
        if (-not $Suffix) { $Suffix = "-huntbgin" }
    }
    'SuperROTT' {
        $configureArgs += @("-DROTT_SHAREWARE=OFF", "-DROTT_SUPERROTT=ON", "-DROTT_SITELICENSE=OFF")
        if (-not $Suffix) { $Suffix = "-rottcd" }
    }
    'SiteLicense' {
        $configureArgs += @("-DROTT_SHAREWARE=OFF", "-DROTT_SUPERROTT=OFF", "-DROTT_SITELICENSE=ON")
        if (-not $Suffix) { $Suffix = "-sitelicense" }
    }
    'Full' {
        $configureArgs += @("-DROTT_SHAREWARE=OFF", "-DROTT_SUPERROTT=OFF", "-DROTT_SITELICENSE=OFF")
        if (-not $Suffix) { $Suffix = "-darkwar" }
    }
}

if ($Suffix) {
    $configureArgs += "-DROTT_SUFFIX=$Suffix"
}

if ($CMakeVersion -lt [version]'3.13.0') {
    $configureArgs = @(
        "-G", "NMake Makefiles",
        "-DCMAKE_BUILD_TYPE=$Configuration"
    )

    switch ($BuildType) {
        'Shareware' {
            $configureArgs += @("-DROTT_SHAREWARE=ON", "-DROTT_SUPERROTT=OFF", "-DROTT_SITELICENSE=OFF")
        }
        'SuperROTT' {
            $configureArgs += @("-DROTT_SHAREWARE=OFF", "-DROTT_SUPERROTT=ON", "-DROTT_SITELICENSE=OFF")
        }
        'SiteLicense' {
            $configureArgs += @("-DROTT_SHAREWARE=OFF", "-DROTT_SUPERROTT=OFF", "-DROTT_SITELICENSE=ON")
        }
        'Full' {
            $configureArgs += @("-DROTT_SHAREWARE=OFF", "-DROTT_SUPERROTT=OFF", "-DROTT_SITELICENSE=OFF")
        }
    }

    if ($Suffix) {
        $configureArgs += "-DROTT_SUFFIX=$Suffix"
    }

    Write-Host "cmake $SourceDirResolved -G NMake Makefiles $($configureArgs -join ' ')" -ForegroundColor Gray
    Push-Location $BuildDirResolved
    try {
        & cmake $SourceDirResolved @configureArgs
        if ($LASTEXITCODE -ne 0) {
            throw "CMake configuration failed with exit code $LASTEXITCODE"
        }
    } finally {
        Pop-Location
    }
} else {
    $modernConfigureArgs = @(
        "-S", $SourceDirResolved,
        "-B", $BuildDirResolved,
        "-G", "Visual Studio 15 2017"
    ) + $configureArgs

    Write-Host "cmake $($modernConfigureArgs -join ' ')" -ForegroundColor Gray

    & cmake @modernConfigureArgs

    if ($LASTEXITCODE -ne 0) {
        throw "CMake configuration failed with exit code $LASTEXITCODE"
    }
}

# Build
Write-Host "Building project..." -ForegroundColor Green
& cmake --build $BuildDirResolved --config $Configuration --target rott

if ($LASTEXITCODE -ne 0) {
    throw "CMake build failed with exit code $LASTEXITCODE"
}

# Copy output
Write-Host "Copying binaries..." -ForegroundColor Green
Get-ChildItem $BuildDirResolved -Recurse -Filter 'rott*.exe' | Copy-Item -Destination $OutputDirResolved -Force
Get-ChildItem $BuildDirResolved -Recurse -Filter 'SDL3*.dll' | Copy-Item -Destination $OutputDirResolved -Force

Write-Host "Build completed successfully!" -ForegroundColor Green
