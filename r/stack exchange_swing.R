homedir<-getSrcDirectory(function(x) {x})
# Setting working directory
# need to be adjusted to server conditions
if (homedir==""|is.na(homedir)){
  homedir <-"C://Users//Neal//Documents//www.DAYTRADINGLOGIC.com//_neal//swing//R"
}

# homedir <-"~//Zlecenia//ODesk//Neal Webster//swing prediction"
setwd(homedir)


# Load data (using iris dataset from Google Drive because link @ uci.edu was not working for me today)
#iris <- read.csv(url("http://archive.ics.uci.edu/ml/machine-learning-databases/iris/iris.data"), header = FALSE)
#iris <- read.csv(url("https://docs.google.com/spreadsheets/d/1ovz31Y6PrV5OwpqFI_wvNHlMTf9IiPfVy1c3fiQJMcg/pub?gid=811038462&single=true&output=csv"), header = FALSE)
iris <- read.csv("vw_barFeatures_train_r_SwingHiLo.csv", header = TRUE)
dataSet <- iris

#assign column names
names(dataSet) <- c("SepalLength", "SepalWidth", "PetalLength", "PetalWidth", "Species")

#col names
dsColNames <- as.character(names(dataSet))

#num of columns and rows
dsColCount <- as.integer(ncol(dataSet))
dsRowCount <- as.integer(nrow(dataSet))

#class ordinality and name
classColumn <- 1
classColumnName <- dsColNames[classColumn]
y_col_pos <- classColumn

#features ordinality
x_col_start_pos <- 2
x_col_end_pos <- 28

#distinct list and count of classes from column assumed to contain class values
dsClassValues <- as.character(unique(dataSet[,classColumn])) #levels(dataSet[,classColumn])
dsClassCount <- as.integer(length(dsClassValues)) #sqldf("select distinct(x) from df1")
dsClasses <- dataSet[,classColumn]

#class distribution in terms of row count/freqency and percentage/proportions
dsClassFreq <- table(dsClasses)
dsClassDistribution <- round(prop.table(table(dsClasses)) * 100, digits = 1)
dsClassFreq
dsClassDistribution

# % of [dataset] reserved for training/test and validation  
set.seed(10)
sampleAmt <- 0.10
mainSplit <- sample(2, dsRowCount, replace=TRUE, prob=c(sampleAmt, 1-sampleAmt))

#split [dataSet] into two sets
dsTrainingTest <- dataSet[mainSplit==1, 1:x_col_end_pos] 
dsValidation <- dataSet[mainSplit==2, 1:x_col_end_pos]
nrow(dsTrainingTest);nrow(dsValidation);

# % of [dsTrainingTest] reserved for training
sampleAmt <- 0.5
secondarySplit <- sample(2, nrow(dsTrainingTest), replace=TRUE, prob=c(sampleAmt, 1-sampleAmt))

#split [dsTrainingTest] into two sets 
dsTraining <- dsTrainingTest[secondarySplit==1, 1:x_col_end_pos]
dsTest <- dsTrainingTest[secondarySplit==2, 1:x_col_end_pos]
nrow(dsTraining);nrow(dsTest);

nrow(dataSet) == nrow(dsTrainingTest)+nrow(dsValidation)
nrow(dsTrainingTest) == nrow(dsTraining)+nrow(dsTest)

#0. complete dataset
str(dataSet)
#1. Training dataset
str(dsTraining)
#2. Test dataset
str(dsTest)
#4. Validation dataset
str(dsValidation)

#install.packages("data.table")
library(data.table)

dsSummary <- data.table(nrow(dataSet),nrow(dsTraining),nrow(dsTest),nrow(dsValidation))
names(dsSummary) <- c("completeDataset", "TrainingDataset", "TestDataset", "ValidationDataset")
ncol(dsSummary)
dsSummary

