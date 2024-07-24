function max_interval = find_max_consecutive_interval(residuals)
    max_interval = 0;
    current_interval = 1;
    previous_sign = sign(residuals(1));

    for i = 2:length(residuals)
        current_sign = sign(residuals(i));
        if current_sign == previous_sign
            current_interval = current_interval + 1;
        else
            max_interval = max(max_interval, current_interval);
            current_interval = 1;
        end
        previous_sign = current_sign;
    end

    max_interval = max(max_interval, current_interval);
end
