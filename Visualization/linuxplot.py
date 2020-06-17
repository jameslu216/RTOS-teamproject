import pandas as pd
import matplotlib.pyplot as plt

os_list = ["Raspbian"] # Fill in your csv name without extension
measure_list= ["IL(ns)", "TCST(us)", "TPT(us)"]

df_list = [pd.read_csv(os_list[i]+'.csv') for i in range(len(os_list))]

fig, axes = plt.subplots(3, 1, figsize=(10,10))
color = dict(boxes='DarkGreen',whiskers='DarkOrange',medians='DarkBlue',caps='Gray')

for i in range(len(measure_list)):
	plot_df = pd.DataFrame()
	max_value = 0
	min_value = 2147483647
	for j in range(len(os_list)):
		plot_df[os_list[j]] = df_list[j][measure_list[i]]
		max_value = max(max_value, plot_df[os_list[j]].max())
		min_value = min(min_value, plot_df[os_list[j]].min())

	plot_df.plot.box(
		ylim = [min_value-2, max_value+2],
        grid = True,
        color = color,
        ax = axes[i],
        title = measure_list[i]
	)

plt.savefig('comparison_result_linux.png')
