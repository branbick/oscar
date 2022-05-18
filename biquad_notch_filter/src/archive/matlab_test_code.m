%% Single-sided amplitude spectrums (https://www.mathworks.com/help/matlab/ref/fft.html)

% wave1
wave1 = [];  % TODO: Copy and paste elements from tester.cpp output
L = length(wave1);
w = 2*pi*(1/0.001)*(0:(L/2))/L;  % TODO: Update sampling frequency as necessary
Y1 = fft(wave1);
P21 = abs(Y1/L);
P11 = P21(1:L/2+1);
P11(2:end-1) = 2*P11(2:end-1);

plot(w,P11)
title('Single-sided amplitude spectrum of wave1(t)')
xlabel('w (rad/s)')
ylabel('|P11(w)|')

% wave2
wave2 = [];  % TODO: Copy and paste elements from tester.cpp output
Y2 = fft(wave2);
P22 = abs(Y2/L);
P12 = P22(1:L/2+1);
P12(2:end-1) = 2*P12(2:end-1);

figure
plot(w,P12)
title('Single-sided amplitude spectrum of wave2(t)')
xlabel('w (rad/s)')
ylabel('|P12(w)|')

% waveSuper
waveSuper = [];  % TODO: Copy and paste elements from tester.cpp output
YS = fft(waveSuper);
P2S = abs(YS/L);
P1S = P2S(1:L/2+1);
P1S(2:end-1) = 2*P1S(2:end-1);

figure
plot(w,P1S)
title('Single-sided amplitude spectrum of waveSuper(t)')
xlabel('w (rad/s)')
ylabel('|P1S(w)|')

% waveFilt
waveFilt = [];  % TODO: Copy and paste elements from tester.cpp output
YF = fft(waveFilt);
P2F = abs(YF/L);
P1F = P2F(1:L/2+1);
P1F(2:end-1) = 2*P1F(2:end-1);

figure
plot(w,P1F)
title('Single-sided amplitude spectrum of waveFilt(t)')
xlabel('w (rad/s)')
ylabel('|P1F(w)|')

%% Waves in time
figure
plot(wave1)
hold on
plot(wave2)
plot(waveSuper)
plot(waveFilt)
legend('1', '2', 'Super', 'Filt')