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
getwd()

#if ("package:gbm" %in% search()) { detach("package:gbm", unload=TRUE) }
#if ("gbm" %in% rownames(installed.packages())) { remove.packages("gbm") }

#install.packages("inTrees")
library(inTrees);
#install.packages("gbm")
library(gbm);

dataSet1 <- read.csv("data/data_extract_1_2017-02-23_141110.csv", header = TRUE)
dataSet2 <- read.csv("data/data_extract_3_2017-02-23_133048.csv", header = TRUE)
dataSet3 <- read.csv("data/data_extract_2_2017-02-23_131341.csv", header = TRUE)

as.integer(ncol(dataSet1))
as.integer(nrow(dataSet1))
dataSet1[1:5,1:10]

as.integer(ncol(dataSet2))
as.integer(nrow(dataSet2))
dataSet2[1:5,1:10]
class(dataSet2)

as.integer(ncol(dataSet3))
as.integer(nrow(dataSet3))
dataSet3[1:5,1:10]

dataSet <- data.frame()
dataSet <- cbind(dataSet1[,1:ncol(dataSet1)],dataSet2[,2:ncol(dataSet2)],dataSet3[,2:ncol(dataSet3)])

total_columns <- as.integer(ncol(dataSet1)) + as.integer(ncol(dataSet2)) + as.integer(ncol(dataSet3)) - 2
total_columns
as.integer(ncol(dataSet))
as.integer(nrow(dataSet))
dataSet[1:5,1:10]
table(dataSet[,1])

#extract merged files, close RSession
write.csv(dataSet, file = "data/dataSet_20170304_134502.csv",row.names=FALSE)


#install.packages("beepr")
library(beepr)
beep <- function(n){
  Sys.sleep(.5)
  beepr::beep(10)
  cat("Work complete...............\n")
  for(i in seq(n)){
    Sys.sleep(.9)
    beepr::beep(2);    Sys.sleep(.1)#system("rundll32 user32.dll,MessageBeep -1")
    beepr::beep(2);    Sys.sleep(.2)
    beepr::beep(2);    Sys.sleep(.4)
    beepr::beep(2);    Sys.sleep(.1)
  }
  Sys.sleep(.5)
  beepr::beep(10)
  cat("Now get back to work...............\n")
}

beep(3)

work_complete <- function() {
  cat("Work complete. Press esc to sound the fanfare!!!\n")
  on.exit(beepr::beep(3))
  
  while (TRUE) {
    beepr::beep(4)
    Sys.sleep(1)
  }
}

work_complete()

#load file
dataSet <- read.csv("data/dataSet_20170304_134502.csv", header = TRUE); beep(3)
dim(dataSet);
table(dataSet[,1]);
round(prop.table(table(dataSet[,1])) * 100, digits = 2) ;


# dsTrainingTest <- read.csv("data/vw_barFeatures_train_r_20161123_balanced_TrainingTest.csv", header = TRUE)
# dsValidation <- read.csv("data/vw_barFeatures_train_r_20161123_balanced_Validation.csv", header = TRUE)
# 
# dsTrainingTest <- read.csv("data/vw_barFeatures_train_r_20161123_balanced_SwingHi_TrainingTest.csv", header = TRUE)
# dsValidation <- read.csv("data/vw_barFeatures_train_r_20161123_balanced_SwingHi_Validation.csv.csv", header = TRUE)
# 
# dsTrainingTest <- read.csv("data/vw_barFeatures_train_r_20161123_balanced_SwingLo_TrainingTest.csv", header = TRUE)
# dsValidation <- read.csv("data/vw_barFeatures_train_r_20161123_balanced_SwingLo_Validation.csv", header = TRUE)
# 
# dsTrainingTest <- read.csv("data/vw_barFeatures_train_r_20161123_subsampled_SwingNULL.csv", header = TRUE)
# dsValidation <- read.csv("data/vw_barFeatures_train_r_20161123_clean.csv", header = TRUE)
# 
# dataSet <- read.csv("data/vw_barFeatures_train_r_20161123_clean.csv", header = TRUE)
# dataSet <- read.csv("data/vw_barFeatures_train_r_20161123_subsampled_SwingNULL.csv", header = TRUE)
# 
# dsTrainingTest <- read.csv("dsTrainingTest_20161224.csv", header = TRUE)



