clear; clc; close all;

% --- 1. Pilih dan muat file data ---
[filename, pathname] = uigetfile('*.csv', 'Pilih file data CSV');
if isequal(filename, 0)
    disp('User batal memilih file');
    return
else
    fullpath = fullfile(pathname, filename);
    disp(['File yang dipilih: ', fullpath]);
end

opts = detectImportOptions(fullpath);
opts.VariableNamesLine = 0;
data = readtable(fullpath, opts);

% --- 2. Ekstraksi data ---
time = data.Var1 / 1000;  % konversi ms ke detik
input = data.Var2;
output = data.Var3;
Ts = mean(diff(time));
data_id = iddata(output, input, Ts);

% --- 3. Estimasi transfer function dari data ---
sys_est = tfest(data_id, 2);
disp('Transfer Function Estimasi:');
disp(sys_est);

% --- 4. Buat model referensi orde-2 ---
wn = 2 * pi;  % frekuensi natural
zeta_underdamped = 0.3;
zeta_critically = 1.0;
zeta_overdamped = 2.0;

G_under = tf(wn^2, [1, 2*zeta_underdamped*wn, wn^2]);
G_crit = tf(wn^2, [1, 2*zeta_critically*wn, wn^2]);
G_over = tf(wn^2, [1, 2*zeta_overdamped*wn, wn^2]);

% --- 5. Buat time vector dan simulasi step ---
t = 0:0.01:5;
[y_under, ~] = step(G_under, t);
[y_crit, ~] = step(G_crit, t);
[y_over, ~] = step(G_over, t);
[y_est, ~] = step(sys_est, t);

% --- 6. Plot semua hasil step response ---
figure;
plot(t, y_under, 'b-', 'LineWidth', 2); hold on;
plot(t, y_crit, 'r--', 'LineWidth', 2);
plot(t, y_over, 'g-.', 'LineWidth', 2);
plot(t, y_est, 'k:', 'LineWidth', 2);  % model estimasi dari CSV

grid on;
title('Second-Order Ball & Beam Model');
xlabel('Time (s)');
ylabel('Output');

legend('Underdamped (\zeta = 0.3)', ...
       'Critically Damped (\zeta = 1.0)', ...
       'Overdamped (\zeta = 2.0)', ...
       'Hasil Estimasi Data BNB', ...
       'Location', 'SouthEast');

% --- 7. Tampilkan parameter Step Response dari model estimasi ---
stepInfo = stepinfo(sys_est);
fprintf('\n--- Informasi Step Response dari Model Estimasi ---\n');
fprintf('Rise Time (Tr)     : %.4f s\n', stepInfo.RiseTime);
fprintf('Peak Time (Tp)     : %.4f s\n', stepInfo.PeakTime);
fprintf('Overshoot (%%OS)    : %.2f %%\n', stepInfo.Overshoot);
fprintf('Settling Time (Ts) : %.4f s\n', stepInfo.SettlingTime);

% --- 8. Hitung Steady-State Error (ess) dan Persentasenya ---
steady_state_value = y_est(end);  % nilai akhir output model estimasi
ref = 1;  % nilai referensi untuk step input
ess = abs(ref - steady_state_value);  % error steady state
ess_percent = (ess / ref) * 100;

fprintf('Steady-State Value       : %.4f\n', steady_state_value);
fprintf('Steady-State Error (ess) : %.4f\n', ess);
fprintf('Persentase Error (ess)   : %.2f %%\n', ess_percent);

