load logs.dat

T = logs(:, 1);

p1 = logs(:, 3);
unp1 = logs(:, 4);
p2 = logs(:, 6);
unp2 = logs(:, 7);
p3 = logs(:, 9);
unp3 = logs(:, 10);
p4 = logs(:, 12);
unp4 = logs(:, 13);
p5 = logs(:, 15);
unp5 = logs(:, 16);
p6 = logs(:, 18);
unp6 = logs(:, 19);
p7 = logs(:, 21);
unp7 = logs(:, 22);
p8 = logs(:, 24);
unp8 = logs(:, 25);
p9 = logs(:, 27);
unp9 = logs(:, 28);
p10 = logs(:, 30);
unp10 = logs(:, 31);
% p11 = logs(:, 33);
% unp12 = logs(:, 34);
% p13 = logs(:, 36);
% unp13 = logs(:, 37);
% p14 = logs(:, 39);
% unp14 = logs(:, 40);

%plot(T, p1, T, unp1, T, p2, T, unp2, T, p3, T, unp3,T, p4, T, unp4,T, p5, T, unp5,T, p6, T, unp6,T, p7, T, unp7,T, p8, T, unp8,T, p9, T, unp9,T, p10, T, unp10,T, p11, T, unp11,T, p12, T, unp12,T, p13, T, unp13 );
plot(T, p1, 'c-', T, unp1, 'c:', T, p2, 'm-', T, unp2, 'm:', T, p3, 'y-', T, unp3, 'y:', T, p4, 'k-', T, unp4, 'k:', T, p5, 'r-', T, unp5, 'r:', T, p6, 'g-', T, unp6, 'g:',T, p7, 'b-', T, unp7, 'b:',T, p8, 'w-', T, unp8, 'w:',T, p9, '-', T, unp9, ':',T, p10, '-', T, unp10, ':');
%plot(T, p1, 'c-', T, unp1, 'c:', T, p2, 'm-', T, unp2, 'm:', T, p3, 'y-', T, unp3, 'y:', T, p4, 'k-', T, unp4, 'k:', T, p5, 'r-', T, unp5, 'r:');


