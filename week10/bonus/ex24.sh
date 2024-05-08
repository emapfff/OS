#!bin/bash
rm -rf ./tmp
ln -s ./tmp tmp1
ls -li
mkdir ./tmp
ls -li
echo "The difference is before creating of ./tmp folder tmp1 points to folder which did not exist and type of link is ln -s ./tmp tmp1, but after creating folder ./tmp points on existing folder and type of this link is symbolic link to ./tmp."
bash gen.sh 5 ex1.txt
mv ex1.txt ./tmp
bash gen.sh 5 ex1.txt
mv ex1.txt ./tmp1/tmp2
echo "I noticed that it results in an infinite loop or recursion. This happens because the symbolic link 'tmp2' points to the './tmp' folder, which in turn contains the symbolic link 'tmp2' again, creating an infinite loop of symbolic links."
rm -rf ./tmp
find -type l -exec rm {} \;
