#!/bin/bash

OLD_DIR="$(pwd)"
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${DIR}/.."

if [ "$#" -gt 1 ]; then
    echo "Usage: format.sh [dir]"
    exit 1
fi

if [ "$#" -eq 0 ]; then
    format_path=("src/commun" "src/robot" "src/simulateur" "src/Lidar")
else
    format_path=("$1")
fi

find "${format_path[@]}" \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \) -not -path "src/commun/Units*" -not -path "third_parties/*" -not -path "src/simulateur/legacy*" -exec clang-format -i {} \;

if [ "$?" -eq 0 ]; then
    echo "Formatting done."
else
    echo "Formatting failed."
fi

cd "${OLD_DIR}"
