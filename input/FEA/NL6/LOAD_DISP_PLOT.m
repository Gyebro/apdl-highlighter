close all
clear all
clc

set(0, 'defaultAxesFontSize', 12)
set(0, 'defaultTextInterpreter','latex');
set(0, 'defaultAxesTickLabelInterpreter','latex');
set(0, 'defaultLegendInterpreter','latex');
set(0, 'defaultLineLineWidth',1.5)
set(0, 'defaultLineMarkerSize',8)

load('AZD.OUT')
load('BZD.OUT')
load('CXD.OUT')

figure
hold on
plot(1e3*AZD(:,2),1e-3*AZD(:,1))
plot(1e3*BZD(:,2),1e-3*BZD(:,1))
plot(1e3*abs(CXD(:,2)),1e-3*CXD(:,1))
xlabel('Displacement [mm]')
ylabel('Load [kN]')
legend('$u_{z\mathrm{A}}$','$u_{z\mathrm{B}}$','$\left|u_{x\mathrm{C}}\right|$','Location','southeast')
grid on

fig = gcf;
set(fig,'PaperUnits','centimeters');
set(fig,'PaperPositionMode','manual');
set(fig,'PaperSize',[12,8]);
set(fig,'PaperPosition',[0,0,12,8]);
set(fig,'renderer','painters');
name = 'LOAD_DISP.png';
print(name,'-dpng')