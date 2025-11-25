@echo off
chcp 65001 >nul
echo ========================================
echo   Script de Build - Jogo Triatlo
echo ========================================
echo.

REM Verificar se CMake está instalado
where cmake >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo [ERRO] CMake nao encontrado!
    echo Instale o CMake: https://cmake.org/download/
    pause
    exit /b 1
)

echo [OK] CMake encontrado
echo.

REM Verificar compilador MSVC (Visual Studio)
where cl >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [OK] Compilador MSVC encontrado
    goto :build_msvc
)

REM Verificar se Visual Studio está instalado (mesmo sem estar no PATH)
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
    echo [OK] Visual Studio 2022 encontrado
    goto :build_vs2022
)

if exist "C:\Program Files\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat" (
    echo [OK] Visual Studio 2019 encontrado
    goto :build_vs2019
)

REM Verificar compilador MinGW
where g++ >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [OK] Compilador MinGW encontrado
    goto :build_mingw
)

REM Nenhum compilador encontrado
echo.
echo [ERRO] Nenhum compilador C++ encontrado!
echo.
echo Por favor, instale uma das opcoes:
echo.
echo 1. Visual Studio Community (recomendado)
echo    https://visualstudio.microsoft.com/downloads/
echo    - Selecione "Desenvolvimento para Desktop com C++"
echo.
echo 2. MinGW-w64
echo    https://www.mingw-w64.org/downloads/
echo    - Ou use MSYS2: https://www.msys2.org/
echo.
pause
exit /b 1

:build_vs2022
echo.
echo Configurando ambiente Visual Studio 2022...
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
goto :build_msvc

:build_vs2019
echo.
echo Configurando ambiente Visual Studio 2019...
call "C:\Program Files\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
goto :build_msvc

:build_msvc
echo.
echo ========================================
echo Compilando com MSVC/Visual Studio...
echo ========================================
echo.

REM Limpar build anterior
if exist build (
    echo Limpando build anterior...
    rmdir /s /q build
)

mkdir build
cd build

REM Tentar diferentes geradores do Visual Studio
echo Tentando gerador: Visual Studio 17 2022...
cmake .. -G "Visual Studio 17 2022" -A x64
if %ERRORLEVEL% EQU 0 goto :build_exec

echo Tentando gerador: Visual Studio 16 2019...
cmake .. -G "Visual Studio 16 2019" -A x64
if %ERRORLEVEL% EQU 0 goto :build_exec

echo Tentando gerador: NMake Makefiles...
cmake .. -G "NMake Makefiles"
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERRO] Falha ao configurar CMake!
    echo Verifique se o Raylib esta instalado corretamente.
    cd ..
    pause
    exit /b 1
)

:build_exec
echo.
echo Compilando projeto...
cmake --build . --config Release
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERRO] Falha na compilacao!
    cd ..
    pause
    exit /b 1
)

goto :copy_files

:build_mingw
echo.
echo ========================================
echo Compilando com MinGW...
echo ========================================
echo.

REM Limpar build anterior
if exist build (
    echo Limpando build anterior...
    rmdir /s /q build
)

mkdir build
cd build

echo Configurando CMake com MinGW...
cmake .. -G "MinGW Makefiles"
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERRO] Falha ao configurar CMake!
    echo Verifique se o Raylib esta instalado corretamente.
    cd ..
    pause
    exit /b 1
)

echo.
echo Compilando projeto...
cmake --build . --config Release
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERRO] Falha na compilacao!
    cd ..
    pause
    exit /b 1
)

:copy_files
echo.
echo ========================================
echo Copiando arquivos necessarios...
echo ========================================

REM Copiar arquivo de musica para a pasta do executavel
if exist "Release\JogoTriatlo.exe" (
    echo Copiando musica para Release...
    copy "..\src\Dave the Diver OST - On the boat.mp3" "Release\" >nul 2>&1
    if %ERRORLEVEL% EQU 0 (
        echo [OK] Musica copiada para Release\
    )
    set EXE_PATH=Release\JogoTriatlo.exe
) else if exist "JogoTriatlo.exe" (
    echo Copiando musica para build...
    copy "..\src\Dave the Diver OST - On the boat.mp3" "." >nul 2>&1
    if %ERRORLEVEL% EQU 0 (
        echo [OK] Musica copiada para build\
    )
    set EXE_PATH=JogoTriatlo.exe
) else (
    echo [AVISO] Executavel nao encontrado na localizacao esperada
)

cd ..

echo.
echo ========================================
echo Compilacao concluida com sucesso!
echo ========================================
echo.
if defined EXE_PATH (
    echo Executavel: build\%EXE_PATH%
    echo.
    echo Para executar:
    echo   cd build
    if exist "build\Release" (
        echo   cd Release
    )
    echo   JogoTriatlo.exe
) else (
    echo Procure o executavel na pasta build\
)
echo.
pause