write.csv(dataSet, file = "export_completeDataset.csv",row.names=TRUE)
write.csv(dsTraining, file = "export_TrainingDataset.csv",row.names=TRUE)
write.csv(dsTest, file = "export_TestDataset.csv",row.names=TRUE)
write.csv(dsValidation, file = "export_ValidationDataset.csv",row.names=TRUE)

library(randomGLM)

trainSetEnum <- dsTraining[,1:x_col_end_pos]
trainSetEnum[,classColumn] <- as.character(trainSetEnum[,classColumn])
trainSetEnum[,classColumn][trainSetEnum[,classColumn]=="SwingHi"] <- 1 
trainSetEnum[,classColumn][trainSetEnum[,classColumn]=="SwingLo"] <- 2 
trainSetEnum[,classColumn] <- as.integer(trainSetEnum[,classColumn])

x <- as.matrix(trainSetEnum[,x_col_start_pos:x_col_end_pos])
y <- as.factor(trainSetEnum[,classColumn:classColumn])


testSetEnum <- dsTest[,1:x_col_end_pos]
testSetEnum[,classColumn] <- as.character(testSetEnum[,classColumn])
testSetEnum[,classColumn][testSetEnum[,classColumn]=="SwingHi"] <- 1 
testSetEnum[,classColumn][testSetEnum[,classColumn]=="SwingLo"] <- 2 
testSetEnum[,classColumn] <- as.integer(testSetEnum[,classColumn])

xTest <- as.matrix(testSetEnum[,x_col_start_pos:x_col_end_pos])
yTest <- as.factor(testSetEnum[,classColumn:classColumn])

nrow(x)
nrow(y)
nrow(xTest)
nrow(yTest)

# number of features
N <- ncol(x)

# define function misclassification.rate
if (exists("misclassification.rate") ) rm(misclassification.rate);
misclassification.rate<-function(tab){
  num1<-sum(diag(tab))
  denom1<-sum(tab)
  signif(1-num1/denom1,3)
}

#Fit randomGLM model - Ensemble predictor comprised of individual generalized linear model predictors
RGLM <- randomGLM(x, y, xtest = xTest,randomSeed=1002
                    ,classify=TRUE, keepModels=TRUE
                    #,interactionsMandatory = TRUE
                    #,maxInteractionOrder = 2
                    #,interactionSeparatorForCoefNames = ".times."
                    ,replace = TRUE
                    #,verbose = 1
                    #,nBags = 1001
                    ,nThreads = 4
                  )

RGLM$thresholdClassProb

tab1 <- table(y, RGLM$predictedOOB)
tab1[,]
accurFinal1 <- tab1[1,1]/(tab1[1,1]+tab1[1,2])
accurFinal2 <- tab1[2,2]/(tab1[2,1]+tab1[2,2])
accurFinal1
accurFinal2

tab1Test <- table(yTest, RGLM$predictedTest)
tab1Test[,]
accurFinalTest1 <- tab1Test[1,1]/(tab1Test[1,1]+tab1Test[1,2])
accurFinalTest2 <- tab1Test[2,2]/(tab1[2,1]+tab1Test[2,2])
accurFinalTest1
accurFinalTest2


# accuracy
1-misclassification.rate(tab1)

# variable importance measure
varImp = RGLM$timesSelectedByForwardRegression
sum(varImp>=0)
varImp
table(varImp)

# select most important features
impF = colnames(x)[varImp>=20]
impF
features <- fTbl <- table(colnames(x)[varImp])
features
# build single GLM model with most important features
model1 = glm(y~., data=as.data.frame(x[, impF]), family = binomial(link='logit'))

tbl2 <- table(model1$y, ifelse(fitted.values(model1) > 0.5, 1, 0))
tbl2[,]
tbl2Final1 <- tbl2[1,1]/(tbl2[1,1]+tbl2[1,2])
tbl2Final2 <- tbl2[2,2]/(tbl2[2,1]+tbl2[2,2])
tbl2Final1
tbl2Final2


str(model1)
a <- coef(model1)
str(a)
#(Intercept) PetalLength  PetalWidth 
#-31.938998   -7.501714   63.670583 
a
exp(coef(model1))

