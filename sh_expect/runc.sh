#!/bin/bash

function usage() {
	echo "Usage: args [-n cname] [-a as_ip] [-m mountpath] [-i imagename]"
	echo "-n container name"
	echo "-a aerospike ip"
	echo "-m mountpath"
	echo "-i image name and tag"
	echo "-h help info"
}

set -e
while getopts "n:a:m:i:h" OPT
do
	case $OPT in
		n)	CNAME=${OPTARG}
			echo CNAME:"${CNAME}"
			;;
		a)	ASHOST=${OPTARG}
			echo ASHOST:"${ASHOST}"
			;;
		m)	MOUNTPATH=${OPTARG}
			echo MOUNTPATH:"${MOUNTPATH}"
			;;
		i)	IMAGENAME=${OPTARG}
			echo IMAGENAME:"${IMAGENAME}"
			;;
		h)	usage
			exit 0
			;;
#		\?)	echo "[error]:unkonw argument"  # unkonw argument
#			usage
#			exit 1
#			;;
	esac
done

shift $((OPTIND-1))
# if [ $# -gt $((OPTIND-1)) ] ; then 	
# fi 

echo "docker run -it -d --privileged=true -e DBCONF=ip=${AS_IP}:3000 --name=${CNAME} --hostname=${CNAME} -v ${MOUNTPATH}:/home/share $@ ${IMAGENAME}"
docker run -it -d --privileged="true" -e DBCONF="ip=${AS_IP}:3000" --name="${CNAME}" --hostname="${CNAME}" -v ${MOUNTPATH}:/home/share "$@" ${IMAGENAME}
set +e


