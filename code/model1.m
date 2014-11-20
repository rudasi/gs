% testing discrete lowpass filter
steps = 1000;
x = uint16(zeros(1, steps));
input = zeros(1, steps);
input(1:100:end) = 150;
y = zeros(1, steps) ;
x(1) = input(1);
y(1) = input(1);
alpha = 255;
for i = 2:steps
% foo=uint16(x(i?1)) * uint16 (alpha);
% x(i)=bitshift(foo,?8);

    bar = uint16(x(i-1)) * uint16 (256) - uint16(x(i-1)) * uint16(alpha);
    decrement = bitshift(bar, -8);

    r = uint8(255*rand) ;

    if r < mod(bar, 2^8)
        decrement = decrement + 1 ;
    end
    x(i) = min(255 , x( i-1) - decrement + input(i));

    y(i) = y(i-1) * alpha /256 + input(i);
end

figure , hold on , plot (x , 'r .') , plot (y , 'g')
