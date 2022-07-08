#!/bin/bash

root="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

wiredtiger=${root}/third-party/wiredtiger
gflags=${root}/third-party/gflags
glog=${root}/third-party/glog
local=${root}/third-party/local

function init() {
    git submodule init
    git submodule update --remote
}

function install_wiredtiger() {
    cd ${wiredtiger}
    cversion=`rpm -q centos-release|cut -d- -f3`
    if [ $cversion -eq 7 ]
    then
        git checkout f8cb8f23f
        ./autogen.sh
        ./configure --prefix=${local}
        make -j12 && make install
    else
        git checkout develop
        mkdir -p build && cd build
        cmake -DCMAKE_INSTALL_PREFIX=${local} \
            -DCMAKE_BUILD_TYPE=Release -DENABLE_STATIC=1 ..
        make -j4 && make install
    fi
}


function install_gflags() {
    cd ${gflags}
    rm -rf builds
    mkdir -p builds && cd builds
    cmake -DCMAKE_INSTALL_PREFIX=${local} \
        -DCMAKE_PREFIX_PATH=${local} \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_STATIC_LIBS=ON -DINSTALL_HEADERS=ON \
        -DINSTALL_SHARED_LIBS=ON -DINSTALL_STATIC_LIBS=ON ..
    make -j4 && make install
}


function install_glog() {
    cd ${glog}
    rm -rf build
    mkdir -p build && cd build
    cmake -DCMAKE_INSTALL_PREFIX=${local} \
        -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF ..
    make -j4 && make install
}

#init
#install_wiredtiger
install_gflags
#install_glog

