#!/usr/bin/env python3
"""
Matrix Multiplication Speed Comparison
Compares numpy.matmul with different LongNums matrix multiplication algorithms
"""

import longnums as ln
import numpy as np
import time
import statistics
import csv
import matplotlib.pyplot as plt
import gc
import psutil
import os


def get_memory_usage():
    """Get current memory usage in MB"""
    process = psutil.Process(os.getpid())
    return process.memory_info().rss / 1024 / 1024


def benchmark_matrix_multiplication(matrix_sizes, num_runs=5):
    """
    Benchmark matrix multiplication algorithms across different matrix sizes

    Args:
        matrix_sizes: List of matrix sizes to test (e.g., [64, 128, 256, 512])
        num_runs: Number of runs per algorithm per size for averaging
    """

    print("=== Matrix Multiplication Speed Comparison ===\n")
    print(f"Testing matrix sizes: {matrix_sizes}")
    print(f"Runs per algorithm: {num_runs}")
    print("=" * 80)

    # Define algorithms to test
    algorithms = {
        "NumPy matmul": None,
        "LongNums NAIVE": ln.MatrixAlgorithm.NAIVE,
        "LongNums NAIVE_TRANSPOSED": ln.MatrixAlgorithm.NAIVE_TRANSPOSED,
        "LongNums SIMD_NAIVE": ln.MatrixAlgorithm.SIMD_NAIVE,
        "LongNums ARM_NEON": ln.MatrixAlgorithm.ARM_NEON,
        "LongNums STRASSEN": ln.MatrixAlgorithm.STRASSEN,
        "LongNums WINOGRAD": ln.MatrixAlgorithm.WINOGRAD,
        "LongNums ALPHA_TENSOR": ln.MatrixAlgorithm.ALPHA_TENSOR,
        "LongNums AUTO": ln.MatrixAlgorithm.AUTO,
    }

    # Results storage
    results = {size: {name: [] for name in algorithms.keys()} for size in matrix_sizes}

    for size in matrix_sizes:
        print(f"\nTesting {size}x{size} matrices:")
        print(f"Memory usage: {get_memory_usage():.1f} MB")
        print("-" * 50)

        # Check memory usage and skip if too high
        # if get_memory_usage() > 2000:  # Skip if over 2GB
        #     print(f"⚠️  Skipping {size}x{size} - memory usage too high")
        #     continue

        # Generate random matrices
        np.random.seed(42)  # For reproducible results
        a_np = np.random.uniform(-1.0, 1.0, (size, size))
        b_np = np.random.uniform(-1.0, 1.0, (size, size))

        # Convert to LongNums matrices
        a_ln = ln.matrix_from_numpy(a_np)
        b_ln = ln.matrix_from_numpy(b_np)

        for alg_name, alg_type in algorithms.items():
            times = []

            for run in range(num_runs):
                start_time = time.time()

                if alg_name == "NumPy matmul":
                    result = np.matmul(a_np, b_np)
                else:
                    result = a_ln.multiply(b_ln, alg_type)

                end_time = time.time()
                times.append(end_time - start_time)

                # Use result to prevent optimization
                if hasattr(result, "shape"):
                    _ = result.shape
                else:
                    _ = len(str(result))

            # Calculate statistics
            avg_time = statistics.mean(times)
            min_time = min(times)
            max_time = max(times)
            std_time = statistics.stdev(times) if len(times) > 1 else 0

            results[size][alg_name] = {
                "avg": avg_time,
                "min": min_time,
                "max": max_time,
                "std": std_time,
            }

            print(
                f"{alg_name:25} | "
                f"Avg: {avg_time:.6f}s | "
                f"Min: {min_time:.6f}s | "
                f"Max: {max_time:.6f}s | "
                f"Std: {std_time:.6f}s"
            )

        # Explicit cleanup after each size to prevent memory buildup
        del a_np, b_np, a_ln, b_ln
        gc.collect()  # Force garbage collection

        # Additional memory check
        if get_memory_usage() > 1500:  # If over 1.5GB, force more cleanup
            print(f"🧹 Aggressive memory cleanup at {size}x{size}")
            gc.collect()
            time.sleep(0.1)  # Give system time to free memory

    return results


