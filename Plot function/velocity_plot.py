
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns
# Load the data

    

# Function to process and plot data for a given demand value
def plot_speed_comparison_for_demand(avindex_value, demand_value, df, conversion_factor):
    # Filter the dataframe for the specified demand
    demand_filtered_df = df[(df['avindex'] == avindex_value) & (df['Demand'] == demand_value)]

    # Convert speeds from km/h to ft/s
    demand_filtered_df['SPEEDAVG_AVG_AV_fts'] = demand_filtered_df['SPEEDAVG_AVG_AV'] * conversion_factor
    demand_filtered_df['SPEEDAVG_AVG_HV_fts'] = demand_filtered_df['SPEEDAVG_AVG_HV'] * conversion_factor
    demand_filtered_df['SPEEDAVG_AVG_All_fts'] = demand_filtered_df['SPEEDAVG_AVG_All'] * conversion_factor

    # Calculate error bars
    error_av = [
        demand_filtered_df['SPEEDAVG_AVG_AV_fts'] - demand_filtered_df['SPEEDAVG_MIN_AV'] * conversion_factor,
        demand_filtered_df['SPEEDAVG_MAX_AV'] * conversion_factor - demand_filtered_df['SPEEDAVG_AVG_AV_fts']
    ]
    error_hv = [
        demand_filtered_df['SPEEDAVG_AVG_HV_fts'] - demand_filtered_df['SPEEDAVG_MIN_HV'] * conversion_factor,
        demand_filtered_df['SPEEDAVG_MAX_HV'] * conversion_factor - demand_filtered_df['SPEEDAVG_AVG_HV_fts']
    ]
    error_all = [
        demand_filtered_df['SPEEDAVG_AVG_All_fts'] - demand_filtered_df['SPEEDAVG_MIN_All'] * conversion_factor,
        demand_filtered_df['SPEEDAVG_MAX_All'] * conversion_factor - demand_filtered_df['SPEEDAVG_AVG_All_fts']
    ]

    # Setting the positions and width for the bars
    positions = np.arange(len(demand_filtered_df['Av_Pen']))
    bar_width = 0.25
    filtered_df = df[df['avindex'] == avindex_value]

    # Plotting
    percentage_labels = [f'{i}%' for i in range(0, 101, 25)]

    fig, ax = plt.subplots(figsize=(10, 6))

    # Plotting bars with error bars
    ax.bar(positions - bar_width, demand_filtered_df['SPEEDAVG_AVG_All_fts'], color='red', width=bar_width, edgecolor='black', yerr=error_all, capsize=5, label="All Vehicles' Average Speed", hatch='//')
    ax.bar(positions, demand_filtered_df['SPEEDAVG_AVG_HV_fts'], color='green', width=bar_width, edgecolor='black', yerr=error_hv, capsize=5, label="HV's Average Speed", hatch='\\\\')
    ax.bar(positions + bar_width, demand_filtered_df['SPEEDAVG_AVG_AV_fts'], color='blue', width=bar_width, edgecolor='black', yerr=error_av, capsize=5, label="ACC Vehicles' Average Speed", hatch='xx')
    ax.set_xticks(positions)
    ax.set_xticklabels(percentage_labels, fontsize=16)
    
    # Labels and Title
    ax.set_xlabel('MPR of ACC Vehicles', fontweight='bold', fontsize=16)
    ax.set_ylabel('Speed (mph)', fontweight='bold', fontsize=16)
    ax.set_ylim(0, 65)
    ax.tick_params(axis='y', labelsize=16)  # Increase font size of y-axis labels

    ax.set_xticks(positions)
    ax.set_xticks(np.linspace(0, len(demand_filtered_df['Av_Pen']) - 1, len(percentage_labels)))
    ax.set_xticklabels(percentage_labels, fontsize=16)
    # ax.set_title(f'Speed Comparison by Vehicle Type at Different MPR of ACC Vehicles for Demand {demand_value}', fontsize=16)
    ax.legend(fontsize=16)
    # Show the plot
    plt.show()
    
def fd_function1(data, avindex, av_pen):
    # Filter the data based on the given conditions
    filtered_data = data[(data['avindex'] == avindex) & (data['Av_Pen'] == av_pen)]

    # Modify Density1_AVG unit (times 1000)
    filtered_data['Density1_AVG'] = filtered_data['Density1_AVG'] * 1609/3
    filtered_data['Volume1_AVG'] = filtered_data['Volume1_AVG']/3
    #filtered_data = filtered_data.iloc[0:8]
    # Plotting with updated font sizes and density unit
    plt.figure(figsize=(12, 6))
    sns.scatterplot(x='Density1_AVG', y='Volume1_AVG', data=filtered_data, s=150, color='blue') 
    plt.title(f'Fundamental Diagram on the first point (PAV{int((avindex-650)/10+1)}, MPR={av_pen}%)', fontsize=16)
    plt.xlabel('Density (Vehicles per mile)', fontsize=16)
    plt.ylabel('Volume (vehicles per hour)', fontsize=16)
    plt.xticks(fontsize=16)
    plt.yticks(fontsize=16)
    #plt.ylim(200, 1700)
    plt.tick_params(axis='both', labelsize=16)
    plt.grid(True)
    #plt.ylim(1000, 6000)
    plt.show()
    
