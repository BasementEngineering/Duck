# Run the Python program
Write-Host "Running the Python program to encode the Arduino files"
python encodeInArduino.py

# Copy the files to the desired location
# Print statements
Write-Host "Copying the files to the desired location"
$sourceFolder = Join-Path -Path $PSScriptRoot -ChildPath "dist"
$destinationFolder = Join-Path -Path (Split-Path -Path $PSScriptRoot -Parent) -ChildPath "duck_code"

Write-Host "Source folder: $sourceFolder"
Write-Host "Destination folder: $destinationFolder"
Copy-Item -Path "$sourceFolder\website_content.h" -Destination $destinationFolder
Copy-Item -Path "$sourceFolder\website_functions.h" -Destination $destinationFolder