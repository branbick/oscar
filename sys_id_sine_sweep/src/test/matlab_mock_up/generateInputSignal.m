function inputSignal = generateInputSignal(kAmplitude, kAngFreqs, ...
   kSamplesPerFreq, kSamplingPeriod)

   inputSignal = NaN(1, sum(kSamplesPerFreq));
   
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