# #Randomly sample a percentage of rows to balance class distribution
#mydf <- mydf[ sample( which(mydf$swingRecordType=="SwingNULL"), round(0.1235*length(which(mydf$swingRecordType=="SwingNULL")))), ]
dataSet <- dataSet_bkp
#Randomly sample "SwingNULL" rows
dataSet.SwingNULL <- dataSet[ sample( which(dataSet$ClassChar==0), round(.0625*length(which(dataSet$ClassChar==0))),replace = TRUE), ];
beep(1);
table(dataSet.SwingNULL[,classColumn])
round(prop.table(table(dataSet.SwingNULL[,classColumn])) * 100, digits = 2)
nrow(dataSet)
nrow(dataSet.SwingNULL)


#Randomly sample "SwingHi" rows
# dataSet.SwingHi <- dataSet[ sample( which(dataSet$ClassChar==2), round(43.82*length(which(dataSet$ClassChar==2))),replace = TRUE), ]
# table(dataSet.SwingHi[,classColumn])
# round(prop.table(table(dataSet.SwingHi[,classColumn])) * 100, digits = 2)

#Randomly sample "SwingLo" rows
# dataSet.SwingLo <- dataSet[ sample( which(dataSet$ClassChar==-2), round(41.92*length(which(dataSet$ClassChar==-2))),replace = TRUE), ]
# table(dataSet.SwingLo[,classColumn])
# round(prop.table(table(dataSet.SwingLo[,classColumn])) * 100, digits = 2)

#append all "SwingHi" and "SwingLo" rows to "SwingNULL" rows
#dataSet2 <- rbind(dataSet[ which(dataSet$ClassChar=="SwingNULL"), ],dataSet.SwingHi,dataSet.SwingLo)

#append randomly sampled "SwingNULL" rows to "SwingHi" , "SwingLo" rows
blah <- nrow(dataSet[ which(dataSet$ClassChar!=0), ])
blah
blah <- nrow(dataSet.SwingNULL)
blah
blah <- nrow(dataSet[ which(dataSet$ClassChar!=0), ]) + nrow(dataSet.SwingNULL)
blah

dataSet2 <- rbind(dataSet[ which(dataSet$ClassChar!=0), ],dataSet.SwingNULL);
beep(1);
nrow(dataSet2)
dim(dataSet2)
table(dataSet2[,classColumn])
round(prop.table(table(dataSet2[,classColumn])) * 100, digits = 2)

dataSet_bkp <- dataSet
dim(dataSet_bkp)
dataSet <- dataSet2
dim(dataSet)
#Free RAM. Remove objects / data from workspace
rm(dataSet2,dataSet.SwingNULL)
gc()

table(dataSet[,classColumn])
round(prop.table(table(dataSet[,classColumn])) * 100, digits = 2)

dataSet[,classColumn] <- as.factor(dataSet[,classColumn])
dataSet[,classColumn] <- as.character(dataSet[,classColumn])
dataSet[1:10,1:5]
dataSet$ClassChar[dataSet$ClassChar == "-2"] <- "SwingLo"
dataSet$ClassChar[dataSet$ClassChar == "0"] <- "SwingNULL"
dataSet$ClassChar[dataSet$ClassChar == "2"] <- "SwingHi"
dataSet[1:10,1:5]
head(dataSet[,1:5])
tail(dataSet[,1:5])

table(dataSet[,classColumn])
round(prop.table(table(dataSet[,classColumn])) * 100, digits = 2)


write.csv(dataSet, file = "data/dataSet_20170304_134502_clean_rebalanced.csv",row.names=FALSE,append = FALSE);beep(1);

#load data set
dataSet <- read.csv("data/dataSet_20170304_134502_clean_rebalanced.csv", header = TRUE); beep(1)
dim(dataSet);
table(dataSet[,1]);
round(prop.table(table(dataSet[,1])) * 100, digits = 2) ;

