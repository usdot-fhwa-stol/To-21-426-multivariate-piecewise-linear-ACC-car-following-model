
## Summary

This repository contains the source code of a nonlinear model development, mixed traffic simulation model, and plot function. The source code of nonlinear model development was developed to generate a nonlinear car-following model for adaptive cruise control (ACC)-equipped vehicles. The source code of the mixed traffic simulation model was imported into PTV VISSIM to conduct mixed traffic simulations. The source code of the plot function was developed to plot basic traffic statistics based on the simulation results. The source code was programmed in MATLAB 2022b, Microsoft Visual Studio 2019 with C++, and Python 3.8.


# Outline
* Project Description 
* Prerequisites
* Usage
	* Building ACC Model
	* Testing and Execution Simulation
* License
* Contributions
* Contact Information
* Acknowledgements

# Project Description

This project is dedicated to advancing the simulation and understanding of ACC-equipped vehicles through the development of a Multivariate Piecewise Linear (MPL) car-following modeling methodology. The primary goal is to overcome the limitations of existing ACC models that struggle to account for the unique nonlinear dynamics of different vehicles. By introducing a model that utilizes empirically determined breakpoints from actual trajectory data (continues position, velocity, acceleration data), this project aims to enhance the predictive accuracy and reliability of ACC simulations.

# Prerequisites

Before starting this project, ensure you have MATLAB 2022b and the necessary toolbox, Microsoft Visual Studio 2019, and PTV VISSIM installed on the system. The project supports Windows 10 or newer. Additionally, install Python for additional data processing. Check that your machine has at least 8 GB of RAM and 20 GB of free disk space.

# Usage
Follow these detailed instructions to set up and run the software components in this project.

## Building ACC Model

This project involves software that does not require building or compilation in the traditional sense, as it primarily uses MATLAB scripts and PTV VISSIM for simulations. However, the C++ components will need to be compiled:  
Step 1: Install Required Software:  
Install Matlab 2022b:  
Download and install Matlab 2022b (some packages need to be installed according to the software required).
Download and install Microsoft Visual Studio 2019. Ensure the "Desktop development with C++" workload is selected during installation.  

Step 2: Generate ACC Vehicle Model: 
Navigate to the project directory \AV model function\MPL Modeling methodology\.  
Open and run the project file AMS_piecewise_casestudy.m. A data example named "data_example" is uploaded in \AV model function\MPL Modeling methodology\. (Need to change the data path to run the code) 
"Final_BPs" and "Best_vars" represent the breakpoint value and position (1 for velocity, 2 for speed difference, and 3 for distance). "Cof" represents each piece's coefficients (in the order of constant term, coefficients of the velocity of the following vehicle, coefficients of velocity difference, and coefficients of distance). (Since 70% of data are used to train models, the results vary in different runs.) 

Step 3: Generate .dll files for MPL models.  
Open Microsoft Visual Studio and build the DLL files based on the value of the coefficients in "Cof" to generate each piece of MPL models. Set up the 'bp1','bp2' or more according to the breakpoint value and position generated in step 2.    
Four generated examples of MPL models can be found in the directory of SImulationCode, including DriverModel_DLL_PAV2, DriverModel_DLL_PAV3, DriverModel_DLL_PAV4, and DriverModel_DLL_PAV5.  

