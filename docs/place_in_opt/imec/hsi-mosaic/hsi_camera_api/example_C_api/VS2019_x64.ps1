# This script will create the Visual Studio solution for VS2019 with x64 architecture

$build_folder = "./build"

# Remove the build folder if it already exists
If (Test-Path -Path $build_folder)
{
    Remove-Item $build_folder -Recurse
}

# Create the build folder
New-Item -Path "." -Name $build_folder -ItemType "directory"

# Move into the build folder
cd $build_folder

# Create the solution
cmake .. -G "Visual Studio 16 2019" -A x64

# Move back to the example folder
cd ..