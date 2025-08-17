#!/usr/bin/env python3
"""
Simple test to check basic functionality
"""

try:
    import longnums as ln

    print("✓ Module imported successfully")

    # Test Long class
    a = ln.Long(123)
    b = ln.Long(456)
    c = a + b
    print(f"✓ Long arithmetic: {a} + {b} = {c}")

    # Test Matrix class
    A = ln.Matrix(2, 2, 1.0)
    B = ln.Matrix(2, 2, 2.0)
    print(f"✓ Matrix created: {A.rows()}x{A.cols()}")

    # Test mathematical functions
    x = ln.Long(48)
    y = ln.Long(18)
    gcd_result = ln.gcd(x, y)
    print(f"✓ GCD calculation: gcd(48, 18) = {gcd_result}")

    print("🎉 Basic functionality works!")

except Exception as e:
    print(f"✗ Error: {e}")
    import traceback

    traceback.print_exc()
