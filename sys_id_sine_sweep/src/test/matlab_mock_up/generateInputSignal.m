function inputSignal = generateInputSignal(kAmplitude, kAngFreqs, ...
   kSamplesPerFreq, kSamplingPeriod)

   inputSignal = zeros(1, sum(kSamplesPerFreq));
   
   % Calculate the elemental values of inputSignal by progressing through
   % kAngFreqs, advancing to kAngFreqs(freqIndex + 1) once the number of
   % samples collected for kAngFreqs(freqIndex)--i.e., numSamples--equals
   % kSamplesPerFreq(freqIndex). Note that time also corresponds to
   % kAngFreqs(freqIndex), and resetting it to 0.0 once the aforementioned
   % equivalence is reached ensures each "new" sinusoid starts with a range
   % value of 0.0.
   freqIndex = 1;
   time = 0.0;  % (s)
   numSamples = 0;
   for i = 1:length(inputSignal)
      inputSignal(i) = kAmplitude * sin(kAngFreqs(freqIndex) * time);
      time = time + kSamplingPeriod;
      numSamples = numSamples + 1;
      if numSamples == kSamplesPerFreq(freqIndex)
         freqIndex = freqIndex + 1;
         time = 0.0;
         numSamples = 0;
      end
   end

end
