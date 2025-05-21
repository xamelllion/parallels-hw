import argparse
import subprocess
import time
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from tqdm import tqdm
import os

def run_benchmark(args):
    modes = ['seq', 'pixel', 'row', 'column', 'grid']
    filters = ['id', 'bl', 'mb', 'ed', 'sr']
    results = []
    
    total_combinations = len(modes) * len(filters) * args['runs']
    progress = tqdm(total=total_combinations, desc="Benchmarking")

    os.makedirs('/tmp/pct/', exist_ok=True)

    for mode in modes:
        for fltr in filters:
            times = []
            for _ in range(args['runs']):
                output_file = f"/tmp/pct/pct_out_{mode}_{fltr}_{time.time()}.jpg"
                cmd = [
                    './build/src/pct',
                    '-i', args['input'],
                    '-o', output_file,
                    '-f', fltr,
                    '-m', mode,
                    '-t', str(args['threads']) if mode != 'seq' else '1',
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
                avg_time = sum(times) / len(times)
                results.append({
                    'Mode': mode.upper(),
                    'Filter': fltr.upper(),
                    'Time (s)': avg_time
                })

    progress.close()
    return pd.DataFrame(results)

def plot_results(df, output_dir):
    sns.set_style("whitegrid")
    os.makedirs(output_dir, exist_ok=True)

    filters = df['Filter'].unique()
    for fltr in filters:
        plt.figure(figsize=(10, 6))
        ax = sns.barplot(
            data=df[df['Filter'] == fltr],
            x='Mode',
            y='Time (s)',
            palette="rocket",
            errorbar=None,
            hue='Mode',
            legend=False
        )

        plt.title(f"Performance for {fltr} Filter", pad=20)
        plt.xlabel("Processing Mode")
        plt.ylabel("Execution Time (s)")
        
        for p in ax.patches:
            ax.annotate(
                f"{p.get_height():.2f}",
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
    args = {
        'input': 'examples/image.png',
        'output_dir': 'benchmark/plots',
        'threads': 4,
        'runs': 1,
    }
    
    print(f"Starting benchmark with configuration:")
    print(f"  Input image: {args['input']}")
    print(f"  Threads: {args['threads']}")
    print(f"  Runs per config: {args['runs']}")
    print(f"  Output directory: {args['output_dir']}\n")
    
    results_df = run_benchmark(args)
    
    if not results_df.empty:
        plot_results(results_df, args['output_dir'])
        print("\nFinal Results:")
        print(results_df.to_markdown(index=False))
    else:
        print("\nNo valid results collected.")
