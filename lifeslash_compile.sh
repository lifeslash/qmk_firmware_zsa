macBin="voyager_lifeslash_mac_ja.bin"
if [ -e $macBin ]; then
    rm $macBin
fi
make voyager:lifeslash_mac_ja
