function freqResponse = calcMagAndPhase(kOutputSignal, kAmplitude, ...
   kAngFreqs, kSamplesPerFreq, kSamplingPeriod, kCyclesToIgnorePerFreq)

   % TODO: Clean up comments

   kConst1 = 2 * pi / kSamplingPeriod;  % (rad/s)
   kConst2 = kConst1 * kCyclesToIgnorePerFreq;  % (rad/s)
   dynamicSampleSize = 0;
   kNumFreqs = length(kAngFreqs);
   freqResponse.magnitude = zeros(1, kNumFreqs);
   freqResponse.phase = zeros(1, kNumFreqs);
   kRadToDeg = 180 / pi;  % (deg/rad)
   for i = 1:kNumFreqs
      % Calculate the number of samples to collect for one cycle
      samplesForOneCycle = floor(kConst1 / kAngFreqs(i) + 1);

      % Calculate the number of samples to ignore, which correspond to the
      % transient response of kOutputSignal
      samplesToIgnore = floor(kConst2 / kAngFreqs(i) + 1);
      % TODO: Ensure that samplesToIgnore < kSamplesPerFreq(i) before
      % proceeding

      % Calculate the integrands of the first in-phase and quadrature Fourier-
      % series coefficients--i.e., b1 and a1, respectively
      dynamicSampleSize = dynamicSampleSize + kSamplesPerFreq(i);
      kStartSampleIndex = dynamicSampleSize - kSamplesPerFreq(i) ...
         + samplesToIgnore + 1;
      kAngFreq = kAngFreqs(i);  % (rad/s)
      inPhaseIntegrand1 = zeros(1, samplesForOneCycle);
      quadratureIntegrand1 = zeros(1, samplesForOneCycle);
      for j = kStartSampleIndex : kStartSampleIndex + samplesForOneCycle - 1
         % (j - 1) * kSamplingPeriod == time (s)
         inPhaseIntegrand1(j - kStartSampleIndex + 1) = kOutputSignal(j) ...
            * sin(kAngFreq * (j - 1) * kSamplingPeriod);
         quadratureIntegrand1(j - kStartSampleIndex + 1) = kOutputSignal(j) ...
            * cos(kAngFreq * (j - 1) * kSamplingPeriod);
      end
      
      % Calculate b1 and a1
      % - Start of MATLAB-specific code
      time = (0 : samplesForOneCycle - 1) * kSamplingPeriod;
      % - End of MATLAB-specific code
      kInPhaseCoeff1 = kAngFreq / pi * trapz(time, inPhaseIntegrand1);
      kQuadratureCoeff1 = kAngFreq / pi * trapz(time, quadratureIntegrand1);
      
      % Calculate the frequency response: magnitude and phase
      freqResponse.magnitude(i) = 20 * log10(sqrt(kInPhaseCoeff1^2 ...
         + kQuadratureCoeff1^2) / kAmplitude);
      freqResponse.phase(i) = kRadToDeg * atan2(kQuadratureCoeff1, ...
         kInPhaseCoeff1);
   end

end
