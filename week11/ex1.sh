#!/bin/bash

# Create a 50MB empty disk image file filled with zeros
sudo dd if=/dev/zero of=lofs.img bs=1M count=50

# Associate the loop device with the disk image
sudo losetup -f lofs.img

# Get the loop device path
n=$(sudo losetup -a | grep lofs.img | cut -d ' ' -f 1 | tr -d :)

# Create an ext4 filesystem on the loop device
sudo mkfs.ext4 $n

# Create a directory to mount the loop device
mkdir ./lofsdisk

# Mount the loop device to the created directory
sudo mount -o loop $n ./lofsdisk

# Create and write "Emil" to file1 in the mounted filesystem
sudo touch ./lofsdisk/file1
echo "Emil" | sudo tee ./lofsdisk/file1 >/dev/null

# Create and write "Davlityarov" to file2 in the mounted filesystem
sudo touch ./lofsdisk/file2
echo "Davlityarov" | sudo tee ./lofsdisk/file2 >/dev/null

# Create the necessary directory structure for the chroot environment
sudo mkdir -p ./lofsdisk/bin

# Function to retrieve the required libraries for a given binary executable
get_libs() {
  # Take the path of the binary executable as an argument
  binary_path=$1

  # Use ldd to list the shared libraries required by the binary
  # Extract the third column of the output (the library paths)
  libs=$(ldd "$binary_path" | awk '/=>/ {print $3}')

  # Print the list of libraries
  echo $libs
}

# Function to add required libraries for a command to the chroot environment
add_libs() {
  # Take the command name as an argument
  path=$(which "$1")

  # Print the path of the command
  echo "Command path $path"

  # Use the get_libs function to get the required libraries for the command
  libs=$(get_libs "$path")

  # Print the list of libraries
  echo $libs

  # Copy the required libraries and their directory structure to the chroot environment
  sudo cp --parents $libs ./lofsdisk/

  # Copy the command itself to the bin directory in the chroot environment
  sudo cp "$path" ./lofsdisk/bin
}

# Add necessary libraries for the specified commands to the chroot environment
add_libs "bash"
add_libs "ls"
add_libs "cat"
add_libs "echo"

# Create a directory structure in the chroot environment to mimic /lib64
sudo mkdir -p ./lofsdisk/lib64

# Bind-mount the actual /lib64 directory into the chroot environment at ./lofsdisk/lib64
sudo mount --bind /lib64 ./lofsdisk/lib64

# Create a directory structure in the chroot environment to mimic /lib
sudo mkdir -p ./lofsdisk/lib

# Bind-mount the actual /lib directory into the chroot environment at ./lofsdisk/lib
sudo mount --bind /lib ./lofsdisk/lib

# Compile ex1.c to create an executable named ex1
gcc ex1.c -o ex1

# Move the compiled executable to the chroot environment
sudo mv ex1 ./lofsdisk

# Run ex1 within the chroot environment and redirect the output to ex1.txt
sudo chroot ./lofsdisk /bin/bash -c "./ex1 > /ex1.txt"

# Compile ex1.c again (outside the chroot environment)
gcc ex1.c -o ex1

# Append the result of running ex1 to ex1.txt (outside the chroot environment)
sudo echo "Running without chroot:" >> ./lofsdisk/ex1.txt
nn=$(./ex1)
sudo echo "$nn" >> ./lofsdisk/ex1.txt
sudo echo "The diffrence conclude in that a chroot is an operation that changes the apparent root directory for the current running process and their children, and after running ex1, which prints folders in root, at the moment chroot we see that ex1 prints files and folders in ./lofsdisk, but after chroot ex1 prints original root files and folders."
