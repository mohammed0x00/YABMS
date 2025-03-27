import matplotlib.pyplot as plt
import numpy as np
import os

# Load the CSV file
files_path = "./runtime_data"

files = os.listdir(files_path)

for file in files:
    if file.endswith(".csv"):
        csv_path = files_path + "/" + file
        data = {}
        with open(csv_path, "r") as file:
            lines = file.readlines()
            for line in lines:
                key, *values = line.strip().split(',')
                data[key] = values

        # Extract runtimes and convert to numpy array
        runtimes = np.array(data['runtimes'], dtype=float)

        # Initially, all entries are active
        mask = np.array([True] * len(runtimes))
        nstd = 3
        pass_num = 0

        # Iteratively remove outliers
        while True:
            pass_num += 1
            active_values = runtimes[mask]
            avg = np.mean(active_values)
            std = np.std(active_values)
            
            # Check for new outliers
            deviations = np.abs(runtimes - avg)
            new_mask = deviations <= (nstd * std)
            
            if np.array_equal(mask, new_mask):
                break  # No change => stopfinal_avg
            
            mask = new_mask

        # Final stats
        active_indices = np.where(mask)[0]
        outlier_indices = np.where(~mask)[0]
        final_avg = np.mean(runtimes[mask])

        # Plotting
        plt.figure(figsize=(10, 7))
        x_vals = np.arange(1, len(runtimes) + 1)

        # Plot active values
        plt.scatter(x_vals[mask], runtimes[mask], color='blue', label='Active Elements')
        # Plot outliers
        plt.scatter(x_vals[~mask], runtimes[~mask], color='orange', label='Outliers')
        # Plot average line
        plt.axhline(y=final_avg, color='green', linestyle='--', label=f'Outlier-Free Average = {int(final_avg)} ns')

        # Labels and title
        plt.title("Small Dataset Runtimes with Outlier Filtering")
        plt.xlabel("Execution Index")
        plt.ylabel("Runtime (ns)")
        plt.xticks(x_vals)
        plt.grid(True, linestyle='--', alpha=0.5)
        plt.legend()
        plt.tight_layout()
        plt.savefig(file.name[0:-4] + ".png")

