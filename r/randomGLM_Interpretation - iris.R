# Load data 
iris <- read.csv(url("http://archive.ics.uci.edu/ml/machine-learning-databases/iris/iris.data"), header = FALSE)

#dataset from system of record is retained in "Staging", not to be altered in Staging
dataStg <- iris

#maintain integrity of original dataset - begin transformations on a copy of the original dataset
dataSet <- dataStg

#assign column names
names(dataSet) <- c("SepalLength", "SepalWidth", "PetalLength", "PetalWidth", "Species")

#col names
dsColNames <- as.character(names(dataSet))

#num of columns and rows
dsColCount <- as.integer(ncol(dataSet))
dsRowCount <- as.integer(nrow(dataSet))

#class ordinality and name
classColumn <- 5 
classColumnName <- dsColNames[classColumn]
x_col_start_pos <- 1
x_col_end_pos <- 4
y_col_pos <- classColumn

#distinct list and count of classes from column assumed to contain class values
dsClassValues <- as.character(unique(dataSet[,classColumn])) #levels(dataSet[,classColumn])
dsClassCount <- as.integer(length(dsClassValues)) #sqldf("select distinct(x) from df1")

dsClasses <- dataSet[,classColumn]
# dsClasses <- as.character(dsClasses)
#   dsClasses[dsClasses=="Iris-setosa"] <- 0 
#   dsClasses[dsClasses=="Iris-versicolor"] <- 1 
#   dsClasses[dsClasses=="Iris-virginica"] <- 2 
# dsClasses <- as.integer(dsClasses)
unique(dsClasses)  
str(dsClasses)

dataSetEnum <- dataSet    
dataSetEnum[,5] <- as.character(dataSetEnum[,5])
dataSetEnum[,5][dataSetEnum[,5]=="Iris-setosa"] <- 0 
dataSetEnum[,5][dataSetEnum[,5]=="Iris-versicolor"] <- 1 
dataSetEnum[,5][dataSetEnum[,5]=="Iris-virginica"] <- 0 
dataSetEnum[,5] <- as.integer(dataSetEnum[,5])
unique(dataSetEnum[,5])  
str(dataSetEnum)

var1 <- dataSet[,1]
var2 <- dataSet[,2]
var3 <- dataSet[,3]
var4 <- dataSet[,4]

#Initial Overview Of The Data Set
#install.packages('ggvis')
library(ggvis)

#create scatter plot - is there a visible correlation between var1 and var2 or var3 and var4 for all classes?
dataSet %>% ggvis(~var1, ~var1, fill = ~dsClasses) %>% layer_points()
dataSet %>% ggvis(~var2, ~var2, fill = ~dsClasses) %>% layer_points()
dataSet %>% ggvis(~var3, ~var3, fill = ~dsClasses) %>% layer_points()
dataSet %>% ggvis(~var4, ~var4, fill = ~dsClasses) %>% layer_points()
dataSet %>% ggvis(~var3, ~var4, fill = ~dsClasses) %>% layer_points()

for (pNum in seq(1,4,1)) {
  dataSet %>% ggvis(~dataSet[,pNum], ~dataSet[,pNum], fill = ~dsClasses) %>% layer_points()
}  

#class distribution in terms of row count/freqency and percentage/proportions
dsClassFreq <- table(dsClasses)
dsClassDistribution <- round(prop.table(table(dsClasses)) * 100, digits = 1)
dsClassFreq
dsClassDistribution

set.seed(1234)

#% of [dataset] reserved for training/test, (1-sampleAmt)% used for validation  
sampleAmt <- 0.4
mainSplit <- sample(2, dsRowCount, replace=TRUE, prob=c(sampleAmt, 1-sampleAmt))

#split [dataSet] or [dataSetEnum] into two sets per sampleAmt% and (1-sampleAmt)%
dsTrainingTest <- dataSet[mainSplit==1, 1:5] 
dsValidation <- dataSet[mainSplit==2, 1:5]
nrow(dsTrainingTest);nrow(dsValidation);

