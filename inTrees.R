# decimalplaces <- function(x) {
#   if ((x %% 1) != 0) {
#     nchar(strsplit(sub('0+$', '', as.character(x)), ".", fixed=TRUE)[[1]][[2]])
#   } else {
#     return(0)
#   }
# }
# decimalplaces(xn)
# 
# num.decimals <- function(x) {
#   stopifnot(class(x)=="numeric")
#   x <- sub("0+$","",x)
#   x <- sub("^.+[.]","",x)
#   nchar(x)
# }
# 
# num.decimals(xn)
# 
# 
# 
# x <- c("0.0000", "0", "159.283", "1.45e+10", "1.4599E+10","11.12342525256789123456" )
# x <- as.character(dataSet[,notIntCols])
# x<-as.character(blah)
# Ndec(x);min(Ndec(x));max(Ndec(x))
# num.dec <- as.integer(Ndec(x))
# Ndec(x)
# num.dec
# paste(rep(paste(vec2-vec2,2,sep = ",",collapse = ""), 1),collapse = ",")
# paste((vec2-vec2),rep(vec2, 01),sep = ",",collapse = "")
# 
# rep(vec2, 1)
# rep((vec2-vec2), 1)
# 
# sapply(rep(vec2, 1)*rep((vec2-vec2)+2, 1), as.numeric)
# 
# vec <- Ndec(x)
# vec <- as.data.frame(cbind(dec.places=vec,tens=10))
# vec
# str(vec)
# 
# 
# 
# vec2<- sapply(vec+1,as.numeric)
# vec2
# 
# paste(rep(c(rep((vec2-vec2), 1)), times = c(vec2)),collapse = ",")
# 
# x=vec2
# 
# # Method 1
# rep(x,times = c(vec2))
# 
# # Method 2
# matrix(x,length(x),c(vec2))
# 
# 
# 
# xn <- 11.12342525256789123456
# 
# min(match(TRUE, round(xn, 1:20) == xn))
# 
# min(xn)
# 
# specify_decimal <- function(x, k) format(x, nsmall=k)
# specify_decimal(.11, 10)
# 
# 
# 
# sub(".", "", as.character(specify_decimal(1, 10)))
# 
# specify_decimal(Ndec(x), decimalplaces(Ndec(x)))

roundUp <- function(x,to=10)
{
  to*(x%/%to + as.logical(x%%to))
}
roundUp(0,50)
is.finite.data.frame <- function(obj){
  sapply(obj,FUN = function(x) all(is.finite(x)))
}

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
}


homedir<-getSrcDirectory(function(x) {x})
# Setting working directory
# need to be adjusted to server conditions
if (homedir==""|is.na(homedir)){
  homedir <-"C://Users//Neal//Documents//www.DAYTRADINGLOGIC.com//_neal//swing//R"
  #homedir <-"C://Users//nbb//Downloads//rscripts20160529"
}

# homedir <-"~//Zlecenia//ODesk//Neal Webster//swing prediction"
setwd(homedir)

# Load data (using iris dataset from Google Drive because link @ uci.edu was not working for me today)
#iris <- read.csv(url("http://archive.ics.uci.edu/ml/machine-learning-databases/iris/iris.data"), header = FALSE)
#iris <- read.csv(url("https://docs.google.com/spreadsheets/d/1ovz31Y6PrV5OwpqFI_wvNHlMTf9IiPfVy1c3fiQJMcg/pub?gid=811038462&single=true&output=csv"), header = FALSE)
#iris <- read.csv("vw_barFeatures_train_r_SwingLo.csv", header = TRUE)
dataSet <- read.csv("data/DataSet_Balanced_20160819.csv", header = TRUE)
dataSet <- read.csv("data/vw_barFeatures_train_r_20160819_SUBSET_BALANCED_TRAIN.csv", header = TRUE)
dataSet <- read.csv("data/vw_barFeatures_train_r_20160819_SUBSET_BALANCED.csv", header = TRUE)
dataSet <- read.csv("data/vw_barFeatures_train_r_20161123.csv", header = TRUE)
dataSet <- read.csv("data/vw_barFeatures_train_r_20161123_balanced.csv", header = TRUE)

rc(dataSet)

levels(as.factor(sapply(dataSet, class)))
table(sapply(dataSet, class))
table(dataSet[,1])

intCols <- sapply(dataSet, is.integer)
notIntCols <- !sapply(dataSet, is.integer)
notIntCols <- as.character(names(dataSet[,notIntCols]))
notIntCols <- notIntCols[which(notIntCols!="swingRecordType")]
notIntCols <- notIntCols[which(notIntCols!="numMinutesBarWasOpn")]
notIntCols;ncol(dataSet[,notIntCols]);
numCols <- sapply(dataSet, is.numeric)
notNumCols <- !sapply(dataSet, is.numeric)
factCols <- sapply(dataSet, is.factor)
notFactCols <- !sapply(dataSet, is.factor)

#min(dataSet[,notIntCols]);min(dataSet[,intCols]);
#max(dataSet[,notIntCols]);max(dataSet[,intCols]);
#tst <- -0.00143417*100000000;tst

#str(dataSet)
#linearlyRescale where [x] is some lookback period and range is "hard coded" by setting [floor] and [limit] values to literals
#(limit - floor) / (max[x] - min[x]) * (x - max[x]) + limit 
#linearlyRescale <- function(x,floor,limit) (limit - floor) / ((max(x) - min(x)) * (x - max(x)) + limit) 
#rescale <- function(x) (x-min(x))/(max(x) - min(x)) * 10000000000
notIntCols
#install.packages("DescTools")
library(DescTools)
x <- as.character(dataSet[,notIntCols])
paste(Ndec(x),collapse = ",")
#numDec <- as.data.frame(as.integer(Ndec(x)))
#df <- as.data.frame(numDec);colnames(df)[1] <- "Column_A";
#df
#B<-10**(df$Column_A)
#df<-cbind(df,B)
#df


# #create dataframe
# num_row <- nrow(df);num_col <- ncol(dataSet[,notIntCols]);
# m <- as.data.frame(matrix(df[,1], ncol = 1, nrow = num_row))
# m <- cbind(m, 10^m[,1])
# m <- rbind(m, blah)
# m <- m[2,]
# m
# ncol(m);ncol(dataSet[,notIntCols]);
# as.data.frame(dataSet[3,notIntCols])
# str(dataSet[3,notIntCols])
# str(as.data.frame(as.list(m[,2])))
# str(m[,2])
# str(df[,2])
# str(mults)
# #multiply numeric columns by multiple
# str(df[,2])
# df.aree <- as.data.frame(t(df[,2]))
# df.aree

dataSet[,notIntCols]<- mapply("*",as.data.frame(dataSet[,notIntCols]),100000000)
#dataSet[,notIntCols]<- sapply(dataSet[,notIntCols]*df[,2],as.numeric)

x2 <- as.character(dataSet[,notIntCols])
paste(Ndec(x2),collapse = ",")
#min(dataSet[,notIntCols]);min(dataSet[,intCols]);
#max(dataSet[,notIntCols]);max(dataSet[,intCols]);
#"7,7,7,7,4,5,5,5,5,5,8,11,7,8,8,2,11,11,11,2"
#"0,0,0,0,0,0,0,0,0,0,0,2 ,0,0,0,8,3 ,3 ,2 ,8"

dataSet[,notIntCols] <- sapply(dataSet[,notIntCols],as.integer)
table(sapply(dataSet[,2], class))

notIntCols <- !sapply(dataSet, is.integer)
notIntCols <- as.character(names(dataSet[,notIntCols]))
notIntCols <- notIntCols[which(notIntCols!="swingRecordType")]
dataSet[,notIntCols] <- sapply(dataSet[,notIntCols],as.integer)

table(sapply(dataSet, class))

#assign column names
#names(dataSet) <- c("SepalLength", "SepalWidth", "PetalLength", "PetalWidth", "Species")

#backup dataset after transformations are complete
dataSet.bkup <- dataSet
#features ordinality
x_col_start_pos <- 5
x_col_end_pos <- 3249
#dataSet <- dataSet[,c(1,x_col_start_pos:x_col_end_pos)]
names(dataSet)

#col names
dsColNames <- as.character(names(dataSet))
dsColNames
as.character(names(dataSet[,1:2]))
#num of columns and rows
dsColCount <- as.integer(ncol(dataSet))
dsRowCount <- as.integer(nrow(dataSet))
dsColCount
dsRowCount

#class ordinality and name
classColumn <- 2
classColumnName <- dsColNames[classColumn]
y_col_pos <- classColumn
unique(dataSet[,classColumn])
unique(dataSet[,1])
unique(dataSet[,2])

dataSet <- dataSet[,c(classColumn,x_col_start_pos:x_col_end_pos)]
nrow(dataSet)
ncol(dataSet)

classColumn <- 1
classColumnName <- dsColNames[classColumn]
y_col_pos <- classColumn
unique(dataSet[,classColumn])
unique(dataSet[,1])

unique(dataSet.bkup[,2:3])


#features ordinality
x_col_start_pos <- 2
x_col_end_pos <- 3246

y_col_pos
x_col_start_pos
x_col_end_pos

firstCol <- ifelse(x_col_start_pos < classColumn, x_col_start_pos, classColumn)
firstCol

lastCol <- ifelse(x_col_end_pos > classColumn, x_col_end_pos, classColumn)
lastCol

#distinct list and count of classes from column assumed to contain class values
dsClassValues <- as.character(unique(dataSet[,classColumn])) #levels(dataSet[,classColumn])
dsClassCount <- as.integer(length(dsClassValues)) #sqldf("select distinct(x) from df1")
dsClasses <- dataSet[,classColumn]
dsClassCount
dsClassValues

dataSet[1,1:10]
#levels(as.factor(dataSet[,123]))

#class distribution in terms of row count/freqency and percentage/proportions
dsClassFreq <- table(dsClasses)
dsClassDistribution <- round(prop.table(table(dsClasses)) * 100, digits = 2)
dsClassFreq
dsClassDistribution
 
# #Randomly sample a percentage of rows to balance class distribution
#mydf <- mydf[ sample( which(mydf$swingRecordType=="SwingNULL"), round(0.1235*length(which(mydf$swingRecordType=="SwingNULL")))), ]

#get all "SwingHi" rows
dataSet.SwingHi <- dataSet[ sample( which(dataSet$ClassChar=="SwingHi"), round(43.82*length(which(dataSet$ClassChar=="SwingHi"))),replace = TRUE), ]
table(dataSet.SwingHi[,classColumn])
round(prop.table(table(dataSet.SwingHi[,classColumn])) * 100, digits = 2)

