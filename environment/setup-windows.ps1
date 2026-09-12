[CmdletBinding()]
param(
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Release"
)

$ErrorActionPreference = "Stop"

function Test-CommandAvailable {
    param([string]$Name)

    return $null -ne (Get-Command $Name -ErrorAction SilentlyContinue)
}

function Invoke-WingetInstall {
    param(
        [string]$PackageId,
        [string[]]$AdditionalArguments = @()
    )

    & winget install --id $PackageId --exact --source winget --accept-package-agreements --accept-source-agreements @AdditionalArguments
    if ($LASTEXITCODE -ne 0) {
        throw "winget could not install $PackageId."
    }
}

function Find-CmakeExecutable {
    $cmakeCommand = Get-Command cmake -ErrorAction SilentlyContinue
    if ($null -ne $cmakeCommand) {
        return $cmakeCommand.Source
    }

    $defaultCmakePath = Join-Path $env:ProgramFiles "CMake\bin\cmake.exe"
    if (Test-Path $defaultCmakePath) {
        return $defaultCmakePath
    }

    throw "CMake was installed but cmake.exe could not be found. Open a new PowerShell session and run this script again."
}

$projectRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$vswherePath = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
$hasMsvcToolchain = (Test-Path $vswherePath) -and $null -ne (& $vswherePath -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath)

if (-not (Test-CommandAvailable winget)) {
    throw "winget is required to install the Windows build dependencies. Install App Installer from the Microsoft Store, then run this script again."
}

if (-not (Test-CommandAvailable cmake) -and -not (Test-Path (Join-Path $env:ProgramFiles "CMake\bin\cmake.exe"))) {
    Invoke-WingetInstall -PackageId "Kitware.CMake"
}

if (-not $hasMsvcToolchain) {
    Invoke-WingetInstall -PackageId "Microsoft.VisualStudio.2022.BuildTools" -AdditionalArguments @(
        "--override",
        "--wait --quiet --norestart --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"
    )
}

$cmake = Find-CmakeExecutable
$buildDirectory = Join-Path $projectRoot "build\windows"

& $cmake -S $projectRoot -B $buildDirectory -G "Visual Studio 17 2022" -A x64 -DBUILD_TESTING=ON
if ($LASTEXITCODE -ne 0) {
    throw "CMake configuration failed."
}

& $cmake --build $buildDirectory --config $Configuration
if ($LASTEXITCODE -ne 0) {
    throw "Build failed."
}

Write-Host "Build complete: $buildDirectory\$Configuration\Hail.exe"