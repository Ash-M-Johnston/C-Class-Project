echo "Building C project"
#Current working directory
dir="$(pwd)"
#Check if CMake is installed, if not, install it
if ! command -v cmake 2>&1 >/dev/null
then
  echo "CMake not installed! Do you want to install it?"
  read -p "Install CMake? (Y/N): " do_run && [[ $do_run == [yY] || $do_run == [yY][eE][sS] ]] || { echo "Cannot build. Exiting..."; exit 1; }
  sudo apt-get update
  sudo apt install cmake
fi
#Run CMake
cmake -G "Unix Makefiles" -B "$dir/cmake-build-main"
cmake --build  "$dir/cmake-build-main"
do_run=""
# Ternary operator, do_run == "y" ? run program : exit
read -p "Run project? (Y/N): " do_run && [[ $do_run == [yY] || $do_run == [yY][eE][sS] ]] || { echo "Exiting..."; exit 0; }
#If Y was input, run the program
./run.sh "$@"