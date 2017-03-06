#install.packages("xgboost")
library(xgboost)

data(agaricus.train, package='xgboost')
data(agaricus.test, package='xgboost')
train <- agaricus.train
test <- agaricus.test
bst <- xgboost(data = train$data, label = train$label, max.depth = 2,
               eta = 1, nthread = 2, nround = 2,objective = "binary:logistic")
# save the model in file 'xgb.model.dump'
xgb.dump(bst, 'xgb.model.dump', with.stats = TRUE)

# print the model without saving it to a file
print(xgb.dump(bst))

#Let's see what the model looks like.
model <- xgb.dump(bst, with.stats = T)
model[1:10]
#Basically each line represents a branch, there is the:
# - tree ID, 
# - feature ID,
# - the point where it splits, 
# - and information regarding the next branches (left, right, when the row for this feature is N/A).
#Hopefully, XGBoost offers a better representation: feature importance.
#Feature importance is about averaging the gain of each feature for all split and all trees.

# Get the feature real names
names <- dimnames(trainMatrix)[[2]]
# Compute feature importance matrix
importance_matrix <- xgb.importance(names, model = bst)
# Nice graph
xgb.plot.importance(importance_matrix[1:10,])
#Tree graph
xgb.plot.tree(feature_names = names, model = bst, n_first_tree = 2)


#View the trees from a model
#You can dump the tree you learned using xgb.dump into a text file.
xgb.dump(bst, with.stats = F)
?xgb.dump
#You can plot the trees from your model using `xgb.plot.tree
xgb.plot.tree(model = bst,filename_dump = "C:/Users/Neal/Documents/www.DAYTRADINGLOGIC.com/_neal/swing/R")
?xgb.plot.tree
#if you provide a path to fname parameter you can save the trees to your hard drive.

xgb.dump(bst, "dump.raw2.txt", with.stats = F)


