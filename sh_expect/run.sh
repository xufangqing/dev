#!/bin/bash
IMAGE_TAR_PATH=$1
SCRIPT_ROOT=$(pwd)
if [ "" == "$1" ]; then 
	echo "[warning]: No image_tar_path specified, use current path[$(SCRIPT_ROOT)]"
	IMAGE_TAR_PATH=$(SCRIPT_ROOT)
fi
set -e
MOUNT_PATH=/home/share
############################### tcfs aerospike ################################
echo -e "\n\n############################### tcfs aerospike ################################\n"
AS_TAR=${IMAGE_TAR_PATH}/aerospike-tcfs-512.tar
AS_IMAGE_NAME=aerospike-tcfs-512
AS_CNAME=tcfsdb
AS_IP=unknown

docker rm -f ${AS_CNAME} 2> /dev/null || :
docker rmi -f ${AS_IMAGE_NAME} 2> /dev/null || :
docker load -i ${AS_TAR}
./as.sh -c
AS_IP=`./as.sh -a`

################################# nfoam  ########################################
echo -e "\n\n################################# nfoam  ########################################\n"
NFOAM_TAR=${IMAGE_TAR_PATH}/centos-nf-oam.tar
NFOAM_IMAGE_NAME=centos-nf-oam
NFOAM_CNAME=nfoam

docker rm -f ${NFOAM_CNAME} 2> /dev/null || :
docker rmi -f ${NFOAM_IMAGE_NAME} 2> /dev/null || :
docker load -i ${NFOAM_TAR}
docker run -it -d --privileged="true" -e DBCONF="ip=${AS_IP}:3000" \
    --name="${NFOAM_CNAME}" --hostname="${NFOAM_CNAME}" \
    -v ${MOUNT_PATH}:/home/share -e LOGCONF='1' \
    -p 6001:6001  --env IN_PAAS="n" \
    --env TCFS="service=s1/microserviceType=m1/serviceLet=oambs" \
    ${NFOAM_IMAGE_NAME}

################################# oambs  #######################################
echo -e "\n################################# oambs  #######################################\n"
OAMBS_TAR=${IMAGE_TAR_PATH}/image_oambs.tar
OAMBS_IMAGE_NAME=itran/oambs
OAMBS_CNAME=oambs

docker rm -f ${OAMBS_CNAME} 2> /dev/null || :
docker rmi -f ${OAMBS_IMAGE_NAME} 2> /dev/null || :
docker load -i ${OAMBS_TAR}
# ./runc.sh -n ${OAMBS_CNAME} -a ${AS_IP} -m ${MOUNT_PATH} -i ${OAMBS_IMAGE_NAME}
docker run -it -d --privileged="true" -e DBCONF="ip=${AS_IP}:3000" \
    --name="${OAMBS_CNAME}" --hostname="${OAMBS_CNAME}" \
    -v ${MOUNT_PATH}:/home/share \
    ${OAMBS_IMAGE_NAME}

################################# vdbs  #######################################
echo -e "\n################################# vdbs  #######################################\n"
VDBS_TAR=${IMAGE_TAR_PATH}/image_vdbs.tar
VDBS_IMAGE_NAME=itran/vdbs
VDBS_CNAME=vdbs

docker rm -f ${VDBS_CNAME} 2> /dev/null || :
docker rmi -f ${VDBS_IMAGE_NAME} 2> /dev/null || :
docker load -i ${VDBS_TAR}
# ./runc.sh -n ${VDBS_CNAME} -a ${AS_IP} -m ${MOUNT_PATH} -i ${VDBS_IMAGE_NAME}
docker run -it -d --privileged="true" -e DBCONF="ip=${AS_IP}:3000" \
    --name="${VDBS_CNAME}" --hostname="${VDBS_CNAME}" \
    -v ${MOUNT_PATH}:/home/share \
    ${VDBS_IMAGE_NAME}

