$ErrorActionPreference = 'Stop'

$configName = $env:APPVEYOR_CONFIGURATION_NAME
$cmakeArgs = @('-S', '.', '-B', 'build', '-A', 'Win32')

switch ($configName) {
  'Release Shareware' {
    $cmakeArgs += @(
      '-DROTT_SHAREWARE=ON',
      '-DROTT_SUPERROTT=OFF',
      '-DROTT_SITELICENSE=OFF',
      '-DROTT_SUFFIX=-huntbgin'
    )
  }
  'Release CDROM' {
    $cmakeArgs += @(
      '-DROTT_SHAREWARE=OFF',
      '-DROTT_SUPERROTT=ON',
      '-DROTT_SITELICENSE=OFF',
      '-DROTT_SUFFIX=-rottcd'
    )
  }
  'Release' {
    $cmakeArgs += @(
      '-DROTT_SHAREWARE=OFF',
      '-DROTT_SUPERROTT=OFF',
      '-DROTT_SITELICENSE=OFF',
      '-DROTT_SUFFIX=-darkwar'
    )
  }
  default {
    throw "Unsupported Windows configuration: $configName"
  }
}

cmake @cmakeArgs
cmake --build build --config Release --target rott

New-Item -ItemType Directory -Force Windows\out | Out-Null

Get-ChildItem build -Recurse -Filter 'rott*.exe' | Copy-Item -Destination Windows\out -Force
Get-ChildItem build -Recurse -Filter 'SDL3*.dll' | Copy-Item -Destination Windows\out -Force
