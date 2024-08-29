# Linux System Monitoring Tool

A Linux system monitoring tool written in C++ that provides detailed information about the system, including statistical analysis and console-based plots of various OS properties (RAM, processes, etc.).

## Features

- System information display
- Statistical analysis
- Console plots for multiple Linux OS properties

## Monitored Files

The application gathers data from the following system files:

- `/etc/issue`
- `/proc/version`
- `/proc/diskstats`
- `/proc/meminfo`
- `/proc/cpuinfo`
- `/proc/stat`
- `/proc/vmstat`
- `/proc/uptime`

## Technologies Used

- **C++**
- **Boost Library**

