clear
close all
clc

%% Inputs
kMinFreq = 10.0;  % (rad/s)
kMmaxFreq = 100.0;  % (rad/s)
kNumFreqs = 5;

kCyclesPerFreq = 5;
kSamplingFreq = 50.0;  % Must be >= kMmaxFreq / pi (Hz)

kAmplitude = 0.5;

kCyclesToIgnorePerFreq = 2;  % Must be < kCyclesPerFreq

% Print options: true == ON / "please print"; false == OFF / "don't print"
PRINT_TEST1 = true;
PRINT_TEST2 = true;
PRINT_TEST3 = false;
PRINT_TEST4 = false;
PRINT_TEST5 = false;

%% Generate and optionally print a logarithmically spaced angular frequency vector
kAngFreqs = logspace(log10(kMinFreq), log10(kMmaxFreq), kNumFreqs);

if PRINT_TEST1
   fprintf('logSpace test:\n')
   for i = 1:length(kAngFreqs)
      fprintf('%.4f ', kAngFreqs(i))
   end
   fprintf('\n')
end

%% Calculate and optionally print the number of samples for each element of kAngFreqs
kSamplesPerFreq = calcSamplesPerFreq(kAngFreqs, kCyclesPerFreq, kSamplingFreq);

if PRINT_TEST2
   fprintf('calcSamplesPerFreq test:\n')
   for i = 1:length(kSamplesPerFreq)
      fprintf('%d ', kSamplesPerFreq(i))
   end
   fprintf('\n')
end

%% Generate and optionally print (to a .csv file) a corresponding input signal
kSamplingPeriod = 1 / kSamplingFreq;  % (s)
kInputSignal = generateInputSignal(kAmplitude, kAngFreqs, kSamplesPerFreq, ...
   kSamplingPeriod);

if PRINT_TEST3
   inputSignalData = fopen(fullfile(pwd, 'signal_data\input.csv'), 'a');
   kInputSignalSizeMinus1 = length(kInputSignal) - 1;
   for i = 1:kInputSignalSizeMinus1
      fprintf(inputSignalData, '%.6f, ', kInputSignal(i));
   end
   fprintf(inputSignalData, '%.6f', kInputSignal(end));
   fclose(inputSignalData);
end