#num of columns and rows
dsColCount <- as.integer(ncol(dataSet))
dsRowCount <- as.integer(nrow(dataSet))
dsColCount
dsRowCount

#ordinality of class and features 
classColumn <- 1
y_col_pos <- classColumn
x_col_start_pos <- 2
x_col_end_pos <- 9868
colnames(dataSet)[classColumn] <- "ClassChar" #"barClass"

y_col_pos;  x_col_start_pos;  x_col_end_pos

#save column names
dsColNames <- as.character(names(dataSet))
classColumnName <- dsColNames[classColumn]
classColumnName
class(dataSet[,1])


# % of dataset reserved for training/test and validation  
set.seed(712)
sampleAmt <- 0.5
mainSplit <- sample(2, dsRowCount, replace=TRUE, prob=c(sampleAmt, 1-sampleAmt))
table(mainSplit)
round(prop.table(table(mainSplit)) * 100, digits = 2)

#split dataSet into two sets
dsTrainingTest <- dataSet[mainSplit==1, ] 
dsValidation <- dataSet[mainSplit==2, ]
beep(1);
rc(dsTrainingTest)
rc(dsValidation)

#get class frequency
class_frequency <-  data.frame(data_set = c("dataSet","dsTrainingTest","dsValidation"),
                               class_freq = rbind(table(dataSet[,classColumn]),table(dsTrainingTest[,classColumn]),table(dsValidation[,classColumn])))
colnames(class_frequency) <- c("data set","[class = -2]","[class = 0]","[class = 2]")
class_frequency_percent <-  data.frame(data_set = c("dataSet","dsTrainingTest","dsValidation"),
                                       class_freq = rbind(round(prop.table(table(dataSet[,classColumn])) * 100, digits = 2),
                                                          round(prop.table(table(dsTrainingTest[,classColumn])) * 100, digits = 2),
                                                          round(prop.table(table(dsValidation[,classColumn])) * 100, digits = 2)))
colnames(class_frequency_percent) <- c("data set","[class = -2]","[class = 0]","[class = 2]")

#view class distribution for all data sets
class_frequency
class_frequency_percent


write.csv(dsTrainingTest, file = "data/dataSet_20170304_134502_clean_rebalanced_dsTrainingTest.csv",row.names=FALSE,append = FALSE);beep(1);
write.csv(dsValidation, file = "data/dataSet_20170304_134502_clean_rebalanced_dsValidation.csv",row.names=FALSE,append = FALSE);beep(1);

dsTrainingTest <- read.csv("data/dataSet_20170304_134502_clean_rebalanced_dsTrainingTest.csv", header = TRUE); beep(3)
dsValidation <- read.csv("data/dataSet_20170304_134502_clean_rebalanced_dsTrainingTest.csv", header = TRUE); beep(3)
dim(dataSet);
table(dataSet[,1]);
round(prop.table(table(dataSet[,1])) * 100, digits = 2) ;

colnames(dataSet)[classColumn] <- "SwingRecordType"
dataSet[1:3,1:5]

colnames(dsTrainingTest)[classColumn] <- "SwingRecordType"
dsTrainingTest[1:3,1:5]

colnames(dsValidation)[classColumn] <- "SwingRecordType"
dsValidation[1:3,1:5]

dsColNames <- as.character(names(dataSet))
classColumnName <- dsColNames[classColumn]
classColumnName
class(dataSet[,1])

dim(dataSet);
table(dataSet[,1]);
round(prop.table(table(dataSet[,1])) * 100, digits = 2) ;

dim(dsTrainingTest);
table(dsTrainingTest[,1]);
round(prop.table(table(dsTrainingTest[,1])) * 100, digits = 2) ;

dim(dsValidation);
table(dsValidation[,1]);
round(prop.table(table(dsValidation[,1])) * 100, digits = 2) ;

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