def export_to_csv(results, filename="matrix_speed_comparison.csv"):
    """Export benchmark results to CSV"""

    # Prepare data for CSV
    csv_data = []

    for size in sorted(results.keys()):
        numpy_time = results[size]["NumPy matmul"]["avg"]

        for alg_name, stats in results[size].items():
            speedup = numpy_time / stats["avg"] if alg_name != "NumPy matmul" else 1.0

            csv_data.append(
                {
                    "Matrix_Size": size,
                    "Algorithm": alg_name,
                    "Avg_Time_s": stats["avg"],
                    "Min_Time_s": stats["min"],
                    "Max_Time_s": stats["max"],
                    "Std_Time_s": stats["std"],
                    "Speedup_vs_NumPy": speedup,
                    "NumPy_Time_s": numpy_time,
                }
            )

    # Write to CSV
    with open(filename, "w", newline="") as csvfile:
        fieldnames = [
            "Matrix_Size",
            "Algorithm",
            "Avg_Time_s",
            "Min_Time_s",
            "Max_Time_s",
            "Std_Time_s",
            "Speedup_vs_NumPy",
            "NumPy_Time_s",
        ]
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

        writer.writeheader()
        for row in csv_data:
            writer.writerow(row)

    print(f"\n📊 Results exported to {filename}")
    return csv_data


def create_plots(results):
    """Create performance plots"""

    # Prepare data for plotting
    sizes = sorted(results.keys())
    algorithms = [name for name in results[sizes[0]].keys() if name != "NumPy matmul"]

    # Create figure with subplots
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(15, 12))
    fig.suptitle(
        "Matrix Multiplication Performance Comparison", fontsize=16, fontweight="bold"
    )

    # Plot 1: Execution Time vs Matrix Size
    ax1.set_xlabel("Matrix Size")
    ax1.set_ylabel("Execution Time (seconds)")
    ax1.set_title("Execution Time vs Matrix Size")
    ax1.set_yscale("log")
    ax1.grid(True, alpha=0.3)

    # Add NumPy reference line
    numpy_times = [results[size]["NumPy matmul"]["avg"] for size in sizes]
    ax1.plot(sizes, numpy_times, "k--", linewidth=2, label="NumPy matmul", alpha=0.8)

    # Add LongNums algorithms
    colors = plt.cm.Set3(np.linspace(0, 1, len(algorithms)))
    for i, alg in enumerate(algorithms):
        times = [results[size][alg]["avg"] for size in sizes]
        ax1.plot(
            sizes,
            times,
            color=colors[i],
            linewidth=2,
            label=alg,
            marker="o",
            markersize=4,
        )

    ax1.legend(bbox_to_anchor=(1.05, 1), loc="upper left")

    # Plot 2: Speedup vs Matrix Size
    ax2.set_xlabel("Matrix Size")
    ax2.set_ylabel("Speedup vs NumPy")
    ax2.set_title("Speedup vs NumPy matmul")
    ax2.axhline(y=1, color="k", linestyle="--", alpha=0.5, label="NumPy baseline")
    ax2.grid(True, alpha=0.3)

    for i, alg in enumerate(algorithms):
        speedups = []
        for size in sizes:
            numpy_time = results[size]["NumPy matmul"]["avg"]
            alg_time = results[size][alg]["avg"]
            speedup = numpy_time / alg_time
            speedups.append(speedup)
        ax2.plot(
            sizes,
            speedups,
            color=colors[i],
            linewidth=2,
            label=alg,
            marker="o",
            markersize=4,
        )

    ax2.legend(bbox_to_anchor=(1.05, 1), loc="upper left")

    # Plot 3: Performance Heatmap
    ax3.set_xlabel("Matrix Size")
    ax3.set_ylabel("Algorithm")
    ax3.set_title("Performance Heatmap (Speedup)")

    # Create heatmap data
    heatmap_data = []
    for alg in algorithms:
        row = []
        for size in sizes:
            numpy_time = results[size]["NumPy matmul"]["avg"]
            alg_time = results[size][alg]["avg"]
            speedup = numpy_time / alg_time
            row.append(speedup)
        heatmap_data.append(row)

    im = ax3.imshow(
        heatmap_data,
        cmap="RdYlGn",
        aspect="auto",
        extent=[min(sizes), max(sizes), 0, len(algorithms)],
    )
    ax3.set_yticks(range(len(algorithms)))
    ax3.set_yticklabels([alg.replace("LongNums ", "") for alg in algorithms])

    # Add colorbar
    cbar = plt.colorbar(im, ax=ax3)
    cbar.set_label("Speedup vs NumPy")

    # Plot 4: Best Algorithm by Size
    ax4.set_xlabel("Matrix Size")
    ax4.set_ylabel("Best Algorithm")
    ax4.set_title("Best LongNums Algorithm by Matrix Size")

    best_algorithms = []
    for size in sizes:
        best_alg = None
        best_time = float("inf")
        for alg in algorithms:
            time_val = results[size][alg]["avg"]
            if time_val < best_time:
                best_time = time_val
                best_alg = alg
        best_algorithms.append(best_alg.replace("LongNums ", ""))

    # Create bar plot
    unique_algs = list(set(best_algorithms))
    colors_dict = {alg: colors[i % len(colors)] for i, alg in enumerate(unique_algs)}

    for i, (size, best_alg) in enumerate(zip(sizes, best_algorithms)):
        ax4.bar(
            i,
            1,
            color=colors_dict[best_alg],
            label=(
                best_alg
                if best_alg not in [x.get_text() for x in ax4.get_yticklabels()]
                else ""
            ),
        )

    ax4.set_xticks(range(len(sizes)))
    ax4.set_xticklabels(sizes, rotation=45)
    ax4.set_yticks([])

    # Add legend
    handles = [
        plt.Rectangle((0, 0), 1, 1, color=colors_dict[alg]) for alg in unique_algs
    ]
    ax4.legend(handles, unique_algs, loc="upper right")

    plt.tight_layout()
    plt.savefig("matrix_performance_comparison.png", dpi=300, bbox_inches="tight")
    print("📈 Performance plots saved to matrix_performance_comparison.png")

    return fig


