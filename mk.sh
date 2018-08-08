rm -rf common/moxa_bios

mkdir -p common/moxa_bios/

file=(`find common/moxa_src/common/ -type f`)

for ((i=0; i<${#file[@]}; i++)); do
      cp -a ${file[$i]} common/moxa_bios
done

cp common/cmd_bios.h common/moxa_bios
cp common/moxa_src/common/model.h include/

export PATH="$PATH:/usr/local/arm-linux-gnueabihf-6.3/usr/bin"
#make -o ./mx7 CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm mx7dsabresd_issi_defconfig
make -o ./mx7 CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm mx7dsabresd_moxa_defconfig
make -o ./mx7 CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm -j 30

python2.7 build_spi.py header-tmp u-boot.imx .