################################# vbrs  #######################################
echo -e "\n################################# vbrs  #######################################\n"
VBRS_TAR=${IMAGE_TAR_PATH}/brs.tar
VBRS_IMAGE_NAME=image_brs
VBRS_CNAME=vbrs

docker rm -f ${VBRS_CNAME} 2> /dev/null || :
docker rmi -f ${VBRS_IMAGE_NAME} 2> /dev/null || :
docker load -i ${VBRS_TAR}
# ./runc.sh -n ${VBRS_CNAME} -a ${AS_IP} -m ${MOUNT_PATH} -i ${VBRS_IMAGE_NAME}
docker run -it -d --privileged="true" -e DBCONF="ip=${AS_IP}:3000" \
    --name="${VBRS_CNAME}" --hostname="${VBRS_CNAME}" \
    -v ${MOUNT_PATH}:/home/share \
    ${VBRS_IMAGE_NAME}

################################# vdps  #######################################
echo -e "\n################################# vdps  #######################################\n"
VDPS_TAR=${IMAGE_TAR_PATH}/image_vdps.tar
VDPS_IMAGE_NAME=itran/vdps
VDPS_CNAME=vdps

docker rm -f ${VDPS_CNAME} 2> /dev/null || :
docker rmi -f ${VDPS_IMAGE_NAME} 2> /dev/null || :
docker load -i ${VDPS_TAR}
# ./runc.sh -n ${VDPS_CNAME} -a ${AS_IP} -m ${MOUNT_PATH} -i ${VDPS_IMAGE_NAME}  
docker run -it -d --privileged="true" -e DBCONF="ip=${AS_IP}:3000" \
    --name="${VDPS_CNAME}" --hostname="${VDPS_CNAME}" \
    -v ${MOUNT_PATH}:/home/share \
    -v ${MOUNT_PATH}/containers/vdps/vmdeploy.json:/vmdeploy.json \
    ${VDPS_IMAGE_NAME}

################################# psproxy0  #######################################
echo -e "\n################################# psproxy0  #######################################\n"
PSPROXY0_TAR=${IMAGE_TAR_PATH}/image_psproxy0.tar
PSPROXY0_IMAGE_NAME=itran/psproxy0
PSPROXY0_CNAME=psproxy0

docker rm -f ${PSPROXY0_CNAME} 2> /dev/null || :
docker rmi -f ${PSPROXY0_IMAGE_NAME} 2> /dev/null || :
docker load -i ${PSPROXY0_TAR}
# ./runc.sh -n ${PSPROXY0_CNAME} -a ${AS_IP} -m ${MOUNT_PATH} -i ${PSPROXY0_IMAGE_NAME}
docker run -it -d --privileged="true" -e DBCONF="ip=${AS_IP}:3000" \
    --name="${PSPROXY0_CNAME}" --hostname="${PSPROXY0_CNAME}" \
    -v ${MOUNT_PATH}:/home/share \
    ${PSPROXY0_IMAGE_NAME}


################################# psproxy5  #######################################
echo -e "\n################################# psproxy5  #######################################\n"
PSPROXY5_TAR=${IMAGE_TAR_PATH}/image_psproxy5.tar
PSPROXY5_IMAGE_NAME=itran/psproxy5
PSPROXY5_CNAME=psproxy5

docker rm -f ${PSPROXY5_CNAME} 2> /dev/null || :
docker rmi -f ${PSPROXY5_IMAGE_NAME} 2> /dev/null || :
docker load -i ${PSPROXY5_TAR}
# ./runc.sh -n ${PSPROXY5_CNAME} -a ${AS_IP} -m ${MOUNT_PATH} -i ${PSPROXY5_IMAGE_NAME}
docker run -it -d --privileged="true" -e DBCONF="ip=${AS_IP}:3000" \
    --name="${PSPROXY5_CNAME}" --hostname="${PSPROXY5_CNAME}" \
    -v ${MOUNT_PATH}:/home/share \
    ${PSPROXY5_IMAGE_NAME}

set +e
