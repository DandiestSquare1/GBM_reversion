
value_set <- data.frame()
loop_num <- 1
t <- Sys.time(); options(digits.secs=2);

for (rv1 in seq(from=.030, to=.4, by=.010)){#model$shrinkage
  for (rv2 in seq(from=1, to=31, by=5)){#model$interaction.depth
    for (rv3 in seq(from=1, to=21, by=5)){#model$n.minobsinnode
      for (rv4 in seq(from=.5, to=.9, by=.1)){#model$bag.fraction
        for (rv5 in seq(from=.6, to=.8, by=.1)){#model$train.fraction
          value_set <- data.frame(rbind(value_set,cbind(loop_num=loop_num,rv1=rv1,rv2=rv2,rv3=rv3,rv4=rv4,rv5=rv5)))
          loop_num <- loop_num+1
          }
        #print(value_set[nrow(value_set)-10:nrow(value_set),],row.names=FALSE)
        }
      }
    print(loop_num,row.names=FALSE)
    } 
  if (rv2 == max(seq(from=.030, to=.4, by=.010)) && rv3 == max(eq(from=1, to=31, by=5))) {
    name <- "value_sets"
    file_type <- ".csv"
    file_name <- paste(name,format(t,"%Y%m%d_%H%M%S"),file_type,sep = "_")
    file_name
    write.csv(value_set[,1:ncol(value_set)], file = file_name,row.names=FALSE)
    print("write.csv DONE")
  }  
}


value_set <- data.frame()
loop_num <- 1
for (rv2 in seq(from=1, to=31, by=5)){#model$interaction.depth
  for (rv3 in seq(from=1, to=21, by=4)){#model$n.minobsinnode
    value_set <- data.frame(rbind(value_set,cbind(loop_num=loop_num,rv2=rv2,rv3=rv3,result=rv2*rv3)))
    loop_num <- loop_num+1
    }
  if (rv2 == max(seq(from=1, to=31, by=5)) && rv3 == max(seq(from=1, to=21, by=4))) {
    name <- "value_sets"
    t <- Sys.time(); options(digits.secs=2);
    
    file_type <- ".csv"
    file_name <- paste(name,format(t,"%Y%m%d_%H%M%S"),file_type,sep = "_")
    file_name
    write.csv(value_set[,1:ncol(value_set)], file = file_name,row.names=FALSE)
    print("write.csv DONE")
    }  
}



t <- Sys.time(); options(digits.secs=2);
name <- "seed_sets"
file_type <- ".csv"
file_name <- paste(name,format(t,"%Y%m%d_%H%M%S"),file_type,sep = "_")
sampleAmt <- 0.85
value_set <- data.frame()
loop_num <- 1
for (rv2 in seq(from=0, to=90000, by=1)){#model$interaction.depth
    
    # % of [dataset] reserved for training/test and validation   
    set.seed(rv2)
    mainSplit <- sample(2, dsRowCount, replace=TRUE, prob=c(sampleAmt, 1-sampleAmt))
    dsTrainingTest <- dataSet[mainSplit==1, ]#firstCol:lastCol] 
    dsValidation <- dataSet[mainSplit==2, ]#firstCol:lastCol]
    sample_result <- data.frame(table(dsTrainingTest[,classColumn])) 
    
    if (sample_result[1,2]==sample_result[2,2]) {
      compare <- "equal"
    } else if (abs(sample_result[1,2]-sample_result[2,2])<=2) {
      compare <- "approx"
    } else {
      compare <- "-"
    }

      value_set <- data.frame(rbind(value_set,cbind(
        loop_num=loop_num,
        seed=rv2,
        SwingHi=sample_result[1,2],
        SwingLo=sample_result[2,2],
        SwingNULL=sample_result[3,2],
        compare=compare)))

    print(paste(loop_num, sample_result[1,2],sample_result[2,2],sample_result[3,2],compare, sep=" "))
    loop_num <- loop_num+1
    write.csv(value_set[,1:ncol(value_set)], file = file_name,row.names=FALSE)
    
    if (rv2 == 90000) {
    write.csv(value_set[,1:ncol(value_set)], file = file_name,row.names=FALSE)
    print("write .csv DONE")
  }  
}


seq(from=1, to=31, by=5)
seq(from=1, to=31, by=7)

seq(from=1, to=21, by=4)
seq(from=1, to=21, by=6)

seq(from=.010, to=.090, by=.010)

nealsForLoop <- data.frame()
for (rv1 in seq(from=.001, to=.090, by=.001)){#model$shrinkage
  for (rv2 in seq(from=2, to=8, by=6)){#model$interaction.depth
    for (rv3 in seq(from=2, to=2, by=1)){#model$n.minobsinnode
      for (rv4 in seq(from=.6, to=.6, by=.1)){#model$bag.fraction
        for (rv5 in seq(from=.9, to=.9, by=.1)){#model$train.fraction
          nealsForLoop <- rbind(nealsForLoop,cbind(rv1=rv1,rv2=rv2,rv3=rv3,rv4=rv4,rv5=rv5))
          print(nrow(nealsForLoop))
          }
        }
      }
    } 
}
nealsForLoop
          
