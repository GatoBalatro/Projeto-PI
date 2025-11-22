# Projeto-PI
Projeto da disciplina de programação imperativa

## Jogo Triatlo

Jogo desenvolvido em C++ usando a biblioteca Raylib.

## Pré-requisitos

### Windows

1. **CMake** (versão 3.16 ou superior)
   - Download: https://cmake.org/download/
   - Ou via Chocolatey: `choco install cmake`
   - Ou via winget: `winget install Kitware.CMake`

2. **Compilador C++**
   - **Visual Studio** (recomendado): Instale o Visual Studio Community com "Desenvolvimento para Desktop com C++"
   - Ou **MinGW-w64**: https://www.mingw-w64.org/

3. **Raylib**
   - **Opção 1 - vcpkg (Recomendado)**:
     ```powershell
     # Instalar vcpkg (se ainda não tiver)
     git clone https://github.com/Microsoft/vcpkg.git
     cd vcpkg
     .\bootstrap-vcpkg.bat
     
     # Instalar raylib
     .\vcpkg install raylib:x64-windows
     
     # Configurar CMake para usar vcpkg
     .\vcpkg integrate install
     ```
   
   - **Opção 2 - Build manual**:
     - Download: https://github.com/raysan5/raylib/releases
     - Compilar seguindo as instruções do repositório

## Como Compilar e Executar

### Windows (PowerShell)

1. **Navegue até a pasta do projeto**:
   ```powershell
   cd F:\PI\Projeto-PI
   ```

2. **Crie a pasta de build**:
   ```powershell
   mkdir build
   cd build
   ```

3. **Configure o CMake**:
   ```powershell
   # Se estiver usando vcpkg, especifique o toolchain:
   cmake .. -DCMAKE_TOOLCHAIN_FILE=[caminho-para-vcpkg]/scripts/buildsystems/vcpkg.cmake
   
   # Ou se raylib estiver instalado globalmente:
   cmake ..
   ```

4. **Compile o projeto**:
   ```powershell
   cmake --build . --config Release
   ```
   Ou se estiver usando Visual Studio:
   ```powershell
   cmake --build . --config Release
   ```
   Ou abra o arquivo `JogoTriatlo.sln` no Visual Studio e compile de lá.

5. **Execute o jogo**:
   ```powershell
   .\Release\JogoTriatlo.exe
   ```
   Ou se compilou em Debug:
   ```powershell
   .\Debug\JogoTriatlo.exe
   ```

### Usando o Script Automático

Execute o script `build_windows.bat` (criado abaixo) que automatiza todo o processo:
```powershell
.\build_windows.bat
```

## Controles do Jogo

- **W, A, S, D**: Mover o jogador
- **ESC**: Voltar ao menu / Sair
- **Mouse**: Clicar nos botões do menu
- **Tecla 1**: Iniciar jogo (no menu)
- **Tecla 2**: Ver créditos (no menu)

## Estrutura do Projeto

```
Projeto-PI/
├── src/
│   ├── main.cpp    # Ponto de entrada
│   ├── game.h      # Definições e estruturas
│   └── game.cpp    # Lógica do jogo
├── CMakeLists.txt  # Configuração CMake
└── README.md       # Este arquivo
```

## Desenvolvedores

- Integrante 0: elso
- Integrante 1: phsm2
- Integrante 2: vssb
- Integrante 3: ycms