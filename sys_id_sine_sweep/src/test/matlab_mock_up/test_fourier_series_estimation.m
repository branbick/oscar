clear
close all
clc

%% Inputs
f0 = 10;  % Frequency of signal (Hz)
fs = 100;  % Sampling frequency (Hz)
N = 3;  % Number of signal periods to analyze (--)

%% Check #1
if fs < 2 * f0
   error('fs must be >= 2 * f0')
end

%% Calculations
% - Intermediate
w0 = 2 * pi * f0;  % Frequency of signal (rad/s)
T0 = 1 / f0;  % Period of signal (s)
Ts = 1 / fs;  % Sampling period (s)

% - Signal
t = (0:100) * Ts;  % Time (s)
f = sin(w0 * t);  % Range [arbitrary unit(s)]

%% Check #2
if N * T0 > t(end)
   error('N * T0 must be < t(end)')
end

%% Calculations (cont.)

% - Fourier: analyze N period(s) of f
nIndex = find(t < 1.000001 * N * T0 & t > 0.999999 * N * T0);
tN = t(1:nIndex);
fN = f(1:nIndex);

% - - Coefficients
a1 = 2 / (N * T0) * trapz(tN, fN .* cos(w0 * tN));
b1 = 2 / (N * T0) * trapz(tN, fN .* sin(w0 * tN));

% - - Estimation
fEst = a1 * cos(w0 * tN) + b1 * sin(w0 * tN);

%% Plot
plot(t, f, tN, fEst), legend('f', 'fEst')
