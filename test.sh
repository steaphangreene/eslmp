#!/bin/bash

if [ ! -x eslmp_dec ] || [ ! -x eslmp_enc ]; then
  echo "ERROR: No tool present."
  echo "Run 'make' to build it."
  exit
fi

for file in "$@"; do
  echo -n "Testing on '$file'..."
  correct=$(md5sum "$file" | cut -f1 -d' ')
  result=$(./eslmp_dec "$file" | ./eslmp_enc | md5sum - | cut -f1 -d' ')
  if [ "$result" != "$correct" ]; then
    echo "failed."
    echo "Output:  $result"
    echo "Correct: $correct"
  else
    echo "works."
  fi
done
