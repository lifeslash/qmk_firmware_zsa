#git remote -v
#git fetch upstream firmware23
#git pull upstream firmware23:firmware23
#git push origin firmware23

macBin="voyager_lifeslash_mac_ja.bin"
if [ -e $macBin ]; then
    rm $macBin
fi
make voyager:lifeslash_mac_ja
