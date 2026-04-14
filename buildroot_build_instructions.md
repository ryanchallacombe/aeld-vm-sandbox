### Buildroot build instructions: based on Assignment 4 part 2 and assignment 5 part 2 and assignment 7 part 2

## Setup
setup actions runner
used instructions here for secret key setup https://github.com/cu-ecen-aeld/aesd-assignments/wiki/Setting-up-Github-Actions

Perform any needed cloning/merging/pulling from git

# Add buildroot as a submodule

`git submodule add https://gitlab.com/buildroot.org/buildroot/`

`cd buildroot/`

`git checkout 2024.02.x`
	Branch '2024.02.x' set up to track remote branch '2024.02.x' from 'origin'.
	Switched to a new branch '2024.02.x

- Can confirm the branch is correct by looking at the CHANGES file to confirm it is from 2024
`more CHANGES`


- verify that it is there
`cat .gitmodules
[submodule "assignment-autotest"]
	path = assignment-autotest
	url = https://github.com/cu-ecen-5013/assignment-autotest
[submodule "buildroot"]
	path = buildroot
	url = https://gitlab.com/buildroot.org/buildroot/ `

 I sepnt a lot of time trying to figure out the next step, but the post below was helpful 
 Lots of info here in the
https://stackoverflow.com/questions/1777854/how-can-i-specify-a-branch-tag-when-adding-a-git-submodule

`git add buildroot/ `

`git commit -m "added buildroot submodule with branch 2024.02.x" `

`git push`

## Create any needed files for your buildroot module
In assingment 4 we did the following
- created 3 files in base_external dir. Pasted all contents directly from the hello_world example provided in lecutre slides, but changed dirname in Config.in
- Updated aesd-assignments.mk per lecture 
- no changes to base_external/package/aesd-assignments/Config.in
- Implement finder-test.sh updates described in 4.c of the assignment instructions. Pushed changes from /home/ryan/projects/assignment-1-ryanchallacombe
	Updated the commit hash in the aesd-assignments.mk based on this new commit

- ran ./build.sh and ./save-config.sh
Then can compare file contents of the defconfig vs. the aesd defconfig

############################################
cat qemu_aarch64_virt_defconfig
# Architecture
BR2_aarch64=y
BR2_cortex_a53=y

# System
BR2_SYSTEM_DHCP="eth0"

# Filesystem
BR2_TARGET_ROOTFS_EXT2=y
BR2_TARGET_ROOTFS_EXT2_4=y
# BR2_TARGET_ROOTFS_TAR is not set

# Image
BR2_ROOTFS_POST_IMAGE_SCRIPT="board/qemu/post-image.sh"
BR2_ROOTFS_POST_SCRIPT_ARGS="$(BR2_DEFCONFIG)"

# Linux headers same as kernel
BR2_PACKAGE_HOST_LINUX_HEADERS_CUSTOM_6_1=y

# Kernel
BR2_LINUX_KERNEL=y
BR2_LINUX_KERNEL_CUSTOM_VERSION=y
BR2_LINUX_KERNEL_CUSTOM_VERSION_VALUE="6.1.44"
BR2_LINUX_KERNEL_USE_CUSTOM_CONFIG=y
BR2_LINUX_KERNEL_CUSTOM_CONFIG_FILE="board/qemu/aarch64-virt/linux.config"
BR2_LINUX_KERNEL_NEEDS_HOST_OPENSSL=y

# host-qemu for gitlab testing
BR2_PACKAGE_HOST_QEMU=y
BR2_PACKAGE_HOST_QEMU_SYSTEM_MODE=y

############################################
$ cat base_external/configs/aesd_qemu_defconfig  
BR2_aarch64=y
BR2_PACKAGE_HOST_LINUX_HEADERS_CUSTOM_6_1=y
BR2_SYSTEM_DHCP="eth0"
BR2_ROOTFS_POST_IMAGE_SCRIPT="board/qemu/post-image.sh"
BR2_ROOTFS_POST_SCRIPT_ARGS="$(BR2_DEFCONFIG)"
BR2_LINUX_KERNEL=y
BR2_LINUX_KERNEL_CUSTOM_VERSION=y
BR2_LINUX_KERNEL_CUSTOM_VERSION_VALUE="6.1.44"
BR2_LINUX_KERNEL_USE_CUSTOM_CONFIG=y
BR2_LINUX_KERNEL_CUSTOM_CONFIG_FILE="board/qemu/aarch64-virt/linux.config"
BR2_LINUX_KERNEL_NEEDS_HOST_OPENSSL=y
BR2_TARGET_ROOTFS_EXT2=y
BR2_TARGET_ROOTFS_EXT2_4=y
# BR2_TARGET_ROOTFS_TAR is not set
BR2_PACKAGE_HOST_QEMU=y
BR2_PACKAGE_HOST_QEMU_SYSTEM_MODE=y
############################################


