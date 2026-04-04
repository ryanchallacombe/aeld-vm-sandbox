#!/bin/bash

set -e
set -u

FILE_LOC_1=/tmp
FNAME_1=deleteme.txt
SYSROOT_CROSS_COMPILER=/home/ryan/projects/aarch64_toolchain_install_dir/install/arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-linux-gnu/aarch64-none-linux-gnu/libc
FNAME_2=lib/ld-linux-aarch64.so.1


# add path to my cross compiler
export PATH=$PATH:/home/ryan/projects/aarch64_toolchain_install_dir/install/arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-linux-gnu/bin



if [ -f "${SYSROOT_CROSS_COMPILER}/${FNAME_2}" ]; then
    echo "**** File ${SYSROOT_CROSS_COMPILER}/${FNAME_2} was found"
    echo "EXITING WITH 0"
    exit 0
else
    echo "**** UNABLE TO FIND ${SYSROOT_CROSS_COMPILER}/${FNAME_2}"
    echo "EXITING WITH 1"
    exit 1
fi

if [ -f "${FILE_LOC_1}/${FNAME_1}" ]; then
    echo "**** File ${FILE_LOC_1}/${FNAME_1} was found"
    echo "EXITING WITH 0"
    exit 0
else
    echo "**** UNABLE TO FIND .${FILE_LOC_1}/${FNAME_1}"
    echo "EXITING WITH 1"
    exit 1
fi


######################################################
# Expirmental

mkdir -p ${OUTDIR}/rootfs/lib
mkdir -p ${OUTDIR}/rootfs/lib64

echo "Moving back to ${OUTDIR}/rootfs"
cd ${OUTDIR}/rootfs

echo "********* Printing working directory ********* "
echo $(pwd)
echo "********* ls ********* "
echo $(ls)

echo "Moving to starting directory ${FINDER_APP_DIR}"
cd ${FINDER_APP_DIR}

echo "********* Printing working directory ********* "
echo $(pwd)
echo "********* ls ********* "
echo $(ls)



cp ld-linux-aarch64.so.1 ${OUTDIR}/rootfs/lib
cp libm.so.6 ${OUTDIR}/rootfs/lib64
cp libresolv.so.2 ${OUTDIR}/rootfs/lib64
cp libc.so.6 ${OUTDIR}/rootfs/lib64

echo "Moving back to ${OUTDIR}/rootfs"
cd ${OUTDIR}/rootfs

echo "********* Printing working directory ********* "
echo $(pwd)
echo "********* ls ********* "
echo $(ls)


exit 0

######################################################

#######################
# Experimental

echo "********* Printing working directory ********* "
echo $(pwd)

echo "Making Pipe with mknod"
sudo mknod -m 666 my_pipe p

echo "********* ls -l ********* "
echo $(ls -l)


exit 1

#######################