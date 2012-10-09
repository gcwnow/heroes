#!/usr/bin/perl

while (<>) {
    if (/^\w/) {
	print;
	while (<>) {
	    exit if (/^\w/);
	    print;
	}
    }
}
