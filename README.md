# Projeto-PI
Projeto da disciplina de programação imperativa
# Jogo Triatlo

Projeto da disciplina de Programação Imperativa - Um jogo 2D desenvolvido em C++ usando a biblioteca Raylib.

## 📋 Descrição

Jogo Triatlo é um jogo 2D onde o jogador controla um personagem que deve navegar por uma pista, evitando obstáculos. O jogo apresenta um menu principal, sistema de créditos, música de fundo e animações de sprite.

## ✨ Funcionalidades

- 🎮 **Menu Principal**: Interface com opções para iniciar o jogo, ver créditos e sair
- 🎵 **Música de Fundo**: Trilha sonora no menu (formato OGG)
- 🎨 **Sprites Animados**: Personagem com animação de 8 frames durante o movimento
- 🎯 **Sistema de Jogo**: Movimento com WASD, detecção de colisão com obstáculos
- 📊 **Estados do Jogo**: Menu, Jogo e Créditos
- 🎬 **Câmera Dinâmica**: Câmera que segue o jogador

## 🛠️ Tecnologias Utilizadas

- **Linguagem**: C++11
- **Biblioteca Gráfica**: Raylib
- **Build System**: CMake 3.16+
- **Compiladores Suportados**: 
  - MSVC (Visual Studio)
  - MinGW-w64
  - GCC (Linux)

## 📦 Requisitos

### Windows
- CMake 3.16 ou superior
- Compilador C++ (Visual Studio ou MinGW)
- Raylib instalado ou na pasta do projeto

### Linux
- CMake 3.16 ou superior
- GCC/G++ com suporte a C++11
- Raylib instalado via gerenciador de pacotes

## 🚀 Instalação e Compilação

### Windows

#### Opção 1: Usando o Script Automático (Recomendado)ell
.\build_windows.bat#### Opção 2: Compilação Manualll
# Criar pasta de build
mkdir build
cd build

# Configurar projeto
cmake ..

# Compilar
cmake --build . --config Release### Linux
# Executar script de build
chmod +x run_linux.sh
./run_linux.sh

# Ou manualmente:
mkdir build && cd build
cmake ..
make -j$(nproc)
## 🎮 Como Executar

### Windowsl
cd build
.\JogoTriatlo.exeOu se compilou em Release:ershell
cd build\Release
.\JogoTriatlo.exe### Linuxh
cd build
./JogoTriatlo## 🎯 Controles

- **W** - Mover para cima
- **A** - Mover para esquerda
- **S** - Mover para baixo
- **D** - Mover para direita
- **ESC** - Voltar ao menu / Sair do jogo
- **Mouse** - Navegar pelos botões do menu
- **Enter** ou **1** - Iniciar jogo (no menu)
- **2** - Ver créditos (no menu)

## 📁 Estrutura do Projeto
