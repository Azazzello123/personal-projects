#!/bin/bach

echo  "Cleaning up the system..."

echo "Updating package list..."


sudo apt update -y

echo "Upgrading installed packages..."

sudo apt upgrade -y

echo "Removing unused packages..."

sudo apt autoremove -y
sudo apt autoclean -y
sudo apt clean -y

echo "Clearing systemd journal logs older than 10 days..."

sudo journalctl --vacuum-time=10d

echo "Clearing temporary files..."

sudo rm -rf /tmp/*
sudo rm -rf /var/tmp/*

echo "Finished cleaning up the system!"
