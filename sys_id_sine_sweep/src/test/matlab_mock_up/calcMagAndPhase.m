function freqResponse = calcMagAndPhase(kOutputSignal, kAmplitude, ...
   kAngFreqs, kSamplesPerFreq, kSamplingPeriod, kCyclesPerFreq, ...
   kCyclesToIgnorePerFreq)

   % TODO: Clean up comments

   % Calculate the number of samples to ignore--which correspond to the
   % transient response of kOutputSignal--for each element of kAngFreqs
   kNumFreqs = length(kAngFreqs);
   samplesToIgnorePerFreq = zeros(1, kNumFreqs);
   kConst = 2 * pi / kSamplingPeriod * kCyclesToIgnorePerFreq;  % (rad/s)
   for i = 1:kNumFreqs
      samplesToIgnorePerFreq(i) = floor(kConst / kAngFreqs(i) + 1);
   end
      % TODO: Ensure that samplesToIgnorePerFreq(i) < kSamplesPerFreq(i)
      % before proceeding

   % Calculate the frequency response: magnitude and phase
   dynamicSampleSize = 0;
   freqResponse.magnitude = zeros(1, kNumFreqs);
   freqResponse.phase = zeros(1, kNumFreqs);
   kRadToDeg = 180 / pi;  % (deg/rad)
   for i = 1:kNumFreqs
      % Calculate the integrands of the first in-phase and quadrature Fourier-
      % series coefficients--i.e., b1 and a1, respectively
      dynamicSampleSize = dynamicSampleSize + kSamplesPerFreq(i);
      kStartSampleIndex = dynamicSampleSize - kSamplesPerFreq(i) ...
         + samplesToIgnorePerFreq(i) + 1;
      kAngFreq = kAngFreqs(i);  % (rad/s)
      inPhaseIntegrand1 = zeros(1, dynamicSampleSize - kStartSampleIndex + 1);
      quadratureIntegrand1 = zeros(1, dynamicSampleSize - kStartSampleIndex ...
         + 1);
      for j = kStartSampleIndex:dynamicSampleSize
         % (j - 1) * kSamplingPeriod == time (s)
         inPhaseIntegrand1(j - kStartSampleIndex + 1) = kOutputSignal(j) ...
            * sin(kAngFreq * (j - 1) * kSamplingPeriod);
         quadratureIntegrand1(j - kStartSampleIndex + 1) = kOutputSignal(j) ...
            * cos(kAngFreq * (j - 1) * kSamplingPeriod);
      end
      
      % Calculate b1 and a1
      % - Start of MATLAB-specific code
      timePoints = (0 : length(inPhaseIntegrand1) - 1) * kSamplingPeriod;
      % - End of MATLAB-specific code
      kInPhaseCoeff1 = 2 / ((kCyclesPerFreq - kCyclesToIgnorePerFreq) ...
         * kSamplingPeriod) * trapz(timePoints, inPhaseIntegrand1);
      kQuadratureCoeff1 = 2 / ((kCyclesPerFreq - kCyclesToIgnorePerFreq) ...
         * kSamplingPeriod) * trapz(timePoints, quadratureIntegrand1);
      
      % Calculate the magnitude and phase
      freqResponse.magnitude(i) = 20 * log10(sqrt(kInPhaseCoeff1^2 ...
         + kQuadratureCoeff1^2) / kAmplitude);
      freqResponse.phase(i) = kRadToDeg * atan2(kQuadratureCoeff1, ...
         kInPhaseCoeff1);
   end

end