#get all "SwingLo" rows
dataSet.SwingLo <- dataSet[ sample( which(dataSet$ClassChar=="SwingLo"), round(41.92*length(which(dataSet$ClassChar=="SwingLo"))),replace = TRUE), ]
table(dataSet.SwingLo[,classColumn])
round(prop.table(table(dataSet.SwingLo[,classColumn])) * 100, digits = 2)

#get all "SwingNULL" rows and append all "SwingHi" and "SwingLo" rows
dataSet2 <- rbind(dataSet[ which(dataSet$ClassChar=="SwingNULL"), ],dataSet.SwingHi,dataSet.SwingLo)
table(dataSet2[,classColumn])
round(prop.table(table(dataSet2[,classColumn])) * 100, digits = 2)

dataSet <- dataSet2
#Free RAM. Remove objects / data from workspace
rm(dataSet.SwingHi, dataSet.SwingLo, dataSet2)
gc()


write.csv(dataSet, file = "data/vw_barFeatures_train_r_20161123_balanced.csv",row.names=FALSE,append = FALSE)


#num of columns and rows
dsColCount <- as.integer(ncol(dataSet))
dsRowCount <- as.integer(nrow(dataSet))
dsColCount
dsRowCount

# % of [dataset] reserved for training/test and validation  
set.seed(123)
sampleAmt <- 0.5
mainSplit <- sample(2, dsRowCount, replace=TRUE, prob=c(sampleAmt, 1-sampleAmt))

#split [dataSet] into two sets
dsTrainingTest <- dataSet[mainSplit==1, ]#firstCol:lastCol] 
dsValidation <- dataSet[mainSplit==2, ]#firstCol:lastCol]

nrow(dataSet);nrow(dsTrainingTest);nrow(dsValidation);

nrow(dataSet) == nrow(dsTrainingTest)+nrow(dsValidation)

print(round(prop.table(table(dataSet[,classColumn]))* 100, digits = 1))
print(round(prop.table(table(dsTrainingTest[,classColumn]))* 100, digits = 1))
print(round(prop.table(table(dsValidation[,classColumn]))* 100, digits = 1))


write.csv(dsTrainingTest, file = "data/vw_barFeatures_train_r_20161123_balanced_TrainingTest.csv",row.names=FALSE,append = FALSE)
write.csv(dsValidation, file = "data/vw_barFeatures_train_r_20161123_balanced_Validation.csv",row.names=FALSE,append = FALSE)

#SwingHi   SwingLo SwingNULL 
#33.3      33.4      33.3 
#33.4      33.2      33.4 

# % of [dsTrainingTest] reserved for training
sampleAmt <- 0.5
secondarySplit <- sample(2, nrow(dsTrainingTest), replace=TRUE, prob=c(sampleAmt, 1-sampleAmt))

#split [dsTrainingTest] into two sets 
dsTraining <- dsTrainingTest[secondarySplit==1, ]#firstCol:lastCol]
dsTest <- dsTrainingTest[secondarySplit==2, ]#firstCol:lastCol]
nrow(dsTraining);nrow(dsTest);

nrow(dataSet) == nrow(dsTrainingTest)+nrow(dsValidation)
nrow(dsTrainingTest) == nrow(dsTraining)+nrow(dsTest)

# % of [dsValidation] reserved for Validation
sampleAmt <- 0.5
secondarySplit <- sample(2, nrow(dsValidation), replace=TRUE, prob=c(sampleAmt, 1-sampleAmt))

# #split [dsValidation] into two sets 
# dsValp1 <- dsValidation[secondarySplit==1, firstCol:lastCol]
# dsValp2 <- dsValidation[secondarySplit==2, firstCol:lastCol]
# nrow(dsValp1);nrow(dsValp2);

# nrow(dataSet) == nrow(dsTrainingTest)+nrow(dsValp1)+nrow(dsValp2)
# nrow(dsValidation) == nrow(dsValp1)+nrow(dsValp2)

# # % of [dsValp1] reserved for Validation
# sampleAmt <- 0.5075
# secondarySplit <- sample(2, nrow(dsValp1), replace=TRUE, prob=c(sampleAmt, 1-sampleAmt))
# 
# #split [dsValp1] into two sets 
# dsValp1a <- dsValp1[secondarySplit==1, firstCol:lastCol]
# dsValp1b <- dsValp1[secondarySplit==2, firstCol:lastCol]
# nrow(dsValp1a);nrow(dsValp1b);
# 
# nrow(dataSet) == nrow(dsTrainingTest)+nrow(dsValp1a)+nrow(dsValp1b)+nrow(dsValp2)
# nrow(dsValp1) == nrow(dsValp1a)+nrow(dsValp1b)
# 
# # % of [dsValp2] reserved for Validation
# sampleAmt <- 0.5075
# secondarySplit <- sample(2, nrow(dsValp2), replace=TRUE, prob=c(sampleAmt, 1-sampleAmt))
# 
# #split [dsValp2] into two sets 
# dsValp2a <- dsValp2[secondarySplit==1, firstCol:lastCol]
# dsValp2b <- dsValp2[secondarySplit==2, firstCol:lastCol]
# nrow(dsValp2a);nrow(dsValp2b);

# nrow(dataSet) ==  nrow(dsTrainingTest)+nrow(dsValp1a)+nrow(dsValp1b)+nrow(dsValp2a)+nrow(dsValp2b)
# nrow(dsValp2) == nrow(dsValp2a)+nrow(dsValp2b)

nrow(dataSet) ==  nrow(dsTrainingTest)+nrow(dsValidation)
nrow(dataSet) ==  nrow(dsTraining)+nrow(dsTest)+nrow(dsValidation)

#Free RAM. Remove objects / data from workspace
#rm(dsTest,dsTraining,dsValidation,x)
rm(dsTrainingTest,dataSet)
gc()

# #install.packages("data.table")
# library(data.table)
# 
# dsSummary <- as.data.frame(data.table(nrow(dataSet),nrow(dsTrainingTest),nrow(dsTraining),nrow(dsTest),nrow(dsValidation),nrow(dsValp1a),nrow(dsValp1b),nrow(dsValp2a),nrow(dsValp2b)))
# names(dsSummary) <- c("completeDataset","dsTrainingTest", "TrainingDataset", "TestDataset", "ValidationDataset","dsValp1a","dsValp1b","dsValp2a","dsValp2b")
# ncol(dsSummary)
# dsSummary

#0. complete dataset
nrow(dataSet.bkup);
nrow(dataSet);nrow(dsTrainingTest);nrow(dsValidation);
#1. Training dataset #2. Test dataset #4. Validation datasets
nrow(dsTraining);nrow(dsTest);nrow(dsValidation);


# nrow(dsValp1a);nrow(dsValp1b);nrow(dsValp2a);nrow(dsValp2b);

#is.finite.data.frame(dataSet)

#print(round(prop.table(table(dataSet.bkup[,classColumn]))* 100, digits = 1))
print(round(prop.table(table(dataSet[,classColumn]))* 100, digits = 1))
print(round(prop.table(table(dsTrainingTest[,classColumn]))* 100, digits = 1))

print(round(prop.table(table(dsTraining[,classColumn]))* 100, digits = 1))
print(round(prop.table(table(dsTest[,classColumn]))* 100, digits = 1))
print(round(prop.table(table(dsValidation[,classColumn]))* 100, digits = 1))

#install.packages("DMwR")
library(DMwR)

# maxTrainIteration <- 500
# trainIteration <- 0;  loopNum <- 0; bestTrainNum <- NULL;
# while (trainIteration <= maxTrainIteration) {
#   #for (num0 in seq(1,10,1)) {for (num1 in seq(1,10,1)){
#   orig.distPercent <- as.data.frame(round(prop.table(table(dsTraining.bal[,classColumn]))* 100, digits = 1))
#   orig.distRecordCount <- as.data.frame(table(dsTraining.bal[,classColumn]))
#   #set random values for "perc.over" and "perc.under"
#   randVar0 <- roundUp(sample(4:19,1,replace=TRUE),1)*50;randVar1 <- roundUp(sample(2:6,1,replace=TRUE),1)*50;
#   randVar2 <- roundUp(sample(4:19,1,replace=TRUE),1)*50;randVar3 <- roundUp(sample(2:6,1,replace=TRUE),1)*50;
#   
#   if (loopNum == 0) {
#     bestParams <- data.frame(cbind(recordType=1,SwingHi.p=orig.distPercent[1,2],SwingLo.p=orig.distPercent[2,2]
#                                    ,SwingNULL.p=orig.distPercent[3,2],SwingHi.c=orig.distRecordCount[1,2]
#                                    ,SwingLo.c=orig.distRecordCount[2,2],SwingNULL.c=orig.distRecordCount[3,2]
#                                    ,perc.over=abs(randVar0-randVar0),perc.under=abs(randVar1-randVar1),perc.over2=abs(randVar2-randVar2),perc.under2=abs(randVar3-randVar3)
#                                    ,ratioClassAvsBvsC=(orig.distPercent[1,2]-orig.distPercent[2,2])+orig.distPercent[3,2]))
#     print(bestParams)
#   }
#   #randVar0 <- roundUp(num0,1)*100;randVar1 <- roundUp(num1,1)*100;
#   print(paste("Begin test:",loopNum,"params --> perc.over",randVar0,"/ perc.under",randVar1,"perc.over2",randVar2,"/ perc.under2",randVar3));
# 
#   ## use SMOTE to balance classes
#   dsTraining.bal <- SMOTE(swingRecordType ~ ., dsTraining, perc.over=randVar0,perc.under=randVar1)
#   dsTraining.bal2 <- SMOTE(swingRecordType ~ ., dsTraining.bal, perc.over=randVar2,perc.under=randVar3)
# 
#   # Calculate error
#   distPercent <- as.data.frame(round(prop.table(table(dsTraining.bal2[,classColumn]))* 100, digits = 1))
#   distRecordCount <- as.data.frame(table(dsTraining.bal2[,classColumn]))
#   error1 <- distPercent[1,2]; error2 <- distPercent[2,2]; error3 <- distPercent[3,2];ratioClassAvsBvsC <- (error1-error2)+error3
# 
#   #output ideal params
#   if ( (ratioClassAvsBvsC >= 19 & ratioClassAvsBvsC <= 41) ) {
#     bestTrainNum <- loopNum
#     new_row <- c(0,distPercent[1,2],distPercent[2,2],distPercent[3,2],distRecordCount[1,2],distRecordCount[2,2]
#                  ,distRecordCount[3,2],randVar0,randVar1,randVar2,randVar3,ratioClassAvsBvsC)
#     bestParams <- rbind(bestParams, new_row)
#     bestParams <- bestParams[order(bestParams$recordType,bestParams$ratioClassAvsBvsC,bestParams$SwingHi.p,bestParams$SwingLo.p,bestParams$SwingNULL.p, decreasing=TRUE),]
#     print(paste("--> Class Distribution",error1,error2,error3,"bestTrainNum =",bestTrainNum))
#     print(bestParams)
#   }    
#    else {
#      print(paste("--> Class Distribution",error1,error2,error3
#                  ,"bestTrainNum =",bestTrainNum))
#    }#}}
#   trainIteration <- trainIteration+1;loopNum <- loopNum + 1;
#   gc()
# }
#   
#   bestParams

  # use SMOTE to balance classes
