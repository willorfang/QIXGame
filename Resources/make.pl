#!/usr/bin/perl

#use strict;
#use warnings;

my $file_dest = "resource.plist";

my $dir_image = "image";
my $dir_sound = "sound";
my $dir_font = "fonts";
my $dir_animation = "animation";

sub dealWithOneDir {
    my $fh_dest = shift;
    my $dir = shift;

    # header
    print $fh_dest ("    <key>$dir</key>\n");
    print $fh_dest ("    <array>\n");
    # content
    opendir(my $dh, $dir) or die "Couldn't open $dir";
    foreach my $file (readdir $dh) {
        if ($file ne "." && $file ne "..") {
            print $fh_dest ("        <string>$dir/$file</string>\n");
        }
    }
    closedir($dh);
    # tail
    print $fh_dest ("    </array>\n");
}

# plist header
open(OUTPUT, ">$file_dest") or die "Couldn't open $file_dest!";
print OUTPUT ("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
print OUTPUT ("<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n");
print OUTPUT ("<plist version=\"1.0\">\n");
print OUTPUT ("<dict>\n");

# content
my $handle = \*OUTPUT;
&dealWithOneDir($handle, $dir_image);
&dealWithOneDir($handle, $dir_sound);

# tail
print OUTPUT ("</dict>\n");
print OUTPUT ("</plist>\n");
close(OUTPUT);
