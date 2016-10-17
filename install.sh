#!/bin/bash

cd .git/hooks
ln -f ../../hooks/pre-commit pre-commit
echo "Installation réussie."
