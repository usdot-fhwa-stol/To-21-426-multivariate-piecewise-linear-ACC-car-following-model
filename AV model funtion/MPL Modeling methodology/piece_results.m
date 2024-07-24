function [Cof] = piece_results(Final_BPs, Best_vars, current_data)
if length(Final_BPs)==1


    piecewise_datasets1 = current_data((current_data(:,Best_vars(1)) <= Final_BPs(1)), :);
    mdl1 = fitlm(piecewise_datasets1(:,1:3), piecewise_datasets1(:,4));
    coefficients1 = mdl1.Coefficients.Estimate; 
    coefficients1 = coefficients1(coefficients1 ~= 0);
    
    piecewise_datasets2 = current_data( (current_data(:,Best_vars(1)) > Final_BPs(1)) , :);
    mdl2 = fitlm(piecewise_datasets2(:,1:3), piecewise_datasets2(:,4));
    coefficients2 = mdl2.Coefficients.Estimate; 
    coefficients2 = coefficients2(coefficients2 ~= 0);
    
    
    Cof = [coefficients1, coefficients2];

end

if length(Final_BPs)==2
    piecewise_datasets1 = current_data((current_data(:,Best_vars(1)) <= Final_BPs(1)) & (current_data(:,Best_vars(2)) <= Final_BPs(2)), :);
    mdl1 = fitlm(piecewise_datasets1(:,1:3), piecewise_datasets1(:,4));
    coefficients1 = mdl1.Coefficients.Estimate; 
    coefficients1 = coefficients1(coefficients1 ~= 0);
    
    piecewise_datasets2 = current_data( (current_data(:,Best_vars(1)) <= Final_BPs(1)) & (current_data(:,Best_vars(2)) > Final_BPs(2)), :);
    mdl2 = fitlm(piecewise_datasets2(:,1:3), piecewise_datasets2(:,4));
    coefficients2 = mdl2.Coefficients.Estimate; 
    coefficients2 = coefficients2(coefficients2 ~= 0);
    
    piecewise_datasets3 = current_data( (current_data(:,Best_vars(1)) > Final_BPs(1)) & (current_data(:,Best_vars(2)) <= Final_BPs(2)), :);
    mdl3 = fitlm(piecewise_datasets3(:,1:3), piecewise_datasets3(:,4));
    coefficients3 = mdl3.Coefficients.Estimate; 
    coefficients3 = coefficients3(coefficients3 ~= 0);
    
    piecewise_datasets4 = current_data( (current_data(:,Best_vars(1)) > Final_BPs(1)) & (current_data(:,Best_vars(2)) > Final_BPs(2)), :);
    mdl4 = fitlm(piecewise_datasets4(:,1:3), piecewise_datasets4(:,4));
    coefficients4 = mdl4.Coefficients.Estimate; 
    coefficients4 = coefficients4(coefficients4 ~= 0);
    
    Cof = [coefficients1, coefficients2, coefficients3, coefficients4];

end

if length(Final_BPs)==3
    piecewise_datasets1 = current_data((current_data(:,Best_vars(1)) <= Final_BPs(1)) & (current_data(:,Best_vars(2)) <= Final_BPs(2)) & (current_data(:,Best_vars(3)) <= Final_BPs(3)), :);
    mdl1 = fitlm(piecewise_datasets1(:,1:3), piecewise_datasets1(:,4));
    coefficients1 = mdl1.Coefficients.Estimate; 
    coefficients1 = coefficients1(coefficients1 ~= 0);
    
    piecewise_datasets2 = current_data( (current_data(:,Best_vars(1)) <= Final_BPs(1)) & (current_data(:,Best_vars(2)) <= Final_BPs(2)) & (current_data(:,Best_vars(3)) > Final_BPs(3)), :);
    mdl2 = fitlm(piecewise_datasets2(:,1:3), piecewise_datasets2(:,4));
    coefficients2 = mdl2.Coefficients.Estimate; 
    coefficients2 = coefficients2(coefficients2 ~= 0);
    
    piecewise_datasets3 = current_data( (current_data(:,Best_vars(1)) <= Final_BPs(1)) & (current_data(:,Best_vars(2)) > Final_BPs(2)) & (current_data(:,Best_vars(3)) <= Final_BPs(3)), :);
    mdl3 = fitlm(piecewise_datasets3(:,1:3), piecewise_datasets3(:,4));
    coefficients3 = mdl3.Coefficients.Estimate; 
    coefficients3 = coefficients3(coefficients3 ~= 0);
    
    piecewise_datasets4 = current_data( (current_data(:,Best_vars(1)) <= Final_BPs(1)) & (current_data(:,Best_vars(2)) > Final_BPs(2)) & (current_data(:,Best_vars(3)) > Final_BPs(3)), :);
    mdl4 = fitlm(piecewise_datasets4(:,1:3), piecewise_datasets4(:,4));
    coefficients4 = mdl4.Coefficients.Estimate; 
    coefficients4 = coefficients4(coefficients4 ~= 0);

    piecewise_datasets5 = current_data((current_data(:,Best_vars(1)) > Final_BPs(1)) & (current_data(:,Best_vars(2)) <= Final_BPs(2)) & (current_data(:,Best_vars(3)) <= Final_BPs(3)), :);
    mdl5 = fitlm(piecewise_datasets5(:,1:3), piecewise_datasets5(:,4));
    coefficients5 = mdl5.Coefficients.Estimate; 
    coefficients5 = coefficients5(coefficients5 ~= 0);
    
    piecewise_datasets6 = current_data( (current_data(:,Best_vars(1)) > Final_BPs(1)) & (current_data(:,Best_vars(2)) <= Final_BPs(2)) & (current_data(:,Best_vars(3)) > Final_BPs(3)), :);
    mdl6 = fitlm(piecewise_datasets6(:,1:3), piecewise_datasets6(:,4));
    coefficients6 = mdl6.Coefficients.Estimate; 
    coefficients6= coefficients6(coefficients6 ~= 0);
    
    piecewise_datasets7 = current_data( (current_data(:,Best_vars(1)) > Final_BPs(1)) & (current_data(:,Best_vars(2)) > Final_BPs(2)) & (current_data(:,Best_vars(3)) <= Final_BPs(3)), :);
    mdl7 = fitlm(piecewise_datasets7(:,1:3), piecewise_datasets7(:,4));
    coefficients7 = mdl7.Coefficients.Estimate; 
    coefficients7 = coefficients7(coefficients7 ~= 0);
    
    piecewise_datasets8 = current_data( (current_data(:,Best_vars(1)) > Final_BPs(1)) & (current_data(:,Best_vars(2)) > Final_BPs(2)) & (current_data(:,Best_vars(3)) > Final_BPs(3)), :);
    mdl8 = fitlm(piecewise_datasets8(:,1:3), piecewise_datasets8(:,4));
    coefficients8 = mdl8.Coefficients.Estimate; 
    coefficients8 = coefficients8(coefficients8 ~= 0);
    
    Cof = [coefficients1, coefficients2, coefficients3, coefficients4, coefficients5, coefficients6, coefficients7, coefficients8];

end

end