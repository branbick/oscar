function freqResponse = calcMagAndPhase(kOutputSignal, kAmplitude, ...
   kAngFreqs, kSamplesPerFreq, kSamplingPeriod, kCyclesToIgnorePerFreq)

   % TODO: Clean up comments

   kConst1 = 2 * pi / kSamplingPeriod;  % (rad/s)
   kConst2 = kConst1 * kCyclesToIgnorePerFreq;  % (rad/s)
   startSampleIndex = 1;
   kNumFreqs = length(kAngFreqs);
   freqResponse.magnitude = zeros(1, kNumFreqs);
   freqResponse.phase = zeros(1, kNumFreqs);
   for i = 1:kNumFreqs
      % Calculate the number of samples to collect for one cycle
      kSamplesForOneCycle = round(kConst1 / kAngFreqs(i)) + 1;

      % Calculate the number of samples to ignore, which correspond to the
      % transient response of kOutputSignal
      kSamplesToIgnore = round(kConst2 / kAngFreqs(i));
      % TODO: Ensure that samplesToIgnore < kSamplesPerFreq(i) before
      % proceeding

      % Calculate the integrands of the first in-phase and quadrature Fourier-
      % series coefficients--i.e., b1 and a1, respectively
      startSampleIndex = startSampleIndex + kSamplesToIgnore;
      kAngFreq = kAngFreqs(i);  % (rad/s)
      inPhaseIntegrand1 = zeros(1, kSamplesForOneCycle);
      quadratureIntegrand1 = zeros(1, kSamplesForOneCycle);
      for j = startSampleIndex : startSampleIndex + kSamplesForOneCycle - 1
         % (j - 1) * kSamplingPeriod == time (s)
         inPhaseIntegrand1(j - startSampleIndex + 1) = kOutputSignal(j) ...
            * sin(kAngFreq * (j - 1) * kSamplingPeriod);
         quadratureIntegrand1(j - startSampleIndex + 1) = kOutputSignal(j) ...
            * cos(kAngFreq * (j - 1) * kSamplingPeriod);
      end
      
      % Calculate b1 and a1
      % - Start of MATLAB-specific code
      time = (0 : kSamplesForOneCycle - 1) * kSamplingPeriod;
      % - End of MATLAB-specific code
      kInPhaseCoeff1 = kAngFreq / pi * trapz(time, inPhaseIntegrand1);
      kQuadratureCoeff1 = kAngFreq / pi * trapz(time, quadratureIntegrand1);
      
      % Calculate the frequency response: magnitude and phase
      freqResponse.magnitude(i) = 20 * log10(sqrt(kInPhaseCoeff1^2 ...
         + kQuadratureCoeff1^2) / kAmplitude);
      freqResponse.phase(i) = atan2d(kQuadratureCoeff1, kInPhaseCoeff1);

      % Advance startSampleIndex to prepare for the next iteration
      startSampleIndex = sum(kSamplesPerFreq(1:i)) + 1;
   end

end
