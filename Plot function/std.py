
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# Load the data
#file_path = 'C:/Users/Ke/OneDrive/文档/GitHub/AMS-project-TRC-data-process/PAV2_data.csv'
file_path = 'C:/Users/Ke/Box/Project/AMS Piecewise linear model/AMS Data Set/01.05 data/Simdata-sta2.csv'
df = pd.read_csv(file_path)

# Conversion factor from km/h to mph
conversion_factor = 0.621371
avindex_value = 690
# Function to process and plot data for a given demand value
def plot_speed_comparison_for_demand(demand_value, df, conversion_factor):
    # Filter the dataframe for the specified demand
    demand_filtered_df = df[(df['avindex'] == avindex_value) & (df['Demand'] == demand_value)].drop_duplicates()
    

    # Convert speeds from km/h to ft/s
    demand_filtered_df['SPEEDAVG_AVG_AV_fts'] = demand_filtered_df['SPEEDSTD_AVG_AV'] * conversion_factor
    demand_filtered_df['SPEEDAVG_AVG_HV_fts'] = demand_filtered_df['SPEEDSTD_AVG_HV'] * conversion_factor
    demand_filtered_df['SPEEDAVG_AVG_All_fts'] = demand_filtered_df['SPEEDSTD_AVG_All'] * conversion_factor

    # Calculate error bars
    error_av = [
        demand_filtered_df['SPEEDAVG_AVG_AV_fts'] - demand_filtered_df['SPEEDSTD_MIN_AV'] * conversion_factor,
        demand_filtered_df['SPEEDSTD_MAX_AV'] * conversion_factor - demand_filtered_df['SPEEDAVG_AVG_AV_fts']
    ]
    error_hv = [
        demand_filtered_df['SPEEDAVG_AVG_HV_fts'] - demand_filtered_df['SPEEDSTD_MIN_HV'] * conversion_factor,
        demand_filtered_df['SPEEDSTD_MAX_HV'] * conversion_factor - demand_filtered_df['SPEEDAVG_AVG_HV_fts']
    ]
    error_all = [
        demand_filtered_df['SPEEDAVG_AVG_All_fts'] - demand_filtered_df['SPEEDSTD_MIN_All'] * conversion_factor,
        demand_filtered_df['SPEEDSTD_MAX_All'] * conversion_factor - demand_filtered_df['SPEEDAVG_AVG_All_fts']
    ]

    # Setting the positions and width for the bars
    positions = np.arange(len(demand_filtered_df['Av_Pen']))
    bar_width = 0.25

    # Plotting
    fig, ax = plt.subplots(figsize=(10, 6))

    # Plotting bars with error bars
    ax.bar(positions - bar_width, demand_filtered_df['SPEEDAVG_AVG_All_fts'], color='red', width=bar_width, edgecolor='black', yerr=error_all, capsize=5, label="All Vehicles' Average Speed", hatch='//')
    ax.bar(positions, demand_filtered_df['SPEEDAVG_AVG_HV_fts'], color='green', width=bar_width, edgecolor='black', yerr=error_hv, capsize=5, label="HV's Average Speed", hatch='\\\\')
    ax.bar(positions + bar_width, demand_filtered_df['SPEEDAVG_AVG_AV_fts'], color='blue', width=bar_width, edgecolor='black', yerr=error_av, capsize=5, label="ACC Vehicles' Average Speed", hatch='xx')

    # Labels and Title
    ax.set_xlabel('MPR of ACC Vehicles', fontweight='bold', fontsize=16)
    ax.set_ylabel('Speed STD(mph)', fontweight='bold', fontsize=16)
    ax.set_xticks(positions)
    ax.set_xticklabels([f"{pen*1:.0f}%" for pen in demand_filtered_df['Av_Pen']], fontsize=16)
    ax.set_ylim(0, 30)
    ax.tick_params(axis='y', labelsize=16)  # Increase font size of y-axis labels

    ax.legend(['All Vehicles'' Speed STD', 'HV''s Speed STD', 'ACC Vehicles Speed STD'], fontsize=16)
    #ax.set_title(f'Speed STD Comparison at Different MPR of ACC Vehicles in Demand {demand_value}', fontsize=16)
    
    # Show the plot
    plt.show()

# Call the function for Demand = 3000 and 4500
plot_speed_comparison_for_demand(3000, df, conversion_factor)
plot_speed_comparison_for_demand(4500, df, conversion_factor)












