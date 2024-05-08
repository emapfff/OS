touch ex5.txt
chmod ugo-w ex5.txt
chmod uo+rwx ex5.txt
chmod -R g=u ex5.txt
echo "660 permission means: I (owner) can write and read the file. Group members can write and read the file."
echo "The chmod 775 is an essential command that assigns read, write, and execute permission to a specific user, group, or others."
echo "Setting 777 permissions to a file or directory means that it will be readable, writable and executable by all users and may pose a huge security risk."
