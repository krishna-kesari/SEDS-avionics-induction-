import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from scipy.signal import savgol_filter

# 1. Load data
data = pd.read_csv("Depth Data.csv")

# Convert depth column to numeric, handle errors like "#VALUE!"
data['Depth (m)'] = pd.to_numeric(data['Depth (m)'], errors='coerce')


# Interpolate missing values
data['Depth (m)'] = data['Depth (m)'].interpolate()

# Fill any remaining NaNs at edges
data['Depth (m)'] = data['Depth (m)'].bfill().ffill()

# If still empty, bail out gracefully
if data['Depth (m)'].dropna().empty:
    raise ValueError("No valid depth data available after cleaning.")

# Convert to NumPy array
depth_array = data['Depth (m)'].to_numpy(dtype=float)
time = data['Point'].to_numpy(dtype=int)

# 2. Smooth noisy sensor data
n = len(depth_array)
if n < 5:
    depth_smooth = depth_array  # too few points, skip smoothing
else:
    # Ensure window_length is odd and <= n
    window_len = min(11, n if n % 2 == 1 else n - 1)
    if window_len < 3:
        window_len = 3
    polyorder = min(2, window_len - 1)
    depth_smooth = savgol_filter(depth_array, window_length=window_len, polyorder=polyorder)

# 3. Plot setup
fig, ax = plt.subplots()
line, = ax.plot([], [], lw=2, color='navy')
ax.set_xlim(0, len(time))

# Protect against empty arrays
ymin = min(depth_smooth) - 20 if len(depth_smooth) > 0 else -10
ymax = max(depth_smooth) + 20 if len(depth_smooth) > 0 else 10
ax.set_ylim(ymin, ymax)

ax.set_xlabel("Time (s)")
ax.set_ylabel("Depth below sea surface (m)")
ax.set_title("Odysseus Ship Depth Monitoring")

# Optional: add a threshold line
ax.axhline(y=50, color='red', linestyle='--', label="Danger Threshold")
ax.legend()

# 4. Animation function
def update(frame):
    line.set_data(time[:frame], depth_smooth[:frame])
    return line,

ani = FuncAnimation(fig, update, frames=len(time), interval=1000, blit=True)
plt.show()
