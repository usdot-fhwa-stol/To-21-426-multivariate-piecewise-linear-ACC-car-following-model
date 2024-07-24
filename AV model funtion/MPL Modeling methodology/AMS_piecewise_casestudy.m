% Load the data
clear;
clc;
% Initialize variables to store final BPs and their variables
final_BPs = [];
best_vars = [];

data = readtable('data_example.csv');

data.Speed1_ft_per_s = data.Speed1 * 3.28084;
data.Speed2_ft_per_s = data.Speed2 * 3.28084;
data.Speed3_ft_per_s = data.Speed3 * 3.28084;
data.Speed4_ft_per_s = data.Speed4 * 3.28084;
data.Speed5_ft_per_s = data.Speed5 * 3.28084;
data.IVS1_ft = data.IVS1 * 3.28084;
data.IVS2_ft = data.IVS2 * 3.28084;
data.IVS3_ft = data.IVS3 * 3.28084;
data.IVS4_ft = data.IVS4 * 3.28084; 
drop_num = 1;
data(data.Speed1_ft_per_s < drop_num*3.28084, :) = [];
data(data.Speed2_ft_per_s < drop_num*3.28084, :) = [];
data(data.Speed3_ft_per_s < drop_num*3.28084, :) = [];
data(data.Speed4_ft_per_s < drop_num*3.28084, :) = [];
data(data.Speed5_ft_per_s < drop_num*3.28084, :) = [];
test_num = 2;
fieldName_pre_speed = ['Speed', num2str(test_num-1), '_ft_per_s'];
fieldName_fol_speed = ['Speed', num2str(test_num), '_ft_per_s'];
fieldName_IVS = ['IVS', num2str(test_num), '_ft'];

% Create new columns for time-shifted variables
data.Speed_t_1 = [NaN; data.(fieldName_fol_speed)(1:end-1)];  % Time-shifted speed (ft/s)
data.Speed_diff_t_1 = [NaN; (data.(fieldName_pre_speed)(1:end-1) - data.(fieldName_fol_speed)(1:end-1))];  % Time-shifted speed difference (ft/s)
data.IVS_t_1 = [NaN; data.(fieldName_IVS)(1:end-1)];  % Time-shifted IVS (ft)
data.Acceleration2_t = [NaN; diff(data.(fieldName_fol_speed)) / 0.1];  % Assuming the time step is 0.1 seconds

data = rmmissing(data, 'DataVariables', {'Speed_t_1', 'Speed_diff_t_1', 'IVS_t_1', 'Acceleration2_t'});
origin_data  = [data.Speed_t_1, data.Speed_diff_t_1, data.IVS_t_1, data.Acceleration2_t];
nRows = size(data, 1);
threshold =1;
origin_data  = [data.Speed_t_1, data.Speed_diff_t_1, data.IVS_t_1, data.Acceleration2_t];
%%
randIndices = randperm(nRows);
splitIndex = floor(nRows*0.7);
Rand_data1 = origin_data(randIndices(1:splitIndex), :);
Rand_data2 = origin_data(randIndices(splitIndex + 1:end), :);
%%
X = [Rand_data1(:,1), Rand_data1(:,2), Rand_data1(:,3)];
y = Rand_data1(:,4);
count_num = round(0.0005*length(y));
mdl = fitlm(X, y);
y_pred = predict(mdl, X);
combined_residuals = y - y_pred;
y_mean = mean(y);
r2 = mdl.Rsquared.Ordinary;
SS_tot = sum((y - y_mean).^2);
% combined_residuals = residuals;
n_iterations = 10;
R2 = [r2];
k=3;
currentSubsets = {{X, y}};
residualsPerPiece = {}; % Store residuals for each piece
overallR2 = zeros(1, n_iterations); % Store R^2 values for each iteration

% Sort each variable and get the corresponding indices
[sorted_velocity, idx_velocity] = sort(X(:,1));
[sorted_speed_diff, idx_speed_diff] = sort(X(:,2));
[sorted_distance, idx_distance] = sort(X(:,3));

%%
for iter = 1:n_iterations
% Get the residuals corresponding to the sorted indices
residuals = combined_residuals;
sorted_residual_velocity = residuals(idx_velocity);
sorted_residual_speed_diff = residuals(idx_speed_diff);
sorted_residual_distance = residuals(idx_distance);

% Find target interval with most data points for each sorted variable
intervals_velocity = find_intervals_with_details(sorted_residual_velocity);
intervals_speed_diff = find_intervals_with_details(sorted_residual_speed_diff);
intervals_distance = find_intervals_with_details(sorted_residual_distance);

% Select the interval with most data points
[~, max_idx_velocity] = max(intervals_velocity(2:end-1, 4));
[~, max_idx_speed_diff] = max(intervals_speed_diff(2:end-1, 4));
[~, max_idx_distance] = max(intervals_distance(2:end-1, 4));

