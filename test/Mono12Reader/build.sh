#!/bin/bash

# Activate virtual environment if you have one
#source /path/to/your/venv/bin/activate  # Uncomment and modify this line if you're using a virtual environment

# Navigate to the directory of your Python script
cd "$(dirname "$0")" || exit

# Clean up previous builds and distributions
rm -rf build/ dist/ Mono12Reader_V0.2.spec

# Use PyInstaller to package the script
pyinstaller --onefile --hidden-import=PIL._tkinter_finder --hidden-import=PIL.ImageTk --hidden-import=tkinter Mono12Reader_V0.2.py


# Check if the dist directory and executable exist
if [[ -d "dist" && -f "dist/Mono12Reader_V0.2" ]]; then
    # Move the executable to the desired location
    mv dist/Mono12Reader_V0.2 ../../data/
    echo "Executable moved to ../../data/"
else
    echo "Failed to create the executable."
fi

# Optional: clean up PyInstaller output directories
rm -rf build/ dist/ Mono12Reader_V0.2.spec

echo "Packaging complete."
