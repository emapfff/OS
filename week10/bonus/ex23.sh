bash gen.sh 10 ex1.txt
ln ex1.txt ex11.txt
ln ex1.txt ex12.txt
echo "Content of files ex1.txt, ex11.txt and ex12.txt are the same. Because these links ex11.txt and ex12.txt points to file ex1.txt and they have identical inodes, size, links and total size and size of each block, therefore contents are as in ex1.txt. Essentially there are the same files with different names."
stat -c "%i" ex1.txt >> output.txt
stat -c "%i" ex11.txt >> output.txt
stat -c "%i" ex12.txt >> output.txt
ln -d ex1.txt ex13.txt
mv ex13.txt /tmp
echo "Founded files with the same inodes in the current directory"
find ./ -inum $(stat -c "%i" ex1.txt)
echo "Founded files with the same inodes in the root directory"
find / -inum $(stat -c "%i" ex1.txt)
echo "The difference is that founded files in the the current directory are ex11.txt and ex12.txt, but we created link between ex1.txt and ex13.txt and via find in root directory we can see that after moved ex13.txt to /tmp, search in the root allow find all files in the system with the same inode as ex1.txt, but search in the current does not allow." 
n=$(stat -c "%h" ex1.txt)
echo "Number of hard links of ex1.txt = $n"
p=$(stat -c "%i" ex1.txt)
find / -inum $p -exec rm {} \;
