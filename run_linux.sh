# rm -rf build  # Limpa build antigo
# mkdir build && cd build
# cmake ..
# make -j$(nproc)
# ./JogoTriatlo

#!/bin/bash
set -e  # Stops the script if any command fails (important!)

# 1. Clean old build (optional, but safer for big changes)
rm -rf build

# 2. Create and enter build folder
mkdir build
cd build

# 3. Configure and Build
cmake ..
make -j$(nproc)

# 4. Run the executable
./JogoTriatlo