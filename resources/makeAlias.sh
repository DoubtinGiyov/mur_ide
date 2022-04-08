#!/bin/bash

osascript <<END_SCRIPT
    tell application "Finder" to make alias file to alias (POSIX file "$1") at (POSIX file "/Applications/")
END_SCRIPT
