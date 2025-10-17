rm -rf build  # Limpa build antigo
mkdir build && cd build
cmake ..
make -j$(nproc)
./JogoTriatlo