function freqResponse = calcMagAndPhase(kOutputSignal, kAmplitude, ...
   kAngFreqs, kSamplesPerFreq, kSamplingPeriod, kCyclesToIgnorePerFreq)

   % TODO: Check the validity of kCyclesToIgnorePerFreq: It must be > 0

   kConst1 = 2 * pi / kSamplingPeriod;  % (rad/s)
   kConst2 = kConst1 * kCyclesToIgnorePerFreq;  % (rad/s)
   startSampleIndex = 1;  % Starting index for kOutputSignal
   kNumFreqs = length(kAngFreqs);
   freqResponse.magnitude = zeros(1, kNumFreqs);
   freqResponse.phase = zeros(1, kNumFreqs);
   for i = 1:kNumFreqs
      % Calculate the number of samples to 1) collect for one cycle and 2)
      % ignore
      kAngFreq = kAngFreqs(i);  % (rad/s)
      kSamplesForOneCycle = round(kConst1 / kAngFreq) + 1;
      kSamplesToIgnore = round(kConst2 / kAngFreq);
      % TODO: Before proceeding, ensure ...
      % 1. kSamplesToIgnore < kSamplesPerFreq(i) (to prevent the entire portion
      %    of kOutputSignal corresponding to kAngFreq from being ignored)
      % 2. sum(kSamplesPerFreq(1 : i - 1)) + kSamplesToIgnore
      %    + kSamplesForOneCycle - 1 < length(kOutputSignal) [to prevent
      %    kOutputSignal(...) from reporting an out-of-range error in the for
      %    loop below*]
      %
      % * This situation could be avoided by flooring instead of rounding in
      % the calculations of both kSamplesForOneCycle and kSamplesToIgnore
      % above, but doing that would decrease the accuracy of freqResponse.

      % Calculate the integrands of the first in-phase and quadrature Fourier-
      % series coefficients--i.e., b1 and a1, respectively
      inPhaseIntegrand1 = zeros(1, kSamplesForOneCycle);
      quadratureIntegrand1 = zeros(1, kSamplesForOneCycle);
      startSampleIndex = startSampleIndex + kSamplesToIgnore;
      for j = 1:kSamplesForOneCycle
         % (j - 1) * kSamplingPeriod == time (s)
         inPhaseIntegrand1(j) = kOutputSignal(startSampleIndex + j - 1) ...
            * sin(kAngFreq * (j - 1) * kSamplingPeriod);
         quadratureIntegrand1(j) = kOutputSignal(startSampleIndex + j - 1) ...
            * cos(kAngFreq * (j - 1) * kSamplingPeriod);
      end
      
      % Calculate b1 and a1
      time = (0 : kSamplesForOneCycle - 1) * kSamplingPeriod;
      kInPhaseCoeff1 = kAngFreq / pi * trapz(time, inPhaseIntegrand1);
      kQuadratureCoeff1 = kAngFreq / pi * trapz(time, quadratureIntegrand1);
      
      % Calculate the frequency response (magnitude and phase)
      freqResponse.magnitude(i) = 20 * log10(sqrt(kInPhaseCoeff1^2 ...
         + kQuadratureCoeff1^2) / kAmplitude);
      freqResponse.phase(i) = atan2d(kQuadratureCoeff1, kInPhaseCoeff1);

      % Prepare for the next iteration
      startSampleIndex = sum(kSamplesPerFreq(1:i)) + 1;
   end

end