dsTraining.bal <- SMOTE(swingRecordType ~ ., dsTraining, perc.over=900,perc.under=100)#perc.over=950,perc.under=100)
dsTraining.bal2 <- SMOTE(swingRecordType ~ ., dsTraining.bal, perc.over=950,perc.under=200)#perc.over=700,perc.under=200)
  round(prop.table(table(dsTraining[,classColumn]))* 100, digits = 1);round(prop.table(table(dsTraining.bal[,classColumn]))* 100, digits = 1);round(prop.table(table(dsTraining.bal2[,classColumn]))* 100, digits = 1);
  table(dsTraining[,classColumn]);  table(dsTraining.bal[,classColumn]);table(dsTraining.bal2[,classColumn]);
  
  
dsTest.bal <- SMOTE(swingRecordType ~ ., dsTest, perc.over=900,perc.under=100)
dsTest.bal2 <- SMOTE(swingRecordType ~ ., dsTest.bal, perc.over=950,perc.under=200)
  round(prop.table(table(dsTest[,classColumn]))* 100, digits = 1);round(prop.table(table(dsTest.bal[,classColumn]))* 100, digits = 1);round(prop.table(table(dsTest.bal2[,classColumn]))* 100, digits = 1)
  table(dsTest[,classColumn]);table(dsTest.bal[,classColumn]);table(dsTest.bal2[,classColumn])

dsValidation.bal <- SMOTE(swingRecordType ~ ., dsValidation, perc.over=900,perc.under=100)
dsValidation.bal2 <- SMOTE(swingRecordType ~ ., dsValidation.bal, perc.over=950,perc.under=200)
  round(prop.table(table(dsValidation[,classColumn]))* 100, digits = 1);round(prop.table(table(dsValidation.bal[,classColumn]))* 100, digits = 1);round(prop.table(table(dsValidation.bal2[,classColumn]))* 100, digits = 1)
  table(dsValidation[,classColumn]);table(dsValidation.bal[,classColumn]);table(dsValidation.bal2[,classColumn])

  
dsValp1a.bal <- SMOTE(swingRecordType ~ ., dsValp1a, perc.over=900,perc.under=100)
dsValp1a.bal2 <- SMOTE(swingRecordType ~ ., dsValp1a.bal, perc.over=950,perc.under=200)
  round(prop.table(table(dsValp1a[,classColumn]))* 100, digits = 1);round(prop.table(table(dsValp1a.bal[,classColumn]))* 100, digits = 1);round(prop.table(table(dsValp1a.bal2[,classColumn]))* 100, digits = 1)
  table(dsValp1a[,classColumn]);table(dsValp1a.bal[,classColumn]);table(dsValp1a.bal2[,classColumn])

dsValp1b.bal <- SMOTE(swingRecordType ~ ., dsValp1b, perc.over=900,perc.under=100)
dsValp1b.bal2 <- SMOTE(swingRecordType ~ ., dsValp1b.bal, perc.over=950,perc.under=200)
  round(prop.table(table(dsValp2b[,classColumn]))* 100, digits = 1);round(prop.table(table(dsValp2b.bal[,classColumn]))* 100, digits = 1);round(prop.table(table(dsValp2b.bal2[,classColumn]))* 100, digits = 1)
  table(dsValp2b[,classColumn]);table(dsValp2b.bal[,classColumn]);table(dsValp2b.bal2[,classColumn])

dsValp2a.bal <- SMOTE(swingRecordType ~ ., dsValp2a, perc.over=900,perc.under=100)
dsValp2a.bal2 <- SMOTE(swingRecordType ~ ., dsValp2a.bal, perc.over=950,perc.under=200)
  round(prop.table(table(dsValp2a[,classColumn]))* 100, digits = 1);round(prop.table(table(dsValp2a.bal[,classColumn]))* 100, digits = 1);round(prop.table(table(dsValp2a.bal2[,classColumn]))* 100, digits = 1)
  table(dsValp2a[,classColumn]);table(dsValp2a.bal[,classColumn]);table(dsValp2a.bal2[,classColumn])
  
dsValp2b.bal <- SMOTE(swingRecordType ~ ., dsValp2b, perc.over=900,perc.under=100)
dsValp2b.bal2 <- SMOTE(swingRecordType ~ ., dsValp2b.bal, perc.over=950,perc.under=200)
  round(prop.table(table(dsValp2b[,classColumn]))* 100, digits = 1);round(prop.table(table(dsValp2b.bal[,classColumn]))* 100, digits = 1);round(prop.table(table(dsValp2b.bal2[,classColumn]))* 100, digits = 1)
  table(dsValp2b[,classColumn]);table(dsValp2b.bal[,classColumn]);table(dsValp2b.bal2[,classColumn])

# print(round(prop.table(table(dsTraining.bal2[,classColumn]))* 100, digits = 1))
# print(round(prop.table(table(dsTest.bal2[,classColumn]))* 100, digits = 1))
# print(round(prop.table(table(dsValp1a.bal2[,classColumn]))* 100, digits = 1))
# print(round(prop.table(table(dsValp1b.bal2[,classColumn]))* 100, digits = 1))
# print(round(prop.table(table(dsValp2a.bal2[,classColumn]))* 100, digits = 1))
# print(round(prop.table(table(dsValp2b.bal2[,classColumn]))* 100, digits = 1))
# dsTrainingTest <- dsTrainingTest[,c(1,x_col_start_pos:x_col_end_pos)]
# dsTest <- dsTest[,c(1,x_col_start_pos:x_col_end_pos)]
# dsValidation <- dsValidation[,c(1,x_col_start_pos:x_col_end_pos)]

  dsTraining.bal <- dsTrainingTest
  dsTest.bal <- dsTest
  dsValidation.bal <- dsValidation
  #dsTraining.bal <- dsTraining
  #dsTest.bal <- dsTest
  
#   dsValp1a.bal <- dsValp1a#.bal2  
#   dsValp1b.bal <- dsValp1b#.bal2
#   dsValp2a.bal <- dsValp2a#.bal2
#   dsValp2b.bal <- dsValp2b#.bal2

  # blah <- dsTraining.bal; meta.na <- as.data.frame(is.finite.data.frame(blah));
  # ncol(blah);nrow(blah);table(sapply(blah, class));table(meta.na[,]);table(blah[,classColumn]);round(prop.table(table(blah[,classColumn])) * 100, digits = 2);
  # 
  # blah <- dsTest.bal; meta.na <- as.data.frame(is.finite.data.frame(blah));
  # ncol(blah);nrow(blah);table(sapply(blah, class));table(meta.na[,]);table(blah[,classColumn]);round(prop.table(table(blah[,classColumn])) * 100, digits = 2);
  # 
  # blah <- dataSet.bkup; meta.na <- as.data.frame(is.finite.data.frame(blah));
  # ncol(blah);nrow(blah);table(sapply(blah, class));table(meta.na[,]);table(blah[,classColumn]);round(prop.table(table(blah[,classColumn])) * 100, digits = 2);
  
  
#print(round(prop.table(table(dataSet.bal[,classColumn]))* 100, digits = 1))
print(round(prop.table(table(dsTraining.bal[,classColumn]))* 100, digits = 1))
print(round(prop.table(table(dsTest.bal[,classColumn]))* 100, digits = 1))
print(round(prop.table(table(dsValidation.bal[,classColumn]))* 100, digits = 1))
print(round(prop.table(table(dataSet.bkup[,classColumn]))* 100, digits = 1))
#print(round(prop.table(table(dsValidation.bal[,classColumn]))* 100, digits = 1))
round(prop.table(table(dsTraining.bal[,1])) * 100, digits = 2);nrow(dsTraining.bal);
round(prop.table(table(dsTest.bal[,1])) * 100, digits = 2);nrow(dsTest.bal);
round(prop.table(table(dsValp1a.bal[,1])) * 100, digits = 2);nrow(dsValp1a.bal);
round(prop.table(table(dsValp1b.bal[,1])) * 100, digits = 2);nrow(dsValp1b.bal);
round(prop.table(table(dsValp2a.bal[,1])) * 100, digits = 2);nrow(dsValp2a.bal);
round(prop.table(table(dsValp2b.bal[,1])) * 100, digits = 2);nrow(dsValp2b.bal);

table(sapply(dsTraining.bal, class))
table(sapply(dsTest.bal, class))
# dsTraining.bal[,2:822] <- sapply(dsTraining.bal[,2:822],as.integer)
# table(sapply(dsTraining.bal, class))
# dsTest.bal[,2:822] <- sapply(dsTest.bal[,2:822],as.integer)
# table(sapply(dsTest.bal, class))
# 
# dsTraining.bal[,"MAMAP25"] <- sapply(dsTraining.bal[,"MAMAP25"],as.integer)
# table(sapply(dsTraining.bal, class))
# dsTest.bal[,"MAMAP25"] <- sapply(dsTest.bal[,"MAMAP25"],as.integer)
# table(sapply(dsTest.bal, class))
# 
# notIntCols <- !sapply(dsTest.bal, is.integer)
# notIntCols <- as.character(names(dsTest.bal[,notIntCols]))
# notIntCols <- notIntCols[which(notIntCols!="swingRecordType")]
# notIntCols

#dsTest.bal[,notIntCols]<- mapply("*",as.data.frame(dsTest.bal[,notIntCols]),100000000)#df.aree)

# x2 <- as.character(dsTest.bal[,"MAMAP25"])
# max(paste(Ndec(x2),collapse = ","))