#% of [dsTrainingTest] reserved for training, (1-sampleAmt)% used for test
sampleAmt <- 0.5
secondarySplit <- sample(2, nrow(dsTrainingTest), replace=TRUE, prob=c(sampleAmt, 1-sampleAmt))

#split [dsTrainingTest] into two sets per sampleAmt% and (1-sampleAmt)%
dsTraining <- dsTrainingTest[secondarySplit==1, 1:5]
dsTest <- dsTrainingTest[secondarySplit==2, 1:5]
nrow(dsTraining);nrow(dsTest);
dsTrainingNoClass <- dsTraining[,1:4]
dsTestNoClass <- dsTest[,1:4]

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


#RGLMinterpretationTutorial.docx
#1. Data preparation
# load required package
library(randomGLM)
# download data from webpage and load it.
# Importantly, change the path to the data and use back slashes /
setwd("C://Users//nbb//Downloads//rscripts20160529//")
load("srbct.rda")
# check data
names(srbct)
dim(srbct$x)
table(srbct$y)
# 1  2 
# 40 23

dataSetEnum <- dsTraining[,1:5]
str(dataSetEnum)
dataSetEnum[,5] <- as.character(dataSetEnum[,5])
dataSetEnum[,5][dataSetEnum[,5]=="Iris-setosa"] <- 1 
dataSetEnum[,5][dataSetEnum[,5]=="Iris-versicolor"] <- 2 
dataSetEnum[,5][dataSetEnum[,5]=="Iris-virginica"] <- 2 
dataSetEnum[,5] <- as.integer(dataSetEnum[,5])
z <- dataSetEnum
write.csv(z, file = "MyData.csv")

x <- as.matrix(dataSetEnum[,1:4])#srbct$x
y <- as.factor(dataSetEnum[,5:5])#srbct$y
str(x)
str(y)

# number of features
N <- ncol(x)

# define function misclassification.rate, for accuracy calculation
if (exists("misclassification.rate") ) rm(misclassification.rate);
misclassification.rate<-function(tab){
  num1<-sum(diag(tab))
  denom1<-sum(tab)
  signif(1-num1/denom1,3)
}

#2. RGLM prediction (fit model)
#First we do RGLM prediction with default parameter settings. The prediction accuracy is 0.984, with 1 out of 63 observations being misclassified.

RGLM <- randomGLM(x, y, 
                 classify=TRUE, 
                 keepModels=TRUE)
tab1 <- table(y, RGLM$predictedOOB)
tab1
#     1  2
#  1 40  0
#  2  1 22
# accuracy
1-misclassification.rate(tab1)
#[1] 0.9841

#3. Feature selection 
# variable importance measure
varImp = RGLM$timesSelectedByForwardRegression
sum(varImp>0)
# 83
table(varImp)
#varImp
#   0    1    2    3    4    5    6    7    8    9   10   14   15   17 
#2225   52   12    6    3    1    2    1    1    1    1    1    1    1 
# select most important features
impF = colnames(x)[varImp>=5]
impF
# [1] "G246"  "G545"  "G566"  "G1074" "G1319" "G1327" "G1389" "G1954" "G2050"
#[10] "G2117"

#4. RGLM interpretation 
#We build a single GLM to explain the outcome with the 10 most important features only.
#G566 and G1327 are negatively associated with the outcome, while other features are positively associated with the outcome.
# build single GLM model with most important features
model1 = glm(y~., data=as.data.frame(x[, impF]), family = binomial(link='logit'))
coef(model1)
exp(coef(model1))
exp(cbind(OR = coef(model1), confint(model1)))
confint.default(model1)
#Coefficients:
#(Intercept)         G246         G545         G566        G1074        G1319  
#   -29.2645       7.1445       5.0429      -6.9307       3.1406       0.7925  
#      G1327        G1389        G1954        G2050        G2117  
#    -2.1011       4.9900       9.3048       1.3402       3.8649  

#5. Compare single model prediction with original RGLM prediction
#see how the prediction from the above single model using top 10 most important features
#correspond to the original RGLM prediction. In other words, how well does a single model 
#pick up the signal from the RGLM ensemble?
# compare the performance of single model with most important features and original RGLM
# out-of-bag prediction probabilities from RGLM
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
