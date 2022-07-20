clear
close all
clc

% Inputs
kMinFreq = 10.0;  % (rad/s)
kMmaxFreq = 100.0;  % (rad/s)
kNumFreqs = 5;

kCyclesPerFreq = 5;
kSamplingFreq = 50.0;  % Must be >= kMmaxFreq / pi (Hz)

kAmplitude = 0.5;

kCyclesToIgnorePerFreq = 2;  % Must be < kCyclesPerFreq

% - Print options: 1 == "yes, print"; 0 == "no, don't print"
PRINT_TEST1 = 1;
PRINT_TEST2 = 1;
PRINT_TEST3 = 0;
PRINT_TEST4 = 0;
PRINT_TEST5 = 0;

% Generate and optionally print a logarithmically spaced angular frequency
% vector
kAngFreqs = logspace(log10(kMinFreq), log10(kMmaxFreq), kNumFreqs);

if PRINT_TEST1
   fprintf('logSpace test:\n')
   for i = 1:length(kAngFreqs)
      fprintf('%.4f ', kAngFreqs(i))
   end
   fprintf('\n')
end

% Calculate and optionally print the number of samples for each element of
% kAngFreqs
kSamplesPerFreq = calcSamplesPerFreq(kAngFreqs, kCyclesPerFreq, kSamplingFreq);

if PRINT_TEST2
   fprintf('calcSamplesPerFreq test:\n')
   for i = 1:length(kSamplesPerFreq)
      fprintf('%d ', kSamplesPerFreq(i))
   end
   fprintf('\n')
end












