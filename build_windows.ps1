# Script de build para Windows (PowerShell)
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Compilando Jogo Triatlo" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Limpa build anterior
if (Test-Path "build") {
    Write-Host "Limpando build anterior..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force "build"
}

# Cria pasta de build
Write-Host "Criando pasta de build..." -ForegroundColor Green
New-Item -ItemType Directory -Path "build" | Out-Null
Set-Location "build"

# Configura CMake
Write-Host ""
Write-Host "Configurando CMake..." -ForegroundColor Green
$cmakeArgs = @("..", "-DCMAKE_BUILD_TYPE=Release")

# Verifica se vcpkg está configurado (opcional)
$vcpkgRoot = $env:VCPKG_ROOT
if ($vcpkgRoot -and (Test-Path "$vcpkgRoot\scripts\buildsystems\vcpkg.cmake")) {
    Write-Host "Usando vcpkg para gerenciar dependencias..." -ForegroundColor Yellow
    $cmakeArgs += "-DCMAKE_TOOLCHAIN_FILE=$vcpkgRoot\scripts\buildsystems\vcpkg.cmake"
}

& cmake $cmakeArgs
if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "ERRO: Falha ao configurar CMake!" -ForegroundColor Red
    Write-Host "Verifique se o raylib esta instalado corretamente." -ForegroundColor Red
    Read-Host "Pressione Enter para sair"
    exit 1
}

# Compila o projeto
Write-Host ""
Write-Host "Compilando o projeto..." -ForegroundColor Green
& cmake --build . --config Release
if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "ERRO: Falha ao compilar!" -ForegroundColor Red
    Read-Host "Pressione Enter para sair"
    exit 1
}

# Executa o jogo
Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Compilacao concluida com sucesso!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Executando o jogo..." -ForegroundColor Green
Write-Host ""

$exePath = "Release\JogoTriatlo.exe"
if (Test-Path $exePath) {
    & $exePath
} else {
    Write-Host "Executavel nao encontrado em: $exePath" -ForegroundColor Red
    Write-Host "Tente executar manualmente." -ForegroundColor Yellow
}

Read-Host "`nPressione Enter para sair"

