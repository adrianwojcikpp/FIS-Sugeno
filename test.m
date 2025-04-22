% Define input template
inputs = ones(1, 6);

% Values to test for input 6
test_values = 0.1 : 0.5 : 8;

fprintf('Comparing weighted rule outputs (w_i * z_i) from MATLAB:\n');

for x = test_values
    inputs(6) = x;

    %fprintf('Input6 = %.2f\n', x);

    for ruleIdx = 1:3
        % Extract rule info
        mfIdx = myFis.Rules(ruleIdx).Antecedent(6);  % MF for input 6
        outputFcn = myFis.Outputs(1).MembershipFunctions(ruleIdx); % Should be Sugeno constant or linear

        % Get firing strength (w_i)
        w = evalmf(myFis.Inputs(6).MembershipFunctions(mfIdx), x);

        % Compute output (z_i) using rule’s function
        z = evalmf(outputFcn, inputs);  % works for both constant & linear Sugeno outputs

        %fprintf('  Rule %d -> Weighted Output (w*z): %.4f\n', ruleIdx, w * z);
    end
    fprintf('  Output : %.4f\n', evalfis(myFis, inputs));
    %fprintf('---------------------------\n');
end
