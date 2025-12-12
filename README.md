# Projeto-PI
Projeto da disciplina de programação imperativa
# Jogo Triatlo

Projeto da disciplina de Programação Imperativa - Um jogo 2D desenvolvido em C++ usando a biblioteca Raylib.

## 📋 Descrição

Jogo Triatlo é um jogo 2D onde o jogador controla um personagem que deve completar três fases diferentes: corrida, natação e ciclismo, evitando obstáculos em cada fase. O jogo apresenta menu principal, sistema de créditos, músicas de fundo diferentes para cada fase, efeitos sonoros de colisão e animações de sprite completas.

## ✨ Funcionalidades

### 🎮 Sistema de Jogo
- **Menu Principal**: Interface com opções para iniciar o jogo, ver créditos e sair
- **3 Fases Diferentes**: 
  - Fase 1: Corrida (pista horizontal)
  - Fase 2: Natação (pista vertical)
  - Fase 3: Ciclismo (em desenvolvimento)
- **Sistema de Vidas**: Jogador possui 3 vidas, perde uma a cada colisão
- **Sistema de Invencibilidade**: Proteção temporária após colisão
- **Estados do Jogo**: Menu, Fase Corrida, Fase Natação, Fase Ciclismo, Game Over, Créditos

### 🎨 Gráficos e Animações
- **Sprites Animados**: 
  - Jogador: 8 frames de animação durante movimento
  - Nadador: 8 frames de animação na fase de natação
  - Rato (obstáculo): 3 frames de animação na fase de corrida
  - Tubarão (obstáculo): 4 frames de animação na fase de natação com rotação baseada na direção
- **Câmera Dinâmica**: Câmera que segue o jogador em todas as fases
- **Ambientes Únicos**: Cada fase tem seu próprio visual (pista de corrida, ambiente aquático)

### 🎵 Áudio
- **Música de Fundo**: 
  - Menu: "Dave the Diver OST - On the boat" (OGG)
  - Fase de Natação: "Water" (OGG)
- **Efeitos Sonoros**: 
  - SFX de colisão em todas as fases ("Cartoon Bite Sound Effect")
- **Sistema de Áudio Inteligente**: Transição automática entre músicas ao mudar de fase

### 🎯 Mecânicas de Jogo
- **Movimento**: Controle com WASD
- **Detecção de Colisão**: Sistema preciso de colisão com obstáculos
- **Sistema de Progresso**: Fases completas ao chegar ao objetivo
- **Obstáculos Dinâmicos**: Obstáculos se movem em padrões diferentes em cada fase

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

#### Opção 1: Usando o Script Automático (Recomendado)
```powershell
.\build_windows.bat
```

#### Opção 2: Compilação Manual
```powershell
# Criar pasta de build
mkdir build
cd build

# Configurar projeto
cmake ..

# Compilar
cmake --build . --config Release
```

### Linux
```bash
# Executar script de build
chmod +x run_linux.sh
./run_linux.sh

# Ou manualmente:
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## 🎮 Como Executar

### Windows
```powershell
cd build
.\JogoTriatlo.exe
```

Ou se compilou em Release:
```powershell
cd build\Release
.\JogoTriatlo.exe
```

### Linux
```bash
cd build
./JogoTriatlo
```

## 🎯 Controles

- **W** - Mover para cima
- **A** - Mover para esquerda
- **S** - Mover para baixo
- **D** - Mover para direita
- **ESC** - Voltar ao menu / Sair do jogo
- **Mouse** - Navegar pelos botões do menu
- **Enter** ou **1** - Iniciar jogo (no menu)
- **2** - Ver créditos (no menu)

## 📁 Estrutura do Projeto

```
Projeto-PI/
├── CMakeLists.txt          # Configuração do CMake
├── README.md               # Este arquivo
├── LICENSE                 # Licença do projeto
├── build_windows.bat       # Script de build para Windows
├── run_linux.sh            # Script de build para Linux
├── src/
│   ├── main.cpp            # Ponto de entrada do programa
│   ├── game.h              # Cabeçalho com estruturas e funções
│   ├── game.cpp            # Loop principal do jogo
│   ├── menu.h              # Cabeçalho do menu
│   ├── menu.cpp            # Implementação do menu
│   ├── fase_corrida.h      # Fase 1: Corrida
│   ├── fase_natacao.h      # Fase 2: Natação
│   ├── player.png          # Sprite sheet do jogador (8 frames)
│   ├── swimmer.png         # Sprite sheet do nadador (8 frames)
│   ├── rato-Sheet.png      # Sprite sheet do rato/obstáculo (3 frames)
│   ├── hai-fin-shadow-Sheet-Sheet.png  # Sprite sheet do tubarão (4 frames)
│   ├── Dave-the-Diver-OST-On-the-boat.ogg  # Música do menu
│   ├── water.ogg            # Música da fase de natação
│   ├── cartoon_bite_sound_effect.mp3  # SFX de colisão
│   ├── libraylib.dll       # DLL do Raylib (Windows)
│   └── glfw3.dll           # DLL do GLFW (Windows)
└── build/                  # Pasta de build (gerada)
    └── JogoTriatlo.exe     # Executável compilado
