import matplotlib.pyplot as plt
import numpy as np
import os

# Define paths for before and after optimization
before_opt_path = "./runtime_data"
after_opt_path = "./runtime_data_opt"
opt32_path = "./runtime_data_opt32"
opt64_path = "./runtime_data_opt64"

# Function to process CSV files and extract runtimes
def process_csv_files(files_path):
    runtimes_dict = {}
    files = os.listdir(files_path)
    for file in files:
        if file.endswith(".csv"):
            csv_path = os.path.join(files_path, file)
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

            # Iteratively remove outliers
            while True:
                active_values = runtimes[mask]
                avg = np.mean(active_values)
                std = np.std(active_values)

                # Check for new outliers
                deviations = np.abs(runtimes - avg)
                new_mask = deviations <= (nstd * std)

                if np.array_equal(mask, new_mask):
                    break  # No change => stop

                mask = new_mask

            # Store final runtimes without outliers
            runtimes_dict[os.path.basename(csv_path)] = {
                "runtimes": runtimes,
                "mask": mask,
                "final_avg": np.mean(runtimes[mask])
            }
    return runtimes_dict

# Process all datasets
before_opt_data = process_csv_files(before_opt_path)
after_opt_data = process_csv_files(after_opt_path)
opt32_data = process_csv_files(opt32_path)
opt64_data = process_csv_files(opt64_path)

# Plot comparison
for file_name in before_opt_data.keys():
    if file_name in after_opt_data and file_name in opt32_data and file_name in opt64_data:
        before_data = before_opt_data[file_name]
        after_data = after_opt_data[file_name]
        opt32 = opt32_data[file_name]
        opt64 = opt64_data[file_name]

        plt.figure(figsize=(12, 8))
        x_vals_before = np.arange(1, len(before_data["runtimes"]) + 1)
        x_vals_after = np.arange(1, len(after_data["runtimes"]) + 1)
        x_vals_opt32 = np.arange(1, len(opt32["runtimes"]) + 1)
        x_vals_opt64 = np.arange(1, len(opt64["runtimes"]) + 1)

        # Plot before optimization
        plt.scatter(
            x_vals_before[before_data["mask"]],
            before_data["runtimes"][before_data["mask"]],
            color='blue',
            label='Before Optimization (Active Elements)'
        )
        plt.scatter(
            x_vals_before[~before_data["mask"]],
            before_data["runtimes"][~before_data["mask"]],
            color='orange',
            label='Before Optimization (Outliers)'
        )
        plt.axhline(
            y=before_data["final_avg"],
            color='green',
            linestyle='--',
            label=f'Before Opt Avg = {int(before_data["final_avg"])} ns'
        )

        # Plot after optimization
        plt.scatter(
            x_vals_after[after_data["mask"]],
            after_data["runtimes"][after_data["mask"]],
            color='purple',
            label='After Optimization (Active Elements)'
        )
        plt.scatter(
            x_vals_after[~after_data["mask"]],
            after_data["runtimes"][~after_data["mask"]],
            color='red',
            label='After Optimization (Outliers)'
        )
        plt.axhline(
            y=after_data["final_avg"],
            color='brown',
            linestyle='--',
            label=f'After Opt Avg = {int(after_data["final_avg"])} ns'
        )

        # Plot block size 32
        plt.scatter(
            x_vals_opt32[opt32["mask"]],
            opt32["runtimes"][opt32["mask"]],
            color='cyan',
            label='Block Size 32 (Active Elements)'
        )
        plt.scatter(
            x_vals_opt32[~opt32["mask"]],
            opt32["runtimes"][~opt32["mask"]],
            color='magenta',
            label='Block Size 32 (Outliers)'
        )
        plt.axhline(
            y=opt32["final_avg"],
            color='darkcyan',
            linestyle='--',
            label=f'Block Size 32 Avg = {int(opt32["final_avg"])} ns'
        )

        # Plot block size 64
        plt.scatter(
            x_vals_opt64[opt64["mask"]],
            opt64["runtimes"][opt64["mask"]],
            color='yellow',
            label='Block Size 64 (Active Elements)'
        )
        plt.scatter(
            x_vals_opt64[~opt64["mask"]],
            opt64["runtimes"][~opt64["mask"]],
            color='brown',
            label='Block Size 64 (Outliers)'
        )
        plt.axhline(
            y=opt64["final_avg"],
            color='gold',
            linestyle='--',
            label=f'Block Size 64 Avg = {int(opt64["final_avg"])} ns'
        )

        # Labels and title
        plt.title(f"Runtime Comparison for {file_name}")
        plt.xlabel("Execution Index")
        plt.ylabel("Runtime (ns)")
        plt.grid(True, linestyle='--', alpha=0.5)
        plt.legend()
        plt.tight_layout()

        # Save the plot
        plt.savefig(os.path.join(after_opt_path, file_name[0:-4] + ".png"))
        plt.close()