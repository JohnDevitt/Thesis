library(sm)
library(Cairo)
attach(mtcars)

#png(file="fig-density.png", width=640,height=480) ;
#bitmap(file="fig-density.bmp", width=640,height=480);
Cairo(file="fig-density.png", width=640,height=480,type="png",bg="white") ;

data1d <- read.table("data1.txt", header=FALSE)
data1dv <- as.numeric(as.vector(as.matrix(data1d)));

x1=min(data1dv);
x2=max(data1dv);

data2d <- read.table("data2.txt", header=FALSE)
data2dv <- as.numeric(as.vector(as.matrix(data2d)));

y1=min(data2dv);
y2=max(data2dv);

z1=min(x1,y1);
z2=max(x2,y2);

z=(z2-z1)/10;
z1=z1-z;
z2=z2+z;

d1d <- density(data1dv, from=z1, to=z2)
d2d <- density(data2dv, from=z1, to=z2)

ylim1=c(0,4)

#Graphs
par(new=TRUE); plot(d1d, col=rgb(0,0,0), xlab="execution time (sec.)", ylab="density", main="", yaxt = "n", lwd=2)
par(new=TRUE); plot(d2d, col=rgb(0,0,0), xlab="", ylab="", main="", yaxt = "n", lwd=2, lty=2)
