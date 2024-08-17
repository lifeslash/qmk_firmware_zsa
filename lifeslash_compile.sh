#git remote -v
#git fetch upstream
#git checkout -b firmware23 upstream/firmware23
#git push origin firmware23
#qmk git-submodule

macBin="voyager_lifeslash_mac_ja.bin"
if [ -e $macBin ]; then
    rm $macBin
fi
#make voyager:lifeslash_mac_ja
qmk compile -kb voyager -km lifeslash_mac_ja
