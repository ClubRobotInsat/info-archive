#!/bin/bash

OLD_DIR="$(pwd)"
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$DIR"

if [ "$#" -gt 1 ]; then
    echo "Usage: format.sh [dir]"
    exit 1
fi

if [ "$#" -eq 0 ]; then
    format_path=("commun" "robot" "simulateur")
else
    format_path=("$1")
fi

find "${format_path[@]}" \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \) -not -path "commun/Units*" -not -path "simulateur/server/asio*" -not -path "simulateur/server/websocketpp*" -exec clang-format -i {} \;

cd "$OLD_DIR"

echo "Formating done."
