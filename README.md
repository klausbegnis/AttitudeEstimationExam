# Attitude Estimation Exame - Hexagon

by Klaus Edward Becker Begnis

## Task development

The task consisted of developing an algorithm that reads values from a log file with data aquired from a accelerometer.

The accelerometer in question is the [MMA8451Q](https://www.nxp.com/docs/en/data-sheet/MMA8451Q.pdf). When the data were aquired, the accelerometer was setted in the 8*g* configuration, this configuration reflects directyly into the sensibility factor from the reading performed, as Table 1 shows.

#### Table 1		
| Configuration | Sensitivity       |
|---------------|-------------------|
| 2*g*          | 4098 [*counts/g*] |
| 4*g*          | 2048 [*counts/g*] |
| 8*g*          | 1024 [*counts/g*] |

### Calulating pitch and roll

The values obtained reflected acceleration values in a 3-axis system. To convert this values and discover the roll and pitch values in degrees, the subsequent [article](http://www.nxp.com/docs/en/application-note/AN3461.pdf) was followed.

Summarizing the calculations needed to process these information are presented at Equation 1-4, and each variable is presented at Table 2.

### Table 2
| Variable		| Unit       |
|---------------|-------------------|
| $\theta$      | *deg*				|
| $\phi$        | *deg*	 |
| $G_x$         | *g*	 |
| $G_y$         | *g*	 |
| $G_z$         | *g*	 |


### Equation 1

$\phi = tan^{-1}(\frac{G_y}{G_z})$

### Equation 2

$\phi = tan^{-1}\frac{G_y}{\sqrt{G_x^2 + G_z^2}}$

### Equation 3

$\theta = tan^{-1}\frac{-G_x}{\sqrt{G_y^2 + G_z^2}}$

### Equation 4

$\theta = tan^{-1}(\frac{-G_x}{G_z})$

As two solutions are found for each angle, and both have the same module but with opposite signals, to find the correct quadrant of the correct answer, the function $ATAN2(G_i,G_j)% is used. Where for $\theta$ $i = x$ and $j = z$, while $i = y$ and $j = z$ for $\phi$.

### Code overview

The code was developed using object orientation paradigm, which enriched the possibilitie to replicate code and readability.

One class was designed for the log reading/writing, `LogReader`, and it's attributes and methods are shown in Table 3 and Table 4, respectively.

### Table 3

| Attribute   | Type   | Visibility | Description                                |
|-------------|--------|------------|--------------------------------------------|
| `separator` | char   | private    | Character to separate values at log files. |
| `path`      | string | private    | Path to input log file.                    |
| `outputPath`    | string | private    | Path to output log file.                   |

### Table 4

| Method             | Type                 | Visibility | Description                                                                                           |
|--------------------|----------------------|------------|-------------------------------------------------------------------------------------------------------|
| `setPath`          | void                 | public     | Sets attribute `path`.                                                                                |
| `setOutputPath`    | void                 | public     | Sets attribute `outputPath`.                                                                          |
| `setSeparator`     | void                 | public     | Sets attribute `separator`.                                                                           |
| `translateLine`    | accelerometerReading | public     | Translates timestamp, $G_x$, $G_y$ and $G_z$ values.                                                  |
| `readAndTranslate` | void                 | public     | Classe's main method, start the process from reading and writing.                                     |
| `openFile`         | ifstream             | private    | Opens input log file.                                                                                 |
| `autoPath`         | string               | private    | Returns default name for output file. It's called only when output path not specfied.                 |
| `outputLineValue`  | string               | private    | String containing the line to be added at the output file. In the format: timestamp; $\theta$; $\phi$ |