# dsSummary.bal <- data.table(nrow(dsTraining.bal),nrow(dsTest.bal),nrow(dsValp1a.bal),nrow(dsValp1b.bal),nrow(dsValp2a.bal),nrow(dsValp2b.bal))
# names(dsSummary.bal) <- c("TrainingDataset.bal", "TestDataset.bal", "dsValp1a.bal","dsValp1b.bal","dsValp2a.bal","dsValp2b.bal")
# ncol(dsSummary.bal)
# dsSummary.bal
# dsTraining.bal <- as.integer(dsTraining.bal[,])
# write.csv(dataSet, file = "export_completeDataset.csv",row.names=TRUE)
# write.csv(dsTraining, file = "export_TrainingDataset.csv",row.names=TRUE)
# write.csv(dsTest, file = "export_TestDataset.csv",row.names=TRUE)
# write.csv(dsValidation, file = "export_ValidationDataset.csv",row.names=TRUE)
# 
# var1 <- dataSet[,"stofP1"]
# var2 <- dataSet[,"cci"]
# var3 <- dataSet[,"HMAP1"]
# var4 <- dataSet[,"bbdM2"]
# var5 <- dataSet[,"stdebP1"]
# 
# #"stofP1"  "cci"     "HMAP1"   "bbdM2"   "stdebP1"   
# 
# #Initial Overview Of The Data Set
# #install.packages('ggvis')
# library(ggvis)
# 
# #create scatter plot - is there a visible correlation between var1 and var2 or var3 and var4 for all classes?
# dataSet %>% ggvis(~var1, ~var1, fill = ~dsClasses) %>% layer_points()
# dataSet %>% ggvis(~var1, ~var2, fill = ~dsClasses) %>% layer_points(opacity:=1/1.25)
# dataSet %>% ggvis(~var1, ~var3, fill = ~dsClasses) %>% layer_points()
# dataSet %>% ggvis(~var1, ~var4, fill = ~dsClasses) %>% layer_points()
# dataSet %>% ggvis(~var1, ~var5, fill = ~dsClasses) %>% layer_points()
# 
# dataSet %>% ggvis(~var2, ~var1, fill = ~dsClasses) %>% layer_points()
# dataSet %>% ggvis(~var2, ~var2, fill = ~dsClasses) %>% layer_points()
# dataSet %>% ggvis(~var2, ~var3, fill = ~dsClasses) %>% layer_points()
# dataSet %>% ggvis(~var2, ~var4, fill = ~dsClasses) %>% layer_points()
# dataSet %>% ggvis(~var2, ~var5, fill = ~dsClasses) %>% layer_points()
# 
# dataSet %>% ggvis(~var3, ~var1, fill = ~dsClasses) %>% layer_points()
# dataSet %>% ggvis(~var3, ~var2, fill = ~dsClasses) %>% layer_points()
# dataSet %>% ggvis(~var3, ~var3, fill = ~dsClasses) %>% layer_points()
# dataSet %>% ggvis(~var3, ~var4, fill = ~dsClasses) %>% layer_points()
# dataSet %>% ggvis(~var3, ~var5, fill = ~dsClasses) %>% layer_points()
# 
# dataSet %>% ggvis(~var4, ~var1, fill = ~dsClasses) %>% layer_points()
# dataSet %>% ggvis(~var4, ~var2, fill = ~dsClasses) %>% layer_points()
# dataSet %>% ggvis(~var4, ~var3, fill = ~dsClasses) %>% layer_points()
# dataSet %>% ggvis(~var4, ~var4, fill = ~dsClasses) %>% layer_points()
# dataSet %>% ggvis(~var4, ~var5, fill = ~dsClasses) %>% layer_points()
# 
# dataSet %>% ggvis(~var5, ~var1, fill = ~dsClasses) %>% layer_points()
# dataSet %>% ggvis(~var5, ~var2, fill = ~dsClasses) %>% layer_points()
# dataSet %>% ggvis(~var5, ~var3, fill = ~dsClasses) %>% layer_points()
# dataSet %>% ggvis(~var5, ~var4, fill = ~dsClasses) %>% layer_points()
# dataSet %>% ggvis(~var5, ~var5, fill = ~dsClasses) %>% layer_points()


#trainSetEnum <- dsTraining.bal[,firstCol:lastCol]
#trainSetEnum[,classColumn] <- as.character(trainSetEnum[,classColumn])
#trainSetEnum[,classColumn][trainSetEnum[,classColumn]=="SwingHi"] <- 2 
#trainSetEnum[,classColumn][trainSetEnum[,classColumn]=="SwingLo"] <- 1 
#trainSetEnum[,classColumn][trainSetEnum[,classColumn]=="SwingNULL"] <- 2
#trainSetEnum[,classColumn][trainSetEnum[,classColumn]=="NotSwingLo"] <- 2
#trainSetEnum[,classColumn][trainSetEnum[,classColumn]=="NotSwingHi"] <- 2
#trainSetEnum[,classColumn] <- as.integer(trainSetEnum[,classColumn])

#x <- as.matrix(trainSetEnum[,x_col_start_pos:x_col_end_pos])
xTrain.bal <- as.data.frame(dsTraining.bal[,x_col_start_pos:x_col_end_pos])
yTrain.bal <- as.factor(dsTraining.bal[,classColumn])

# xTrain.bal <- as.data.frame(dsTraining.bal2[,x_col_start_pos:x_col_end_pos])
# yTrain.bal <- as.factor(dsTraining.bal2[,classColumn])

#testSetEnum <- dsTest.bal[,firstCol:lastCol]
#testSetEnum[,classColumn] <- as.character(testSetEnum[,classColumn])
#testSetEnum[,classColumn][testSetEnum[,classColumn]=="SwingHi"] <- 2 
#testSetEnum[,classColumn][testSetEnum[,classColumn]=="SwingLo"] <- 1 
#testSetEnum[,classColumn][testSetEnum[,classColumn]=="SwingNULL"] <- 2 
#testSetEnum[,classColumn][testSetEnum[,classColumn]=="NotSwingLo"] <- 2 
#testSetEnum[,classColumn][testSetEnum[,classColumn]=="NotSwingHi"] <- 2 
#testSetEnum[,classColumn] <- as.integer(testSetEnum[,classColumn])

#xTest <- as.matrix(testSetEnum[,x_col_start_pos:x_col_end_pos])
xTest.bal <- as.data.frame(dsTest.bal[,x_col_start_pos:x_col_end_pos])
yTest.bal <- as.factor(dsTest.bal[,classColumn])

# xTest.bal <- as.data.frame(dsTest.bal2[,x_col_start_pos:x_col_end_pos])
# yTest.bal <- as.factor(dsTest.bal2[,classColumn])


# xValp1a.bal <- as.data.frame(dsValp1a.bal[,x_col_start_pos:x_col_end_pos])
# yValp1a.bal <- as.factor(dsValp1a.bal[,classColumn])
# 
# xValp1b.bal <- as.data.frame(dsValp1b.bal[,x_col_start_pos:x_col_end_pos])
# yValp1b.bal <- as.factor(dsValp1b.bal[,classColumn])
# 
# xValp2a.bal <- as.data.frame(dsValp2a.bal[,x_col_start_pos:x_col_end_pos])
# yValp2a.bal <- as.factor(dsValp2a.bal[,classColumn])
# 
# xValp2b.bal <- as.data.frame(dsValp2b.bal[,x_col_start_pos:x_col_end_pos])
# yValp2b.bal <- as.factor(dsValp2b.bal[,classColumn])

#nrow(dsValidation);nrow(dsValp1a);nrow(dsValp1b);nrow(dsValp2a);nrow(dsValp2b);
xVal.bal <- as.data.frame(dsValidation.bal[,x_col_start_pos:x_col_end_pos])
yVal.bal <- as.factor(dsValidation.bal[,classColumn])#dsValidation[,classColumn])

# xVal.bal <- as.data.frame(dsValidation.bal2[,x_col_start_pos:x_col_end_pos])
# yVal.bal <- as.factor(dsValidation.bal2[,classColumn])#dsValidation[,classColumn])

xBkup.bal <- as.data.frame(dataSet.bkup[,11:3870])
yBkup.bal <- as.factor(dataSet.bkup[,classColumn])

# xValp1a <- as.data.frame(dsValp1a[,x_col_start_pos:x_col_end_pos])
# yValp1a <- as.factor(dsValp1a[,classColumn])
# 
# xValp1b <- as.data.frame(dsValp1b[,x_col_start_pos:x_col_end_pos])
# yValp1b <- as.factor(dsValp1b[,classColumn])
# 
# xValp2a <- as.data.frame(dsValp2a[,x_col_start_pos:x_col_end_pos])
# yValp2a <- as.factor(dsValp2a[,classColumn])
# 
# xValp2b <- as.data.frame(dsValp2b[,x_col_start_pos:x_col_end_pos])
# yValp2b <- as.factor(dsValp2b[,classColumn])
# 
# 
# blah <- dsTraining.bal; meta.na <- as.data.frame(is.finite.data.frame(blah));
# ncol(blah);nrow(blah);table(sapply(blah, class));table(meta.na[,]);table(blah[,classColumn]);round(prop.table(table(blah[,classColumn])) * 100, digits = 2);

#https://dl.dropboxusercontent.com/u/45301435/inTreesDemo.R
#install.packages("inTrees");
#install.packages("randomForest");
#install.packages("RRF");
#install.packages("gbm");
#install.packages("foreach");
#install.packages("ranger");
#library(foreach);
detach("package:ranger", unload=TRUE)
library(ranger);
library(inTrees);
library(randomForest);
library(RRF);
library(gbm);
set.seed(1) 
#rm(list=ls(all=TRUE));graphics.off() 
lists = list()

# measure user-defined conditions
#myRule <- "X[,3] > 5 & X[,4] > 1"
#measureRule(myRule,X,target)  # without providing the outcome of the condition
#measureRule(myRule,X,target,"versicolor")  # providing the outcome of the condition

