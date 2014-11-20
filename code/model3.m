% Testing similarity of exponential decay vs. linear decay with
% exponential input.

dt = .001; %time step (s)
T = .25; %length of run (s)
time = dt:dt:T;
tau = .02; %filter time constant

u = .06*ones(1,length(time)) + .05*randn(1, length(time)); %random input

x = zeros(1,length(time)); %filter state variable

for i = 2: length(x)
	x(i) = x(i-1) - dt*x(i-1)/tau + u(i-1);
	if x(i) >= 1
		x(i) = 0;
	end
end
plot(time , x)

peak = 255; %top of byte range
y = zeros(1,length(time), 'uint8'); % note
	%: y is a function of x that decays at
	%a constant rate with zero input,
	%specifically y = (tau/dt) log(x) +
	%peak
for i = 2: length(y)
	y(i) = y(i-1) -1 + tau / dt / (exp(dt/tau*( double(y(i-1))-peak)) + u(i-1) /5) * u(i-1);
	if y(i) < 0 || y(i) >= peak
		y(i) = 0;
	end
end

z = exp(dt/tau*(double(y)-peak)); % z ~= x
hold on , plot(time , z, 'r.')

legend('double', 'uint8')