% Define target intervals
target_interval_velocity = intervals_velocity(max_idx_velocity+1, :);
target_interval_speed_diff = intervals_speed_diff(max_idx_speed_diff+1, :);
target_interval_distance = intervals_distance(max_idx_distance+1, :);
if target_interval_velocity(end)<count_num && target_interval_speed_diff(end)<count_num && target_interval_distance(end)<count_num
    break;
end

CBPs = zeros(1,3);

% Loop through each variable to find the CBP
for var_idx = 1:3
    if var_idx == 1
        interval_data = target_interval_velocity;
        sorted_var_data = sorted_velocity;
        sorted_residuals = sorted_residual_velocity;
    elseif var_idx == 2
        interval_data = target_interval_speed_diff;
        sorted_var_data = sorted_speed_diff;
        sorted_residuals = sorted_residual_speed_diff;
    else
        interval_data = target_interval_distance;
        sorted_var_data = sorted_distance;
        sorted_residuals = sorted_residual_distance;
    end
    start_idx = interval_data(2);
    end_idx = interval_data(3);
    interval_var_data = sorted_var_data(start_idx:end_idx);
    interval_residuals = sorted_residuals(start_idx:end_idx);

    % Step 2.2: Calculate MRTI
    A = 0.5 * sum(interval_var_data(1:end-1) .* interval_residuals(2:end) - ...
                 interval_var_data(2:end) .* interval_residuals(1:end-1));

    % Step 2.3: Find CBP
    C_x = (1 / (6 * A)) * sum((interval_var_data(1:end-1) + interval_var_data(2:end)) .* ((interval_var_data(1:end-1) .* interval_residuals(2:end) - interval_var_data(2:end) .* interval_residuals(1:end-1))));
%     C_x  = mean(interval_var_data);
    if interval_data(4)<count_num
        C_x=0;
    end

    % Store the CBP
    CBPs(var_idx) = C_x;
end

% Step 2.4: Test CBPs with piecewise linear regression and find the best one as the final BP
    [final_BP, best_var] = best_CBP_based_on_piecewise_regression(CBPs, X, y);
    % (1 for velocity, 2 for speed difference, and 3 for distance).
    newSubsets = {}; % Initialize a new cell array to hold the subsets created in this iteration
    final_BPs = [final_BPs, final_BP];
    best_vars = [best_vars, best_var];
%     best_feature_column = subsetX(:, best_var);
%     below_BP = find(best_feature_column <= final_BP);
%     above_BP = find(best_feature_column > final_BP);

    for i = 1:length(currentSubsets)
        % Extract the current subset of X and y
        subsetX = currentSubsets{i}{1};
        subsetY = currentSubsets{i}{2};
        best_feature_column = subsetX(:, best_var);
        below_BP = find(best_feature_column <= final_BP);
        above_BP = find(best_feature_column > final_BP);
%         mdl_below = fitlm(X(below_BP, :), y(below_BP));
%         mdl_above = fitlm(X(above_BP, :), y(above_BP));

        X1 = subsetX(best_feature_column <= final_BP , :);
        y1 = subsetY(best_feature_column <= final_BP);
    
        X2 = subsetX(best_feature_column > final_BP, :);
        y2 = subsetY(best_feature_column > final_BP);
        
        
        index1 = best_feature_column <= final_BP;
        index2 = best_feature_column > final_BP;
    
        mdl1_BP = fitlm(X1,y1);
        mdl2_BP = fitlm(X2,y2);
        % disp(mdl1_BP.Coefficients.Estimate);
        % disp(mdl2_BP.Coefficients.Estimate);
        y_pred_1 = predict(mdl1_BP, X1);
        y_pred_2 = predict(mdl2_BP, X2);

        residuals_1 = y1 - y_pred_1;
        residuals_2 = y2 - y_pred_2;
        residualsPerPiece{end + 1} = residuals_1;
        residualsPerPiece{end + 1} = residuals_2;
        newSubsets{end + 1} = {X1, y1};
        newSubsets{end + 1} = {X2, y2};
        combined_residuals(index1) = residuals_1;
        combined_residuals(index2) = residuals_2;
    end
    SS_res = sum(combined_residuals.^2);    
    R_squared = 1 - (SS_res*(count_num-k) / SS_tot/(count_num-1));
    if R_squared-0.01<R2(end)
        break;
    end
    R2 = [R2, R_squared]; 
    currentSubsets = newSubsets;

end
keep_idx = true(1, length(final_BPs));
for i = 1:length(final_BPs)-1
    for j = i+1:length(final_BPs)
        if best_vars(i) == best_vars(j) && abs(final_BPs(1) - final_BPs(j)) < threshold
            keep_idx([i, j]) = false; 
        end
    end
end
% R2_final = R2(keep_idx);
Final_BPs = final_BPs(keep_idx)
Best_vars = best_vars(keep_idx)

% (1 for velocity, 2 for speed difference, and 3 for distance).
% piecewise_coefficients
%%
current_data = origin_data;
[Cof] = piece_results(Final_BPs, Best_vars, current_data);
Cof


