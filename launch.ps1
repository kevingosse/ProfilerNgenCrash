$root = $PSScriptRoot

echo "Root: $root"


$fileContent = Get-Content -Path "$root\applicationHost.config.template" -Raw

$modifiedContent = $fileContent -replace "\{BaseDirectory\}", $root

Set-Content -Path "$root\applicationHost.config" -Value $modifiedContent

$env:COR_ENABLE_PROFILING=1
$env:COR_PROFILER_PATH_64="$root\x64\Debug\Profiler.dll"
$env:COR_PROFILER="{CFAD7566-01F8-4841-AD1E-75F6D7D9567F}"

& "C:\Program Files\IIS Express\iisexpress.exe"  /config:"$root\applicationhost.config" /site:"WebFormsTestApp" /apppool:"Clr4IntegratedAppPool"