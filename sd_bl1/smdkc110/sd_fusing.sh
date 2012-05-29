####################################
reader_type1="/dev/sdb"
reader_type2="/dev/mmcblk0"

if [ -z $1 ]
then
    echo "usage: ./sd_fusing.sh <SD Reader's device file>"
    exit 0
fi

if [ $1 = $reader_type1 ]
then 
    partition1="$11"
    partition2="$12"
    partition3="$13"
    partition4="$14"

elif [ $1 = $reader_type2 ]
then 
    partition1="$1p1"
    partition2="$1p2"
    partition3="$1p3"
    partition4="$1p4"

else
    echo "Unsupported SD reader"
    exit 0
fi

if [ -b $1 ]
then
    echo "$1 reader is identified."
else
    echo "$1 is NOT identified."
    exit 0
fi

####################################
# make partition
echo "make sd card partition"
echo "./sd_fdisk $1" 
./sd_fdisk $1 
dd iflag=dsync oflag=dsync if=sd_mbr.dat of=$1 
rm sd_mbr.dat
 
sync

get_char(){
   SAVEDSTTY=`stty -g`
   stty -echo
   stty raw
   dd if=/dev/tty bs=1 count=1 2> /dev/null
   stty -raw
   stty echo
   stty $SAVEDSTTY
}

echo "Please Re-inserted SD card and Press Enter to continue..."
char=`get_char`
read -s -n 10 k
echo $k

####################################
# format
umount $partition1 2> /dev/null
umount $partition2 2> /dev/null
umount $partition3 2> /dev/null
umount $partition4 2> /dev/null

echo "mkfs.vfat -F 32 $partition1"
mkfs.vfat -F 32 $partition1

#echo "mkfs.ext2 $partition2"
#mkfs.ext2 $partition2  

#echo "mkfs.ext2 $partition3"
#mkfs.ext2 $partition3  

#echo "mkfs.ext2 $partition4"
#mkfs.ext2 $partition4  

####################################
# mount 
#umount /media/sd 2> /dev/null
#mkdir -p /media/sd
#echo "mount -t vfat $partition1 /media/sd"
#mount -t vfat $partition1 /media/sd

####################################
#<BL1 fusing>
bl1_position=1
uboot_position=17

echo "BL1 fusing"
dd iflag=dsync oflag=dsync if=skdv210-BL1-sd2.bin.padding of=$1 seek=$bl1_position

####################################
#<u-boot fusing>
echo "u-boot fusing"
dd iflag=dsync oflag=dsync if=../../u-boot.bin of=$1 seek=$uboot_position

####################################
#<Message Display>
echo "U-boot image is fused successfully."
echo "Eject SD card and insert it again."