dsColNames
dsColNames <- as.character(names(dataSet))
column_names_df <-as.list(dsColNames) 
head(column_names_df)
tail(column_names_df)
column_names_df <- paste(column_names_df,"]", sep="")
column_names_df <- gsub("indSMALLp", "indSMALL[RightStrength+", column_names_df)
column_names_df <- gsub("indMEDp", "indMED[RightStrength+", column_names_df)
column_names_df <- gsub("indBIGp", "indBIG[RightStrength+", column_names_df)
column_names_df <- gsub("indBXp", "indBX[RightStrength+", column_names_df)
column_names_df <- gsub("indBXLp", "indBXL[RightStrength+", column_names_df)
column_names_df <- gsub("indSMALLm", "indSMALL[RightStrength-", column_names_df)
column_names_df <- gsub("indMEDm", "indMED[RightStrength-", column_names_df)
column_names_df <- gsub("indBIGm", "indBIG[RightStrength-", column_names_df)
column_names_df <- gsub("indBXm", "indBX[RightStrength-", column_names_df)
column_names_df <- gsub("indBXLm", "indBXL[RightStrength-", column_names_df)

column_names_df
column_names_df_new <- data.frame()
column_names_df_new <-  data.frame(c(0,column_names_df))
column_names_df_new <- column_names_df_new[c(-1,-2),]
#extract file and then manually open in excel to transpose rows to columns
write.csv(column_names_df_new, file = "data/dataSet_20170304_134502_header.csv",row.names=FALSE,append = FALSE);beep(1);


hdr <- read.csv("data/dataSet_20170304_134502_header.csv", sep = ",", skip = 0, header = TRUE,comment.char = "", check.names = FALSE)
hdr <- as.character(names(hdr), check.names = FALSE)
print(hdr)
#set column nates in dataset to those from  header.csv
names(X) <- hdr


nrow(dsTrainingTest);table(dsTrainingTest[,classColumn])



dsValidationBig <- dataSet
xBig <- dsValidationBig[,x_col_start_pos:x_col_end_pos];  yBig <- dsValidationBig[,classColumn];
#write.csv(dsValidationBig, file = "dsValidationBig_20161224.csv", row.names=FALSE)

x_data <- as.matrix(dsTrainingTest[,x_col_start_pos:x_col_end_pos])
y_data <- as.matrix(dsTrainingTest[,classColumn])

rm(x,y)
gc()



file_type <- ".csv"
model_type_name <- "GBM"
rules_doc_name <- "Readable_Rules"; rules_doc_name <- paste(model_type_name,rules_doc_name,sep = "_");
pva_doc_name <- "predicted_vs_actual"; pva_doc_name <- paste(model_type_name,pva_doc_name,sep = "_");
model_perf_doc_name <- "best_Params"; model_perf_doc_name <- paste(model_type_name,model_perf_doc_name,sep = "_");
t <- Sys.time(); options(digits.secs=2);
file_name_rules <- paste(rules_doc_name,format(t,"_%Y%m%d-%H%M%S"),file_type,sep = "")
file_name_pva <- paste(pva_doc_name,format(t,"_%Y%m%d-%H%M%S"),file_type,sep = "")
file_name_perf <- paste(model_perf_doc_name,format(t,"_%Y%m%d-%H%M%S"),file_type,sep = "")

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
 
 for (rv1 in seq(from=.010, to=.090, by=.010)){#model$shrinkage
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
  print(paste("________________ n.trees=",150,"shrinkage=",rv1,"interaction.depth=",rv2,"n.minobsinnode=",rv3,"bag.fraction=",rv4,"train.fraction=",rv5));
  classColumnName
  #train GBM
  set.seed=1
  gbm1 <- gbm.fit(x=as.matrix(dsTrainingTest[,x_col_start_pos:x_col_end_pos]), y=as.matrix(dsTrainingTest[,classColumn]),verbose=T
                  ,response.name = classColumnName#"SwingRecordType" 
                  ,n.trees=750,shrinkage=rv1,interaction.depth=rv2,n.minobsinnode = rv3,bag.fraction = rv4,train.fraction = rv5
                  ,distribution="multinomial")
  
  #write.csv(dsTrainingTest, file = "dsTrainingTest_20161224.csv", row.names=FALSE)
  #save model as new object
  model <- gbm1
  beepr::beep(2);
  
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
    beepr::beep(2);
  }
}

