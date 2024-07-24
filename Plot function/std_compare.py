
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load the data
file_path = 'C:/Users/Ke/Box/Project/AMS Piecewise linear model/AMS Data Set/01.15 data/Simdata.csv'
#file_path = 'E:/Common/WeChatFile/WeChat Files/wxid_pazt9l56eygz22/FileStorage/File/2024-02/Simdata(2).csv'
data = pd.read_csv(file_path)

filtered_data = data[(data['Demand'] == 4500) & (data['Av_Pen'].isin([25, 50, 75]))]

# Define avindex values to consider
avindex_values = [660, 670, 680, 690]

# Conversion factor from km/h to mph
kmh_to_mph = 0.621371

# Update speed values to mph
for column in ['SPEEDSTD_MAX_AV', 'SPEEDSTD_MIN_AV', 'SPEEDSTD_AVG_AV']:
    filtered_data[column] *= kmh_to_mph

# Define colors and hatches for the bar plots
colors = ['red', 'green', 'blue']
hatches = ['//', '\\\\', 'xx']
av_pen_values = [25, 50, 75]

# Update the legends to represent MPR values
legends = ['25% MPR', '50% MPR', '75% MPR']

# Update x-axis labels
x_labels = ['PAV1', 'PAV2', 'PAV3', 'PAV4']

# Function to create final bar plots with error bars
def create_complete_bar_plot_with_error_bars(speed_metric, title, ylabel):
    fig, ax = plt.subplots(figsize=(10, 6))
    bar_width = 0.2
    positions = np.arange(len(avindex_values))

    for i, av_pen in enumerate(av_pen_values):
        mean_speeds = []
        errors = []
        for av in avindex_values:
            subset = filtered_data[(filtered_data['avindex'] == av) & (filtered_data['Av_Pen'] == av_pen)]
            mean_speed = subset[speed_metric].mean()
            error = (subset['SPEEDSTD_MAX_AV'] - subset['SPEEDSTD_MIN_AV']) / 2
            mean_speeds.append(mean_speed)
            errors.append(error.mean())
        
        ax.bar(positions + i * bar_width, mean_speeds, width=bar_width, color=colors[i], hatch=hatches[i], yerr=errors, capsize=5, label=legends[i])

    ax.set_xlabel('ACC Vehicle Index', fontsize=16)
    ax.set_ylabel('Speed Std (mph)', fontsize=16)
    ax.set_xticks(positions + bar_width / 2)
    ax.set_xticklabels(x_labels, fontsize=16)
    ax.legend(fontsize=16)
    ax.tick_params(axis='y', labelsize=16)
    ax.set_ylim(0, 30)
    return fig, ax

# Create complete bar plots for each speed metric with error bars
fig_av_complete, ax_av_complete = create_complete_bar_plot_with_error_bars('SPEEDSTD_AVG_AV', 'Average Speed of Automated Vehicles', 'Speed (mph)')

# Show the plots
plt.show()