maxTrainIteration <- 3
trainIteration <- 0;  loopNum <- 0; bestTrainNum <- NULL; smallestError <- 100;
while (trainIteration <= maxTrainIteration) {
  #for (num0 in seq(1,10,1)) {for (num1 in seq(1,10,1)){
  orig.distPercent <- as.data.frame(round(prop.table(table(dsTraining.bal[,classColumn]))* 100, digits = 1))
  orig.distRecordCount <- as.data.frame(table(dsTraining.bal[,classColumn]))
  ratioClassAvsBvsC <- (orig.distPercent[1,2]-orig.distPercent[2,2])+orig.distPercent[3,2]
  #set random values for model params
  randVar0 <- roundUp(sample(10:85,1,replace=TRUE),5);#ntree
  randVar1 <- roundUp(sample(1000:(nrow(xTrain.bal)*.6),1,replace=TRUE),100);#sampsize
  randVar2 <- roundUp(sample(10:(nrow(xTrain.bal)*.05),1,replace=TRUE),5);#nodesize
  randVar3 <- roundUp(sample((sqrt(((2*randVar1 - 16*randVar2)/randVar2)^2)):(sqrt(((2*randVar1 - 200*randVar2)/randVar2)^2)),1,replace=TRUE),5);#maxnodes
  randVar4 <- roundUp(sample(10:(ncol(xTrain.bal)*.2),1,replace=TRUE),5);#mtry
  rv5 <- roundUp(sample(1:100,1,replace=TRUE),5);
  rv6 <- roundUp(sample(1:100,1,replace=TRUE),5);
  
  if (loopNum == 0) {
    bestParams <- data.frame(cbind(recordType=1
                             ,rv0=0,rv1=0,rv2=0,rv3=0
                             ,rv4=0,rv5=0,rv6=0
                             ,type=0#rf$type
                             ,localImportance=0#rf$localImportance
                             ,proximity=0#rf$proximity
                             ,mtry=0#rf$mtry
                             ,nrnodes=0#rf$forest$nrnodes
                             ,ntree=0L#rf$forest$ntree
                             ,Hi_Hi=0,Hi_Lo=0,Hi_NULL=0
                             ,Lo_Lo=0,Lo_Hi=0,Lo_NULL=0
                             ,NULL_NULL=0,NULL_Hi=0,NULL_Lo=0
                             ,Hi.Err.test=0.1,Lo.Err.test=0.1,NULL.Err.test=0.1
                             ,Hi.Err.train=0.1,Lo.Err.train=0.1,NULL.Err.train=0.1
                             ,Hi.Err.diff=0.1,Lo.Err.diff=0.1,NULL.Err.diff=0.1
                             ,smallest.Error=0.1,TrainNum=0
                             ))
    print(bestParams)
  }
  loopNum <- loopNum + 1
  print(paste("Begin test:",loopNum,"sampsize=",randVar1,"nodesize=",randVar2,"maxnodes=",randVar3));

  # train RandomForest
  rf <- ranger(data = dsTraining,dependent.variable.name = "ClassChar", num.trees=50#,mtry = 15,min.node.size = 25,num.threads = 4
          ,replace=TRUE,importance = "impurity",classification = TRUE,write.forest = TRUE
          ,verbose = TRUE,save.memory = FALSE)

  classwt <- c(.95,.95,.025)
  rf <- randomForest(dsTraining[,x_col_start_pos:x_col_end_pos],dsTraining[,classColumn],nodesize = 15,ntree=250#,sampsize=randVar1,,maxnodes= randVar3,mtry = randVar4
                      ,replace=TRUE,importance = TRUE,do.trace=TRUE,classwt=classwt)#,classwt=c("SwingHi"=rv5,"SwingLo"=rv5,"SwingNULL"=rv6)

   #    )
                     #,set.seed(1)
                     #,samplesize = c("SwingNULL" = 100, "SwingHi" = 50, "SwingLo" = 50)
                     #,strata = yTrain.bal
  # rf <- foreach(ntree=rep(randVar0, 4), .combine=combine, .packages='randomForest') %dopar% + 
  #       randomForest(xTrain.bal,yTrain.bal,ntree=ntree,sampsize=randVar1#, maxnodes= randVar3
  #       ,nodesize = randVar2,mtry = randVar4,replace=TRUE,importance = TRUE,set.seed(1))
 
  #rf.all <- combine(rf, rf2, rf3, rf4)
  #print(rf.all)
  #rf <- rf.all
  # predict
  #predictions <- predict(rf, xTest.bal); predictions.t <- predict(rf, xTrain.bal);
  predictions   <- predict(rf, dsTest[,x_col_start_pos:x_col_end_pos],type="response");
  predictions.t <- predict(rf, dsTraining[,x_col_start_pos:x_col_end_pos],type="response");
  predictions.v <- predict(rf, dsValidation[,x_col_start_pos:x_col_end_pos],type="response");
  
  #create confussion matrix
  actual   <- as.matrix(dsTest[,classColumn]); #test
  actual.t <- as.matrix(dsTraining[,classColumn]);  #train
  actual.v <- as.matrix(dsValidation[,classColumn]);  #validate
  predicted   <- as.matrix(predictions); 
  predicted.t <- as.matrix(predictions.t);
  predicted.v <- as.matrix(predictions.v);
  #predicted <- as.matrix(predictions$predictions); predicted.t <- as.matrix(predictions.t$predictions);  
  conf_matrix   <- table(actual,predicted); 
  conf_matrix.t <- as.data.frame(rf$confusion);
  conf_matrix.t <- conf_matrix.t[,1:3]  
  conf_matrix.v   <- table(actual.v,predicted.v);
  
  conf_matrix.t #train
  
  conf_matrix #test
  
  conf_matrix.v #validate
  
  rf <- randomForest(rf.form,
                                cross.sell.dev,
                                ntree=500,
                                importance=T)
  
  plot(cross.sell.rf)
  
  # Variable Importance Plot
  varImpPlot(rf,sort = T,main="Variable Importance",n.var=50)

   
   # Variable Importance Table
  var.imp <- data.frame(importance(rf,type=2))
  # make row names as columns
  var.imp$Variables <- row.names(var.imp)
  VariableImportanceTable <- as.data.frame(var.imp[order(var.imp$MeanDecreaseGini,decreasing = T),])
  VariableImportanceTable[1:10,]
  
  #(1=mean decrease in accuracy, 2=mean decrease in node impurity
  plot(margin(rf,sort=T))

  rf$forest  
  
  importantVars <- importantVars[order(conf_matrix.df$predicted,conf_matrix.df$actual, decreasing=FALSE),]
  conf_matrix.df <- conf_matrix.df[order(conf_matrix.df$predicted,conf_matrix.df$actual, decreasing=FALSE),]
  ?order
  # Calculate error  
  Class1error <- sum(conf_matrix[1,2:3])/sum(conf_matrix[1,])*100;#SwingHi
  Class2error <- sum(conf_matrix[2,c(1,3)])/sum(conf_matrix[2,])*100;#SwingLo
  Class3error <- sum(conf_matrix[3,1:2])/sum(conf_matrix[3,])*100;#SwingNULL
  Class1error.t <- sum(conf_matrix.t[1,2:3])/sum(conf_matrix.t[1,])*100;#SwingHi
  Class2error.t <- sum(conf_matrix.t[2,c(1,3)])/sum(conf_matrix.t[2,])*100;#SwingLo
  Class3error.t <- sum(conf_matrix.t[3,1:2])/sum(conf_matrix.t[3,])*100;#SwingNULL
  ClassErrs <- c(SwingHi=Class1error,SwingLo=Class2error,SwingNULL=Class3error)
  avgClassErr <- mean(exp(ClassErrs/100)/(exp(ClassErrs/100)+1))
  err <- avgClassErr;
  conf_matrix.df <- as.data.frame(conf_matrix)
  conf_matrix.df <- conf_matrix.df[order(conf_matrix.df$predicted,conf_matrix.df$actual, decreasing=FALSE),]
  conf_matrix.df <- cbind(conf_matrix.df,Err=c(Class1error,Class1error,Class1error,Class2error,Class2error,Class2error,Class3error,Class3error,Class3error))
  
  #output ideal params
  if ( (err <= smallestError+(smallestError/30)) ) {
    delta <- (smallestError - err)
    smallestError <- err
    bestTrainNum <- loopNum
    new_row <- c(0,randVar0,randVar1,randVar2,randVar3
                 ,randVar4,rv5,rv6
                 ,ifelse(is.null(rf$type),0,rf$type)
                 ,ifelse(is.null(rf$localImportance),0,rf$localImportance)
                 ,ifelse(is.null(rf$proximity),0,rf$proximity)
                 ,ifelse(is.null(rf$mtry),0,rf$mtry)
                 ,ifelse(is.null(rf$forest$nrnodes),0,rf$forest$nrnodes)
                 ,ifelse(is.null(rf$forest$ntree),0,rf$forest$ntree)
                 ,conf_matrix.df[1,3],conf_matrix.df[2,3],conf_matrix.df[3,3]
                 ,conf_matrix.df[5,3],conf_matrix.df[4,3],conf_matrix.df[6,3]
                 ,conf_matrix.df[9,3],conf_matrix.df[7,3],conf_matrix.df[8,3]
                 ,sprintf("%.4f",Class1error),sprintf("%.4f",Class2error),sprintf("%.4f",Class3error)
                 ,sprintf("%.4f",Class1error.t),sprintf("%.4f",Class2error.t),sprintf("%.4f",Class3error.t)
                 ,sprintf("%.4f",Class1error.t-Class1error),sprintf("%.4f",Class2error.t-Class2error),sprintf("%.4f",Class3error.t-Class3error)
                 ,sprintf("%.6f",smallestError),bestTrainNum)
    bestParams <- rbind(bestParams, new_row)
    bestParams <- bestParams[order(bestParams$smallest.Error,decreasing=FALSE),]
    print(bestParams)
    
    #Extract raw rules from a random forest:
    treeList <- RF2List(rf)  # transform rf object to an inTrees' format
    exec <- extractRules(treeList,dsTrainingTest) # R-executable conditions
    
    #Measure rules. "len" is the number of variable-value pairs in a condition, "freq" is the percentage of data satisfying a condition, "pred" is the outcome of a rule, i.e., "condition" => "pred", "err" is the error rate of a rule.
    target <- yTest.bal
    X <- xTest.bal
    ruleMetric <- getRuleMetric(exec,X,target) # get rule metrics
    ruleMetric_orig <- ruleMetric
    
    #Prune each rule:
    ruleMetric <- pruneRule(ruleMetric,X,target)
    ruleMetric_pruned <- ruleMetric
    
    #Select a compact rule set:
    ruleMetric <- selectRuleRRF(ruleMetric,X,target)
    #ruleMetric_compact <- ruleMetric
    
    #Build an ordered rule list as a classifier:
    #learner <- buildLearner(ruleMetric,X,target)
    #learner_orig <- learner
    
    #Make rules more readable:
    #readableRules <- presentRules(learner,colnames(X))  
    
    readableRules <- presentRules(ruleMetric,colnames(X))  
    
    readableRules <- presentRules(ruleMetric_pruned,colnames(X))  
    readableRules.df <- as.data.frame(readableRules)
    readableRules.df[,1:3] <- sapply(readableRules.df[,1:3],as.character)
    readableRules.df[,1:3] <- sapply(readableRules.df[,1:3],as.numeric)
    table(sapply(readableRules.df, class))
    df <- readableRules.df[ order(-readableRules.df[,2], readableRules.df[,3]), ]
    dfu <- unique(df[,1:5])
    dfu <- cbind(dfu,bestParams[2,])
    if (nrow(bestParams) <= 2) {
      write.csv(df, file = "readableRules.csv",row.names=TRUE,append = FALSE)
      write.csv(dfu, file = "readableRulesUnique.csv",row.names=TRUE,append = FALSE)
    }
    else {
      write.csv(df, file = "readableRules.csv",row.names=TRUE,append = T)
      write.csv(dfu, file = "readableRulesUnique.csv",row.names=TRUE,append = T)
    }
    library(xtable)
    print(xtable(dfu), include.rownames=FALSE)
    
    }    
  #else {
  #  print(paste("bestTrainNum: ",bestTrainNum))
  #}
  #}}
  if (trainIteration == maxTrainIteration) {
    write.csv(bestParams, file = "bestParams_RF.csv",row.names=TRUE)
  }
  trainIteration <- trainIteration+1;
  gc()
}


