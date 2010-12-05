clear;
load logs.dat

hold on;

T = logs(:, 1);
timeSize = size(T);
i = 1;
plotHandles = [];
legends = {};
%markers = {'+k';'ok';'*k';'xk';'sk';'dk';'vk';'^k';'>k';'<k';'pk';'hk'};
%markers = {'<k';'>k';'^k';'vk';'dk';'sk';'*k';'ok';'+k'};
markers = {'<';'>';'^';'v';'d';'s';'*';'o'};
grey = [0.5 0.5 0.5];
pink = [0.5 0 0];
markerFaceCol = {'y';'m';'c';pink;'g';'b';'w';grey};

%Plot predator population
plot(T, logs(:, 2), '-k');
p0 = plot(T(1:35:timeSize), logs(1:35:timeSize, 2), 'h', 'MarkerEdgeColor','k','MarkerFaceColor','r');

%Plot all the 8 rings of prey population both palatable and unpalatable.
for c = 3:3:size(logs, 2)
    p1 = plot(T, logs(:, c + 1), '-k');
    p2 = plot(T(1:50:timeSize), logs(1:50:timeSize, c + 1), markers{i},'MarkerEdgeColor','k','MarkerFaceColor',markerFaceCol{i});
%    p2 = plot(T(1:c*3:timeSize), logs(1:c*3:timeSize, c + 1), markers{i});
    p3 = plot(T, logs(:, c + 2), ':k');
    plot(T(1:75:timeSize), logs(1:75:timeSize, c + 2), markers{i},'MarkerEdgeColor','k','MarkerFaceColor',markerFaceCol{i});
%    plot(T(1:c*7:timeSize), logs(1:c*7:timeSize, c + 2), markers{i});
    plotHandles = [plotHandles p2];
    legends = [legends; strcat( 'CA Rule: ', num2str(logs(1, c)))];
    i = i + 1;
    if (i > 12) 
        i = 1;
    end
end

hold off;

plotHandles = [plotHandles p1 p3 p0];
legends = [legends;'Palatable';'Unpalatable';'Predator'];

legend(plotHandles, legends,'fontsize',14);
set(gca, 'fontsize',14);

xlabel('Simulation Time','fontsize',14);
ylabel('Population','fontsize',14);
title('Mimicry Ring population','fontsize',16);
