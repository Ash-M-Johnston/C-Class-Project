echo "Building C project"
#Current working directory
dir="$(pwd)"
#Check if gcc is installed, if not, install it
if ! command -v gcc 2>&1 >/dev/null
then
  echo "gcc not installed! Do you want to install it?"
  read -p "Install gcc? (Y/N): " do_run && [[ $do_run == [yY] || $do_run == [yY][eE][sS] ]] || { echo "Cannot build. Exiting..."; exit 1; }
  sudo apt install gcc
fi
#Check if make is installed, if not, install it
if ! command -v make 2>&1 >/dev/null
then
  echo "make not installed! Do you want to install it?"
  read -p "Install make? (Y/N): " do_run && [[ $do_run == [yY] || $do_run == [yY][eE][sS] ]] || { echo "Cannot build. Exiting..."; exit 1; }
  sudo apt install make
fi
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