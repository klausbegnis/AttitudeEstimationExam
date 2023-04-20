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

As two solutions are found for each angle, and both with same module but with opposite signals. To find the correct quadrant of the correct response, the function $ATAN2(G_i,G_j)% is used, where for $\theta$ $i = x$ and $j = z$, while $i = y$ and $j = z$ for $\phi$.