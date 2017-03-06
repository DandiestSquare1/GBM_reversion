rc <- function(x) {
  if (nrow(x)>0 || ncol(x)>0) {
    rows <- nrow(x)
    columns <- ncol(x)
    result <- paste('rows:',rows,'columns:', columns,sep = " ")
  }
  else {
    result <- paste('rows:',NULL,'columns:', NULL,sep = ",")
  }
  return(result)
  dfName <- names(x)
  return(dfName)
}

homedir<-getSrcDirectory(function(x) {x})
# Setting working directory. need to be adjusted to server conditions
if (homedir==""|is.na(homedir)){
  homedir <-"C://Users//Neal//Documents//www.DAYTRADINGLOGIC.com//_neal//swing//R"
}

setwd(homedir)

dat <- read.csv("interaction_formula.csv", header = TRUE)

rc(dat)
names(dat)
#ROUND(9/(((B2+1)*900003)+B2)^(100001/(100001+C2)),20)
#http://mathoverflow.net/questions/69244/encoding-n-natural-numbers-into-one-and-back/162519#162519
#http://stackoverflow.com/questions/919612/mapping-two-integers-to-one-in-a-unique-and-deterministic-way/13871379#13871379
#http://codepen.io/sachmata/post/elegant-pairing
#http://mathoverflow.net/questions/69244/encoding-n-natural-numbers-into-one-and-back/162519#162519
#https://ctszkin.com/2013/04/04/construct-an-unique-index-from-two-integer-pairing-function/
#encoding two numbers into deterministic distinct key
x <- dat[,2];
y <- dat[,3];
N <- 0.5*(x+y)*(x+y+1) +  x
#decoding key 
w= floor( (sqrt(8*N+1) - 1)/2 )
t = w*(w+1)/2
a <- N-t
b <- w-N+t


pairingTbl <- data.frame()
pairingTbl <- cbind(x=x,y=y,key=N,unpair_key_a=a,unpair_key_b=b)
colnames(pairingTbl) <- c("x","y","key","unpair_key_x","unpair_key_y")
str(pairingTbl)
rc(pairingTbl)
print(pairingTbl[100905:101205,])

intCol <- data.frame()
intCol <- N
colnames(intCol)<- c("interaction")

orderedIntCol <- data.frame()
orderedIntCol <- as.data.frame(dat[,2]*.00001 + dat[,3]*.00000001)
colnames(orderedIntCol)<- c("orderedIntCol")


rowId <- data.frame()
rowId <- as.data.frame(seq(1,nrow(dat),1))
colnames(rowId) <- c("rowId")


datN <- cbind(rowId,dat,intCol,orderedIntCol)
rc(datN)
str(datN)
names(datN)

#write.csv(datN, file = "datN.csv",row.names=TRUE,append = FALSE)
#dat <- read.csv("datN.csv", header = TRUE)
#names(dat)

dat2 <- datN[1:190000,]
names(dat2)
library('ggplot2')
p <- ggplot(dat2, aes(dat2[,1], dat2[,5]))
p <- ggplot(dat2, aes(dat2[,6], dat2[,5]))
p + geom_path(aes(colour = dat2[,6]))

p + geom_point(alpha = 1/20,aes(colour = dat2[,4]))
p + geom_point(aes(colour = dat2[,6]))

p + geom_jitter(aes(colour = dat2[,4]))
p + geom_line(aes(colour = dat2[,4]))
p + geom_step(aes(colour = dat2[,4]))
p + geom_point(alpha = 1/20,aes(colour = dat2[,4])) + geom_line(aes(colour = dat2[,4]))

# #install.packages('ggvis')
library(ggvis)
# install.packages("devtools")
#devtools::install_github("hadley/bigvis")
#install.packages("ggplot2")
library('bigvis')




# use bigvis to bin, condense, smooth and present data
library('bigvis')
library('ggplot2')


# 1d summaries -----------------------------
x <- rchallenge(1e4)
z <- x + rt(length(x), df = 2)
xsum <- condense(bin(x, 0.1))
zsum <- condense(bin(x, 0.1), z = z)

autoplot(xsum)
autoplot(peel(xsum))

autoplot(zsum)
autoplot(peel(zsum, keep = 1))
autoplot(peel(zsum))



# subset the diamonds data
mydiamonds <- subset(diamonds, carat < 2.75)

# condense avg price based on bins of carat sizes of .1 carat intervals
myd <- condense(bin(mydiamonds$carat, .1), z=mydiamonds$price, summary="mean")
# smooth out the trend
myds <- smooth(myd, 50, var=".mean", type="robust")

# plot the orginal binned prices vs the smoothed trend line
ggplot() + geom_line(data=myd, aes(x=mydiamonds.carat, y=.mean, colour="Avg Price")) + 
  geom_line(data=myds, aes(x=mydiamonds.carat, y=.mean, colour="Smoothed")) + 
  ggtitle("Avg Diamond prices by binned Carat") + 
  ylab("Avg Price") + 
  xlab("Carats") + 
  scale_colour_manual("", breaks=c("Avg Price","Smoothed"), values=c("blue", "black"))


#create scatter plot - is there a visible correlation between var1 and var2 or var3 and var4 for all classes?
datN %>% ggvis(~var1, ~var1) %>% layer_points()
datN %>% ggvis(~var1, ~var2, fill = ~unique(datN$x1)) %>% layer_points(opacity:=1/1.25)


df <- do.call(cbind, combn(colnames(dat), 2, FUN= function(x) 
  list(setNames(data.frame( 
    (
      #=ROUND(1/(10001+B2)^(1001/(1001+C2)),8)
      round(1/(10001+dat[,x[1]])^ (1001/(1001+dat[,x[2]])), 8)
    ) 
  ), 
  paste(x, collapse="_")) ))
)

rc(df)

rm(dat,fieldNames,fn1,fn2,fn3)
gc()

write.csv(df, file = "DataSet_interacts_1-5000.csv",row.names=FALSE,append = FALSE)