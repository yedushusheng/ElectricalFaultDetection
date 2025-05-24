# Embedded Fault Detection Software for Porcelain Insulator Based on Qt

This is an embedded software application developed using Qt for fault detection in porcelain insulators.

## Basic Features

1. **Online Analysis of `.wav` Files**  
   The software supports real-time analysis of audio data collected from `.wav` files.

2. **Frequency Spectrum Distribution & Fault Detection**  
   It generates the frequency spectrum distribution and determines whether the insulator is damaged based on frequency characteristics.

## Features to be Improved

1. **Manual Damage Assessment**  
   Currently, the determination of whether the insulator is damaged and the severity of the damage relies on human judgment.

2. **FFT Algorithm Performance**  
   The current FFT algorithm is relatively slow. It is recommended to use an improved split-radix FFT for better performance.

3. **UI and Data Security Enhancements**  
   The user interface needs further improvements, particularly in data encryption and overall functionality.
