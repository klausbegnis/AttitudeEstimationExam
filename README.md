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



#### Table 2

"""{=latex}
\begin{table}[]
\begin{tabular}{ll}
\hline
\multicolumn{1}{|l|}{Variable} & \multicolumn{1}{l|}{Unit}  \\ \hline
\multicolumn{1}{|l|}{$\theta$} & \multicolumn{1}{l|}{*deg*} \\ \hline
\multicolumn{1}{|l|}{$\phi$}   & *deg*                      \\ \hline
\multicolumn{1}{l|}{$G_x$}     & \multicolumn{1}{l|}{*g*}   \\ \cline{2-2} 
$G_y$                          & *g*                        \\
$G_z$                          & *g*                       
\end{tabular}
\end{table}
"""