## Testing and Execution Simulation
Step 1: Generate Simulation Network. Open PTV VISSIM and load the I-75.inpx file located in \SimulationCode\Vissim\.  
Step 2: Define ACC Vehicles. Open the "Vehicle Types" interface and add four vehicle types by clicking the "+" button as illustrated in the figure below. Name these vehicles AV2, AV3, AV4, and AV5.\
![ill1](https://github.com/QinzhengW/assets/blob/main/VISSIM_screenshot1.png)  
Step 3: Specify the Car-Following Model for Each ACC Vehicle. Here we use AV2 as an example and the concrete steps are as follows:  
  (1) Click the "Edit" in the interface of PTV VISSIM.  
  (2) Click the "External Drive Model" in the pop-up window.  
  (3) Browse and select the DriverModel.dll generated in "Step 3: Generate .dll files for MPL models" in the "Building ACC Model" section for AV2. In this example, the model path is “/SimulationCode/DriverModel_DLL_PAV2/x64/Release/DriverModel.dll”
The above steps are shown in the following figure. 
![ill2](https://github.com/QinzhengW/assets/blob/main/VISSIM_screenshot2.PNG)  

Use the same steps to specify the car-following models for AV3, AV4, and AV5.  

Step 4:Define Simulation Scenarios  
* Homogeneous Scenario: Only one type of ACC vehicle is included in the simulation, alongside human-driven vehicles. For example, if only AV2 is in the simulation, set the traffic flow proportion of AV3, AV4, and AV5 to 0.001 (as VISSIM does not permit a RelFlow value of 0) to indicate their absence. The setting is illustrated in the figure below.
![ill3](https://github.com/QinzhengW/assets/blob/main/VISSIM_screenshot3.PNG)  
* Heterogeneous Scenario: All four types of ACC vehicles are included in the simulation. Define all of them in the "Vehicle Composition" in VISSIM (in this example, AV 1 at 15.1 percent MPR, AV 2 at 14.4 percent MPR, AV 3 at 11.5 percent MPR, and AV 4 at 8.1 percent MPR, the residual 50.9 percent of the vehicle population in the simulation is assumed to be HVs). The setting is shown in the following figure for the main road.
![ill4](https://github.com/QinzhengW/assets/blob/main/VISSIM_screenshot4.PNG)

Follow the same procedure to define the vehicle composition for the on-ramp.

Step 5: Run and Analyze the Simulation  
Execute the Simulation:  
Start the simulation in PTV VISSIM to observe the behavior of ACC-equipped vehicles.  
Evaluate Performance:  
Most results are directly available via VISSIM's "Vehicle Network Performance Evaluation" module.  
For detailed analysis like standard deviation of speed, use the provided MATLAB scripts located in \SimulationCode\Vissim\Process.m.  

Step 6: Plot statistical results
For detailed statistical analysis or custom visualizations, execute the Python file in \FHWA project\Plot function to process the simulation output.   
"speedstd.py" draws the speed and speed stand deviation plots. "trajectroy_all.py" concludes the inverse time-to-collosion results.(other py. files are testing files)
Every appearance of “640” represents the appearance of HV. Appearances of “660”, “670”, “680”, and “690”  represent the appearance of different ACC models.  


**Status:** This project is in the release phase.


# License
This project is licensed under the Creative Commons 1.0 Universal (CC0 1.0) License - see the [License.MD](https://github.com/usdot-jpo-codehub/codehub-readme-template/blob/master/LICENSE) for more details. 

# Contributions

Please read [CONTRIBUTING.md](https://github.com/usdot-jpo-codehub/codehub-readme-template/blob/master/Contributing.MD) for details on our Code of Conduct, the process for submitting pull requests to us, and how contributions will be released.

# Contact Information

Contact Name: Ke Ma
Contact Information: kma62@wisc.edu

# Acknowledgements
This research is supported by Federal Highway Administration (FHWA) Office.

## Citing this code
To track how this government-funded code is used, we request that if you decide to build additional software using this code please acknowledge its Digital Object Identifier in your software's README/documentation.

> Digital Object Identifier: https://doi.org/xxx.xxx/xxxx

To cite this code in a publication or report, please cite our associated report/paper and/or our source code. Below is a sample citation for this code:

> ITS CodeHub Team. (2021). _ITS CodeHub README Template_ (0.1) [Source code]. Provided by ITS CodeHub through GitHub.com. Accessed 2021-01-27 from https://doi.org/xxx.xxx/xxxx.

When you copy or adapt from this code, please include the original URL you copied the source code from and the date of retrieval as a comment in your code. Additional information on how to cite can be found in the [ITS CodeHub FAQ](https://its.dot.gov/code/#/faqs).
