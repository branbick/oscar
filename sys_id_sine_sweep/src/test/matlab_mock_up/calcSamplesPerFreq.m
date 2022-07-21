function samplesPerFreq = calcSamplesPerFreq(kAngFreqs, kCyclesPerFreq, ...
   kSamplingFreq)

   % TODO: Compare kAngFreqs(end) and kSamplingFreq to check if the Nyquist-
   % Shannon sampling theorem is adhered to. Also, check kCyclesPerFreq: it
   % must be > 0.

   % In words, the number of samples for kAngFreqs(i) equals the floor of the
   % following quantity: the period corresponding to kAngFreqs(i) [2 * gkPi
   % / kAngFreqs(i)], divided by the sampling period (1 / kSamplingFreq),
   % multiplied by the number of cycles--where the latter two terms are
   % constant across kAngFreqs--plus one.
   kNumFreqs = length(kAngFreqs);
   samplesPerFreq = zeros(1, kNumFreqs);
   kConst = 2 * pi * kSamplingFreq * kCyclesPerFreq;  % (rad/s)
   for i = 1:kNumFreqs
      samplesPerFreq(i) = floor(kConst / kAngFreqs(i) + 1);
   end

end
