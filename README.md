Installation on Ubuntu
====

    apt-get install libtool git-core
    git clone http://github.com/admp/json-c.git
    cd json-c/src
    sh autogen.sh
    ./configure
    make
    sudo make install
    sudo ldconfig

Installation on OpenEmbedded
====

    bitbake json-c
