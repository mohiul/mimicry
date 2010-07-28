clear;
load rings.dat

T = rings(:, 1);
R = rings(:, 2);
%Plot ring size
plot(T, R, '-k');
axis([0 T(size(T, 1)) 0 R(size(R, 1))+2 ]);
xlabel('Simulation Time');
ylabel('Number of Rings');
title('Mimicry number of ring variation');
