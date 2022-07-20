function samplesPerFreq = calcSamplesPerFreq(kAngFreqs, kCyclesPerFreq, kSamplingFreq)
   kNumFreqs = length(kAngFreqs);
   samplesPerFreq = NaN(1, kNumFreqs);
   kConst = 2 * pi * kSamplingFreq * kCyclesPerFreq;  % (rad/s)
   for i = 1:kNumFreqs
     samplesPerFreq(i) = floor(kConst / kAngFreqs(i) + 1);
   end
end
