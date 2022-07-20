clear
close all
clc

%% Inputs
kMinFreq = 10.0;  % (rad/s)
kMaxFreq = 100.0;  % (rad/s)
kNumFreqs = 5;

kCyclesPerFreq = 5;
kSamplingFreq = 50.0;  % Must be >= kMaxFreq / pi (Hz)

kAmplitude = 0.5;

kCyclesToIgnorePerFreq = 2;  % Must be < kCyclesPerFreq

% MATLAB-specific
% - Print options: true == ON / "please print"; false == OFF / "don't print"
PRINT_TEST1 = false;
PRINT_TEST2 = false;
PRINT_TEST3 = false;
PRINT_TEST4 = true;

% - Parameters for system being "identified"
naturalFreq = 100;  % (rad/s)
dampingRatio = 0.5;

%% Generate and optionally print a logarithmically spaced angular frequency vector
kAngFreqs = logspace(log10(kMinFreq), log10(kMaxFreq), kNumFreqs);

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

%% Calculate and optionally print the corresponding frequency response: magnitude and phase
% MATLAB-specific code
% - lsim
time = (0 : length(kInputSignal) - 1) * kSamplingPeriod;  % (s)
s = tf('s');
sysBeingIDed = naturalFreq^2 / (s^2 + 2 * dampingRatio * naturalFreq * s ...
   + naturalFreq^2);
outputSignal = lsim(sysBeingIDed, kInputSignal, time)';

% - Plots
plot(time, kInputSignal, time, outputSignal)
xlabel('Time (s)'), ylabel('Signal')
legend('in', 'out')
grid on, grid minor
figure; bode(sysBeingIDed)
xlim([kMinFreq kMaxFreq])
grid on, grid minor

% C++ code
kFreqResponse = calcMagAndPhase(outputSignal, kAmplitude, kAngFreqs, ...
   kSamplesPerFreq, kSamplingPeriod, kCyclesPerFreq, kCyclesToIgnorePerFreq);

if PRINT_TEST4
   fprintf('calcMagAndPhase test:\n')
   fprintf('- magnitude: ')
   kMagAndPhaseSize = length(kFreqResponse.magnitude);
   for i = 1:kMagAndPhaseSize
      fprintf('%.4f ', kFreqResponse.magnitude(i))
   end
   fprintf('\n- phase: ')
   for i = 1:kMagAndPhaseSize
      fprintf('%.4f ', kFreqResponse.phase(i))
   end
   fprintf('\n')
end