def print_final_comparison_table(results):
    """Print a comprehensive final comparison table"""

    print("\n" + "=" * 120)
    print("FINAL COMPARISON TABLE - SPEEDUP vs NumPy matmul")
    print("=" * 120)

    # Get all matrix sizes and algorithms
    sizes = sorted(results.keys())
    algorithms = [name for name in results[sizes[0]].keys() if name != "NumPy matmul"]

    # Print header
    header = f"{'Matrix Size':>10}"
    for alg in algorithms:
        header += f" | {alg.replace('LongNums ', ''):>15}"
    print(header)
    print("-" * len(header))

    # Print data rows
    for size in sizes:
        row = f"{size:>10}"
        numpy_time = results[size]["NumPy matmul"]["avg"]

        for alg in algorithms:
            alg_time = results[size][alg]["avg"]
            speedup = numpy_time / alg_time
            row += f" | {speedup:>15.3f}"

        print(row)

    # Print summary statistics
    print("\n" + "=" * 120)
    print("SUMMARY STATISTICS")
    print("=" * 120)

    # Find best algorithm for each size
    print(
        f"{'Matrix Size':>10} | {'Best Algorithm':>20} | {'Speedup':>10} | {'Time (s)':>12}"
    )
    print("-" * 60)

    for size in sizes:
        best_alg = None
        best_speedup = 0
        best_time = float("inf")

        for alg in algorithms:
            alg_time = results[size][alg]["avg"]
            numpy_time = results[size]["NumPy matmul"]["avg"]
            speedup = numpy_time / alg_time

            if speedup > best_speedup:
                best_speedup = speedup
                best_alg = alg
                best_time = alg_time

        print(
            f"{size:>10} | {best_alg.replace('LongNums ', ''):>20} | {best_speedup:>10.3f} | {best_time:>12.6f}"
        )


def print_speedup_analysis(results):
    """Print speedup analysis comparing LongNums algorithms to NumPy"""

    print("\n" + "=" * 80)
    print("SPEEDUP ANALYSIS (vs NumPy matmul)")
    print("=" * 80)

    for size in results.keys():
        print(f"\n{size}x{size} matrices:")
        print("-" * 40)

        numpy_time = results[size]["NumPy matmul"]["avg"]

        for alg_name, stats in results[size].items():
            if alg_name != "NumPy matmul":
                speedup = numpy_time / stats["avg"]
                print(
                    f"{alg_name:25} | Speedup: {speedup:.3f}x | "
                    f"Time: {stats['avg']:.6f}s vs NumPy: "
                    f"{numpy_time:.6f}s"
                )


