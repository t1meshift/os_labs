#!/usr/bin/env bash

set -euo pipefail
IFS=$'\n\t'

echo -e "\nTesting script started...\n"

if [ -f "$1/.execme" ]; then
  echo "Found .execme file!"
  if [ -x "$1/.execme" ]; then
    echo "Executing..."
    "$1/.execme" "$1"
    exit $?
  else
    echo ".execme couldn't be executed!"
    echo "Run chmod +x \"$1/.execme\" ."
    exit 1
  fi
fi

echo "Using default exec all policy..."

while IFS= read -r -d '' FILE
do
  echo "Executing $FILE..."
  $FILE
done<   <(find "$1" -name '*_run' -print0 | sort -z)


echo "Fin."
echo