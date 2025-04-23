% Assume you have a 1D array called 'arr'
arr = u.Data(:, 1);  % Example: take the first column from ts.Data
filename = 'test2_output_array.c';

fid = fopen(filename, 'w');

% Write the array declaration
fprintf(fid, 'float test2_outputs[200] = {\n');

% Write the values with formatting
for i = 1:200
    if mod(i, 5) == 1
        fprintf(fid, '    ');  % indentation for every new line group
    end

    if i < length(arr)
        fprintf(fid, '%.15f, ', arr(i));
    else
        fprintf(fid, '%.15f', arr(i));  % no comma for last element
    end

    if mod(i, 5) == 0 || i == 200
        fprintf(fid, '\n');  % new line after every 5 elements
    end
end

% Close the array
fprintf(fid, '};\n');

fclose(fid);
