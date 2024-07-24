

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# Load the data
#file_path = 'C:/Users/Ke/OneDrive/文档/GitHub/AMS-project-TRC-data-process/PAV23_data2.csv'
#file_path = 'C:/Users/Ke/Box/Project/AMS Piecewise linear model/AMS Data Set/Simulation data/simulation_data.csv'
file_path = 'C:/Users/Ke/Box/Project/AMS Piecewise linear model/AMS Data Set/01.02 data/Simdata_new.csv'
df = pd.read_csv(file_path)

# Conversion factor from km/h to mph
conversion_factor = 0.621371 
avindex_value = 660
    
    

# Function to process and plot data for a given demand value
def plot_speed_comparison_for_demand(demand_value, df, conversion_factor,avindex_value):
    # Filter the dataframe for the specified demand
    demand_filtered_df = df[(df['avindex'] == avindex_value) & (df['Demand'] == demand_value)]

    # Convert speeds from km/h to ft/s
    demand_filtered_df['SPEEDSTD_AVG_AV_fts'] = demand_filtered_df['SPEEDSTD_AVG_AV'] * conversion_factor
    demand_filtered_df['SPEEDSTD_AVG_HV_fts'] = demand_filtered_df['SPEEDSTD_AVG_HV'] * conversion_factor
    demand_filtered_df['SPEEDSTD_AVG_All_fts'] = demand_filtered_df['SPEEDSTD_AVG_All'] * conversion_factor

    # Calculate error bars
    error_av = [
        demand_filtered_df['SPEEDSTD_AVG_AV_fts'] - demand_filtered_df['SPEEDSTD_MIN_AV'] * conversion_factor,
        demand_filtered_df['SPEEDSTD_MAX_AV'] * conversion_factor - demand_filtered_df['SPEEDSTD_AVG_AV_fts']
    ]
    error_hv = [
        demand_filtered_df['SPEEDSTD_AVG_HV_fts'] - demand_filtered_df['SPEEDSTD_MIN_HV'] * conversion_factor,
        demand_filtered_df['SPEEDSTD_MAX_HV'] * conversion_factor - demand_filtered_df['SPEEDSTD_AVG_HV_fts']
    ]
    error_all = [
        demand_filtered_df['SPEEDSTD_AVG_All_fts'] - demand_filtered_df['SPEEDSTD_MIN_All'] * conversion_factor,
        demand_filtered_df['SPEEDSTD_MAX_All'] * conversion_factor - demand_filtered_df['SPEEDSTD_AVG_All_fts']
    ]

    # Setting the positions and width for the bars
    positions = np.arange(len(demand_filtered_df['Av_Pen']))
    bar_width = 0.25
    filtered_df = df[df['avindex'] == avindex_value]

    # Plotting
    percentage_labels = [f'{i}%' for i in range(0, 101, 10)]

    fig, ax = plt.subplots(figsize=(10, 6))

    # Plotting bars with error bars
    ax.bar(positions - bar_width, demand_filtered_df['SPEEDSTD_AVG_All_fts'], color='red', width=bar_width, edgecolor='black', yerr=error_all, capsize=5, label="All Vehicles' Average Speed STD", hatch='//')
    ax.bar(positions, demand_filtered_df['SPEEDSTD_AVG_HV_fts'], color='green', width=bar_width, edgecolor='black', yerr=error_hv, capsize=5, label="HV's Average Speed STD", hatch='\\\\')
    ax.bar(positions + bar_width, demand_filtered_df['SPEEDSTD_AVG_AV_fts'], color='blue', width=bar_width, edgecolor='black', yerr=error_av, capsize=5, label="ACC Vehicles' Average Speed STD", hatch='xx')
    ax.set_xticks(positions)
    ax.set_xticklabels(percentage_labels, fontsize=16)
    
    # Labels and Title
    ax.set_xlabel('MPR of ACC Vehicles', fontweight='bold', fontsize=16)
    ax.set_ylabel('Speed STD(mph)', fontweight='bold', fontsize=16)
    ax.set_xticks(positions)
    ax.set_xticks(np.linspace(0, len(demand_filtered_df['Av_Pen']) - 1, len(percentage_labels)))
    ax.set_xticklabels(percentage_labels, fontsize=16)
    # ax.set_title(f'Speed Comparison by Vehicle Type at Different MPR of ACC Vehicles for Demand {demand_value}', fontsize=16)
    ax.legend(fontsize=16)
    # Show the plot
    plt.show()

# Call the function for Demand = 3000 and 4500
plot_speed_comparison_for_demand(3000, df, conversion_factor,avindex_value)
plot_speed_comparison_for_demand(4500, df, conversion_factor,avindex_value)











