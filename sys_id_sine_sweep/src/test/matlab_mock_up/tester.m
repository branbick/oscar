clear
close all
clc

%% Inputs
kMinFreq = 10.0;  % (rad/s)
kMaxFreq = 100.0;  % (rad/s)
kNumFreqs = 3;

kCyclesPerFreq = 5;
kSamplingFreq = 100.0;  % Must be >= kMaxFreq / pi (Hz)

kAmplitude = 1.0;

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
kAngFreqs = logspace(log10(kMinFreq), log10(kMaxFreq), kNumFreqs);  % (rad/s)

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

% - - Standard transfer function of a second-order system
sysBeingIdentified = naturalFreq^2 / (s^2 + 2 * dampingRatio * naturalFreq ...
   * s + naturalFreq^2);

outputSignal = lsim(sysBeingIdentified, kInputSignal, time)';

% - Plots
% - - Input and output signals
figure; plot(time, kInputSignal, time, outputSignal)
xlabel('Time (s)'), ylabel('Signal')
legend('input', 'output', 'Location', 'best')
grid on, grid minor

% - - Bode
figure; bode(sysBeingIdentified)
hold on
bode(sysBeingIdentified, kAngFreqs)
hold off
legend('nominal', 'kAngFreqs', 'Location', 'best')
xlim([kMinFreq kMaxFreq])
grid on, grid minor

% C++ code
kFreqResponse = calcMagAndPhase(outputSignal, kAmplitude, kAngFreqs, ...
   kSamplesPerFreq, kSamplingPeriod, kCyclesToIgnorePerFreq);

if PRINT_TEST4
   fprintf('calcMagAndPhase test:\n')
   fprintf('- frequency (rad/s): ')
   for i = 1:kNumFreqs
      fprintf('% 9.4f ', kAngFreqs(i))
   end
   fprintf('\n- magnitude (dB)   : ')
   for i = 1:kNumFreqs
      fprintf('% 9.4f ', kFreqResponse.magnitude(i))
   end
   fprintf('\n- phase     (deg)  : ')
   for i = 1:kNumFreqs
      fprintf('% 9.4f ', kFreqResponse.phase(i))
   end
   fprintf('\n')
end
