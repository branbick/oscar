function freqResponse = calcMagAndPhase(kOutputSignal, kAmplitude, ...
   kAngFreqs, kSamplesPerFreq, kSamplingPeriod, kCyclesPerFreq, ...
   kCyclesToIgnorePerFreq)

   kNumFreqs = length(kAngFreqs);
   samplesToIgnorePerFreq = NaN(1, kNumFreqs);
   kConst = 2 * pi / kSamplingPeriod * kCyclesToIgnorePerFreq;  % (rad/s)
   for i = 1:kNumFreqs
       samplesToIgnorePerFreq(i) = floor(kConst / kAngFreqs(i) + 1);
   end

   dynamicSampleSize = 0;
   freqResponse.magnitude = NaN(1, kNumFreqs);
   freqResponse.phase = NaN(1, kNumFreqs);
   kRadToDeg = 180 / pi;  % (deg/rad)
   for i = 1:kNumFreqs
      dynamicSampleSize = dynamicSampleSize + kSamplesPerFreq(i);
      kStartSampleIndex = dynamicSampleSize - kSamplesPerFreq(i) ...
         + samplesToIgnorePerFreq(i) + 1;
      kAngFreq = kAngFreqs(i);  % (rad/s)
      inPhaseIntegrand1 = NaN(1, dynamicSampleSize - kStartSampleIndex + 1);
      quadratureIntegrand1 = NaN(1, dynamicSampleSize - kStartSampleIndex + 1);
      for j = kStartSampleIndex:dynamicSampleSize
         inPhaseIntegrand1(j - kStartSampleIndex + 1) = kOutputSignal(j) ...
            * sin(kAngFreq * j * kSamplingPeriod);
         quadratureIntegrand1(j - kStartSampleIndex + 1) = kOutputSignal(j) ...
            * cos(kAngFreq * j * kSamplingPeriod);
      end
      
      % START MATLAB-specific code
      timePoints = (0 : length(inPhaseIntegrand1) - 1) * kSamplingPeriod;
      % END MATLAB-specific code
      kInPhaseCoeff1 = 2 / ((kCyclesPerFreq - kCyclesToIgnorePerFreq) ...
         * kSamplingPeriod) * trapz(timePoints, inPhaseIntegrand1);
      kQuadratureCoeff1 = 2 / ((kCyclesPerFreq - kCyclesToIgnorePerFreq) ...
         * kSamplingPeriod) * trapz(timePoints, quadratureIntegrand1);
      
      freqResponse.magnitude(i) = 20 * log10(sqrt(kInPhaseCoeff1^2 ...
         + kQuadratureCoeff1^2) / kAmplitude);
      freqResponse.phase(i) = kRadToDeg * atan2(kQuadratureCoeff1, ...
         kInPhaseCoeff1);
   end

end
