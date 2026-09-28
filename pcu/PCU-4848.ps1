# PCU-4848: Windows -> usbipd -> WSL/Ubuntu launcher for ESP32-S3-4848S040.
# It persists the physical BUSID (for example 1-1 or 2-2), attaches the device
# to WSL and delegates status/flash/monitor work to pcu-4848.sh.
[CmdletBinding()]
param(
    [string]$BusId,
    [switch]$Flash,
    [switch]$Monitor,
    [string]$Distro
)

$ErrorActionPreference = "Stop"

function Test-Administrator {
    $identity = [Security.Principal.WindowsIdentity]::GetCurrent()
    $principal = New-Object Security.Principal.WindowsPrincipal($identity)
    return $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
}

function Restart-Elevated {
    $args = @('-NoProfile', '-ExecutionPolicy', 'Bypass', '-File', ('"' + $PSCommandPath + '"'))
    if ($BusId) { $args += @('-BusId', ('"' + $BusId + '"')) }
    if ($Flash) { $args += '-Flash' }
    if ($Monitor) { $args += '-Monitor' }
    if ($Distro) { $args += @('-Distro', ('"' + $Distro + '"')) }
    Start-Process powershell.exe -Verb RunAs -ArgumentList ($args -join ' ')
}

function Invoke-Wsl {
    param([string[]]$Arguments)
    $all = @()
    if ($Distro) { $all += @('-d', $Distro) }
    $all += '--'
    $all += $Arguments
    & wsl.exe @all
    if ($LASTEXITCODE -ne 0) {
        throw "WSL devolvio codigo $LASTEXITCODE al ejecutar: $($Arguments -join ' ')"
    }
}

function Get-UsbipdLines {
    return @(& usbipd list | ForEach-Object { "$_" })
}

function Get-Esp32Candidates {
    param([string[]]$Lines)
    $items = @()
    foreach ($line in $Lines) {
        if ($line -match '^\s*(\d+-\d+(?:\.\d+)*)\s+([0-9A-Fa-f]{4}:[0-9A-Fa-f]{4})\s+(.+)$') {
            $id = $matches[1]
            $vidpid = $matches[2]
            $rest = $matches[3]
            $looksLikeEsp32 = ($vidpid -match '^(?i:303a):') -or
                ($rest -match '(?i:Espressif|ESP32|JTAG/serial|USB JTAG|CP210|CH340)')
            if ($looksLikeEsp32) {
                $items += [pscustomobject]@{ BusId = $id; VidPid = $vidpid; Line = $line }
            }
        }
    }
    return @($items)
}

Write-Host ""
Write-Host "============================================================"
Write-Host " PCU 4848 - ESP32-S3-4848S040 / WSL2"
Write-Host "============================================================"

if (-not (Get-Command wsl.exe -ErrorAction SilentlyContinue)) {
    throw "WSL no esta disponible en este Windows."
}
if (-not (Get-Command usbipd -ErrorAction SilentlyContinue)) {
    throw "usbipd-win no esta instalado o no esta en PATH."
}

if (-not (Test-Administrator)) {
    Write-Host "Se requiere elevacion para compartir el USB con WSL. Abriendo PCU como administrador..."
    Restart-Elevated
    exit 0
}

$stateDir = Join-Path $env:LOCALAPPDATA 'PCU-4848'
$stateFile = Join-Path $stateDir 'busid.txt'
New-Item -ItemType Directory -Force -Path $stateDir | Out-Null

$lines = Get-UsbipdLines
$candidates = Get-Esp32Candidates -Lines $lines

if (-not $BusId -and (Test-Path $stateFile)) {
    $saved = (Get-Content $stateFile -ErrorAction SilentlyContinue | Select-Object -First 1).Trim()
    if ($saved -and ($lines -match ('^\s*' + [regex]::Escape($saved) + '\s+'))) {
        $BusId = $saved
        Write-Host "BUSID recordado: $BusId"
    }
}