J. Ran make menucofig from the buildroot dir. Under external, the aesd-assignments package was there. I selected to add it to the image and saved it to 
/home/ryan/projects/assignment-4-ryanchallacombe/buildroot/.config

K. Run the save-config.sh script in the root folder to save the configuration
Then run $ cat base_external/configs/aesd_qemu_defconfig and see that the line below has been added to the file proving that the package is included in the build.
BR2_PACKAGE_AESD_ASSIGNMENTS=y

L. speedup options:/home/ryan/.dlsh

updated ~/.bashrc with 'export BR2_DL_DIR=${HOME}/.dl'

Verify with:
ryan@Ubuntu22:~$ echo $BR2_DL_DIR

M. Commit changes..........
cd ~/projects/assignment-4-ryanchallacombe/
git add .
git commit -m "initial buildroot configuration completed"
git push origin main

N. Run ./build script

O. create clean.

P. Ran make menucofig from the buildroot dir to add the dropbear package and create the root password as 'root' and saved it to:
/home/ryan/projects/assignment-4-ryanchallacombe/buildroot/.config
Ran the save-config.sh script in the root folder to save the configuration
run $ cat base_external/configs/aesd_qemu_defconfig and see that the lines below have been added to the file

BR2_PACKAGE_DROPBEAR=y
BR2_TARGET_GENERIC_ROOT_PASSWD="root"

Q. Ran clean.sh

R. Ran build.sh

S. Fix some finder-app.sh stuff and rebuild just the aesd-assignments.mk file, then ran from buildroot dir:
make aesd-assignments-rebuild 
then ran ./build.sh again (was very fast)

T. Run qemu
Discovered that we can run just $ finder-test.sh (instead of ./finder-test.sh) because it is in the system's PATH. 
Now I need to go back and edit the script, push to git, update the hash in the aesd-assignments.mk file, run make aesd-assignments-rebuild
the ./build.sh again 


Verification
1. Run finder-test.sh from the root of qemu (see video)

XXXX. SSH into the qemu system
ssh -p 10022 root@localhost
scp -P 10022 root@localhost:/tmp/assignment4-result.txt . 

#########################
### Assignment 5 part 2
#########################
1. Push start stop daemon script to repo. Update the aesd-assignments.mk file with the commit hash
2. Navigate to /home/ryan/projects/assignment-4-ryanchallacombe/buildroot
3. run `make aesd-assignments-rebuild`
4. Navigate to /home/ryan/projects/assignment-4-ryanchallacombe
5. run `./build.sh`

#########################
### Assignment 7 part 2 
#########################
1. Ran build.sh. Had some errors with pulling the aesd-assignments git. Ran clean.sh, followed by build.sh. This worked. 

2. Updated aesd-assignments.mk with latest commit hash. 
make aesd-assignments-rebuild (from buildroot dir)
then ran ./build.sh again (was very fast)

3. Created ldd package in: 
ryan@Ubuntu22:~/projects/assignment-5-ryanchallacombe/base_external/package$ ls -l
total 8
drwxrwxr-x 2 ryan ryan 4096 Apr 10 11:15 aesd-assignments
drwxrwxr-x 2 ryan ryan 4096 Apr 10 11:47 ldd
# files in there are:
ryan@Ubuntu22:~/projects/assignment-5-ryanchallacombe/base_external/package/ldd$ ls -l
total 8
-rw-rw-r-- 1 ryan ryan  195 Apr 10 11:59 Config.in
-rw-rw-r-- 1 ryan ryan 1981 Apr 10 21:06 ldd.mk

4. updated the file below to include the ldd package info: 
/home/ryan/projects/assignment-5-ryanchallacombe/base_external/Config.in

5. Ran make menucofig from the buildroot dir to add the ldd package. saved it to:
/home/ryan/projects/assignment-4-ryanchallacombe/buildroot/.config
Ran the save-config.sh script in the root folder to save the configuration
run $ cat base_external/configs/aesd_qemu_defconfig and see that the lines below have been added to the file:
BR2_PACKAGE_LDD=y