```

## 🎨 Recursos do Jogo

### Sprites
- **player.png**: Sprite sheet horizontal com 8 frames de animação do personagem principal (fase de corrida)
- **swimmer.png**: Sprite sheet horizontal com 8 frames de animação do nadador (fase de natação)
- **rato-Sheet.png**: Sprite sheet horizontal com 3 frames de animação do obstáculo rato (fase de corrida)
- **hai-fin-shadow-Sheet-Sheet.png**: Sprite sheet horizontal com 4 frames de animação do tubarão (fase de natação)

### Áudio
- **Dave-the-Diver-OST-On-the-boat.ogg**: Música de fundo do menu (formato OGG)
- **water.ogg**: Música de fundo da fase de natação (formato OGG)
- **cartoon_bite_sound_effect.mp3**: Efeito sonoro de colisão (formato MP3)

## 🎮 Fases do Jogo

### Fase 1: Corrida
- **Objetivo**: Correr pela pista horizontal evitando ratos
- **Obstáculo**: Ratos animados que se movem em direção ao jogador
- **Mecânica**: Movimento horizontal e vertical, câmera segue o jogador
- **Visual**: Pista de corrida vista de cima com faixas e linha de chegada

### Fase 2: Natação
- **Objetivo**: Nadar verticalmente até a chegada evitando tubarões
- **Obstáculo**: Tubarões animados que se movem em diferentes direções
- **Mecânica**: Movimento em ambiente aquático, câmera vertical
- **Visual**: Ambiente aquático azul com bordas laranja e linha de chegada

### Fase 3: Ciclismo
- **Status**: Em desenvolvimento
- **Objetivo**: Completar o percurso de ciclismo

## 🔧 Configuração do Raylib

O projeto procura o Raylib nos seguintes locais (em ordem):
1. `raylib/include` e `raylib/lib` na pasta do projeto
2. `include` e `lib` na pasta do projeto
3. `C:/raylib/include` e `C:/raylib/lib` (Windows)

### Instalar Raylib

**Windows:**
1. Baixe de: https://github.com/raysan5/raylib/releases
2. Extraia em `C:\raylib\` ou na pasta do projeto

**Linux:**
```bash
sudo apt-get install libraylib-dev  # Ubuntu/Debian
# ou
sudo pacman -S raylib  # Arch Linux
```

## 🐛 Solução de Problemas

### Erro: "Raylib não encontrado"
- Verifique se o Raylib está instalado corretamente
- Certifique-se de que os arquivos estão nos caminhos corretos
- Veja a seção "Configuração do Raylib" acima

### Áudio não funciona
- Verifique se os arquivos OGG/MP3 estão na pasta `src/`
- Certifique-se de que os arquivos foram copiados para a pasta do executável após compilar
- Verifique o volume do sistema
- Confira os logs no console para mensagens de erro

### Sprites não aparecem
- Verifique se os arquivos PNG estão na pasta `src/`
- Confirme que os arquivos foram copiados para a pasta do executável
- O jogo usará retângulos coloridos como fallback se os sprites não forem encontrados

### Música da fase não toca
- Verifique se os arquivos OGG estão na pasta `src/`
- Confirme que os arquivos foram copiados durante a compilação
- Verifique os logs no console para mensagens de carregamento

### SFX de colisão não toca
- Verifique se `cartoon_bite_sound_effect.mp3` está na pasta `src/`
- Confirme que o arquivo foi copiado durante a compilação
- O som só toca quando uma nova colisão é detectada (não repete enquanto colidindo)

## 🎵 Sistema de Áudio

O jogo possui um sistema de áudio completo que:
- Toca música diferente no menu e durante as fases
- Faz transição automática entre as músicas ao mudar de fase
- Toca efeitos sonoros quando há colisões
- Retoma a música do menu ao voltar do jogo
- Descarrega recursos corretamente ao sair

## 🎨 Sistema de Sprites

- **Animações Suaves**: Todos os sprites são animados durante o movimento
- **Orientação Dinâmica**: Tubarões rotacionam baseado na direção do movimento
- **Fallback Visual**: Se sprites não carregarem, o jogo usa formas geométricas coloridas
- **Otimização**: Sprites são carregados apenas uma vez e reutilizados

## 👥 Autores

- Integrante 0: <elso>
- Integrante 1: <phsm2>
- Integrante 2: <vssb>
- Integrante 3: <ycms>

## 📄 Licença

Este projeto está licenciado sob a Mozilla Public License 2.0. Veja o arquivo `LICENSE` para mais detalhes.

## 🙏 Agradecimentos

- **Raylib**: https://www.raylib.com/ - Biblioteca gráfica incrível
- **Música do Menu**: "Dave the Diver OST - On the boat"
- **Música da Natação**: Ghost Data - Beach Episode
- **SFX**: "Cartoon Bite Sound Effect"

## 📝 Notas de Desenvolvimento

- O projeto usa C++11 para máxima compatibilidade
- Sprites devem estar em formato PNG com transparência
- Músicas devem estar em formato OGG para melhor compatibilidade com Raylib
- O sistema de animação funciona apenas quando o jogador está se movendo
- Sistema de invencibilidade previne múltiplas colisões consecutivas
- Cada fase tem sua própria mecânica e visual único
```

O README foi atualizado com:
- Todas as funcionalidades implementadas
- Descrição das 3 fases
- Informações sobre sprites e animações
- Sistema de áudio completo
- Estrutura do projeto atualizada
- Solução de problemas expandida
- Seções sobre sistema de sprites e áudio

O README está completo e documenta todas as funcionalidades do jogo.