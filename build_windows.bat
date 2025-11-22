@echo off
echo ========================================
echo   Compilando Jogo Triatlo
echo ========================================
echo.

REM Limpa build anterior
if exist build (
    echo Limpando build anterior...
    rmdir /s /q build
)

REM Cria pasta de build
echo Criando pasta de build...
mkdir build
cd build

REM Configura CMake
echo.
echo Configurando CMake...
cmake .. -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 (
    echo.
    echo ERRO: Falha ao configurar CMake!
    echo Verifique se o raylib esta instalado corretamente.
    pause
    exit /b 1
)

REM Compila o projeto
echo.
echo Compilando o projeto...
cmake --build . --config Release
if errorlevel 1 (
    echo.
    echo ERRO: Falha ao compilar!
    pause
    exit /b 1
)

REM Executa o jogo
echo.
echo ========================================
echo   Compilacao concluida com sucesso!
echo ========================================
echo.
echo Executando o jogo...
echo.
cd Release
JogoTriatlo.exe

pause

