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

#if ("package:gbm" %in% search()) { detach("package:gbm", unload=TRUE) }
#if ("gbm" %in% rownames(installed.packages())) { remove.packages("gbm") }

#install.packages("inTrees")
library(inTrees);
#install.packages("gbm")
library(gbm);

dsTrainingTest <- read.csv("data/vw_barFeatures_train_r_20161123_balanced_TrainingTest.csv", header = TRUE)
dsValidation <- read.csv("data/vw_barFeatures_train_r_20161123_balanced_Validation.csv", header = TRUE)

dsTrainingTest <- read.csv("data/vw_barFeatures_train_r_20161123_balanced_SwingHi_TrainingTest.csv", header = TRUE)
dsValidation <- read.csv("data/vw_barFeatures_train_r_20161123_balanced_SwingHi_Validation.csv.csv", header = TRUE)

dsTrainingTest <- read.csv("data/vw_barFeatures_train_r_20161123_balanced_SwingLo_TrainingTest.csv", header = TRUE)
dsValidation <- read.csv("data/vw_barFeatures_train_r_20161123_balanced_SwingLo_Validation.csv", header = TRUE)

dsTrainingTest <- read.csv("data/vw_barFeatures_train_r_20161123_subsampled_SwingNULL.csv", header = TRUE)
dsValidation <- read.csv("data/vw_barFeatures_train_r_20161123_clean.csv", header = TRUE)

dataSet <- read.csv("data/vw_barFeatures_train_r_20161123_clean.csv", header = TRUE)
dataSet <- read.csv("data/vw_barFeatures_train_r_20161123_subsampled_SwingNULL.csv", header = TRUE)


#num of columns and rows
dsColCount <- as.integer(ncol(dataSet))
dsRowCount <- as.integer(nrow(dataSet))
dsColCount
dsRowCount

#ordinality of class and features 
classColumn <- 1
y_col_pos <- classColumn
x_col_start_pos <- 2
x_col_end_pos <- 3246

y_col_pos;  x_col_start_pos;  x_col_end_pos

dsColNames <- as.character(names(dsTrainingTest))
classColumnName <- dsColNames[classColumn]


dataSet.SwingNULL <- dataSet[ sample( which(dataSet$ClassChar=="SwingNULL"), round(.25*length(which(dataSet$ClassChar=="SwingNULL"))),replace = TRUE), ]
table(dataSet.SwingNULL[,classColumn])
round(prop.table(table(dataSet.SwingNULL[,classColumn])) * 100, digits = 2)

# % of [dataset] reserved for training/test and validation  
set.seed(8704)
sampleAmt <- 0.85
mainSplit <- sample(2, dsRowCount, replace=TRUE, prob=c(sampleAmt, 1-sampleAmt))
#split [dataSet] into two sets
dsTrainingTest <- dataSet[mainSplit==1, ]#firstCol:lastCol] 
dsValidation <- dataSet[mainSplit==2, ]#firstCol:lastCol]
rc(dsTrainingTest) ;rc(dsValidation)
table(dsTrainingTest[,classColumn]) ;table(dsValidation[,classColumn])
round(prop.table(table(dsTrainingTest[,classColumn])) * 100, digits = 2) ; round(prop.table(table(dsValidation[,classColumn])) * 100, digits = 2)
# SwingHi   SwingLo SwingNULL 
# 221       229       487 
# 
# SwingHi   SwingLo SwingNULL 
# 220       232       479 
# SwingHi   SwingLo SwingNULL 
# 23.59     24.44     51.97 
# 
# SwingHi   SwingLo SwingNULL 
# 23.63     24.92     51.45 

#don't use entire dataset. Use dataset with sub-sampled "SwingNULL"
X <- dsTrainingTest[,x_col_start_pos:x_col_end_pos]; target <- dsTrainingTest[,classColumn]  
#X <- subsampled_SwingNULL[,x_col_start_pos:x_col_end_pos]; target <- subsampled_SwingNULL[,classColumn]  
#get column names that match those in EasyLanguage
hdr <- read.csv("data/header.csv", sep = ",", skip = 0, header = TRUE,comment.char = "", check.names = FALSE)
hdr <- as.character(names(hdr), check.names = FALSE)
print(hdr)
#set column nates in dataset to those from  header.csv
names(X) <- hdr

