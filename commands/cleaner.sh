#!/bin/bash

# Get the directory where the script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# Find and remove directories named '2024-*' in the script's directory
find "$SCRIPT_DIR" -type d -name '2024-*' -exec rm -rf {} +

echo "Cleaned the folder"

