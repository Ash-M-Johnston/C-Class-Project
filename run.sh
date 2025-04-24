echo "Running Project with args "$@""
dir=${pwd}
cd ./cmake-build-main/
./Project "$@"
cd $pwd