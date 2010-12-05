clear;
load rings.dat

T = rings(:, 1);
R = rings(:, 2);
%Plot ring size
plot(T, R, '-k');
%axis([0 T(size(T, 1)) -1 25 ]);
axis([0 T(size(T, 1)) 0 R(size(R, 1))+2 ]);
xlabel('Simulation Time','fontsize',14);
ylabel('Number of Rings','fontsize',14);
title('Mimicry number of ring variation','fontsize',14);
