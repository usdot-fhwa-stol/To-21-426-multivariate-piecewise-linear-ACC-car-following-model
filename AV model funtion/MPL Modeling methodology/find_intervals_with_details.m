function intervals_with_details_matrix = find_intervals_with_details(residuals)
    current_interval = 1;
    previous_sign = sign(residuals(1));
    start_position = 1;
    
    % Initialize an empty matrix to store the interval details
    intervals_matrix = [];
    
    for i = 2:length(residuals)
        current_sign = sign(residuals(i));
        if current_sign == previous_sign
            current_interval = current_interval + 1;
        else
            % Append the details to the matrix
            intervals_matrix = [intervals_matrix; [previous_sign, start_position, i-1, current_interval]];
            current_interval = 1;
            start_position = i;
        end
        previous_sign = current_sign;
    end
    
    % Append the last interval details to the matrix
    intervals_with_details_matrix = [intervals_matrix; [previous_sign, start_position, length(residuals), current_interval]];
end