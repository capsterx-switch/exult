#!/bin/bash
#LDFLAGS=-specs=/opt/devkitpro/libnx/switch.specs PATH=/opt/devkitpro/devkitA64/bin/:/home/jbrannan/bin:/home/jbrannan/.local/bin:/opt/devkitpro/tools/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/usr/share/rvm/bin ./configure --target=aarch64-none-elf --host=aarch64-none-elf --with-sdl-prefix=/opt/devkitpro/portlibs/switch
if [ ! -e Makefile ]
then
  CFLAGS="-fPIC" CXXFLAGS="-fPIC -std=gnu++17" LDFLAGS="-specs=${DEVKITPRO}/libnx/switch.specs" PATH=/opt/devkitpro/devkitA64/bin/:$HOME/switch/exult/bin:$PATH ./configure  --target=aarch64-none-elf --host=aarch64-none-elf --with-sdl-prefix=$DEVKITPRO/portlibs/switch
fi
rm exult     exult.elf  exult.nacp  exult.nso  exult.pfs0
PATH=/opt/devkitpro/devkitA64/bin/:$HOME/switch/exult/bin:$PATH make
mkdir build
mv exult exult.elf
cp exult.elf build
make -f switch.mk
