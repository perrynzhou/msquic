<#

.SYNOPSIS
This script provides helpers for starting, stopping and canceling log collection.

.PARAMETER Start
    Starts the logs being collected with the given profile.

.PARAMETER LogProfile
    The name of the profile to use for log collection.

.PARAMETER Cancel
    Stops the logs from being collected and discards any collected so far.

.PARAMETER Stop
    Stops the logs from being collected and saves them to the -Output location.

.PARAMETER Output
    The output file name or directory for the logs.

.PARAMETER ConvertToText
    Converts the output logs to text.

.PARAMETER InstanceName
    A unique name for the logging instance.

.EXAMPLE
    logs.ps1 -Start -LogProfile Basic.Light

.EXAMPLE
    logs.ps1 -Cancel

.EXAMPLE
    logs.ps1 -Stop -Output quic.etl

#>

param (
    [Parameter(Mandatory = $false, ParameterSetName='Start')]
    [switch]$Start = $false,

    [Parameter(Mandatory = $true, ParameterSetName='Start')]
    [ValidateSet("Basic.Light", "Basic.Verbose", "Full.Light", "Full.Verbose")]
    [string]$LogProfile,

    [Parameter(Mandatory = $false, ParameterSetName='Cancel')]
    [switch]$Cancel = $false,

    [Parameter(Mandatory = $false, ParameterSetName='Stop')]
    [switch]$Stop = $false,

    [Parameter(Mandatory = $true, ParameterSetName='Stop')]
    [string]$OutputDirectory = "",

    [Parameter(Mandatory = $false, ParameterSetName='Stop')]
    [switch]$ConvertToText = $false,

    [Parameter(Mandatory = $false)]
    [string]$InstanceName = "msquic"
)

Set-StrictMode -Version 'Latest'
$PSDefaultParameterValues['*:ErrorAction'] = 'Stop'

# Path for the WPR profile.
$WprpFile = $PSScriptRoot + "\src\manifest\msquic.wprp"

# Path for quicetw.
$QuicEtw = $null
if ($IsWindows) {
    if (Test-Path ($PSScriptRoot + "\artifacts\windows\bin\release")) {
        $QuicEtw = $PSScriptRoot + "\artifacts\windows\bin\release\quicetw.exe"
    } else {
        $QuicEtw = $PSScriptRoot + "\artifacts\windows\bin\debug\quicetw.exe"
    }
} else {
    # TODO
}

# Start log collection.
function Log-Start {
    if ($IsWindows) {
        wpr.exe -start "$($WprpFile)!$($LogProfile)" -filemode -instancename $InstanceName
    } else {
        # TODO
        Write-Warning "Logging not supported yet!"
    }
}

# Cancels log collection, discarding any logs.
function Log-Cancel {
    if ($IsWindows) {
        wpr.exe -cancel -instancename $InstanceName
    } else {
        # TODO
        Write-Warning "Logging not supported yet!"
    }
}

# Stops log collection, keeping the logs.
function Log-Stop {
    if ($IsWindows) {
        $EtlPath = Join-Path $OutputDirectory "quic.etl"
        wpr.exe -stop $EtlPath -instancename $InstanceName
        if ($ConvertToText) {
            $LogPath = Join-Path $OutputDirectory "quic.log"
            & $QuicEtw $EtlPath --trace > $LogPath
        }
    } else {
        # TODO
        Write-Warning "Logging not supported yet!"
    }
}

##############################################################
#                     Main Execution                         #
##############################################################

if ($Start)  { Log-Start }
if ($Cancel) { Log-Cancel }
if ($Stop)   { Log-Stop }