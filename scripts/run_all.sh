#!/usr/bin/env bash

set -euo pipefail
IFS=$'\n\t'

echo -e "\nTesting script started...\n"

while IFS= read -r -d '' FILE
do
  echo "Executing $FILE..."
  $FILE
done<   <(find "$1" -name '*_run' -print0 | sort -z)


echo "Fin."
echo