X <- iris[,1:(ncol(iris)-1)]
target <- iris[,"Species"] 
rf <- RRF(X,as.factor(target),ntree=100) # build an ordinary RF 
treeList <- RF2List(rf)
ruleExec <- extractRules(treeList,X) # transform to R-executable rules



#The MSR and % variance explained are based on OOB or out-of-bag estimates, a very clever device 
#in random forests to get honest error estimates. The model reports that mtry=4, which is the 
#number of variables randomly chosen at each split. Since \( p=13 \) here, we could try all 13 
#possible values of mtry. We will do so, record the results, and make a plot.

#x_col_start_pos <- 2
#x_col_end_pos <- 823

#https://lagunita.stanford.edu/c4x/HumanitiesSciences/StatLearning/asset/ch8.html
oob.err = double(822)
test.err = double(822)
for (mtry in 1:822) {
  fit = randomForest(yTrain.bal ~ ., data = xTrain.bal, mtry = mtry, 
                     ntree = 10)
  pred = predict(fit, xTest.bal)
  #create confussion matrix
  actual <- as.matrix(yTest.bal);    
  predicted <- as.matrix(pred); 
  conf_matrix <- table(pred,actual); 
  # Calculate error  
  Class1error <- sum(conf_matrix[1,2:3])/sum(conf_matrix[1,])*100;#SwingHi
  Class2error <- sum(conf_matrix[2,c(1,3)])/sum(conf_matrix[2,])*100;#SwingLo
  Class3error <- sum(conf_matrix[3,1:2])/sum(conf_matrix[3,])*100;#SwingNULL
  ClassErrs <- c(SwingHi=Class1error,SwingLo=Class2error,SwingNULL=Class3error)
  avgClassErr <- mean(exp(ClassErrs/100)/(exp(ClassErrs/100)+1))

  oob.err[mtry] = mean(ClassErrs)
  
  test.err[mtry] = with(xTest.bal, mean((yTest.bal - pred)^2))
  cat(mtry, "= {");cat(sprintf("%.2f",mean(ClassErrs)),", ");cat(sprintf("%.2f",avgClassErr),"} ");conf_matrix;
}

matplot(1:mtry, cbind(test.err, oob.err), pch = 19, col = c("red", "blue"),   type = "b", ylab = "Mean Squared Error")
legend("topright", legend = c("OOB", "Test"), pch = 19, col = c("red", "blue"))



sampsize <-  1200; nodesize <-  280; maxnodes <- 1200/280;
nrnodes <- 2 * trunc(sampsize / nodesize) + 1
maxnodes > nrnodes

maxnodes < nrnodes

maxnodes = 2 * sampsize / nodesize + 1
(2*sampsize  + 3*nodesize)/nodesize

levels(yTrain.bal)
classwt=c("SwingHi"=0,"SwingLo"=0,"SwingNULL"=0)
# random forest
rf <- randomForest(x,y,ntree=100#,maxnodes = 5
                   ,mtry = floor(sqrt(ncol(x)))/10
                   ,sampsize = .9*nrow(x),nodesize = floor(sqrt(nrow(x)))*1
                   ,replace=TRUE,importance = TRUE) 
lists[['rf']] <- RF2List(rf) # extract a list of trees
rf$confusion
rf$err.rate

rf.cv <- rfcv(xTest.bal, yTest.bal, cv.fold=10)
with(rf.cv, plot(n.var, error.cv))

lists[['rf']]
# regularized random forest
?RRF
rrf <- RRF(x,as.factor(y),ntree=100, flagReg = 1)
lists[['rrf']] <- RF2List(rrf)

X <- xTrain.bal; class <- yTrain.bal;
#ordinary random forest. 
rf <- RRF(X,as.factor(class), flagReg = 0)
impRF <- rf$importance
impRF <- impRF[,"MeanDecreaseGini"]
rf$feaSet

#regularized random forest
rrf <- RRF(X,as.factor(class), flagReg = 1)
rrf$feaSet

#guided regularized random forest
imp <- impRF/(max(impRF))#normalize the importance score
gamma <- 0.5
coefReg <- (1-gamma)+gamma*imp #weighted average
grrf <- RRF(X,as.factor(class),coefReg=coefReg, flagReg=1)
grrf$feaSet

#guided random forest
gamma <- 1
coefReg <- (1-gamma)+gamma*imp 
grf <- RRF(X,as.factor(class),coefReg=coefReg, flagReg=0)
grf$feaSet


# boosted trees
?gbm
gbmFit <- gbm(y~ ., data=cbind(xTest.bal,yTest.bal), n.tree = 100, 
              interaction.depth = 10,distribution="multinomial")
lists[['gbm']] <- GBM2List(gbmFit,x)
dataSet <- read.csv("data/vw_barFeatures_train_r_20160819_SUBSET_BALANCED.csv", header = TRUE)
rc(dataSet)

rc(dataSet)
rc(dsTraining)
rc(dsTest)

unique(dataSet[,1]) 
unique(dsTraining[,1]) 
unique(dsTest[,1]) 


dsColNames <- as.character(names(dataSet))
dsColNames
dsColNames <- as.character(names(dsTraining))
dsColNames
dsColNames <- as.character(names(dsTest))
dsColNames

homedir<-getSrcDirectory(function(x) {x})
# Setting working directory
# need to be adjusted to server conditions
if (homedir==""|is.na(homedir)){
  homedir <-"C://Users//Neal//Documents//www.DAYTRADINGLOGIC.com//_neal//swing//R"
  #homedir <-"C://Users//nbb//Downloads//rscripts20160529"
}

# homedir <-"~//Zlecenia//ODesk//Neal Webster//swing prediction"
setwd(homedir)

#if ("package:gbm" %in% search()) { detach("package:gbm", unload=TRUE) }
#if ("gbm" %in% rownames(installed.packages())) { remove.packages("gbm") }


library(inTrees);
#install.packages("gbm")
library(gbm);

dsTrainingTest <- read.csv("data/vw_barFeatures_train_r_20161123_balanced_TrainingTest.csv", header = TRUE)
dsValidation <- read.csv("data/vw_barFeatures_train_r_20161123_balanced_Validation.csv", header = TRUE)

dsTrainingTest <- read.csv("data/vw_barFeatures_train_r_20161123_balanced_SwingHi_TrainingTest.csv", header = TRUE)
dsValidation <- read.csv("data/vw_barFeatures_train_r_20161123_balanced_SwingHi_Validation.csv.csv", header = TRUE)

dsTrainingTest <- read.csv("data/vw_barFeatures_train_r_20161123_balanced_SwingLo_TrainingTest.csv", header = TRUE)
dsValidation <- read.csv("data/vw_barFeatures_train_r_20161123_balanced_SwingLo_Validation.csv", header = TRUE)


ncol(dsTrainingTest);ncol(dsTrainingTest)==ncol(dsValidation);
nrow(dsTrainingTest);nrow(dsTrainingTest)!=nrow(dsValidation);

#ordinality of class and features 
classColumn <- 1
y_col_pos <- classColumn
x_col_start_pos <- 2
x_col_end_pos <- 3246

y_col_pos;  x_col_start_pos;  x_col_end_pos

dsColNames <- as.character(names(dsTrainingTest))
classColumnName <- dsColNames[classColumn]

print(round(prop.table(table(dsTrainingTest[,classColumn]))* 100, digits = 1))
print(round(prop.table(table(dsValidation[,classColumn]))* 100, digits = 1))

unique(dsTrainingTest[,classColumn])
unique(dsTrainingTest[,1])

gbm1 <- gbm(dsTrainingTest[,classColumn]~ ., data=dsTrainingTest[,x_col_start_pos:x_col_end_pos],n.cores=2,verbose=TRUE
            ,n.trees=15, interaction.depth=10, n.minobsinnode = 2, shrinkage=0.61#,train.fraction = 0.15
            ,distribution="multinomial"#,train.fraction = .1
            #shrinkage=0.00001,bag.fraction = 0.1
            )
      # n.trees=20)#,                 # number of trees
      # shrinkage=0.25,              # learning rate, 0.001 to 0.1 usually work
      # interaction.depth=10,           # 1: additive model, 2: two-way interactions, etc.
      # bag.fraction = 0.25,          # subsampling fraction, 0.5 is probably best
      # train.fraction = 0.15,        # fraction of data for training,first train.fraction*N used for training
      # n.minobsinnode = 60,         # minimum total weight needed in each node
      # cv.folds=10,                # do 3-fold cross-validation  cross-validation returned w/ "cv.error"
      # keep.data=TRUE,              # keep a copy of the dataset with the object
      # verbose=TRUE,               # don't print out progress
      # n.cores=4)                   # use only a single core (detecting #cores is error-prone, so avoided here)
warnings()
# check performance using an out-of-bag estimator
# OOB underestimates the optimal number of iterations
best.iter <- gbm.perf(gbm1,method="OOB")
print(best.iter)

# check performance using a 50% heldout test set
best.iter <- gbm.perf(gbm1,method="test")
print(best.iter)

# check performance using 5-fold cross-validation
best.iter <- gbm.perf(gbm1,method="cv")
print(best.iter)

