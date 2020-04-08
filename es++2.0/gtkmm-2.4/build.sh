#!/bin/bash
# build.sh
# Copyright (c) 2014 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
# 
for dir in ./* 
do
  if [ -d $dir ];then
    if [ -e "$dir/Makefile" ];then
      echo $dir
      cd $dir 
      make clean; make
      cd ../
      fi 
    fi
done
