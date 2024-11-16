hello everyone!
this project contains expense tracking system of a family.

probelm statement : 
"Expense tracking system"

You are tasked with implementing an Expense tracking application to track family expenses. The application should allow users to add monthly expenses for different categories such as rent, utilities, groceries, etc. The goal is to track total as well as category-wise expense of the whole family. You need to implement this application using an array or multiple arrays of structures.

Individual structure must contain the following fields:
User ID 
User name
Income

Family Structure : 
Family ID
Family name
Array of individual IDs belonging to the family (reference to User ID)
Total family income calculated as the sum of individual incomes
Total Monthly family expense calculated as sum of all expenses by family members

Expense Structure : 
Expense ID
User ID (Reference to User ID)
Expense Category: Rent, Utility, Grocery, Stationary, Leisure
Expense Amount
Date of expense

Implement the following functions to create an expense tracking application:

AddUser(): A new user should be added using this function. Every time users are added, they need to be stored in a sorted manner based on USER ID.

AddExpense(): An expense should be added along with references to users. Expenses should be stored in a sorted manner according to user ID. (All expenses by one user should appear together in the array sorted based on expense ID)

CreateFamily(): Each user must belong to a Family. The family can have minimum 1 and maximum 4 members. The total monthly expense of the family should be calculated on the creation of family.

Update_or_delete_individual_Family_details(): This function should be able to update individual or family details. Based on the update to individual details, family details should automatically be updated. It should also delete an individual or family. If a family has only 1 individual, then the family should automatically be deleted on deleting the individual and vice-versa.

Update_delete_expense(): This function should be able to update or delete an existing expense. It should update all the other information where this expense is accounted for.

Get_total_expense(): This function should print the total family expense. It should also print whether the given expense is within the family expense or surpasses it for the month and print the difference between them.

Get_categorical_expense(string category): This function should print the total family expense for the given category. It should also print the individual contribution of each individual in the family to that expense category in a sorted manner. For eg. If total expense for Utilities for a family is 15000. User 1 spent 9000, User 2 spent 4000, User 3 spent 2000.

Get_highest_expense_day(): This function should track all the expenses by the family and print the date on which highest amount is spent.

Get_individual_expense(UserID): This function should print the total individual expense for the month. It should also print individual expense for each category in a sorted manner (descending order).

i have used array of structures to solve this questions, and used file handling for easy inputs and i have provided a userinterface experience in my respository, where you can get an idea about how my code by watching it.