def fd_function2(data, avindex, av_pen):
    # Filter the data based on the given conditions
    filtered_data = data[(data['avindex'] == avindex) & (data['Av_Pen'] == av_pen)]
    #filtered_data = filtered_data.iloc[0:8]
    # Modify Density1_AVG unit (times 1000)
    filtered_data['Density2_AVG'] = filtered_data['Density2_AVG'] * 1609/3
    filtered_data['Volume2_AVG'] = filtered_data['Volume2_AVG']/3
    # Plotting with updated font sizes and density unit
    plt.figure(figsize=(10, 6))
    sns.scatterplot(x='Density2_AVG', y='Volume2_AVG', data=filtered_data, s=150, color='blue') 
    plt.title(f'Fundamental Diagram on the second point (PAV{int((avindex-650)/10+1)}, MPR={av_pen}%)', fontsize=16)
    plt.xlabel('Density (Vehicles per mile)', fontsize=16)
    plt.ylabel('Volume (vehicles per hour)', fontsize=16)
    plt.xticks(fontsize=16)
    plt.yticks(fontsize=16)
    plt.grid(True)
    #plt.ylim(200, 1700)
    plt.tick_params(axis='both', labelsize=16)
    #plt.ylim(1000, 6000)
    plt.show()

def plot_aggregated_speed_comparison(avindex_value, demand_value, df, conversion_factor):
    # Filter the dataframe for the specified demand
    demand_filtered_df = df[(df['avindex'] == avindex_value) & (df['Demand'] == demand_value)]

    # Available categories in the dataset
    categories = ['PAV1', 'AV3', 'AV4', 'AV5', 'HV', 'All']  # Adjusted to include HV and All if data is available

    # Initialize a dictionary to store aggregated speeds
    aggregated_speeds = {}

    # Aggregate (average) speeds from km/h to ft/s for each available vehicle category
    for cat in categories:
        avg_speed_col = f'SPEEDAVG_AVG_{cat}'
        if avg_speed_col in demand_filtered_df.columns:
            aggregated_speeds[cat] = demand_filtered_df[avg_speed_col].mean() * conversion_factor

    # Setting the positions for the bars
    positions = np.arange(len(aggregated_speeds))

    # Different bar styles (colors and hatches)
    bar_colors = ['red', 'green', 'blue', 'orange', 'purple', 'brown']
    bar_hatches = ['/', '\\', '|', '-', '+', 'x']

    # Plotting
    fig, ax = plt.subplots(figsize=(12, 6))

    # Plotting bars for each available vehicle category with different styles
    for i, cat in enumerate(aggregated_speeds.keys()):
        ax.bar(positions[i], aggregated_speeds[cat], width=0.4, color=bar_colors[i], edgecolor='black', hatch=bar_hatches[i], label=f"{cat}'s Average Speed")

    # Labels and Title
    
    ax.set_xlabel('Vehicle Categories', fontweight='bold', fontsize=16)
    ax.set_ylabel('Speed (mph)', fontweight='bold', fontsize=16)
    ax.set_xticks(positions)
    ax.set_xticklabels(aggregated_speeds.keys(), fontsize=16)

   # ax.legend(fontsize=16)

    # Show the plot
    plt.show()

file_path = 'C:/Users/Ke/Box/Project/AMS Piecewise linear model/AMS Data Set/Updated Data/Simdata.csv'
#file_path = 'C:/Users/Ke/Box/Project/AMS Piecewise linear model/AMS Data Set/Updated Data/Simdata.csv'
#file_path = 'C:/Users/Ke/Box/Project/AMS Piecewise linear model/AMS Data Set/Simulation data/simulation_data.csv'
df = pd.read_csv(file_path)
# Conversion factor from km/h to mph
conversion_factor = 0.621371
avindex_value = 660
demand_value = 3000
#plot_aggregated_speed_comparison(avindex_value, demand_value, df, conversion_factor)
# Call the function for Demand = 3000 and 4500
PAV_number = 660
plot_speed_comparison_for_demand(PAV_number, 3000, df, conversion_factor)
plot_speed_comparison_for_demand(PAV_number, 4500, df, conversion_factor)

#fd_function1(df, avindex=PAV_number, av_pen=25)
#fd_function1(df, avindex=PAV_number, av_pen=75)

#fd_function2(df, avindex=PAV_number, av_pen=25)
#fd_function2(df, avindex=PAV_number, av_pen=75)






