import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

# Function definition
def plot_vehicle_positions(data, lane, time_range, type_colors):

    filtered_data = data[(data['LANE'] == lane) & (data['VEHICLESIMSEC'].isin(time_range))]

    # Plotting
    plt.figure(figsize=(10, 6))

    for vehtype, color in type_colors.items():
        # Separate data based on VEHTYPE
        data_type = filtered_data[filtered_data['VEHTYPE'] == vehtype]

        # Plot for each VEHTYPE
        plt.scatter(data_type['VEHICLESIMSEC'], data_type['POS'], color=color, label=f'VEHTYPE {vehtype}', s=3)


    plt.xlabel('VEHICLESIMSEC(s)', fontsize=14)
    plt.ylabel('POS(ft)', fontsize=14)
    plt.title(f'Position by Vehicle Simulation Second in Lane {lane}', fontsize=14)
    plt.legend(['PAV2','HV'], fontsize=14)
    plt.grid(True)
    plt.show()
    
def plot_acceleration_distribution(data, vehtype1, vehtype2):
    # Filter data for acceleration between -5 and 5 m/s^2
    data = data[(data['ACCELERATION'] >= -5) & (data['ACCELERATION'] <= 5)]
    
    # Convert acceleration from m/s^2 to ft/s^2
    data['ACCELERATION'] *= 3.28084
    
    plt.figure(figsize=(18, 6))

    # Plot for all VEHTYPEs combined
    plt.subplot(1, 3, 1)
    sns.histplot(data['ACCELERATION'], kde=False, bins=30)
    plt.title('Acceleration Distribution for All VEHTYPEs (ft/s²)')
    plt.xlabel('Acceleration (ft/s²)')
    plt.ylabel('Frequency')

    # Plot for VEHTYPE1
    plt.subplot(1, 3, 2)
    sns.histplot(data[data['VEHTYPE'] == vehtype1]['ACCELERATION'], kde=False, bins=30, color='orange')
    plt.title(f'Acceleration Distribution for ACC vehicle (ft/s²)')
    plt.xlabel('Acceleration (ft/s²)')
    plt.ylabel('Frequency')

    # Plot for VEHTYPE2
    plt.subplot(1, 3, 3)
    sns.histplot(data[data['VEHTYPE'] == vehtype2]['ACCELERATION'], kde=False, bins=30, color='green')
    plt.title(f'Acceleration Distribution for HV (ft/s²)')
    plt.xlabel('Acceleration (ft/s²)')
    plt.ylabel('Frequency')

    plt.tight_layout()
    plt.show()


def calculate_iTTC(data):
    data['VEHTYPE'] = data['VEHTYPE'].astype(int)
    def calculate_iTTC_for_subset(subset_data):
        valid_data = subset_data[(subset_data['HDWY'] != 0) & (subset_data['SPEEDDIFF'] > 0) & ((3.6 * subset_data['HDWY'] ) != 0)]
        average_ratio = (valid_data['SPEEDDIFF'] / (3.6 * valid_data['HDWY'] )).mean()
        return average_ratio
    total_iTTC = calculate_iTTC_for_subset(data)
    iTTC_640 = calculate_iTTC_for_subset(data[data['VEHTYPE'] == 640])
    iTTC_not_640 = calculate_iTTC_for_subset(data[data['VEHTYPE'] != 640])
    return total_iTTC, iTTC_640, iTTC_not_640
    
def plot_iTTC_results(labels, results):
    total_values = [result[0] for result in results]
    values_640 = [result[1] for result in results]
    values_not_640 = [result[2] for result in results]
    x = np.arange(len(labels))  # the label locations
    width = 0.3
    fig, ax = plt.subplots()
    rects1 = ax.bar(x - width, total_values, width, label='Total iTTC')
    rects2 = ax.bar(x, values_640, width, label='iTTC 640')
    rects3 = ax.bar(x + width, values_not_640, width, label='iTTC Not 640')
    ax.set_xlabel('File Names')
    ax.set_ylabel('iTTC Values')
    ax.set_title('iTTC by File and Vehicle Type')
    ax.set_xticks(x)
    ax.set_xticklabels(labels)
    ax.legend()
    plt.xticks(rotation=45)
    plt.show()

