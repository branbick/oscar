wc = 100;  % (rad/s)
wb = 20;  % (rad/s)
d = 50;  % (dB)

a = (wb + sqrt(wb^2 + 4*wc^2)) / (2*wc);
zetaDen = sqrt((2 - a^2 - 1/a^2) / (4 * (2*10^(-d/10) - 1)))
zetaNum = 10^(-d/20) * zetaDen

s = tf('s');
HbnC = (s^2 + 2*zetaNum*wc*s + wc^2) / (s^2 + 2*zetaDen*wc*s + wc^2)

T = 1/1000;  % (s)
opts = c2dOptions('Method', 'tustin', 'PrewarpFrequency', wc);
HbnDc2d = c2d(HbnC, T, opts)

beta0 = 1 + zetaNum*sin(wc*T);
beta1 = -2*cos(wc*T);
beta2 = 1 - zetaNum*sin(wc*T);
alpha0 = 1 + zetaDen*sin(wc*T);
alpha1 = beta1;
alpha2 = 1 - zetaDen*sin(wc*T);
b0 = beta0/alpha0
b1 = beta1/alpha0
b2 = beta2/alpha0
a1 = b1
a2 = alpha2/alpha0
z = tf('z', T);
HbnD = (b0 + b1*z^(-1) + b2*z^(-2)) / (1 + a1*z^(-1) + a2*z^(-2))

bode(HbnC)
hold on
bode(HbnDc2d)
bode(HbnD)
legend('HbnC', 'HbnDc2d', 'HbnD')