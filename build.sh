echo "Building C project"
#Current working directory
dir="$(pwd)"
#Run CMake
"./cmake-3.31.4-linux-x86_64/bin/cmake" --build  "$dir/cmake-build-main" --target clean
do_run=""
# Ternary operator, do_run == "y" ? run program : exit
read -p "Run project? (Y/N): " do_run && [[ $do_run == [yY] || $do_run == [yY][eE][sS] ]] || echo "Exiting..."; exit 1
./run.sh