def plot_iTTC_results(labels, results):
    total_values = [result[0] for result in results]
    values_640 = [result[1] for result in results]
    values_not_640 = [result[2] for result in results]
    x = np.arange(len(labels))  # the label locations
    width = 0.3  
    fig, ax = plt.subplots()
    rects1 = ax.bar(x - width, total_values, width, label='Total iTTC')
    rects2 = ax.bar(x, values_640, width, label='iTTC 640')
    rects3 = ax.bar(x + width, values_not_640, width, label='iTTC Not 640')
    # Add some text for labels, title and custom x-axis tick labels, etc.
    ax.set_xlabel('File Names')
    ax.set_ylabel('iTTC Values')
    ax.set_title('iTTC by File and Vehicle Type')
    ax.set_xticks(x)
    ax.set_xticklabels(labels)
    ax.legend()

    plt.xticks(rotation=45)
    plt.show()
    
def calculate_iTTC_hete(data):
    data['VEHTYPE'] = data['VEHTYPE'].astype(int)
    def calculate_iTTC_for_subset(subset_data):
        valid_data = subset_data[(subset_data['HDWY'] != 0) & (subset_data['SPEEDDIFF'] > 0) & ((3.6 * subset_data['HDWY'] ) != 0)]
        average_ratio = (valid_data['SPEEDDIFF'] / (3.6 * valid_data['HDWY'] )).mean()
        return average_ratio
    total_iTTC = calculate_iTTC_for_subset(data)
    iTTC_640 = calculate_iTTC_for_subset(data[data['VEHTYPE'] == 640])
    iTTC_660 = calculate_iTTC_for_subset(data[data['VEHTYPE'] == 660])
    iTTC_670 = calculate_iTTC_for_subset(data[data['VEHTYPE'] == 670])
    iTTC_680 = calculate_iTTC_for_subset(data[data['VEHTYPE'] == 680])
    iTTC_690 = calculate_iTTC_for_subset(data[data['VEHTYPE'] == 690])
    return total_iTTC, iTTC_640, iTTC_660, iTTC_670,iTTC_680,iTTC_690
#file_path = "C:\\Users\\Ke Ma\\Box\\Project\\AMS Piecewise linear model\\AMS Data Set\\Simulation data\\670_75_4500_6_copy.csv"
#file_path = "G:\\Trajdata\\copy\\670_50_4500_1_copy.csv"


#file_path = "G:\\Trajdata\\660_50_4500_5.csv"
#file_path = "C:\\Users\\Ke\\Box\\Project\\AMS Piecewise linear model\\AMS Data Set\\01.15 data\\670_25_6500_2.csv"


file_path1 = "E:\\Chrome Downloads\\404_404_4500_1.csv"
#file_path2 = "H:\\Traj0101\\690_100_3000_1.csv"
data1 = pd.read_csv(file_path1)
#data2 = pd.read_csv(file_path2)
#average_ratio1 = calculate_iTTC(data)
iTTC1 = calculate_iTTC_hete(data1)
#iTTC2 = calculate_iTTC(data2)

print(iTTC1)
#print(iTTC2)


###
#base_path = "G:\\Trajdata\\670_{}_4500_5.csv"
#intervals = [0, 25, 50, 75, 100]
#iTTCresults = []
#for interval in intervals:
#    file_path = base_path.format(interval)
#    new_data = pd.read_csv(file_path)
#    total_iTTC, iTTC_640, iTTC_not_640 = calculate_iTTC(new_data)
#    iTTCresults.append((total_iTTC, iTTC_640, iTTC_not_640))
    
  
#plot_acceleration_distribution(new_data,'660','640')
#average_ratio = calculate_iTTC(new_data)
#print(average_ratio)
#selected_lanes = ['1-1']
#filtered_lanes_data = new_data[new_data['LANE'].isin(selected_lanes)].copy()
#filtered_lanes_data['LANE'] = filtered_lanes_data['LANE']
#plot_vehicle_positions(filtered_lanes_data, '1-1', range(1901, 2000), {660: 'red', 640: 'blue'})