# plot the performance # plot variable influence
summary(gbm1,n.trees=1)         # based on the first tree
summary(gbm1,n.trees=best.iter) # based on the estimated best number of trees

# compactly print the first and last trees for curiosity
print(pretty.gbm.tree(gbm1,1))
print(pretty.gbm.tree(gbm1,gbm1$n.trees))


#plot best num of trees
gbm.perf(gbm1)
gbm.perf(gbm1, 
         plot.it = TRUE, 
         oobag.curve = F, 
         overlay = TRUE, 
         method="test")# method="OOB",method="test",method="cv"
gbm1
import <- as.data.frame(relative.influence(gbm1, n.trees = 10))
import

dsTraining <- dataSet

# predict
x <- dsTraining[,x_col_start_pos:x_col_end_pos];    y <- dsTraining[,classColumn];

x <- dsTest[,x_col_start_pos:x_col_end_pos];        y <- dsTest[,classColumn];

x <- dsTrainingTest[,x_col_start_pos:x_col_end_pos];    y <- dsTrainingTest[,classColumn];
nrow(dsTrainingTest)

#x <- dsValidation[,x_col_start_pos:x_col_end_pos];  y <- dsValidation[,classColumn];
#nrow(dsValidation)
#nrow(x)
#x <- xBkup.bal; y <- yBkup.bal;
pred <- as.data.frame(predict(gbm1, x, n.trees = 15,type = 'response'));
#pred <- as.data.frame(pred)
names(pred) <- c(levels(y))#"SwingHi", "SwingLo", "SwingNULL")
#pred <- apply(pred, 1,which.max(pred))
pred.test <- rep(NA,1)
for (i in 1:nrow(x)) {
  pred.test[i] <- colnames(pred)[(which.max(pred[i,]))]
  }
pred <- as.factor(pred.test)
actual <- as.matrix(y);   
predicted <- as.matrix(pred);
conf_matrix <- table(predicted,actual);
conf_matrix


## save this model
save(gbm1, file = "model_GBM_20161202_1.rda")

## load the model
load("model_GBM_20161202_1.rda")

## under sampling NULL class to balance class distributions appears to work best
##gbm1 <- gbm(yTrain.bal~ ., data=cbind(xTrain.bal,yTrain.bal), distribution="multinomial",            
##              interaction.depth=25,n.trees=50,cv.folds=5,n.cores=4,verbose=TRUE)						
# actual
# predicted   SwingHi SwingLo SwingNULL
# SwingHi      1053       2        77
# SwingLo         0    1033        72
# SwingNULL      25      14       908
# 
# actual
# predicted   SwingHi SwingLo SwingNULL
# SwingHi       528       2        40
# SwingLo         0     518        44
# SwingNULL      10       8       467
# 
# actual
# predicted   SwingHi SwingLo SwingNULL
# SwingHi      1023       2        92
# SwingLo         1    1075        86
# SwingNULL      18      24       885
# 
# actual
# predicted   SwingHi SwingLo SwingNULL
# SwingHi      2076       4      1514
# SwingLo         1    2108      1353
# SwingNULL      43      38     14301

gbmFit <- gbm1
#X <- xTrain.bal; target <- yTrain.bal;
#X <- xTest.bal; target <- yTest.bal;
#X <- xVal.bal; target <- yVal.bal;
X <- dataSet[,x_col_start_pos:x_col_end_pos]; target <- dataSet[,classColumn];
treeList <- GBM2List(gbmFit,X)
ruleExec = extractRules(treeList,X,maxdepth = 50)
ruleExec <- unique(ruleExec)
#ruleExec <- ruleExec[1:min(2000,length(ruleExec)),,drop=FALSE]
ruleMetric <- getRuleMetric(ruleExec,X,target)
ruleMetric <- pruneRule(ruleMetric,X,target, maxDecay = 0, typeDecay = 1)
ruleMetric <- unique(ruleMetric)
learner <- buildLearner(ruleMetric,X,target,minFreq = 0.0000001)
pred <- applyLearner(learner,X)
#get column names that match those in EasyLanguage
hdr <- read.csv("header.csv", sep = ",", skip = 0, header = TRUE,comment.char = "", check.names = FALSE)
hdr <- as.character(names(hdr), check.names = FALSE)
#set column nates in dataset to those from  header.csv
names(X) <- hdr
# more readable format
readableLearner <- presentRules(learner,colnames(X))
#err <- 1-sum(pred==target)/length(pred);
write.csv(readableLearner, file = "readableRules_GBM_20160917_3.csv",row.names=TRUE)
readableLearner

library(RRF)
target <- yTrain.bal
X <- xTrain.bal
rf <- RRF(X,as.factor(target),ntree=100) # build an ordinary RF 
treeList <- RF2List(rf)
ruleExec <- extractRules(treeList,X) # transform to R-executable rules
ruleExec <- unique(ruleExec)
ruleMetric <- getRuleMetric(ruleExec,X,target) # measure rules
write.csv(readableLearner, file = "readableRules_RF.csv",row.names=TRUE)

#Extract raw rules from a GBM:
treeList <- GBM2List(gbm1,x)  # transform rf object to an inTrees' format
exec <- extractRules(treeList,X) # R-executable conditions
exec <- unique(exec)

#Measure rules. "len" is the number of variable-value pairs in a condition, "freq" is the percentage of data satisfying a condition, "pred" is the outcome of a rule, i.e., "condition" => "pred", "err" is the error rate of a rule.
ruleMetric <- getRuleMetric(exec,X,target) # get rule metrics
ruleMetric_orig <- ruleMetric

#Prune each rule:
ruleMetric <- pruneRule(ruleMetric,X,target)
ruleMetric_pruned <- ruleMetric

#Select a compact rule set:
ruleMetric <- selectRuleRRF(ruleMetric,X,target)
#ruleMetric_compact <- ruleMetric

#Build an ordered rule list as a classifier:
#learner <- buildLearner(ruleMetric,X,target)
#learner_orig <- learner

#Make rules more readable:
#readableRules <- presentRules(learner,colnames(X))  

readableRules <- presentRules(ruleMetric,colnames(X))  

readableRules <- presentRules(ruleMetric_pruned,colnames(X))  
readableRules.df <- as.data.frame(readableRules)
readableRules.df[,1:3] <- sapply(readableRules.df[,1:3],as.character)
readableRules.df[,1:3] <- sapply(readableRules.df[,1:3],as.numeric)
table(sapply(readableRules.df, class))
df <- readableRules.df[ order(-readableRules.df[,2], readableRules.df[,3]), ]
dfu <- unique(df[,])
dfu <- cbind(dfu,bestParams[2,])
dfu
if (nrow(bestParams) <= 2) {
  write.csv(df, file = "readableRules.csv",row.names=TRUE,append = F)
  write.csv(dfu, file = "readableRulesUnique.csv",row.names=TRUE,append = F)
}
else {
  write.csv(df, file = "readableRules.csv",row.names=TRUE,append = T)
  write.csv(dfu, file = "readableRulesUnique.csv",row.names=TRUE,append = T)
}
library(xtable)
print(xtable(dfu), include.rownames=FALSE)



#http://www.numbrcrunch.com/blog/comparing-tree-based-classification-methods-using-the-kaggle-otto-competition
# Install gbm package
install.packages('gbm')
library(gbm)
# Set a unique seed number so you get the same results everytime you run the below model,
# the number does not matter
set.seed(17)
# Begin recording the time it takes to create the model
ptm5 <- proc.time()
# Create a random forest model using the target field as the response and all 93 features as inputs (.)
fit5 <- gbm(target ~ ., data=strain, distribution="multinomial", n.trees=1000,
            shrinkage=0.05, interaction.depth=12, cv.folds=2)
# Finish timing the model
fit5.time <- proc.time() - ptm5
# Test the boosting model on the holdout test dataset
trees <- gbm.perf(fit5)
fit5.stest <- predict(fit5, stest, n.trees=trees, type="response")
fit5.stest <- as.data.frame(fit5.stest)
names(fit5.stest) <- c("Class_1","Class_2","Class_3","Class_4","Class_5","Class_6","Class_7","Class_8","Class_9")

fit5.stest.pred <- rep(NA,2000)
for (i in 1:nrow(stest)) {
  fit5.stest.pred[i] <- colnames(fit5.stest)[(which.max(fit5.stest[i,]))]}
fit5.pred <- as.factor(fit5.stest.pred)
# Create a confusion matrix of predictions vs actuals
table(fit5.pred,stest$target)
# Determine the error rate for the model
fit5$error <- 1-(sum(fit5.pred==stest$target)/length(stest$target))
fit5$error









y <- yVal.bal

x <- colnames(x)[apply(x, 1 ,which.max)]
x[1:5,]
colnames(x)[apply(x, 1 ,which.max)]

confusion <- function(a, b){
  tbl <- table(a, b)
  mis <- 1 - sum(diag(tbl))/sum(tbl)
  list(table = tbl, misclass.prob = mis)
}


#  Here is the best predictor:
confusion(predict(gbm1, cbind(xVal.bal,yVal.bal),n.trees=5), yVal.bal)

tbl <- table(x,y)

str(yVal.bal)
nrow(as.list(predictions.Test.gbm))
nrow(as.list(yVal.bal))


confusion(predict(fit.gbm1, test.data2, n.trees = 69) > 0, test.data2$y > 0)
$table
b
a       FALSE TRUE
FALSE 4409   694
TRUE   533  4364

$misclass.prob
[1] 0.1227

#  200 is better:
confusion(predict(fit.gbm1, test.data2, n.trees = 200) > 0, test.data2$y > 0)
$table
b
a       FALSE TRUE
FALSE 4635   437
TRUE   307  4621

$misclass.prob
[1] 0.0744

#  Even with 400 trees, not seriously overfit
confusion(predict(fit.gbm1, test.data2) > 0, test.data2$y > 0)
$table
b
a       FALSE TRUE
FALSE 4680   405
TRUE   262  4653

$misclass.prob
[1] 0.0667


#  Note that we have almost perfect classification on training sample

confusion(predict(fit.gbm1, train.data2) > 0, train.data2$y > 0)
$table
b
a       FALSE TRUE
FALSE  994     1
TRUE     0  1005

$misclass.prob
[1] 5e-04








gbm.perf(gbm1, method = "cv")

library(dplyr)
preds <- predict(gbm1,xVal.bal,n.trees=gbm1$n.trees,type='response')
density(preds) %>% plot

install.packages("caret")
library(caret)
#  Here is the best predictor:
confusion(predict(gbm1, test.data2, n.trees = 69) > 0, test.data2$y > 0)$table
getModelInfo()$gbm$type
pretty.gbm.tree(gbm1, i.tree = 1)

