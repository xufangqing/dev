#!/bin/bash
set -e

./nfoam.expect
echo "################result is :$?###########################"
echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

# spawnid=`./xspawn "docker exec -it nfoam /bin/bash" | grep spawnpid |cut -d : -f 2`

# echo "==========spawnid is ${spawnid}==========="

# if [ "" == "${spawnid}" ]; then 
# 	exit 1
# fi

# ./xexpect "ls" "root@nfoam.*# *$" 5 "${spawnid}"

# echo "+++++++++++++++++++++++++++++++++++++++++"
# ./xclose "${spawnid}"



