#!/bin/bash

if [ -z "$1" ]; then 
    echo "Usage: $0 \"Your Commit Message\""
    exit 1
fi

git add .

git commit -m "$1"

git push origin

echo "Changes committed and pushed successfully"