6. commit changes

7. Ran clean.sh

8. Ran build.sh

Errors I see on output:

warning: the compiler differs from the one used to build the kernel
  The kernel was built by: x86_64-linux-gnu-gcc-12 (Ubuntu 12.3.0-1ubuntu1~22.04.3) 12.3.0
  You are using:           aarch64-buildroot-linux-gnu-gcc.br_real (Buildroot 2024.02.13) 12.4.0
  CC [M]  /home/ryan/projects/assignment-5-ryanchallacombe/buildroot/output/build/ldd-5c3cae6ddc96b8645dfa6f6bc4ddbba08aae8789/misc-modules/hello.o
  CC [M]  /home/ryan/projects/assignment-5-ryanchallacombe/buildroot/output/build/ldd-5c3cae6ddc96b8645dfa6f6bc4ddbba08aae8789/misc-modules/hellop.o
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized argument in option ‘-mcmodel=kernel’
aarch64-buildroot-linux-gnu-gcc.br_real: note: valid arguments to ‘-mcmodel=’ are: large small tiny
  CC [M]  /home/ryan/projects/assignment-5-ryanchallacombe/buildroot/output/build/ldd-5c3cae6ddc96b8645dfa6f6bc4ddbba08aae8789/misc-modules/seq.o
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-sse’; did you mean ‘-fno-dse’?
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-mmx’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-sse2’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized argument in option ‘-mcmodel=kernel’
aarch64-buildroot-linux-gnu-gcc.br_real: note: valid arguments to ‘-mcmodel=’ are: large small tiny
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-3dnow’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-sse’; did you mean ‘-fno-dse’?
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-avx’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-m64’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-mmx’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-sse2’
  CC [M]  /home/ryan/projects/assignment-5-ryanchallacombe/buildroot/output/build/ldd-5c3cae6ddc96b8645dfa6f6bc4ddbba08aae8789/misc-modules/jiq.o
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-80387’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-3dnow’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-avx’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-m64’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized argument in option ‘-mcmodel=kernel’
aarch64-buildroot-linux-gnu-gcc.br_real: note: valid arguments to ‘-mcmodel=’ are: large small tiny
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-red-zone’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-80387’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-sse’; did you mean ‘-fno-dse’?
  CC [M]  /home/ryan/projects/assignment-5-ryanchallacombe/buildroot/output/build/ldd-5c3cae6ddc96b8645dfa6f6bc4ddbba08aae8789/misc-modules/sleepy.o
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-mmx’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-red-zone’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-sse2’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-3dnow’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized argument in option ‘-mcmodel=kernel’
aarch64-buildroot-linux-gnu-gcc.br_real: note: valid arguments to ‘-mcmodel=’ are: large small tiny
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-avx’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-m64’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-sse’; did you mean ‘-fno-dse’?
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-mmx’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-80387’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-sse2’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-3dnow’
  CC [M]  /home/ryan/projects/assignment-5-ryanchallacombe/buildroot/output/build/ldd-5c3cae6ddc96b8645dfa6f6bc4ddbba08aae8789/misc-modules/complete.o
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-avx’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-m64’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-red-zone’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized argument in option ‘-mcmodel=kernel’
aarch64-buildroot-linux-gnu-gcc.br_real: note: valid arguments to ‘-mcmodel=’ are: large small tiny
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-80387’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-sse’; did you mean ‘-fno-dse’?
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-mmx’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mfunction-return=thunk-extern’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-red-zone’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-sse2’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-3dnow’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-avx’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-m64’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-80387’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mfunction-return=thunk-extern’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mrecord-mcount’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mfunction-return=thunk-extern’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-red-zone’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mfunction-return=thunk-extern’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized argument in option ‘-mcmodel=kernel’
aarch64-buildroot-linux-gnu-gcc.br_real: note: valid arguments to ‘-mcmodel=’ are: large small tiny
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-sse’; did you mean ‘-fno-dse’?
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-mmx’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-sse2’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mrecord-mcount’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-3dnow’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-avx’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-m64’
make[4]: *** [scripts/Makefile.build:243: /home/ryan/projects/assignment-5-ryanchallacombe/buildroot/output/build/ldd-5c3cae6ddc96b8645dfa6f6bc4ddbba08aae8789/misc-modules/hellop.o] Error 1
make[4]: *** Waiting for unfinished jobs....
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mrecord-mcount’
make[4]: *** [scripts/Makefile.build:243: /home/ryan/projects/assignment-5-ryanchallacombe/buildroot/output/build/ldd-5c3cae6ddc96b8645dfa6f6bc4ddbba08aae8789/misc-modules/jiq.o] Error 1
  CC [M]  /home/ryan/projects/assignment-5-ryanchallacombe/buildroot/output/build/ldd-5c3cae6ddc96b8645dfa6f6bc4ddbba08aae8789/misc-modules/silly.o
