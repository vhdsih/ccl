#!/bin/bash

root="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

wiredtiger=${root}/wiredtiger
gflags=${root}/gflags
glog=${root}/glog
local=${root}/local


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
        rm -rf build
        mkdir -p build && cd build
        cmake -DCMAKE_INSTALL_PREFIX=${local} -DCMAKE_POSITION_INDEPENDENT_CODE=ON ..
        make -j12 && make install
    fi
}


function install_gflags() {
    cd ${gflags}
    rm -rf builds
    mkdir -p builds && cd builds
    cmake -DCMAKE_INSTALL_PREFIX=${local} \
        -DCMAKE_PREFIX_PATH=${local} \
        -DCMAKE_BUILD_TYPE=Release \
        -DINSTALL_HEADERS=ON \
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON ..
    make -j4 && make install
}


function install_glog() {
    cd ${glog}
    rm -rf build
    mkdir -p build && cd build
    cmake -DCMAKE_INSTALL_PREFIX=${local} \
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
        -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON ..
    make -j4 && make install
}

install_wiredtiger
install_gflags
install_glog

