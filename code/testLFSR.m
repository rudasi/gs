% testing 32 bit linear feedback shift register

taps = [32 22 2 1]; % http://www.xilinx.com/support/documentation/application_notes/xapp052.pdf

x = zeros(1,10000, 'uint32');
xi = uint32(2135);
for i = 1:length(x)
    fb = bitget(xi, taps(1));
    for j = 2:length(taps)
        fb = xor(fb, bitget(xi, taps(j)));
    end
    xi = bitshift(xi, 1);
    xi = bitset(xi, 1, fb);
    x(i) = xi
end

figure, plot(x)
figure, hist(double(x))
