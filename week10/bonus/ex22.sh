size=$(stat -c "%s" ex1)
nb=$(stat -c "%b" ex1)
sb=$(stat -c "%o" ex1)
permissions=$(ls -l ex1| awk '{print $1}')
echo "Number of blocks = $nb"
echo "Size of each block = $sb"
echo "Total size = $size bytes"
echo "Permissions: $permissions"
echo "Number of links of ex1 and ex2 files are the same, equals to 1. Inodes of each this file are differrent, because these is a unique identifiers store metadata about each file."
stat -c "%h - %n" /etc/* | grep ^3
echo "3 links means, that these file consists 3 hard links, where hard link is a additional link for file that points to the same inode as a original file."

