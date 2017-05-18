#!/bin/bash

FILES=testScripts/*

for f in $FILES
do
  echo "Processing $f..."
  cat "$f"
  echo
  echo "Running $f"
  echo
  ./compile "$f"
  echo
done
