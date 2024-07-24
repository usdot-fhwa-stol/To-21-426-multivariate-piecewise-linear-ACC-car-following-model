function [final_BP, best_var] = best_CBP_based_on_piecewise_regression(CBPs, X, y)
    best_r2 = -Inf; % Initialize the best R^2 value
    final_BP = 0; % Initialize the best breakpoint
    best_var = 0; % Initialize the variable index for the best breakpoint

    % Loop through each CBP and perform piecewise linear regression
    for i = 1:length(CBPs)
        CBP = CBPs(i);
        
        % Segment the data into two pieces based on the CBP
        X1 = X(X(:, i) <= CBP, :);
        y1 = y(X(:, i) <= CBP);
        
        X2 = X(X(:, i) > CBP, :);
        y2 = y(X(:, i) > CBP);

        % Perform linear regression for each piece
        mdl1 = fitlm(X1, y1);
        mdl2 = fitlm(X2, y2);
        
        % Calculate R^2 for each piece
        r2_1 = mdl1.Rsquared.Ordinary;
        r2_2 = mdl2.Rsquared.Ordinary;
        
        % Combine R^2 as a weighted sum based on the number of points in each piece
        combined_r2 = (length(y1) * r2_1 + length(y2) * r2_2) / (length(y1) + length(y2));

        % Update the best R^2, corresponding CBP, and variable index if needed
        if combined_r2 > best_r2
            best_r2 = combined_r2;
            final_BP = CBP;
            best_var = i;
        end
    end
end
