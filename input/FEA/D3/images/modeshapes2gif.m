
filename = 'testAnimated.gif';
for n = 1:10
    % Capture the plot as an image
    [imind,cm] = rgb2ind(imread([num2str(n),'_eigenshape.png']),256);
    % Write to the GIF File
    if n == 1
        imwrite(imind,cm,filename,'gif', 'Loopcount',inf,'DelayTime',2);
    else
        imwrite(imind,cm,filename,'gif','WriteMode','append','DelayTime',2);
    end
end