if (-not $BusId) {
    if ($candidates.Count -eq 1) {
        $BusId = $candidates[0].BusId
        Write-Host "ESP32 detectado automaticamente: $BusId  $($candidates[0].VidPid)"
    } elseif ($candidates.Count -gt 1) {
        Write-Host "Se detectaron varios candidatos ESP32/serial:"
        for ($i = 0; $i -lt $candidates.Count; $i++) {
            Write-Host "  [$($i + 1)] $($candidates[$i].Line)"
        }
        $choice = Read-Host "Seleccione el numero del ESP32-S3-4848S040"
        $idx = 0
        if (-not [int]::TryParse($choice, [ref]$idx) -or $idx -lt 1 -or $idx -gt $candidates.Count) {
            throw "Seleccion USB invalida."
        }
        $BusId = $candidates[$idx - 1].BusId
    } else {
        Write-Host "usbipd list:"
        $lines | ForEach-Object { Write-Host $_ }
        throw "No se encontro automaticamente un ESP32. Conecte el panel por USB y vuelva a abrir PCU."
    }
}

Set-Content -Path $stateFile -Value $BusId -Encoding ascii
$selectedLine = ($lines | Where-Object { $_ -match ('^\s*' + [regex]::Escape($BusId) + '\s+') } | Select-Object -First 1)
if (-not $selectedLine) { throw "El BUSID $BusId ya no aparece en usbipd list." }

Write-Host ""
Write-Host "BUSID fisico Windows: $BusId"
Write-Host "$selectedLine"

if ($selectedLine -match '(?i)\bNot shared\b') {
    Write-Host "Compartiendo $BusId..."
    & usbipd bind --busid $BusId
    if ($LASTEXITCODE -ne 0) { throw "usbipd bind fallo para $BusId." }
} elseif ($selectedLine -notmatch '(?i)\bShared\b|\bAttached\b') {
    Write-Host "Intentando compartir $BusId..."
    & usbipd bind --busid $BusId
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Aviso: bind devolvio $LASTEXITCODE; se intentara attach porque puede estar ya compartido."
    }
}

$lines = Get-UsbipdLines
$selectedLine = ($lines | Where-Object { $_ -match ('^\s*' + [regex]::Escape($BusId) + '\s+') } | Select-Object -First 1)
if ($selectedLine -notmatch '(?i)\bAttached\b') {
    Write-Host "Adjuntando $BusId a WSL..."
    & usbipd attach --wsl --busid $BusId
    if ($LASTEXITCODE -ne 0) { throw "usbipd attach fallo para $BusId." }
}

Start-Sleep -Seconds 2
Write-Host ""
Write-Host "Estado USB despues de attach:"
(Get-UsbipdLines | Where-Object { $_ -match ('^\s*' + [regex]::Escape($BusId) + '\s+') }) | ForEach-Object { Write-Host $_ }

$linuxDirRaw = Invoke-Wsl -Arguments @('wslpath', '-a', '-u', $PSScriptRoot)
$linuxDir = (($linuxDirRaw | Select-Object -First 1) -as [string]).Trim()
if (-not $linuxDir) { throw "No se pudo convertir la ruta PCU a una ruta WSL." }
$linuxScript = "$linuxDir/pcu-4848.sh"

function Run-PcuLinux {
    param([switch]$DoFlash, [switch]$DoMonitor)
    $args = @('bash', $linuxScript, '--windows-busid', $BusId)
    if ($DoFlash) { $args += '--flash' }
    if ($DoMonitor) { $args += '--monitor' }
    Invoke-Wsl -Arguments $args
}

if ($Flash -or $Monitor) {
    Run-PcuLinux -DoFlash:$Flash -DoMonitor:$Monitor
    exit 0
}

Run-PcuLinux
Write-Host ""
Write-Host "Opciones:"
Write-Host "  [F] Flashear firmware integrado (pedira Wi-Fi/token solo si aun no estan guardados)"
Write-Host "  [M] Abrir monitor serie"
Write-Host "  [B] Flashear y luego abrir monitor"
Write-Host "  [Enter] Salir dejando el USB adjuntado a WSL"
$action = (Read-Host "Opcion").Trim().ToUpperInvariant()
switch ($action) {
    'F' { Run-PcuLinux -DoFlash }
    'M' { Run-PcuLinux -DoMonitor }
    'B' { Run-PcuLinux -DoFlash -DoMonitor }
    default { Write-Host "USB $BusId queda adjuntado a WSL hasta desconectar el cable, apagar WSL o ejecutar usbipd detach." }
}