dsTrainingTest <- read.csv("data/vw_barFeatures_train_r_20161123_subsampled_SwingNULL.csv", header = TRUE)

nrow(dsTrainingTest);table(dsTrainingTest[,classColumn])

dsTrainingTest <- rbind(dsTrainingTest,dataSet.SwingNULL)

nrow(dsTrainingTest);table(dsTrainingTest[,classColumn])

dsValidationBig <- read.csv("data/vw_barFeatures_train_r_20161123_clean.csv", header = TRUE)
xBig <- dsValidationBig[,x_col_start_pos:x_col_end_pos];  yBig <- dsValidationBig[,classColumn];
#write.csv(dsValidationBig, file = "dsValidationBig_20161224.csv", row.names=FALSE)
dsValidationBig

t <- Sys.time(); options(digits.secs=2);
file_type <- ".csv"
rules_doc_name <- "readableRules_GBM"
pva_doc_name <- "predicted_vs_actual_GBM"
model_perf_doc_name <- "bestParams_GBM"
file_name_rules <- paste(rules_doc_name,format(t,"%Y%m%d_%H%M%S"),file_type,sep = "_")
file_name_pva <- paste(pva_doc_name,format(t,"%Y%m%d_%H%M%S"),file_type,sep = "_")
file_name_perf <- paste(model_perf_doc_name,format(t,"%Y%m%d_%H%M%S"),file_type,sep = "_")

file_name_rules
file_name_pva
file_name_perf