make[4]: *** [scripts/Makefile.build:243: /home/ryan/projects/assignment-5-ryanchallacombe/buildroot/output/build/ldd-5c3cae6ddc96b8645dfa6f6bc4ddbba08aae8789/misc-modules/hello.o] Error 1
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mrecord-mcount’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mfunction-return=thunk-extern’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-80387’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized argument in option ‘-mcmodel=kernel’
aarch64-buildroot-linux-gnu-gcc.br_real: note: valid arguments to ‘-mcmodel=’ are: large small tiny
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mrecord-mcount’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-sse’; did you mean ‘-fno-dse’?
make[4]: *** [scripts/Makefile.build:243: /home/ryan/projects/assignment-5-ryanchallacombe/buildroot/output/build/ldd-5c3cae6ddc96b8645dfa6f6bc4ddbba08aae8789/misc-modules/sleepy.o] Error 1
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-mmx’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-sse2’
make[4]: *** [scripts/Makefile.build:243: /home/ryan/projects/assignment-5-ryanchallacombe/buildroot/output/build/ldd-5c3cae6ddc96b8645dfa6f6bc4ddbba08aae8789/misc-modules/seq.o] Error 1
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-red-zone’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-3dnow’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-avx’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-m64’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-80387’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mno-red-zone’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mfunction-return=thunk-extern’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mrecord-mcount’
make[4]: *** [scripts/Makefile.build:243: /home/ryan/projects/assignment-5-ryanchallacombe/buildroot/output/build/ldd-5c3cae6ddc96b8645dfa6f6bc4ddbba08aae8789/misc-modules/complete.o] Error 1
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mfunction-return=thunk-extern’
aarch64-buildroot-linux-gnu-gcc.br_real: error: unrecognized command-line option ‘-mrecord-mcount’
make[4]: *** [scripts/Makefile.build:243: /home/ryan/projects/assignment-5-ryanchallacombe/buildroot/output/build/ldd-5c3cae6ddc96b8645dfa6f6bc4ddbba08aae8789/misc-modules/silly.o] Error 1
make[3]: *** [Makefile:1936: /home/ryan/projects/assignment-5-ryanchallacombe/buildroot/output/build/ldd-5c3cae6ddc96b8645dfa6f6bc4ddbba08aae8789/misc-modules] Error 2
make[2]: *** [Makefile:20: modules] Error 2
make[1]: *** [package/pkg-generic.mk:283: /home/ryan/projects/assignment-5-ryanchallacombe/buildroot/output/build/ldd-5c3cae6ddc96b8645dfa6f6bc4ddbba08aae8789/.stamp_built] Error 2
make: *** [Makefile:82: _all] Error 2
make: Leaving directory '/home/ryan/projects/assignment-5-ryanchallacombe/buildroot'

real	40m49.852s
user	108m25.591s
sys	44m11.075s

9. Updated ldd.mk with by uncommenting $(eval $(kernel-module))
make ldd-rebuild (from buildroot dir)
then ran ./build.sh again (was very fast)

I ran into the same errors. When I took the build BUILD_CMDS section out of ldd.mk, the issues went away. 

10. Ran ./build again. This time it ran fine. Started up QEMU and looked good. Modules are placed into install dir as spec'd in ldd.mk

11. Rootfs overlay / init script
a. make ../base_external/rootfs_overlay dir
b. add an init module script named “S98lddmodules" at the following path: /base_external/rootfs_overlay/etc/init.d

12. Ran make menuconfig from the buildroot dir 
under system config > rootfs directories ..??... 
for the directory to overlay: ../base_external/rootfs_overlay 
saved, then exited menuconfig.
Ran the save-config.sh script in the root folder to save the configuration
Now we see the following new line: BR2_ROOTFS_OVERLAY="../base_external/rootfs_overlay"

13. Ran build script again and everything looked good. 