m <- model.matrix(model1) %*% coef(model1)
m

e <- exp(m) / (1+exp(m))
e

f <- fitted.values(model1)
f

testFit <- ifelse(fitted.values(model1) > 0.5, 2, 1)
testFit


# Write CSV in R
write.csv(dataSetEnum, file = "MyData.csv",row.names=TRUE)


log(exp(coef(model1)))  
#(Intercept)  PetalLength   PetalWidth 
#1.346075e-14 5.521371e-04 4.485211e+27 

confint.default(model1)
#                2.5 %   97.5 %
#(Intercept) -363922.5 363858.6
#PetalLength -360479.0 360464.0
#PetalWidth  -916432.0 916559.4

RGLM$predictedOOB.response


predRGLM = RGLM$predictedOOB.response[,2]
# define function to calculate leave-one-out prediction of single model
LOOlogistic = function(y, x, impF)
{
  nLoops = length(y)
  predLOO = rep(NA, nLoops)
  for (ind in 1:nLoops)
  {
    model = glm(y[-ind]~., data=as.data.frame(x[-ind, impF]), family = binomial(link='logit'))
    predLOO[ind] = predict(model, newdata=as.data.frame(x[ind, impF, drop=F]), type="response")
    rm(model)
  }
  predLOO
}
# leave-one-out predictive prob of single model
predLOO = LOOlogistic(y, x, impF)
# leave-one-out prediction accuracy of single model
1-misclassification.rate(table(y, round(predLOO)))

tabGlm1 <- table(y, round(predLOO))
tabGlm1[,]
accurFinal1 <- tabGlm1[1,1]/(tabGlm1[1,1]+tabGlm1[1,2])
accurFinal2 <- tabGlm1[2,2]/(tabGlm1[2,1]+tabGlm1[2,2])
accurFinal1
accurFinal2


#[1] 0.9841
# Single model LOO prediction achieves the same accuracy as RGLM, and it misclassified the same one observation as RGLM did.
# plot
source("https://bioconductor.org/biocLite.R")
biocLite("GO.db")
biocLite("impute")
biocLite("preprocessCore")
library(GO.db)
library(impute)
library(preprocessCore)
#install.packages("WGCNA")
library(WGCNA)
verboseScatterplot(predLOO, predRGLM, 
                   xlab = paste("LOO predictive prob of a single model with", length(impF), "most important features"),
                   ylab = "RGLM OOB predictive prob",
                   cex.lab=1.2,
                   cex.axis=1.2)
abline(lm(predRGLM~predLOO), lwd=2)
dev.off()
#The single model makes very similar predictions to the original RGLM 
#(cor=0.97, p-value = 3.6*10-39). Therefore in this application, a single model after RGLM 
#feature selection achieves good prediction accuracy and is very easy and straightforward 
#to interpret.

#6. RGLM model coefficients
# get coefficients of GLM models
# check coefficients of RGLM bag 1
coef(RGLM$models[[1]])
# OUTPUT
#(Intercept)       G1954 
#  -61.53254   158.88250 
# create matrix of coefficients of features across bags
nBags = length(RGLM$featuresInForwardRegression)
nBags
coefMat = matrix(0, nBags, RGLM$nFeatures)
for (i in 1:nBags)
{
  coefMat[i, RGLM$featuresInForwardRegression[[i]]] = RGLM$coefOfForwardRegression[[i]]
}
# check mean coefficients of features across bags
coefMean = apply(coefMat, 2, mean)
names(coefMean) = colnames(x)
summary(coefMean)
coefMean
#     Min.   1st Qu.    Median      Mean   3rd Qu.      Max. 
#-44.67000   0.00000   0.00000   0.07888   0.00000  31.27000
coefMean[impF]
#      G246       G545       G566      G1074      G1319      G1327      G1389 
#  8.122109   7.947393  21.349161   4.599621  18.207984  31.269950   7.282620 
#     G1954      G2050      G2117 
#  7.522164 -14.200134  24.644747