maxTrainIteration <- 180
trainIteration <- 0;  loopNum <- 0; bestTrainNum <- NULL; smallestError <- 100;
while (trainIteration <= maxTrainIteration) {
  #set random values for model params
  rv0 <- roundUp(sample(100:550,1,replace=TRUE),50); #model$n.trees
  rv1 <- sample(1:5,1,replace=TRUE) *  10^(-sample(2:4,1,replace=TRUE)); #model$shrinkage
  rv2 <- sample(2:10,1,replace=TRUE); #model$interaction.depth
  rv3 <- sample(1:10,1,replace=TRUE); #model$n.minobsinnode
  rv4 <- sample(4:8,1,replace=TRUE) *.1; #model$bag.fraction
  rv5 <- sample(6:9,1,replace=TRUE) *.1; #model$train.fraction
  rv6 <- roundUp(sample(1:100,1,replace=TRUE),5); #model$nTrain
  
 nealsForLoop <- data.frame(cbind(rv1=rv1,rv2=rv2,rv3=rv3,rv4=rv4,rv5=rv5))
 
 for (rv1 in seq(from=.001, to=.090, by=.001)){#model$shrinkage
   for (rv2 in seq(from=2, to=8, by=6)){#model$interaction.depth
     for (rv3 in seq(from=2, to=2, by=1)){#model$n.minobsinnode
       for (rv4 in seq(from=.6, to=.6, by=.1)){#model$bag.fraction
         for (rv5 in seq(from=.9, to=.9, by=.1)){#model$train.fraction
           nealsForLoop <- rbind(nealsForLoop,cbind(rv1=rv1,rv2=rv2,rv3=rv3,rv4=rv4,rv5=rv5))
            #print(nealsForLoop[(nrow(nealsForLoop)-10):nrow(nealsForLoop),])
#          }
#        }
#      }
#    } 
#    print(nrow(nealsForLoop))
#  }
           seq(from=2, to=8, by=6)
  
  if (loopNum == 0) {
      #create df to store params and model performance
      bestParams <- data.frame(cbind(recordType=1
                                   ,rv0=0,rv1=0.1,rv2=0,rv3=0
                                   ,rv4=0.1,rv5=0.1,rv6=0
                                   ,n.trees=0#model$n.trees#100 rv0
                                   ,shrinkage=0.1#model$shrinkage#0.001 rv1
                                   ,interaction.depth=0#model$interaction.depth#1 rv2
                                   ,n.minobsinnode=0#model$n.minobsinnode#10 rv3
                                   ,bag.fraction=0.1#model$bag.fraction#.5 rv4
                                   ,train.fraction=0.1#model$train.fraction#1 rv5
                                   ,nTrain=0L#model$nTrain
                                   ,Hi_Hi=0,Hi_Lo=0,Hi_NULL=0
                                   ,Lo_Lo=0,Lo_Hi=0,Lo_NULL=0
                                   ,NULL_NULL=0,NULL_Hi=0,NULL_Lo=0
                                   ,Hi.Err.test=0.1,Lo.Err.test=0.1,NULL.Err.test=0.1
                                   ,smallest.Error=10.1,obs.not.classified=0,TrainNum=0
#                                    model <- gbm1
#                                    model$distribution
#                                    model$response.name
#                                    model$num.classes
#                                    model$classes
#                                    model$var.names
#                                    model$train.error
#                                    model$oobag.improve
#                                    model$trees
                                   ))
    #print(bestParams)
  }
  loopNum <- loopNum + 1
  print(paste("________________ Begin test:",loopNum,"_______________________________________________________________________________________________________________"));
  print(paste("________________ ___________",rep("_",nchar(loopNum)),"_______________________________________________________________________________________________________________"));
  print(paste("________________ n.trees=",750,"shrinkage=",rv1,"interaction.depth=",rv2,"n.minobsinnode=",rv3,"bag.fraction=",rv4,"train.fraction=",rv5));
  
  #train GBM
  set.seed=1
  gbm1 <- gbm.fit(x=as.matrix(dsTrainingTest[,x_col_start_pos:x_col_end_pos]), y=as.matrix(dsTrainingTest[,classColumn]),verbose=T
                  ,response.name = "SwingRecordType" 
                  ,n.trees=750,shrinkage=rv1,interaction.depth=rv2,n.minobsinnode = rv3,bag.fraction = rv4,train.fraction = rv5
                  ,distribution="multinomial")
  #write.csv(dsTrainingTest, file = "dsTrainingTest_20161224.csv", row.names=FALSE)
  #save model as new object
  model <- gbm1
  
  #plot train vs. validation deviance
  #check performance using a % heldout test set
  best.iter <- gbm.perf(model,method="test")
  print(best.iter)
  
  #Transform gbm object to a list of trees that can be used for rule condition extraction
  treeList <- GBM2List(model,X)
  #transform to R-executable conditions
  ruleExec <- extractRules(treeList,X,ntree = model$n.trees, maxdepth = 1000) 
  # measure rules
  ruleMetric <- getRuleMetric(ruleExec,X,target) 
  #build the simplified tree ensemble learner
  learner <- buildLearner(ruleMetric,X,target,minFreq = 0.00000001) 
  #present the rules with a more readable format
  readableLearner <- presentRules(learner,colnames(X)) 
  
  if (loopNum == 1) {
    #run one time to extract rules from first model
    extractedRules <- data.frame()
    extractedRules <- data.frame(cbind(readableLearner,ModelName=loopNum))
    newRules <- data.frame()
    }
  #run this after the first model to get rules for each new model
  newRules <- data.frame(cbind(readableLearner,ModelName=loopNum))
  #appened rules from new model to rules from prior models
  extractedRules <- rbind(extractedRules, newRules)
  print(nrow(extractedRules))
  #sort by predcicted class
  extractedRules <- extractedRules[order(extractedRules$pred,decreasing=FALSE),]
  print(table(extractedRules[,6])) ;print(table(extractedRules[,5:6])) ;

  write.csv(extractedRules[,1:ncol(extractedRules)], file = file_name_rules, row.names=FALSE)
  print(paste("write",file_name_rules , "DONE", sep=" "))

  #use model on unseen data  
  pred <- as.data.frame(predict(gbm1, xBig, n.trees = model$n.trees,type = 'response'));
  names(pred) <- c(levels(yBig))
  
  pred.test <- rep(NA,1)
  for (i in 1:nrow(xBig)) {
    pred.test[i] <- colnames(pred)[(which.max(pred[i,]))]
  }
  pred <- as.factor(pred.test)
  actual <- as.matrix(yBig);   
  predicted <- as.matrix(pred);

  #create confussion matrix
  conf_matrix <- table(actual,predicted);
  print(conf_matrix)
         
  if (loopNum == 1) {
    Pred_v_Act  <- data.frame()
    Pred_v_Act  <- data.frame(cbind(predicted=predicted,actual=actual,ModelName=loopNum))
    pva <- data.frame()
  }
  Pred_v_Act <- data.frame(cbind(predicted=predicted,actual=actual,ModelName=loopNum))
  names(Pred_v_Act) <- c("predicted", "actual","ModelName")
  #appened predicted class and actual class from model
  pva <- rbind(pva, Pred_v_Act)

  write.csv(pva, file = file_name_pva, row.names=FALSE)
  print(paste("write",file_name_pva , "DONE", sep=" "))
  
  # Calculate error  
  Class1error <- sum(conf_matrix[1,2:3])/sum(conf_matrix[1,])*100;#SwingHi
  Class2error <- sum(conf_matrix[2,c(1,3)])/sum(conf_matrix[2,])*100;#SwingLo
  Class3error <- sum(conf_matrix[3,1:2])/sum(conf_matrix[3,])*100;#SwingNULL
  ClassErrs <- c(SwingHi=Class1error,SwingLo=Class2error,SwingNULL=Class3error)
  avgClassErr <- mean(exp(ClassErrs/100)/(exp(ClassErrs/100)+1))
  err <- Class3error;
  conf_matrix.df <- as.data.frame(conf_matrix)
  conf_matrix.df <- conf_matrix.df[order(conf_matrix.df$predicted,conf_matrix.df$actual, decreasing=FALSE),]
  conf_matrix.df <- cbind(conf_matrix.df,Err=c(Class1error,Class1error,Class1error,Class2error,Class2error,Class2error,Class3error,Class3error,Class3error))
  print(paste("________________ classified:",sum(conf_matrix.df[,3]),"observations of",nrow(xBig),"available observations.",sum(conf_matrix.df[,3])-nrow(xBig),"observations not classified."))  
  conf_matrix.df
  #log model params and performance 
  #if ( err <= smallestError ) {
  if ( 1 < 2 ) {
    delta <- (smallestError - err)
    smallestError <- err
    bestTrainNum <- loopNum
    new_row <- c(0,rv0,rv1,rv2,rv3
                 ,rv4,rv5,rv6
                 ,ifelse(is.null(model$n.trees),0,model$n.trees)
                 ,ifelse(is.null(model$shrinkage),0,model$shrinkage)
                 ,ifelse(is.null(model$interaction.depth),0,model$interaction.depth)
                 ,ifelse(is.null(model$n.minobsinnode),0,model$n.minobsinnode)
                 ,ifelse(is.null(model$bag.fraction),0,model$bag.fraction)
                 ,ifelse(is.null(model$train.fraction),0,model$train.fraction)
                 ,ifelse(is.null(model$nTrain),0,model$nTrain)
                 ,conf_matrix.df[1,3],conf_matrix.df[2,3],conf_matrix.df[3,3]
                 ,conf_matrix.df[5,3],conf_matrix.df[4,3],conf_matrix.df[6,3]
                 ,conf_matrix.df[9,3],conf_matrix.df[7,3],conf_matrix.df[8,3]
                 ,sprintf("%.4f",Class1error)
                 ,sprintf("%.4f",Class2error)
                 ,sprintf("%.4f",Class3error)
                 ,sprintf("%.6f",smallestError)
                 ,sum(conf_matrix.df[,3])-nrow(xBig)
                 ,loopNum)
    #output ideal params
    bestParams <- rbind(bestParams, new_row)
    bestParams <- bestParams[order(bestParams$smallest.Error,decreasing=FALSE),]
    if (loopNum == 1) {
      bestParams <-   bestParams[which(bestParams$TrainNum!="0"),]
    }
    topParams <- bestParams[,16:30]
    print(paste("________________ bestTrainNum: ",bestParams[1,30]))
    print(topParams[1:5,])
    } else {
      topParams <- bestParams[,16:30]
      print(paste("________________ bestTrainNum: ",bestParams[1,30]))
    }

  if (trainIteration == maxTrainIteration) {
    write.csv(bestParams, file = file_name_perf,row.names=FALSE)
    print(paste("write",file_name_perf , "DONE", sep=" "))
  }
  write.csv(bestParams, file = file_name_perf,row.names=FALSE)
  print(paste("write",file_name_perf , "DONE", sep=" "))
  print(paste("---------------- -----------",rep("-",nchar(loopNum)),"---------------------------------------------------------------------------------------------------------------"));
  
  trainIteration <- trainIteration+1;
  gc();
  
  print(paste("---------------- -----------",rep("-",nchar(loopNum)),"---------------------------------------------------------------------------------------------------------------"));

          }
        }
      }
    } 
    print(nrow(nealsForLoop))
  }
}

