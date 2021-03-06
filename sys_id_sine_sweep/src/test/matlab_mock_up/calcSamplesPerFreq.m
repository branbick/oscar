function samplesPerFreq = calcSamplesPerFreq(kAngFreqs, kCyclesPerFreq, ...
   kSamplingFreq)

   % TODO: Check for invalid parameters based on the following requirements:
   % 1. kSamplingFreq >= 2 * kAngFreqs(end) (Nyquist-Shannon sampling theorem)
   % 2. kCyclesPerFreq > 1

   % In words, the number of samples for kAngFreqs(i) equals the floor of the
   % following quantity, plus one: the period corresponding to kAngFreqs(i) [2
   % * gkPi / kAngFreqs(i)], divided by the sampling period (1
   % / kSamplingFreq), multiplied by the number of cycles--where the latter two
   % terms are constant across kAngFreqs.
   kNumFreqs = length(kAngFreqs);
   samplesPerFreq = zeros(1, kNumFreqs);
   kConst = 2 * pi * kSamplingFreq * kCyclesPerFreq;  % (rad/s)
   for i = 1:kNumFreqs
      samplesPerFreq(i) = floor(kConst / kAngFreqs(i)) + 1;
   end

end
