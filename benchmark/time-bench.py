import argparse
import subprocess
import time
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from tqdm import tqdm
import os
import numpy as np

ARGS = {
    'input': 'examples/image.png',
    'output_dir': 'benchmark/plots',
    'threads': 8,
    'runs': 5,
}

MODES = ['seq', 'pixel', 'row', 'column', 'grid']
FILTERS = ['id', 'bl', 'mb', 'ed', 'sr']

def run_benchmark():
    results = []
    
    total_combinations = len(MODES) * len(FILTERS) * ARGS['runs']
    progress = tqdm(total=total_combinations, desc="Benchmarking")

    for mode in MODES:
        for fltr in FILTERS:
            times = []
            for _ in range(ARGS['runs']):
                cmd = [
                    './build/src/pct',
                    '-i', ARGS['input'],
                    '-o', "/dev/null",
                    '-f', fltr,
                    '-m', mode,
                    '-t', str(ARGS['threads']) if mode != 'seq' else '1',
                    '--log'
                ]

                try:
                    result = subprocess.run(cmd, check=True, 
                                  capture_output=True, text=True)
                    conv_time = float(result.stdout.split(':')[1])
                    times.append(conv_time)
                except subprocess.CalledProcessError as e:
                    print(f"\nError running: {' '.join(cmd)}")
                    print(f"Error message: {str(e)}")
                    continue
                finally:
                    progress.update(1)

            if times:
                mean_time = np.mean(times)
                std_error = np.std(times, ddof=1)
                results.append({
                    'Mode': mode.upper(),
                    'Filter': fltr.upper(),
                    'Time (s)': f"{mean_time:.4f} ± {std_error:.4f}",
                    '_mean': mean_time,
                    '_error': std_error
                })

    progress.close()
    return pd.DataFrame(results)

def plot_results(df, output_dir):
    sns.set_style("whitegrid")
    os.makedirs(output_dir, exist_ok=True)

    FILTERS = df['Filter'].unique()
    for fltr in FILTERS:
        plt.figure(figsize=(10, 6))
        ax = sns.barplot(
            data=df[df['Filter'] == fltr],
            x='Mode',
            y='_mean',
            palette="rocket",
            errorbar=('ci', 68),
            hue='Mode',
            legend=False
        )

        plt.title(f"Performance for {fltr} Filter", pad=20)
        plt.xlabel("Processing Mode")
        plt.ylabel("Execution Time (s)")
        
        for i, p in enumerate(ax.patches):
            row = df[(df['Filter'] == fltr)].iloc[i]
            ax.annotate(
                f"{row['Time (s)']}",
                (p.get_x() + p.get_width() / 2., p.get_height()),
                ha='center', va='center',
                xytext=(0, 5),
                textcoords='offset points'
            )
        
        output_path = os.path.join(output_dir, f"{fltr.lower()}_filter.png")
        plt.tight_layout()
        plt.savefig(output_path)
        plt.close()
        print(f"Saved {fltr} plot to {output_path}")

if __name__ == "__main__":    
    print(f"Starting benchmark with configuration:")
    print(f"  Input image: {ARGS['input']}")
    print(f"  Threads: {ARGS['threads']}")
    print(f"  Runs per config: {ARGS['runs']}")
    print(f"  Output directory: {ARGS['output_dir']}\n")
    
    results_df = run_benchmark()
    
    if not results_df.empty:
        plot_results(results_df, ARGS['output_dir'])
        print("\nFinal Results:")
        print(results_df[['Mode', 'Filter', 'Time (s)']].to_markdown(index=False))

        with open('benchmark/result.txt', 'w')  as f:
            f.write('Final Results:\n')
            f.write(f'{results_df[['Mode', 'Filter', 'Time (s)']].to_markdown(index=False)}\n')
    else:
        print("\nNo valid results collected.")
