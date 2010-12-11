#!/bin/bash
FILESLIST=fileslist.txt
find . -name "*.cpp" | grep -v "./tags" > $FILESLIST
find . -name "*.c" | grep -v "./tags" >> $FILESLIST
find . -name "*.hpp" | grep -v "./tags" >> $FILESLIST
find . -name "*.h" | grep -v "./tags" >> $FILESLIST
find . -name "SConstruct" | grep -v "./tags" >> $FILESLIST
