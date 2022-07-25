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
      kAngFreq = kAngFreqs(i);  % (rad/s)
      kSamplesForOneCycle = round(kConst1 / kAngFreq) + 1;

      % Calculate the number of samples to ignore, which correspond to the
      % transient response of kOutputSignal
      kSamplesToIgnore = round(kConst2 / kAngFreq);
      % TODO: Ensure that kSamplesToIgnore < kSamplesPerFreq(i) before
      % proceeding

      % Calculate the integrands of the first in-phase and quadrature Fourier-
      % series coefficients--i.e., b1 and a1, respectively
      inPhaseIntegrand1 = zeros(1, kSamplesForOneCycle);
      quadratureIntegrand1 = zeros(1, kSamplesForOneCycle);
      startSampleIndex = startSampleIndex + kSamplesToIgnore;
      for j = 1:kSamplesForOneCycle
         % (j - 1) * kSamplingPeriod == time (s)
         inPhaseIntegrand1(j) = kOutputSignal(j - 1 + startSampleIndex) ...
            * sin(kAngFreq * (j - 1) * kSamplingPeriod);
         quadratureIntegrand1(j) = kOutputSignal(j - 1 + startSampleIndex) ...
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