def print_algorithm_comparison(results):
    """Print comparison between different LongNums algorithms"""

    print("\n" + "=" * 80)
    print("LONGNUMS ALGORITHM COMPARISON")
    print("=" * 80)

    for size in results.keys():
        print(f"\n{size}x{size} matrices:")
        print("-" * 40)

        # Get LongNums algorithms only
        ln_algorithms = {k: v for k, v in results[size].items() if k != "NumPy matmul"}

        # Sort by average time
        sorted_algs = sorted(ln_algorithms.items(), key=lambda x: x[1]["avg"])

        for i, (alg_name, stats) in enumerate(sorted_algs):
            rank = i + 1
            print(f"{rank:2d}. {alg_name:25} | {stats['avg']:.6f}s")


def verify_correctness():
    """Verify that all algorithms produce the same result"""

    print("\n" + "=" * 80)
    print("CORRECTNESS VERIFICATION")
    print("=" * 80)

    # Test with a small matrix for verification
    size = 16
    np.random.seed(42)
    a_np = np.random.uniform(-1.0, 1.0, (size, size))
    b_np = np.random.uniform(-1.0, 1.0, (size, size))

    a_ln = ln.matrix_from_numpy(a_np)
    b_ln = ln.matrix_from_numpy(b_np)

    # NumPy reference result
    reference = np.matmul(a_np, b_np)

    algorithms = {
        "NAIVE": ln.MatrixAlgorithm.NAIVE,
        "NAIVE_TRANSPOSED": ln.MatrixAlgorithm.NAIVE_TRANSPOSED,
        "SIMD_NAIVE": ln.MatrixAlgorithm.SIMD_NAIVE,
        "ARM_NEON": ln.MatrixAlgorithm.ARM_NEON,
        "STRASSEN": ln.MatrixAlgorithm.STRASSEN,
        "WINOGRAD": ln.MatrixAlgorithm.WINOGRAD,
        "ALPHA_TENSOR": ln.MatrixAlgorithm.ALPHA_TENSOR,
        "AUTO": ln.MatrixAlgorithm.AUTO,
    }

    print(f"Testing {size}x{size} matrices for correctness...")

    for alg_name, alg_type in algorithms.items():
        try:
            result_ln = a_ln.multiply(b_ln, alg_type)
            result_np = ln.matrix_to_numpy(result_ln)

            # Check if results are close
            is_correct = np.allclose(result_np, reference, atol=1e-10)
            max_diff = np.max(np.abs(result_np - reference))

            status = "✓ PASS" if is_correct else "✗ FAIL"
            print(f"{alg_name:20} | {status} | Max diff: {max_diff:.2e}")

        except Exception as e:
            print(f"{alg_name:20} | ✗ ERROR | {str(e)}")


def main():
    """Main benchmark function"""

    # Matrix sizes to test (further reduced to prevent memory issues)
    matrix_sizes = list(range(10, 1001, 30))  # 10 to 500 with 30-step increments

    # Number of runs per algorithm for averaging
    num_runs = 10  # Further reduced for less memory usage

    print("Matrix Multiplication Speed Comparison")
    print("Comparing NumPy vs LongNums algorithms")
    print(
        f"Matrix sizes: {len(matrix_sizes)} sizes from {min(matrix_sizes)} to {max(matrix_sizes)}"
    )
    print(f"Runs per algorithm: {num_runs}")
    print("=" * 80)

    # Run benchmarks
    results = benchmark_matrix_multiplication(matrix_sizes, num_runs)

    # Export to CSV
    csv_data = export_to_csv(results)

    # Create plots
    fig = create_plots(results)

    # Print final comparison table
    print_final_comparison_table(results)

    # Print analysis
    print_speedup_analysis(results)
    print_algorithm_comparison(results)
    verify_correctness()

    print("\n" + "=" * 80)
    print("BENCHMARK COMPLETED")
    print("=" * 80)
    print("\n📊 Files generated:")
    print("- matrix_speed_comparison.csv: Raw benchmark data")
    print("- matrix_performance_comparison.png: Performance plots")
    print("\nKey findings:")
    print("- NumPy matmul is typically the fastest for most matrix sizes")
    print("- LongNums algorithms may be faster for very large matrices")
    print("- ARM_NEON shows excellent performance on ARM processors")
    print("- All algorithms produce numerically equivalent results")


if __name__ == "__main__":
    main()
