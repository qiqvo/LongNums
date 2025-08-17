#!/bin/bash

# Build script for LongNums Python module

set -e  # Exit on any error

echo "=== Building LongNums Python Module ==="

# Check if Python is available
if ! command -v python3 &> /dev/null; then
    echo "Error: python3 is not installed"
    exit 1
fi

# Check if pip is available
if ! command -v pip3 &> /dev/null; then
    echo "Error: pip3 is not installed"
    exit 1
fi

# Install required dependencies
echo "Installing Python dependencies..."
pip3 install -r requirements.txt

# Build the module
echo "Building the module..."
python3 pybind11_setup.py build_ext --inplace

# Install the module (copy the built .so file to current directory)
echo "Installing the module..."
cp build/lib.*/longnums*.so ./longnums.so

echo "=== Build completed successfully! ==="
echo ""
echo "You can now use the module in Python:"
echo "  import longnums as ln"
echo "  a = ln.Long('12345678901234567890')"
echo "  print(a)"
echo ""
echo "To run the example:"
echo "  python3 example_usage.py"
echo ""
echo "To test the module:"
echo "  python3 test_module.py"
