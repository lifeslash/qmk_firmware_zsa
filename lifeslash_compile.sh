#git remote -v
#git fetch upstream
#git checkout -b firmware23 upstream/firmware23
#git push origin firmware23
#qmk git-submodule

#macBin="voyager_lifeslash_mac_ja.bin"
#linuxBin="ergodox_ez_lifeslash_linux.bin"
#winBin="ergodox_ez_lifeslash_win.bin"

rm *.bin
rm *.hex

make clean

#if [ -e $macBin ]; then
#    rm $macBin
#fi
#if [ -e $linuxBin ]; then
#    rm $linuxBin
#fi
#if [ -e $winBin ]; then
#    rm $winBin
#fi

#make voyager:lifeslash_mac_ja
qmk compile -kb voyager -km lifeslash_mac_ja
qmk compile -kb ergodox_ez -km lifeslash_linux
qmk compile -kb ergodox_ez -km lifeslash_win
