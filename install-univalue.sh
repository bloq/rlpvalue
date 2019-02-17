#!/bin/sh
set -ex
wget https://github.com/jgarzik/univalue/releases/download/v1.0.4/univalue-1.0.4.tar.gz
tar -xzvf univalue-1.0.4.tar.gz
cd univalue-1.0.4 && ./configure --prefix=/usr && make && sudo make install
