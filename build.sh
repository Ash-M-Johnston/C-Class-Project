echo "Building C project"
#Current working directory
dir="$(pwd)"
#cmake="./cmake-3.31.4-linux-x86_64/bin/cmake"
#Run CMake
cmake -G "Unix Makefiles" -B "$dir/cmake-build-main" || exit 1
cmake --build  "$dir/cmake-build-main" || exit 1
do_run=""
# Ternary operator, do_run == "y" ? run program : exit
read -p "Run project? (Y/N): " do_run && [[ $do_run == [yY] || $do_run == [yY][eE][sS] ]] || { echo "Exiting..."; exit 0; }
#If Y was input, run the program
./run.sh "$@"