#!/bin/bash

AS_IMAGE_NAME=aerospike-tcfs-512
# AS_IMAGE_NAME=`docker images | grep aerospike | awk '{print $1}'`

function usage() {
	echo "Usage: args [-a] [-c] [-h]"
	echo "-a get container address"
	echo "-c create as container"
	echo "-h show help infos"
}

function getip() {
	AS_CNAME="aerospike-tcfs-512"
	AS_CID=`docker ps | grep aerospike | awk '{print $1}'`
#	echo ${AS_CID}
	AS_IP=`docker inspect -f '{{ .NetworkSettings.IPAddress }}' ${AS_CID}`
	echo ${AS_IP}
}

function createcontainer() {
	docker rm -f tcfsdb 2> /dev/null || :
	/usr/bin/expect <<-EOF
		set timeout 5     
		spawn docker run -it --name="tcfsdb" --hostname="tcfsdb" ${AS_IMAGE_NAME} /bin/bash  
		expect  {
			"tcfsdb:/" {
				send "asd --foreground\n";
			}
			timeout {
				send_tty "\n\[error\]: timeout for docker run\n"
				exit 254
			}
			eof {
				send_tty "\n\[error\]: eof for docker run\n"
				exit 253
			}
		}
		interact
		
	EOF
#	docker run -it -d --name="tcfsdb" --hostname="tcfsdb" ${TCFS_AEROSPIKE}
#	docker run -it --name="tcfsdb" --hostname="tcfsdb" ${AS_IMAGE_NAME} /bin/bash
}

CID=unknown
set -e
while getopts "acht:" OPT
do
	case $OPT in
		a)	CID=`getip`
			echo "${CID}"
			exit 0
			;;
		c)	createcontainer
			exit 0
			;;
		t)	echo "TEST getopts:"
			echo "OPT:${OPT}, OPTIND:${OPTIND}, OPTARG:${OPTARG}" # ARG:${${OPTIND}}, 
			;;
			 h)	usage
			;;
		\?)	echo "[error]:unknown argument"  # unkonw argument
			usage
			exit 1
			;;
    esac
done
set +e

if [ $# -eq 0 ] ; then 
	echo "[error]: argument wrong!"
	usage
	exit 1; 
fi

if [ $# -gt $((OPTIND-1)) ] ; then 
	shift $((OPTIND-1))
	echo "[error]:unkonw argument: $@"
	usage
	exit 1; 
fi 