confusionMatrix(predictions.Test.gbm,yTest.bal)

?confusionMatrix
summary(gbm1)
print(gbm1)
str(gbm1)
str(predictions.Train.gbm)

table(predicted.gbm<50,actual.gbm)

actual.gbm <- as.matrix(yTest.bal);                         actual.t <- as.matrix(yTrain.bal);   
predicted.gbm <- as.matrix(predictions.Test.gbm);           predicted.t <- as.matrix(predictions.Train.gbm);  
conf_matrix.gbm <- table(predictions.Test.gbm,actual.gbm);  conf_matrix.t <- table(predicted.t,actual.t);

gbm.perf(gbm1, 
         plot.it = TRUE, 
         oobag.curve = FALSE, 
         overlay = TRUE, 
         method="OOB")#method="cv")#method="test")

# Calculate error  
Class1error.gbm <- sum(conf_matrix.gbm[1,2:3])/sum(conf_matrix.gbm[1,])*100;#SwingHi
Class2error.gbm <- sum(conf_matrix.gbm[2,c(1,3)])/sum(conf_matrix.gbm[2,])*100;#SwingLo
Class3error.gbm <- sum(conf_matrix.gbm[3,1:2])/sum(conf_matrix.gbm[3,])*100;#SwingNULL

Class1error.t <- sum(conf_matrix.t[1,2:3])/sum(conf_matrix.t[1,])*100;#SwingHi
Class2error.t <- sum(conf_matrix.t[2,c(1,3)])/sum(conf_matrix.t[2,])*100;#SwingLo
Class3error.t <- sum(conf_matrix.t[3,1:2])/sum(conf_matrix.t[3,])*100;#SwingNULL

ClassErrs.gbm <- c(SwingHi=Class1error.gbm,SwingLo=Class2error.gbm,SwingNULL=Class3error.gbm)
avgClassErr.gbm <- mean(exp(ClassErrs.gbm/100)/(exp(ClassErrs.gbm/100)+1))
err.gbm <- avgClassErr.gbm;
conf_matrix.df.gbm <- as.data.frame(conf_matrix.gbm)
conf_matrix.df.gbm



unseenXData <- xValp1a #xValp1a.bal,xValp1b.bal,xValp2a.bal,xValp2b.bal,xValp1a,xValp1b,xValp2a,xValp2b,xVal
unseenYData <- yValp1a #yValp1a.bal,yValp1b.bal,yValp2a.bal,yValp2b.bal,yValp1a,yValp1b,yValp2a,yValp2b,yVal

# predict
predictions <- predict(rf, unseenXData)

#create confussion matrix
actual <- as.matrix(unseenYData);   predicted <- as.matrix(predictions);  conf_matrix <- table(predicted,actual);conf_matrix;

# Calculate error
error1 <- conf_matrix[1,2]/(conf_matrix[1,1]+conf_matrix[1,2])
error2 <- conf_matrix[2,1]/(conf_matrix[2,1]+conf_matrix[2,2])
avgErr <- ((exp(error1)/(exp(error1)+1))+(exp(error2)/(exp(error2)+1)))/2;error1;error2;avgErr;

#Extract raw rules from a random forest:
treeList <- RF2List(rf)  # transform rf object to an inTrees' format
exec <- extractRules(treeList,dsTrainingTest) # R-executable conditions
exec[1:2,]

#Measure rules. "len" is the number of variable-value pairs in a condition, "freq" is the percentage of data satisfying a condition, "pred" is the outcome of a rule, i.e., "condition" => "pred", "err" is the error rate of a rule.
target <- yVal#yTest.bal
X <- xVal#xTest.bal
ruleMetric <- getRuleMetric(exec,X,target) # get rule metrics
ruleMetric_orig <- ruleMetric
ruleMetric_orig[1:20,]

#Prune each rule:
ruleMetric <- pruneRule(ruleMetric,X,target)
ruleMetric_pruned <- ruleMetric
ruleMetric_pruned[1:20,]

#Select a compact rule set:
ruleMetric <- selectRuleRRF(ruleMetric,X,target)
ruleMetric_compact <- ruleMetric
ruleMetric_compact[,]

#Build an ordered rule list as a classifier:
learner <- buildLearner(ruleMetric,X,target)
learner_orig <- learner
learner_orig

#Make rules more readable:
readableRules <- presentRules(learner,colnames(X))  
readableRules[,]

readableRules <- presentRules(ruleMetric,colnames(X))  
readableRules[,]

readableRules <- presentRules(ruleMetric_pruned,colnames(X))  
readableRules.df <- as.data.frame(readableRules)
readableRules.df[,1:3] <- sapply(readableRules.df[,1:3],as.character)
readableRules.df[,1:3] <- sapply(readableRules.df[,1:3],as.numeric)
table(sapply(readableRules.df, class))
df <- readableRules.df[ order(-readableRules.df[,2], readableRules.df[,3]), ]
dfu <- unique(df[,1:5])
write.csv(df, file = "readableRules.csv",row.names=TRUE)
write.csv(dfu, file = "readableRulesUnique.csv",row.names=TRUE)
append = T

library(xtable)
print(xtable(dfu), include.rownames=FALSE)


#Extract frequent variable interactions (note the rules are not pruned or selected):
treeList <- RF2List(rf)  # transform rf object to an inTrees' format
exec <- extractRules(treeList,X) # R-executable conditions
ruleMetricFreq <- getRuleMetric(exec,X,target) # get rule metrics
freqPattern <- getFreqPattern(ruleMetricFreq)
nrow(freqPattern)
freqRules <- freqPattern[which(as.numeric(freqPattern[,"len"])>3),][,] # interactions of at least two predictor variables
freqRules
readableRulesFreq <- presentRules(freqRules,colnames(X))
readableRulesFreq
library(xtable)
print(xtable(readableRulesFreq), include.rownames=FALSE)











#[1] "Training#: 134 ,trainIteration: 133 params[ 2 , 7 , 6 , 3427 , 30 , 30 ] smallestError == 0.502067331543828 delta == 0.0000 bestTrainNum == 134 error1 0.0006 error2 0.0159"
# actual
# predicted    NotSwingLo SwingLo
# NotSwingLo       3259       2
# SwingLo            48    2966

# random forest
rf <- randomForest(x,y,ntree=100#,maxnodes = 5
                   ,mtry = floor(sqrt(ncol(x)))/10
                   ,sampsize = .9*nrow(x),nodesize = floor(sqrt(nrow(x)))*1
                   ,replace=TRUE,importance = TRUE) 
lists[['rf']] <- RF2List(rf) # extract a list of trees
rf$confusion
rf$err.rate

lists[['rf']]
# regularized random forest
rrf <- RRF(x,as.factor(y),ntree=100)
lists[['rrf']] <- RF2List(rrf)

# boosted trees
gbmFit <- gbm(y~ ., data=cbind(x,y), n.tree = 100, 
              interaction.depth = 10,distribution="multinomial")
lists[['gbm']] <- GBM2List(gbmFit,x)

v <- c("rf","rrf","gbm")
v
v <- c("rf") # only use rf
#v <- c("gbm") # only use gbm
#v <- c("rrf") # only use rf
X <- x
for(i in v){X
  target <- y
  #X <- x
  treeList <- lists[[i]]
  ruleExec0 <- extractRules(treeList,X) # transform to R-executable conditions
  ruleExec <- unique(ruleExec0) # unique rules
  cat( paste("There are ", length(ruleExec), " unique conditions. \n",sep="") )
  
  # Too many conditions could make the following steps time-consuming, 
  # so one could randomly select a subset of the conditions
  ix <- sample(1:length(ruleExec),min(2000,length(ruleExec))) #randomly select 2000 rules
  ruleExec <- ruleExec[ix,,drop=FALSE] 
  
  ruleMetric <- getRuleMetric(ruleExec,X,target) # measure rules
  
  lookup <- lookupRule(ruleMetric,c("X[,4]","X[,3]")) # look up rules including X[,4] and X[,3]
  ruleMetric <- pruneRule(ruleMetric,X,target) # prune each rule

  # selecting rules by threholds of frequency & error
  ix <- which(as.numeric(ruleMetric[,"freq"])>0.001 & as.numeric(ruleMetric[,"err"])< 0.5)

  ruleMetric <- ruleMetric[ix,]
  
  ruleMetric <- selectRuleRRF(ruleMetric,X,target) # rule selection

  learner <- buildLearner(ruleMetric,X,target) #build the simplified tree ensemble learner

  pred <- applyLearner(learner,X) #appy learner to data
  readableLearner <- presentRules(learner,colnames(X)) # present the rules with a more readable format
  # print(readableLearner)
  # -- frequent variable interactions or conditions in a tree ensemble
  # NOTE: the calculation is based on ruleExec0 WITHOUT pruning or selection
  ruleMetric <- getRuleMetric(ruleExec0,X,target) 
  freqPattern <- getFreqPattern(ruleMetric)
  #ruleMetric <- getRuleMetric(freqPattern,X,target)
}
#readableLearner
#ruleMetric
#freqPattern
ruleMetric <- pruneRule(ruleMetric,X,target)
ruleMetric[1:2,]

readableRules <- presentRules(ruleMetric,colnames(X))  
readableRules[1:2,]
#format the rule and metrics as a table in latex code
#install.packages("xtable")
library(xtable)
print(xtable(ruleMetric), include.rownames=F)
print(xtable(readableLearner), include.rownames=F)#use in sharelatex.com

# --- transform regression rules to classification rules
# make Sepal.Length as the target, other as predictors
X <- iris[,-1]; target <- iris[,"Sepal.Length"] 
rf <- randomForest(x,target,ntree=30) # random forest
ruleExec0 <- extractRules(RF2List(rf),X) 
ruleExec <- unique(ruleExec0)
nrow(ruleExec0)
nrow(ruleExec)

target <- dicretizeVector(target) # discretize it into three levels with equal frenquency

# methods for classification rules can then be used for 
# the conditions extracted from the regression trees
ruleMetric <- getRuleMetric(ruleExec,X,target)
ruleMetric
# --- decision tree and logistic regression
X.double <- apply(X,2,as.numeric)
#install.packages("glmnet")
library(glmnet)
cvNet <- cv.glmnet(X.double,as.factor(target), family = "multinomial",type.measure = "class")
coef <- coef(cvNet)

#install.packages("rpart")
library(rpart)
r <- rpart(target ~. , X)

