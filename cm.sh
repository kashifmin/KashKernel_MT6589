CM11_OUT_DIRECTORY=../out/cm11/$TARGET_PRODUCT
OUT_DIRECTORY=../out/$TARGET_PRODUCT
SCRIPTS_DIRECTORY=../scripts/$TARGET_PRODUCT
CERTIFICATES_DIRECTORY=../.certificates
CM11_RAMDISK_DIRECTORY=../ramdisk/cm11/$TARGET_PRODUCT

#Create and clean CM11 out directory for your device
mkdir -p $CM11_OUT_DIRECTORY
if [ "$(ls -A $CM11_OUT_DIRECTORY)" ]; then
rm $CM11_OUT_DIRECTORY/* -R
fi

../mediatek/build/tools/mkimage arch/arm/boot/zImage KERNEL > $CM11_OUT_DIRECTORY/zImage
../mtk-tools/repack-MT65xx.pl -boot $CM11_OUT_DIRECTORY/zImage $CM11_RAMDISK_DIRECTORY $CM11_OUT_DIRECTORY/boot.img
rm $CM11_OUT_DIRECTORY/zImage
cp $OUT_DIRECTORY/system $CM11_OUT_DIRECTORY -R

if [ -d "$SCRIPTS_DIRECTORY" ]; then
cp $SCRIPTS_DIRECTORY/* $CM11_OUT_DIRECTORY -R
FLASHABLE_ZIP="$CM11_OUT_DIRECTORY/`cat DEVICE_NAME`-CM11-`make kernelversion`-`git rev-parse --short HEAD`"
FLASHABLE_ZIP_2="`cat DEVICE_NAME`-CM11-`make kernelversion`-`git rev-parse --short HEAD`"
echo "Creating flashable at '$FLASHABLE_ZIP'.zip"
pushd $CM11_OUT_DIRECTORY
zip -r -0 "$FLASHABLE_ZIP_2".zip .
popd
if [ ! -d "$CERTIFICATES_DIRECTORY" ]; then
echo "Warning ! We can't sign flashable.zip, you need to run ./certificates.sh"
else
java -jar $SCRIPTS_DIRECTORY/../signapk.jar $CERTIFICATES_DIRECTORY/certificate.pem $CERTIFICATES_DIRECTORY/key.pk8 "$FLASHABLE_ZIP".zip "$FLASHABLE_ZIP"-signed.zip
fi
fi
