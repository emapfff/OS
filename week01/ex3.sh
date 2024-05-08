cd ~/week01
mkdir root_folder
echo " "
date
sleep 3
cd root_folder/
touch root.txt
echo " "
date
sleep 3
ls -tr / > root.txt
cd ..
mkdir home_folder
echo " "
date
sleep 3
cd home_folder/
touch home.txt
echo " "
date
sleep 3
ls -tr ~ > home.txt
cd ..
echo " "
cat root_folder/root.txt
echo " "
cat home_folder/home.txt
echo " "
ls root_folder
echo " "
ls home_folder
