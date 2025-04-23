% Assume you have a timeseries object called 'ts'
data = x.Data;  % Extract the [200001 x 5] matrix
filename = 'test2_input_array.c';  % Output file

fid = fopen(filename, 'w');

% Write the array declaration
fprintf(fid, 'float test2_inputs[2000][5] = {\n');

% Write each row of the array
for i = 1:2000
    fprintf(fid, '    {');
    fprintf(fid, '%.15f, ', data(i, 1:end-1));  % all but last with comma
    fprintf(fid, '%.15f}', data(i, end));       % last one without comma

    if i ~= size(data, 1)
        fprintf(fid, ',\n');
    else
        fprintf(fid, '\n');
    end
end

% Close the array
fprintf(fid, '};\n');

fclose(fid);
