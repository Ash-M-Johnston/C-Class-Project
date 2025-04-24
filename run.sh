if [[ -z "$@" ]]
then
  echo "Running Project with no arguments"
else
  echo "Running Project with args ["$@"]"
fi

dir=${pwd}
cd ./cmake-build-main/
./Project "$@"
cd $pwd